#include <Blynk.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define LDR 13
#define RED 23
#define GREEN 18
#define YELLOW 19
#define BUZZER 19

#define A 22
#define B 21
#define C 32
#define D 33
#define E 25
#define FLED 26
#define G 27

#define BLYNK_TEMPLATE_ID "TMPL6DaD9Zyv"
#define BLYNK_DEVICE_NAME "Meu Semaforo"
#define BLYNK_AUTH_TOKEN "k2UTxHMwEIXGCRYDcgKqlmkdnP_5lMY5"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#include "BlynkSimpleEsp32.h"

#define RED_STATE 0
#define YELLOW_STATE 1
#define GREEN_STATE 2
bool sete_segmentos[10][7] = { 
{ 1,1,1,1,1,1,0 }, // = Digito 0
{ 0,1,1,0,0,0,0 }, // = Digito 1
{ 1,1,0,1,1,0,1 }, // = Digito 2
{ 1,1,1,1,0,0,1 }, // = Digito 3
{ 0,1,1,0,0,1,1 }, // = Digito 4
{ 1,0,1,1,0,1,1 }, // = Digito 5
{ 1,0,1,1,1,1,1 }, // = Digito 6
{ 1,1,1,0,0,0,0 }, // = Digito 7
{ 1,1,1,1,1,1,1 }, // = Digito 8
{ 1,1,1,1,0,1,1 }, // = Digito 9
};

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "CINGUESTS";
char pass[] = "acessocin";

BlynkTimer timer;

int redTimer = 10;
int yellowTimer = 10;
int greenTimer = 10;

int previous_ldr = 0;
int state = 0;
int count = 0;

void writeDisplay(int number) {
  bool* display = sete_segmentos[number];
  digitalWrite(A, display[0]);
  digitalWrite(B, display[1]);
  digitalWrite(C, display[2]);
  digitalWrite(D, display[3]);
  digitalWrite(E, display[4]);
  digitalWrite(FLED, display[5]);
  digitalWrite(G, display[6]);
}  

void updateSystem() {
  writeDisplay((count > 9) ? 9 : count); 
  switch(state) {
  case RED_STATE:
    digitalWrite(RED, 1);
    digitalWrite(YELLOW, 0);
    digitalWrite(GREEN, 0);
    if(count < redTimer){
      count++;
    } else {
      count = 0;
      state = GREEN_STATE;
    }
    break;
  case YELLOW_STATE:
    digitalWrite(RED, 0);
    digitalWrite(YELLOW, 1);
    digitalWrite(GREEN, 0);
    if(count < yellowTimer){
      count++;
    } else {
      count = 0;
      state = RED_STATE;
    }
    break;
  case GREEN_STATE:
    digitalWrite(RED, 0);
    digitalWrite(YELLOW, 0);
    digitalWrite(GREEN, 1);
    if(count < greenTimer){
      count++;
    } else {
      count = 0;
      state = YELLOW_STATE;
    }
    break;
  default:
    break;
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  pinMode(LDR, INPUT); //LDR
  
  pinMode(RED, OUTPUT); //vermelho
  pinMode(YELLOW, OUTPUT); //amarelo
  pinMode(GREEN, OUTPUT); //verde
  pinMode(BUZZER, OUTPUT); //buzzer
  
  Serial.begin(115200);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(FLED, OUTPUT);
  pinMode(G, OUTPUT);

  delay(100);
  Blynk.begin(auth,ssid,pass);
  timer.setInterval(1000L, updateSystem);
  
  BLYNK_WRITE(V0) {
    redTimer = param.asInt();
  }
  BLYNK_WRITE(V1) {
    yellowTimer = param.asInt();
  }
  BLYNK_WRITE(V2) {
    greenTimer = param.asInt();
  }
}
  
}

void loop() {
  Blynk.run();
  timer.run();
}

void activateSensor() {
  int ldr_read = digitalRead(LDR);
  Serial.println(pir_read);
  if (ldr_read - previous_ldr > 300) {
    state = TRIGGERED_STATE;
  }
  previous_ldr = ldr_read;
  delay(2000);  
}

void reset() {
  digitalWrite(A, 0);
  digitalWrite(B, 0);
  digitalWrite(C, 0);
  digitalWrite(D, 0);
  digitalWrite(E, 0);
  digitalWrite(FLED, 0);
  digitalWrite(G, 0);
  state = INITIAL_STATE;
}
