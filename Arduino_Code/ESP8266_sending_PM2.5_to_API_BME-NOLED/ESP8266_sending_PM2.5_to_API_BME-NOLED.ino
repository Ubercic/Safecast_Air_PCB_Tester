#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>



//display


//pm2.5 added
#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];
int PM2_5Value=0;         //define PM2.5 value of the air detector module
//end pm2.5 added

// BME defines
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
String BMEdata;


unsigned long delayTime;

void setup() {
  
  // Set up serial console to read web page
  Serial.begin(9600);
  delay(100);
  
  Serial.print("Safecast Air Mini");
  Serial.println(F("BME280 test"));
  Serial.setTimeout(1500);
 


  bool status;

    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        
        while (1);
        
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

  


}
 
void loop() {

  readPMS();
  Serial.print("PM2_5Value=");
  Serial.println(PM2_5Value);


  float bme_temp= bme.readTemperature();
  float bme_pres= bme.readPressure() / 100.0F;
  float bme_hum = bme.readHumidity();


// serial BME print
  Serial.print("Temperature = ");
  Serial.print(bme_temp);
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bme_pres);
  Serial.println(" hPa");  
  Serial.print("Humidity = ");
  Serial.print(bme_hum);
  Serial.println(" %");
  Serial.println();
    
  BMEdata = String("T:"+String(bme_temp)+", P:"+String(bme_pres)+", H:"+String(bme_hum));
  //display data





  // If the server has disconnected, stop the client and WiFi

 delay(5000);
}

void readPMS(){
  //PMS sensor
  if(Serial.find(0x42)){    //start to read when detect 0x42
    Serial.readBytes(buf,LENG);
    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
      }           
    } 
  }
}
char checkValue(unsigned char *thebuf, char leng)
{  
  char receiveflag=0;
  int receiveSum=0;
 
  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;
  
  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}
 
//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }
 
