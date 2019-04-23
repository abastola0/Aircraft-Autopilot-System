//------------------------------read pwm--------------------------------------
//volatile int pwm_value = 0;
//volatile int prev_time = 0;
//int reciever_pin = 4;
//int relay_pin = 16; 
//
//void rising() {
//  attachInterrupt(reciever_pin, falling, FALLING);
//  prev_time = micros();
//}
//void falling() {
//  attachInterrupt(reciever_pin, rising, RISING);
//  pwm_value = micros()-prev_time;
//  
//}
//void setup() {
//  pinMode(relay_pin, OUTPUT);
//    Serial.begin(115200);
//  attachInterrupt(reciever_pin, rising, RISING);
//}
//
//void loop() {
//  Serial.println(pwm_value);
//  if (pwm_value < 1450 && pwm_value > 10 ) // turn on autopilot mode
//  {
//     digitalWrite(relay_pin, HIGH);
//    
//    }
//  else if (pwm_value > 1450){
//     digitalWrite(relay_pin, LOW);
//    }
//  
//}

// -----------------------------------SDCARD----------------------------

///*
//* Connect the SD card to the following pins:
//*
//* SD Card | ESP32
//* D2 -
//* D3 SS
//* CMD MOSI
//* VSS GND
//* VDD 3.3V
//* CLK SCK
//* VSS GND
//* D0 MISO
//* D1 -
//*/
//#include "FS.h"
//#include "SD.h"
//#include "SPI.h"
// 
//void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
//Serial.printf("Listing directory: %s\n", dirname);
// 
//File root = fs.open(dirname);
//if(!root){
//Serial.println("Failed to open directory");
//return;
//}
//if(!root.isDirectory()){
//Serial.println("Not a directory");
//return;
//}
// 
//File file = root.openNextFile();
//while(file){
//if(file.isDirectory()){
//Serial.print(" DIR : ");
//Serial.println(file.name());
//if(levels){
//listDir(fs, file.name(), levels -1);
//}
//} else {
//Serial.print(" FILE: ");
//Serial.print(file.name());
//Serial.print(" SIZE: ");
//Serial.println(file.size());
//}
//file = root.openNextFile();
//}
//}
// 
//void createDir(fs::FS &fs, const char * path){
//Serial.printf("Creating Dir: %s\n", path);
//if(fs.mkdir(path)){
//Serial.println("Dir created");
//} else {
//Serial.println("mkdir failed");
//}
//}
// 
//void removeDir(fs::FS &fs, const char * path){
//Serial.printf("Removing Dir: %s\n", path);
//if(fs.rmdir(path)){
//Serial.println("Dir removed");
//} else {
//Serial.println("rmdir failed");
//}
//}
// 
//void readFile(fs::FS &fs, const char * path){
//Serial.printf("Reading file: %s\n", path);
// 
//File file = fs.open(path);
//if(!file){
//Serial.println("Failed to open file for reading");
//return;
//}
// 
//Serial.print("Read from file: ");
//while(file.available()){
//Serial.write(file.read());
//}
//}
// 
//void writeFile(fs::FS &fs, const char * path, const char * message){
//Serial.printf("Writing file: %s\n", path);
// 
//File file = fs.open(path, FILE_WRITE);
//if(!file){
//Serial.println("Failed to open file for writing");
//return;
//}
//if(file.print(message)){
//Serial.println("File written");
//} else {
//Serial.println("Write failed");
//}
//}
// 
//void appendFile(fs::FS &fs, const char * path, const char * message){
//Serial.printf("Appending to file: %s\n", path);
// 
//File file = fs.open(path, FILE_APPEND);
//if(!file){
//Serial.println("Failed to open file for appending");
//return;
//}
//if(file.print(message)){
//Serial.println("Message appended");
//} else {
//Serial.println("Append failed");
//}
//}
// 
//void renameFile(fs::FS &fs, const char * path1, const char * path2){
//Serial.printf("Renaming file %s to %s\n", path1, path2);
//if (fs.rename(path1, path2)) {
//Serial.println("File renamed");
//} else {
//Serial.println("Rename failed");
//}
//}
// 
//void deleteFile(fs::FS &fs, const char * path){
//Serial.printf("Deleting file: %s\n", path);
//if(fs.remove(path)){
//Serial.println("File deleted");
//} else {
//Serial.println("Delete failed");
//}
//}
// 
//void testFileIO(fs::FS &fs, const char * path){
//File file = fs.open(path);
//static uint8_t buf[512];
//size_t len = 0;
//uint32_t start = millis();
//uint32_t end = start;
//if(file){
//len = file.size();
//size_t flen = len;
//start = millis();
//while(len){
//size_t toRead = len;
//if(toRead > 512){
//toRead = 512;
//}
//file.read(buf, toRead);
//len -= toRead;
//}
//end = millis() - start;
//Serial.printf("%u bytes read for %u ms\n", flen, end);
//file.close();
//} else {
//Serial.println("Failed to open file for reading");
//}
// 
//file = fs.open(path, FILE_WRITE);
//if(!file){
//Serial.println("Failed to open file for writing");
//return;
//}
// 
//size_t i;
//start = millis();
//for(i=0; i<2048; i++){
//file.write(buf, 512);
//}
//end = millis() - start;
//Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
//file.close();
//}
// 
//void setup(){
//Serial.begin(115200);
//if(!SD.begin()){
//Serial.println("Card Mount Failed");
//return;
//}
//uint8_t cardType = SD.cardType();
// 
//if(cardType == CARD_NONE){
//Serial.println("No SD card attached");
//return;
//}
// 
//Serial.print("SD Card Type: ");
//if(cardType == CARD_MMC){
//Serial.println("MMC");
//} else if(cardType == CARD_SD){
//Serial.println("SDSC");
//} else if(cardType == CARD_SDHC){
//Serial.println("SDHC");
//} else {
//Serial.println("UNKNOWN");
//}
// 
//uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//Serial.printf("SD Card Size: %lluMB\n", cardSize);
//
////basic file operations that can be added to a loop
//
//listDir(SD, "/", 0);
//createDir(SD, "/mydir");
//listDir(SD, "/", 0);
//removeDir(SD, "/mydir");
//listDir(SD, "/", 2);
//writeFile(SD, "/hello.txt", "Hello ");
//appendFile(SD, "/hello.txt", "World!\n");
//readFile(SD, "/hello.txt");
//deleteFile(SD, "/foo.txt");
//renameFile(SD, "/hello.txt", "/foo.txt");
//readFile(SD, "/foo.txt");
//testFileIO(SD, "/test.txt");
//}
// 
//void loop(){
// 
//}



