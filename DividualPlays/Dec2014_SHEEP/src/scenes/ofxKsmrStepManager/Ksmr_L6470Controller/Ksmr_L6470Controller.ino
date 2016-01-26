#include <SPI.h>
#include <MsTimer2.h>

#define PIN_SPI_SCK   13
#define PIN_SPI_MOSI  11
#define PIN_SPI_SS    10
#define PIN_SPI_MISO  9

#define PHASE_IDLE 0
#define PHASE_WAITNUMBYTE 1
#define PHASE_WAITSIGNALS 2

int phase;
int numBytes;
int sentBytes;

void setup(){
  
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, OUTPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(57600);

  digitalWrite(PIN_SPI_SS, HIGH);
  
  phase = 0;
  numBytes = 0;
}

void loop(){
  
  while (Serial.available()){
    unsigned char sig = Serial.read();
    
    if ((phase == PHASE_IDLE) && (sig == 0x02)) phase = PHASE_WAITNUMBYTE;
    else if (phase == PHASE_WAITNUMBYTE){
      numBytes = sig;
      phase = PHASE_WAITSIGNALS;
      L6470_open();
      sentBytes = 0;
    }
    else if (phase == PHASE_WAITSIGNALS){
      L6470_send(sig);
      sentBytes++;
      if (sentBytes == numBytes){
        L6470_close();
        phase = PHASE_IDLE;
      }
    }
 
  }
  
}

void L6470_open(){
  digitalWrite(PIN_SPI_SS, LOW);
}

void L6470_send(unsigned char sig){
  SPI.transfer(sig);
}

void L6470_close(){
  digitalWrite(PIN_SPI_SS, HIGH);
}
