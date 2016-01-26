
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x03 };

IPAddress _ip(192, 168, 20, 53);

const unsigned int _inPort = 8528;

// モーター制御
const int motorA = 7;
const int motorB = 8;
const int PWM_mot = 9;
void setup(){
  pinMode(motorA,OUTPUT); //信号用ピン
  pinMode(motorB,OUTPUT); //信号用ピン
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
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/struggle");
     
     if(!message.hasError()) {
          //digitalWrite(9, message.getInt(0) ? HIGH:LOW);
          digitalWrite(motorA, message.getInt(0) ? HIGH:LOW);
          digitalWrite(motorB, message.getInt(1) ? HIGH:LOW);
          analogWrite(PWM_mot, message.getInt(2));
     }
   }
}
