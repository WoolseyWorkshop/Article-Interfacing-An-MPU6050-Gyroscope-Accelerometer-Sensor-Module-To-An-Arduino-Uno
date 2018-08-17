// MPU6050 - MPU6050.ino
//
// Description:
// Retrieves motion data from an MPU6050 sensor module using the
// I2Cdevlib (top level library) and MPU6050 (sub-library) combination.
//
// Created by John Woolsey on 07/21/2018.
// Copyright © 2018 Woolsey Workshop.  All rights reserved.


#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>


// MPU6050 library constructor for module
// Uses default I2C address of 0x68; use mpu6050(0x69) to set alternate address
// if needed.
MPU6050 mpu6050;


int16_t ax, ay, az;  // raw accelerometer data register values
int16_t gx, gy, gz;  // raw gyroscope data register values
int16_t tr;          // raw temperature data register value
char buffer[7];      // temporary string buffer; used with dtostrf() function


void setup() {
   Wire.begin();          // initialize I2C bus (MPU6050 module)
   Serial.begin(9600);    // initialize serial bus (Serial Monitor)
   mpu6050.initialize();  // initialize MPU6050 sensor module

   // Verify module connection
   Serial.print("MPU6050 module connection ");
   Serial.println(mpu6050.testConnection() ? "successful." : "failed.");

   // Optionally update accelerometer and gyroscope data register offsets
   // Offsets are not retained when module is disconnected from power.
   // Use offset values obtained from running IMU_Zero calibration sketch.
   // setMPU6050Offsets(-906, -83, 773, 97, 34, 20);
   
   Serial.println();  // blank line
}


void loop() {
   // Read raw accel/gyro/temp sensor readings from module
   mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   tr = mpu6050.getTemperature();

   printMPU6050Values();  // print sensor readings in human readable format
   delay(1000);           // wait one second between readings
}


// Prints current MPU6050 values
void printMPU6050Values() {
   Serial.print("aX = ");  Serial.print(dtostrf(ax/16384.0, 4, 1, buffer));  Serial.print(" g, ");
   Serial.print("aY = ");  Serial.print(dtostrf(ay/16384.0, 4, 1, buffer));  Serial.print(" g, ");
   Serial.print("aZ = ");  Serial.print(dtostrf(az/16384.0, 4, 1, buffer));  Serial.print(" g, ");
   Serial.print("gX = ");  Serial.print(dtostrf(gx/131.0, 6, 1, buffer));  Serial.print(" °/s, ");
   Serial.print("gY = ");  Serial.print(dtostrf(gy/131.0, 6, 1, buffer));  Serial.print(" °/s, ");
   Serial.print("gZ = ");  Serial.print(dtostrf(gz/131.0, 6, 1, buffer));  Serial.print(" °/s, ");
   Serial.print("T = ");  Serial.print(dtostrf(tr/340.0+36.53, 5, 1, buffer));  Serial.println(" °C");
}


// Prints current MPU6050 offsets
void printMPU6050Offsets() {
    Serial.print("MPU6050 offsets: ");
    Serial.print("aX = ");  Serial.print(dtostrf(mpu6050.getXAccelOffset(), 5, 0, buffer));
    Serial.print(", aY = ");  Serial.print(dtostrf(mpu6050.getYAccelOffset(), 5, 0, buffer));
    Serial.print(", aZ = ");  Serial.print(dtostrf(mpu6050.getZAccelOffset(), 5, 0, buffer));
    Serial.print(", gX = ");  Serial.print(dtostrf(mpu6050.getXGyroOffset(), 5, 0, buffer));
    Serial.print(", gY = ");  Serial.print(dtostrf(mpu6050.getYGyroOffset(), 5, 0, buffer));
    Serial.print(", gZ = ");  Serial.println(dtostrf(mpu6050.getZGyroOffset(), 5, 0, buffer));
}


// Sets new MPU6050 offsets
void setMPU6050Offsets(int16_t aX, int16_t aY, int16_t aZ, int16_t gX, int16_t gY, int16_t gZ) {
   Serial.print("Old ");  printMPU6050Offsets();
   mpu6050.setXAccelOffset(aX);
   mpu6050.setYAccelOffset(aY);
   mpu6050.setZAccelOffset(aZ);
   mpu6050.setXGyroOffset(gX);
   mpu6050.setYGyroOffset(gY);
   mpu6050.setZGyroOffset(gZ);
   Serial.print("New ");  printMPU6050Offsets();
}
