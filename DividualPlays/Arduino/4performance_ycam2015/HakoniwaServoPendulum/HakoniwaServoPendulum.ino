
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    
#include <Servo.h> 
#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x11 };

IPAddress _ip(192, 168, 20, 67);

const unsigned int _inPort = 8528;

Servo myServo;

int pos = 0;

const int SERVO_PIN = 9;
void setup(){
 myServo.attach(SERVO_PIN);
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  
  Serial.begin(9800);
}

void loop(){
  
 // Serial.println(pos);
  
   OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/servoPendulum");
     
     if(!message.hasError()) {
          
          pos = message.getInt(0);
          
          if(pos <= 0)pos = 0;
          if(pos >= 360)pos = 360;
      
          myServo.write(pos);
          
          Serial.println(pos);
     }
   }
}