//----------------------------Gyro------------------------------------
#include <Arduino.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#define MY_PIN 34 // we could choose any pin

const int servo1=35;
const int servo2=32;
const int intr_gyro = 17;
int variation = 2;
 

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

//Servo myservoY; // Roll
//Servo myservoX; // Pitch

MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

#define OUTPUT_READABLE_YAWPITCHROLL

bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  Serial.begin(115200);  
//    pinMode(servo1, OUTPUT);
//    pinMode(servo2, OUTPUT);
    
//     pinMode(MY_PIN, INPUT); 
//     digitalWrite(MY_PIN, HIGH);
  
    
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
//        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

//Attach servo

//  myservoY.attach(33); // Attach Y servo to pin 33
//  myservoX.attach(25);// Attach X servo to pin 25

//  myservoY.write(0);
//  myservoX.write(0);
  

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
   
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
   
    mpu.initialize();

    // verify connection
   
    
/*
    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
*/
    // load and configure the DMP
   
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
    
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        
        attachInterrupt(intr_gyro, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
    
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        
      
    }

    // configure LED for output
  
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
       
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
          
        Serial.println(ypr[1] );
      Serial.println(ypr[2] );
      Serial.print("\n");
         
//            myservoY.write(int( -0.03*variation*ypr[1] * 180/M_PI)+90);   // Rotation around Y and changethe value variation multiplying pitch to change the sensitivity
    
//            myservoX.write(int( 0.03*variation*ypr[2] * -180/M_PI)+90);   // Rotation around X and changethe value variation multiplying roll to change the sensitivity
   
        #endif


       
       
    }
    
}



// -------------------------------servo control -------------------------------------------------

//const int freq = 50;
//const int ch_x = 0;
//const int ch_y = 1;
//const int resolution = 10;
//const int servo_x = 0;
//const int servo_y = 15;
//
//int servo(int ch,int duty){
//    ledcWrite(ch, duty);
//}
//
//void setup(){
//  Serial.begin(115200);
//  // configure Servo PWM functionalitites
//  ledcSetup(ch_x, freq, resolution);
//  ledcSetup(ch_y, freq, resolution);
//  
//  // attach the channel to the GPIO to be controlled
//  ledcAttachPin(servo_x, ch_x);
//  ledcAttachPin(servo_y, ch_y);
//}
//
//void loop(){
//  
// for(int dutyCycle = 25; dutyCycle <= 130; dutyCycle++){   
//    // changing the LED brightness with PWM
//    servo(ch_x, dutyCycle);
//    delay(5);
//  }
//
//  // decrease the LED brightness
//  for(int dutyCycle = 130; dutyCycle >= 25; dutyCycle--){
//    // changing the LED brightness with PWM
//    ledcWrite(ch_x, dutyCycle);   
//    delay(5);
//  }
//
//}
// 


 
