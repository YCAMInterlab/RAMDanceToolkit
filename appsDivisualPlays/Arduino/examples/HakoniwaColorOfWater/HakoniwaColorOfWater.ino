#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

//-- Netowkork --
EthernetUDP _udp;
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xB5, 0x8A };

IPAddress _ip(192, 168, 20, 52);

const unsigned int _inPort = 8528;

//-- Color of Water --
const int kNumPins = 3;
const int kPins[kNumPins] = { 6, 7, 8, };

void setup()
{
  for (int i=0; i<kNumPins; i++) {
    pinMode(kPins[i], OUTPUT);
    digitalWrite(kPins[i], LOW);
  }
  
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
}

void loop()
{ 
       OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/colorOfWater");
     
     if(!message.hasError()) {
          digitalWrite(kPins[message.getInt(0)], message.getInt(1) ? HIGH : LOW);
     }
   }
}
