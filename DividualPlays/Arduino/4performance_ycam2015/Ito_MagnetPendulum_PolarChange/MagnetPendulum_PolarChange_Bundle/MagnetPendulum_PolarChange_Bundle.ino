#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
//byte _mac[] = { 0x85, 0x28, 0x02, 0x00, 0x00, 0x00 };
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x16 };
IPAddress _ip(192, 168, 20, 72);
const unsigned int _inPort = 8528;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);

}

// the loop function runs over and over again forever
void loop() {
  
   OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
    OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/magpen");
     
     if(!message.hasError()) {
       
//       Serial.println(message.getInt(0));

       for (int i = 0; i < 6; i++) {
         if (message.getInt(i) == 1) {
            digitalWrite(i+2, HIGH);  
         } else {
            digitalWrite(i+2, LOW);
         }
       }
       
     }
   }

  
//  for (int i = 0; i < 6; i++) {
//    digitalWrite(i+2, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(200);              // wait for a second
//    digitalWrite(i+2, LOW);    // turn the LED off by making the voltage LOW
//    delay(200);              // wait for a second
//  }
}
