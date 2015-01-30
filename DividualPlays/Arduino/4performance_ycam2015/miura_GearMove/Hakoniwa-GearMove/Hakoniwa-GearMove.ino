#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <MsTimer2.h>

#include <OSCBundle.h>
#include <OSCBoards.h>

#define MOTOR_PHASE_IDLE 0
#define MOTOR_PHASE_WAITNUMBYTE 1
#define MOTOR_PHASE_WAITSIGNALS 2

EthernetUDP _udp;
byte _mac[] = {0x90, 0xA2, 0xDA, 0x85, 0x28, 0x01 };
byte signalStock[128];

IPAddress _ip(192,168,20,51);

const unsigned int _inPort = 8528;
const unsigned int _motor_cs = 8;

int _motor_phase;
int _motor_numBytes;
int _motor_sentBytes;

bool testBlink;

void setup() 
{
  pinMode(_motor_cs, OUTPUT);
  pinMode(9, OUTPUT);
  
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  digitalWrite(_motor_cs, HIGH);
    
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  
  _motor_phase = 0;
  _motor_numBytes = 0;
  _motor_sentBytes = 0;
  
  testBlink = false;
  
  Serial.begin(9600);
}

void loop()
{ 
   OSCBundle bundle;
   OSCMessage* message;
   
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }
     
       message = bundle.getOSCMessage(0);
       int numBytes = message->getInt(0);
     
       if(!message->hasError() && message->match("/dp/hakoniwa/Ksmrmotor")) {
         for (int i = 0;i < numBytes;i++){
           byte s = message->getInt(1 + i) & 0xFF;
           solveMotorSignal(s);
//           Serial.println("sending");
         }
       }
       
   }
}

void solveMotorSignal(byte sig){
  
  if ((_motor_phase == MOTOR_PHASE_IDLE) && (sig == 0x02)) _motor_phase = MOTOR_PHASE_WAITNUMBYTE;
  else if (_motor_phase == MOTOR_PHASE_WAITNUMBYTE){
    _motor_numBytes = sig;
    _motor_phase = MOTOR_PHASE_WAITSIGNALS;
    _motor_sentBytes = 0;
  }
  else if (_motor_phase == MOTOR_PHASE_WAITSIGNALS){
    signalStock[_motor_sentBytes] = sig;
    _motor_sentBytes++;
   
    if (_motor_sentBytes == _motor_numBytes){
      L6470_open();
      for (int i = 0;i < _motor_numBytes;i++){
        L6470_send(signalStock[i]);
      }
      L6470_close();
      _motor_phase = MOTOR_PHASE_IDLE;
    }
  }
}

void L6470_open(){
  digitalWrite(_motor_cs, LOW);
}

void L6470_send(unsigned char sig){
  SPI.transfer(sig);
}

void L6470_close(){
  digitalWrite(_motor_cs, HIGH);
}
