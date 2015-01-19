
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

EthernetUDP _udp;
byte _mac[] = { 0x85, 0x28, 0x0A, 0x00, 0x00, 0x00 };

IPAddress _ip(192, 168, 20, 60);

const unsigned int _inPort = 8528;

const int kNumPins = 4;
const int kPin[kNumPins] = { 2, 3, 4, 5, 6, 7, 8, 9 /* LED */ };

char *numToOSCAddress(int pin);
void routeOsc(OSCMessage &msg, int addrOffset);

void setup() 
{
  for (int i=0; i<kNumPins; i++) {
    pinMode(kPin[i], OUTPUT);
  }  

  delay(1000);
  for (int i=0; i<kNumPins; i++) {
    digitalWrite(kPin[i], LOW);
  }

  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
}

void loop()
{ 
  OSCBundle bundle;
  int size = _udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(_udp.read());
    }

    if(!bundle.hasError()) {
      bundle.route("/dp/hakoniwa/colorOfWater", routeOsc);
    }
  }
}

char *numToOSCAddress(int pin)
{
  static char s[10];
  int i = 9;

  s[i--] = '\0';
  do {
    s[i] = "0123456789"[pin % 10];
    --i;
    pin /= 10;
  }
  while(pin && i);
  s[i] = '/';
  return &s[i];
}

void routeOsc(OSCMessage &msg, int addrOffset)
{
  for (int i=0; i<kNumPins; i++) {
    int pinMatched = msg.match(numToOSCAddress(kPin[i]), addrOffset);
    if (pinMatched) {
      digitalWrite(kPin[i], msg.getInt(0) ? HIGH : LOW);
    }
  }
}
