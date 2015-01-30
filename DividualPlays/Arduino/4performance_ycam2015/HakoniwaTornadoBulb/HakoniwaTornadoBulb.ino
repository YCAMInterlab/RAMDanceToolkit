
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x14 };

IPAddress _ip(192, 168, 20, 70);

const unsigned int _inPort = 8528;

const int MIST_PIN = 2;
const int BULB_PIN = 8;

void setup(){
  pinMode(MIST_PIN,OUTPUT); //信号用ピン
  pinMode(BULB_PIN,OUTPUT); //信号用ピン
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  Serial.begin(9600);
}

void loop(){
  
  /* digitalWrite(MIST_PIN, HIGH);
   delay(1000);
   digitalWrite(MIST_PIN, LOW);
   delay(1000);*/
   
      OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/tornado/bulb");
     
     if(!message.hasError()) {
         
          int flag1 = message.getInt(0);
          int flag2 = message.getInt(1);
          
          Serial.println(flag1);
          Serial.println(flag2);
          digitalWrite(BULB_PIN, flag1 ? HIGH:LOW);
          digitalWrite(MIST_PIN, flag2 ? HIGH:LOW);
          
     }
     
     /*OSCMessage message2 = bundle.getOSCMessage("/dp/hakoniwa/tornado/mist");
     
     if(!message2.hasError()) {
         
          int flag = message2.getInt(0);
          Serial.println(flag);
          digitalWrite(MIST_PIN, flag ? HIGH:LOW);
          
     }*/
    
   }
}
