//------------------------------------------------------
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    
#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x90,0xA2,0xDA,0x85, 0x28, 0x15 };
//------------------------------------------------------

//------------------------------------------------------
IPAddress _ip(192, 168, 20, 71); //IPアドレス
const unsigned int _inPort = 8528; //ポート番号
//------------------------------------------------------

//------------------------------------------------------
const int PWM_mot = 6; //motor回転速度
int onoff = 7; //霧 Switch
//------------------------------------------------------

//------------------------------------------------------
int getmillis;
int onTime = 0; //timer ON時間用変数
int offTime = 0; //timer OFF時間用変数

int re1 = 0;
int re2 = 0;
//------------------------------------------------------

//------------------------------------------------------
void setup(){
  pinMode(PWM_mot,OUTPUT);
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  pinMode(1,OUTPUT); //motor用信号用ピン
  pinMode(2,OUTPUT); //motor用信号用ピン
  pinMode(onoff,OUTPUT); //信号用ピン
  pinMode(9,OUTPUT); //信号用ピン
  
  digitalWrite(onoff, LOW);
  digitalWrite(9, LOW);
}
//------------------------------------------------------

//------------------------------------------------------
void loop(){
  
  getmillis = millis();
  

        
   OSCBundle bundle;
   int size = _udp.parsePacket();
 
   if (size > 0) {
     while (size--) {
       bundle.fill(_udp.read());
     }   
     OSCMessage message = bundle.getOSCMessage("/dp/hakoniwa/tornado/fan");
  
     if(!message.hasError()) {
       
       //Fan制御
       //------------------------------------------------------
       digitalWrite(1,HIGH);
       digitalWrite(2,LOW);
       analogWrite(PWM_mot, message.getInt(0));
       //------------------------------------------------------
       
       //---------------------------------------------------
       re1 = message.getInt(1);
       
       if(re1 != re2 && re1 == 1){
         onTime = getmillis;
         turnON();
       }else if(re1 == re2 && re1 == 1){
         play();
       }

       if(re1 != re2 && re1 == 0){
         offTime = getmillis;
         turnOFF();
       }else if(re1 == re2 && re1 == 0){
         play();
       }
       
       re2 = re1;
       //---------------------------------------------------
     }
   }
}
//------------------------------------------------------

//------------------------------------------------------
void turnON(){
    if (onTime > 0) {
      if(getmillis < onTime + 1000){
         digitalWrite(7, HIGH);
         digitalWrite(9, HIGH);
      }else if (getmillis < onTime + 2000){
        digitalWrite(7, LOW);
        digitalWrite(9, LOW);
      }
      else if (getmillis < onTime + 3000){
        digitalWrite(7, HIGH);
        digitalWrite(9, HIGH);
      }else if(getmillis < onTime + 4000){
        digitalWrite(7, LOW);
        digitalWrite(9, LOW);
      }
    }
}
//------------------------------------------------------

void play(){
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
}

//------------------------------------------------------
void turnOFF(){
    if (offTime > 0) {
      if(getmillis < offTime + 1000){
         digitalWrite(7, HIGH);
         digitalWrite(9, HIGH);
      }else if (getmillis < offTime + 2000){
        digitalWrite(7, LOW);
        digitalWrite(9, LOW);
      }
      else if (getmillis < offTime + 3000){
        digitalWrite(7, HIGH);
        digitalWrite(9, HIGH);
      }else if(getmillis < onTime + 4000){
        digitalWrite(7, LOW);
        digitalWrite(9, LOW);
      }
    }
}
//------------------------------------------------------

