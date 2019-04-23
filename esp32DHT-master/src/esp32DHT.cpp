/*

Copyright 2018 Bert Melis

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONDHTTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "esp32DHT.hpp"  // NOLINT

#define RMT_CLK_DIV 80

DHT::DHT() :
  _status(0),
  _data{0},
  _pin(0),
  _channel(RMT_CHANNEL_0),
  _onData(nullptr),
  _onError(nullptr),
  _timer(nullptr),
  _task(nullptr) {}

DHT::~DHT() {
  if (_timer) {  // if _timer is true, setup() has been called
                 // so RMT driver is loaded and the aux task is
                 // running
    esp_timer_delete(_timer);
    rmt_driver_uninstall(_channel);
    vTaskDelete(_task);
  }
}

void DHT::setup(uint8_t pin, rmt_channel_t channel) {
  _pin = pin;
  _channel = channel;
  esp_timer_create_args_t _timerConfig;
  _timerConfig.arg = static_cast<void*>(this);
  _timerConfig.callback = reinterpret_cast<esp_timer_cb_t>(_handleTimer);
  _timerConfig.dispatch_method = ESP_TIMER_TASK;
  _timerConfig.name = "esp32DHTTimer";
  esp_timer_create(&_timerConfig, &_timer);
  rmt_config_t config;
  config.rmt_mode = RMT_MODE_RX;
  config.channel = _channel;
  config.gpio_num = static_cast<gpio_num_t>(_pin);
  config.mem_block_num = 2;
  config.rx_config.filter_en = 1;
  config.rx_config.filter_ticks_thresh = 10;
  config.rx_config.idle_threshold = 1000;
  config.clk_div = RMT_CLK_DIV;
  rmt_config(&config);
  rmt_driver_install(_channel, 400, 0);  // 400 words for ringbuffer containing pulse trains from DHT
  rmt_get_ringbuf_handle(_channel, &_ringBuf);
  xTaskCreate((TaskFunction_t)&_handleData, "esp32DHT", 2048, this, 5, &_task);
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
}

void DHT::onData(esp32DHTInternals::OnData_CB callback) {
  _onData = callback;
}

void DHT::onError(esp32DHTInternals::OnError_CB callback) {
  _onError = callback;
}

void DHT::read() {
  // _pin should be set to OUTPUT and HIGH
  digitalWrite(_pin, LOW);
  esp_timer_start_once(_timer, 18 * 1000);  // timer is in microseconds
  _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;
  _status = 0;
}

const char* DHT::getError() const {
  if (_status == 1) {
    return "TO";
  } else if (_status == 2) {
    return "NACK";
  } else if (_status == 3) {
    return "DATA";
  } else if (_status == 4) {
    return "CS";
  }
  return "OK";
}

void DHT::_handleTimer(DHT* instance) {
  pinMode(instance->_pin, INPUT);
  rmt_rx_start(instance->_channel, 1);
  rmt_set_pin(instance->_channel, RMT_MODE_RX, static_cast<gpio_num_t>(instance->_pin));  // reset after using pin as output
  xTaskNotifyGive(instance->_task);
}

void DHT::_handleData(DHT* instance) {
  size_t rx_size = 0;
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // block and wait for notification
    // blocks until data is available or timeouts after 1000
    rmt_item32_t* items = static_cast<rmt_item32_t*>(xRingbufferReceive(instance->_ringBuf, &rx_size, 1000));
    if (items) {
      instance->_decode(items, rx_size/sizeof(rmt_item32_t));
      vRingbufferReturnItem(instance->_ringBuf, static_cast<void*>(items));
      rmt_rx_stop(instance->_channel);
      pinMode(instance->_pin, OUTPUT);
      digitalWrite(instance->_pin, HIGH);
    } else {
      instance->_status = 1;  // timeout error
      rmt_rx_stop(instance->_channel);
      pinMode(instance->_pin, OUTPUT);
      digitalWrite(instance->_pin, HIGH);
    }
    instance->_tryCallback();
  }
}

void DHT::_decode(rmt_item32_t* data, int numItems) {
  if (numItems != 42) {
    _status = 5;
  } else if ((data[0].duration0 + data[0].duration1) < 140 && (data[0].duration0 + data[0].duration1) > 180) {
    _status = 2;
  } else {
    for (uint8_t i = 1; i < numItems - 1; ++i) {  // don't include tail
      uint8_t pulse = data[i].duration0 + data[i].duration1;
      if (pulse > 55 && pulse < 145) {
        _data[(i - 1) / 8] <<= 1;  // shift left
        if (pulse > 120) {
          _data[(i - 1) / 8] |= 1;
        }
      } else {
        _status = 3;  // DATA error
        return;
      }
    }
    if (_data[4] == ((_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF)) {
      _status = 1;
    } else {
      _status = 4;  // checksum error
    }
  }
}

void DHT::_tryCallback() {
  if (_status == 0) {
    if (_onData) _onData(_getHumidity(), _getTemperature());
  } else {
    if (_onError) _onError(_status);
  }
}

float DHT11::_getTemperature() {
  if (_status < 1) return NAN;
  return static_cast<float>(_data[2]);
}

float DHT11::_getHumidity() {
  if (_status < 1) return NAN;
  return static_cast<float>(_data[0]);
}

float DHT22::_getTemperature() {
  if (_status < 1) return NAN;
  float temp = (((_data[2] & 0x7F) << 8) | _data[3]) * 0.1;
  if (_data[2] & 0x80) {  // negative temperature
    temp = -temp;
  }
  return temp;
}

float DHT22::_getHumidity() {
  if (_status < 1) return NAN;
  return ((_data[0] << 8) | _data[1]) * 0.1;
}