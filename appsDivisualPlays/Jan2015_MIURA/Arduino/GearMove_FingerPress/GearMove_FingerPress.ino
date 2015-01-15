#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = {0x90, 0xA2, 0xDA, 0x85, 0x28, 0x00 };
byte signalStock[128];

IPAddress _ip(192,168,20,50);

IPAddress _outIp(192, 168, 20, 4);
const unsigned int _outPort = 12400;

int pressPin = 0;
int initReading;
int reading;
boolean startUp;

int di1Pin = 9;

void setup()
{
  Ethernet.begin(_mac, _ip);
  _udp.begin(_outPort);

  initReading = 0;
  reading = analogRead(pressPin);
  startUp = true;
  pinMode(di1Pin, OUTPUT);

//  Serial.begin(9600);
}

void loop()
{
  calibrate();
  reading = analogRead(pressPin);

  OSCBundle bundle;

  if(reading < initReading*.9)
  {
    digitalWrite(di1Pin, HIGH);
  }
  else
  {
    digitalWrite(di1Pin,LOW);
  }

  bundle.add("/dp/gearMove/fingerpress/").add((int32_t)reading);
  _udp.beginPacket(_outIp, _outPort);
  bundle.send(_udp); // send the bytes to the SLIP stream
  _udp.endPacket(); // mark the end of the OSC Packet
  bundle.empty(); // empty the bundle to free room for a new one

  delay(100);
}

void calibrate()
{
  if(startUp == true){
    initReading = reading;
    startUp = false;
  }
}

