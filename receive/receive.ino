#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "E:\Dropbox\Documents\Arduino\telemetry\types.h"
#define CE_PIN   9
#define CSN_PIN 53

RF24 radio(CE_PIN, CSN_PIN); 
float current, voltage;
char crc_local;
packet_t packet;

void setup()   
{
  Serial.begin(115200);
  delay(1000);
  radio.begin();
  //radio.setChannel(CHANNEL);
  //radio.setCRCLength(RF24_CRC_DISABLED);
  radio.disableCRC();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  pinMode(13, OUTPUT);
}

int error_count = 0;
float power = 0;
float power_total = 0;
unsigned long timestamp = 0;
byte flag = 0;

void loop()  
{
  while ( !radio.available() ) {digitalWrite(13, LOW);}
  radio.read( &packet.str, sizeof(packet) );

  crc_local = 0;
  for(unsigned char i=0; i<sizeof(packet)-1; i++)
    crc_local = crc_local^packet.b[i];

  current = (packet.str.current - 124);
  current = ((current > 2)||(current < -2))?current/11.33:0;
  voltage = packet.str.voltage / 50.76;

  /*power = (current + 0.0019) * voltage;
  if (flag) power_total = power_total + (power * (packet.str.timestamp - timestamp)) /1000 /60 /60;
  else flag = 1;
  timestamp = packet.str.timestamp;
  Serial.print("\t");
  Serial.print(power, 2);
  Serial.print("\t");
  Serial.print(power_total, 4);
  Serial.print("\t");//*/

  Serial.print("Timestamp:");
  Serial.print(packet.str.timestamp);
  Serial.print("\tCurrent:");
  Serial.print(current, 2);
  Serial.print("\tVoltage:");
  Serial.print(voltage, 2);
  /*Serial.print("  lat: ");
  Serial.print(packet.str.lat);
  Serial.print("  lon: ");
  Serial.print(packet.str.lon);
  Serial.print("  alt: ");
  Serial.print(packet.str.alt);
  Serial.print("  fire_x: ");
  Serial.print(packet.str.fire_x);
  Serial.print("  fire_y: ");
  Serial.print(packet.str.fire_y);//*/
  //packet.str.timestamp = 0xFFFFFFFF;
  //packet.str.current = 0xFFFF;
  //packet.str.voltage = 0xFFFF;
  char buf[2];
  Serial.print("\tpayload:0x");
  for(char i=8; i < sizeof(packet)-1;i++) {
    sprintf(buf, "%.2X ", (unsigned char)packet.b[i]);
    //Serial.print(packet.b[i]);
    Serial.print(buf);
  }//*/
  Serial.print("\tcrc:");
  Serial.print(packet.str.crc == crc_local?"OK":"ERR");
  Serial.print("\terr:");
  Serial.print(error_count);
  //Serial.print("\tsignal quality: "); // Test whether a signal (carrier or otherwise) greater than or equal to -64dBm is present on the channel.
  //Serial.print(radio.testRPD()?"GOOD":"BAD");
  Serial.println();
  if(packet.str.crc == crc_local) digitalWrite(13, HIGH);
  else {digitalWrite(13, LOW); error_count++;}
}
