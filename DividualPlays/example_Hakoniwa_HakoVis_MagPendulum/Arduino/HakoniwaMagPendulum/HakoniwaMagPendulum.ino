#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;

byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x02 };
IPAddress _ip(192, 168, 20, 52);
const unsigned int _inPort = 8528;
const int NMAGNETS = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
    
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
     
   OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/MagPendulum");

   if(!message.hasError()) {

       for (int i = 0; i < NMAGNETS; i++) {
         if (message.getInt(i) == 1) {
            digitalWrite(i+2, HIGH);
         } else {
            digitalWrite(i+2, LOW);
         }
       }
     }
   }
}
