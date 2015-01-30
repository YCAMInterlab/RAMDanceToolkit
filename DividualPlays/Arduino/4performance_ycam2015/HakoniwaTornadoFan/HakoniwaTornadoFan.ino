
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x15 };

IPAddress _ip(192, 168, 20, 71);

const unsigned int _inPort = 8528;

const int PWM_mot = 9;
void setup(){
  pinMode(MIST_PIN,OUTPUT);
  pinMode(PWM_mot,OUTPUT);
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
}

void loop(){
  
   OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/tornado/fan");
     
     if(!message.hasError()) {
 
          analogWrite(PWM_mot, message.getInt(0));
          
     }
   
   }
}
