
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xB5, 0x8A };

IPAddress _ip(192, 168, 20, 51);

const unsigned int _inPort = 8528;

void setup() 
{
  pinMode(9, OUTPUT);
  
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
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/oscTest");
     
     if(!message.hasError()) {
          digitalWrite(9, message.getInt(0) ? HIGH:LOW);
     }
   }
}


