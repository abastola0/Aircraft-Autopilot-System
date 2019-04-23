#include "MQ9.h"

MQ9::MQ9 (PinName const &p) : _pin(p)
{
    _LPG_ppm = 0.0;
    _CO_ppm = 0.0;
    _Methane_ppm = 0.0;
}

int MQ9::read()
{
    float sensorDat = _pin.read();
    float sensorVolt = sensorDat * 3.3 / 4095;
    
    _LPG_ppm = 26.572*exp(1.2894*sensorVolt);
    _CO_ppm = 10.938*exp(1.7742*sensorVolt);
    _Methane_ppm = 3.027*exp(1.0698*sensorVolt);

    return MQ9_STATUS_PASS;
}

float MQ9::getLPG_ppm()
{
    return _LPG_ppm;
}

float MQ9::getCO_ppm()
{
    return _CO_ppm;
}

float MQ9::getMethane_ppm()
{
    return _Methane_ppm;
}