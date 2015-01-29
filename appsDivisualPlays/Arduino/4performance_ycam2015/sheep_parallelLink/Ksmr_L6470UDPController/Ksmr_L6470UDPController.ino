
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

/*======= Define Hakoniwa ip ======*/
#define HAKONIWA_1

#ifdef HAKONIWA_1
//HAKONIWA 1st
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x85, 0x28, 0x06 };
IPAddress _ip(192,168,20,56);
#endif
#ifdef HAKONIWA_2
//HAKONIWA 2nd
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x85, 0x28, 0x12 };
IPAddress _ip(192,168,20,68);
#endif
#ifdef HAKONIWA_3
//HAKONIWA 3rd
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x85, 0x28, 0x13 };
IPAddress _ip(192,168,20,69);
#endif

byte signalStock[128];
const unsigned int _inPort = 8528;
const unsigned int _motor_cs = 8;

int _motor_phase;
int _motor_numBytes;
int _motor_sentBytes;

bool testBlink;

void setup() 
{

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  

  pinMode(_motor_cs, OUTPUT);
  digitalWrite(_motor_cs, HIGH);
    
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
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
     if (!message->hasError()){
       
       if(message->match("/dp/hakoniwa/Ksmrmotor")) {
         int numBytes = message->getInt(0) >> 24 & 0xFF;
         for (int i = 1;i < (numBytes + 1); i++){
           int32_t pack = message->getInt(i/4);
           byte sg = (pack >> (24 - (i%4) * 8)) & 0xFF;
           solveMotorSignal(sg);
         }
       }
       
       if (message->match("/dp/hakoniwa/digitalWrite")){
         digitalWrite(message->getInt(0),(message->getInt(1) == 1) ? HIGH : LOW);
       }
       if (message->match("/dp/hakoniwa/analogWrite")){
         analogWrite(message->getInt(0),(message->getInt(1)));
         Serial.print("analogWrite");
         Serial.println(message->getInt(0));
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

void L6470_simpleSend(byte sig){
  digitalWrite(_motor_cs, LOW);
  SPI.transfer(sig);
  digitalWrite(_motor_cs, HIGH);
}
