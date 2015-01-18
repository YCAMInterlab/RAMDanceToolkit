#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = {0x90, 0xA2, 0xDA, 0x85, 0x28, 0x15 };

IPAddress _ip(192,168,20,71);

const unsigned int _inPort = 8528;
const unsigned int _power_pin = 2;
const unsigned int _power_flag = 0;

void setup() 
{
  pinMode(_power_pin, OUTPUT);
    
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  
  Serial.begin(9600);
}

void loop()
{ 
  OSCBundle bundle;
  int size = _udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(_udp.read());
    }

    OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/tornado/fan");
     
    if(!message.hasError()) {
//      digitalWrite(_power_pin, message.getInt(0) ? HIGH : LOW);
      Serial.println("check");
      Serial.println(message.getInt(0));
      if(message.getInt(0) == 1){
        digitalWrite(_power_pin, HIGH);
      }else{
        digitalWrite(_power_pin, LOW);
      }
    }
  }
}

