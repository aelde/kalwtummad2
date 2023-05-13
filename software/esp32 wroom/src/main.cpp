#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "LittleFS.h"
#include <WiFiClientSecure.h>


// setup pin
long int duration;
int distance = 0;

// wheel motor l298n
int wheel_spd = 0;
int enA = 13; // 12 // 33  // 13
int in1 = 12; // 14 // 26  // 12
int in2 = 14; // 13 // 27  // 14
int in3 = 27; // 26 // 14  // 26
int in4 = 26; // 33 // 12  // 33
int enB = 33; // 27 // 13  // 27

// status robot control move forward, backward, left, right, autopilot
int fw = 0, bw = 0, right = 0, left = 0, autopilot = 0;

// potentiometer
int pot = 34;
int pwm = 0;

// Lcd
int scl = 22;
int sda = 21;
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 for PCF8574 and 0x3F for PCF8574A

// ultrasonic
int t = 32;
int e = 35;

int led1 = 19;
int dust = 18; 

// dance mode
int dance = 0;

void lcdsetup(){
  lcd.init();
  lcd.backlight();
}
void lcdrefresh(int agr, int lcdrow){
  float lcdcon = agr/10;
  if(lcdcon < 1){lcd.setCursor(5,lcdrow);}
  else if(lcdcon < 10){lcd.setCursor(6,lcdrow);}
  else if(lcdcon < 100){lcd.setCursor(7,lcdrow);}
  else if(lcdcon < 1000){lcd.setCursor(8,lcdrow);}
  lcd.print("                ");
}
void move(int fw, int bw, int right, int left){
  if(fw == 1){
    Serial.println("move forward");
    analogWrite(enA, wheel_spd);
    analogWrite(enB, wheel_spd);
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    digitalWrite(in3, 1);
    digitalWrite(in4, 0);
  }
  else if(bw == 1){
    Serial.println("move backward");
    analogWrite(enA, wheel_spd);
    analogWrite(enB, wheel_spd);
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    digitalWrite(in3, 0);
    digitalWrite(in4, 1);
  }
  else if(right == 1){
    Serial.println("move right");
    analogWrite(enA, wheel_spd);
    analogWrite(enB, wheel_spd);
    digitalWrite(in1, 0);
    digitalWrite(in2, 0);
    digitalWrite(in3, 1);
    digitalWrite(in4, 0);
  }
  else if(left == 1){
    Serial.println("move left");
    analogWrite(enA, wheel_spd);
    analogWrite(enB, wheel_spd);
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    digitalWrite(in3, 0);
    digitalWrite(in4, 0);
  } else if (dance == 1){
      for (int i = 0; i < 3; i++) {
          digitalWrite(led1, HIGH);
          delay(250);
          digitalWrite(led1, LOW);
          delay(250);
      }
    Serial.println("dance");
    analogWrite(enA, 240);
    analogWrite(enB, 240);
    // spin
    digitalWrite(in1, 1);
      digitalWrite(led1,HIGH);
    digitalWrite(in2, 0);
      digitalWrite(led1,LOW);
    digitalWrite(in3, 0);
      digitalWrite(led1,HIGH);
    digitalWrite(in4, 1);
    delay(4000);

    digitalWrite(led1,LOW);
    // spin
    digitalWrite(in1, 0);
    digitalWrite(led1,HIGH);
    digitalWrite(in2, 1);
    digitalWrite(led1,LOW);
    digitalWrite(in3, 1);
    digitalWrite(led1,HIGH);
    digitalWrite(in4, 0);
    delay(4000);

    digitalWrite(led1,LOW);
    for (int i = 0; i < 3; i++) {
          digitalWrite(led1, HIGH);
          delay(100);
          digitalWrite(led1, LOW);
          delay(100);
      }
    // forward a  bit
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    digitalWrite(in3, 1);
    digitalWrite(in4, 0);
      delay(1000);

    for (int i = 0; i < 5; i++) {
          digitalWrite(led1, HIGH);
          delay(50);
          digitalWrite(led1, LOW);
          delay(50);
      }
      digitalWrite(led1,HIGH);
    // backward a bit
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    digitalWrite(in3, 0);
    digitalWrite(in4, 1);
      delay(1000);
      digitalWrite(led1,LOW);

  }
  else{
    Serial.println("stop");
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}
// **********************************************************************************************************************************************************
/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           ""
#define BLYNK_TEMPLATE_NAME         ""
#define BLYNK_AUTH_TOKEN            ""

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
char ssid[] = "SIAM_WIFI";
char pass[] = "";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  dance = value;

  // Update state
  // Blynk.virtualWrite(V1, value);
}
BLYNK_WRITE(V4)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  wheel_spd = value;
  Blynk.virtualWrite(V1, wheel_spd);
  // Blynk.virtualWrite(V5, pwm);
  // Update state
}
BLYNK_WRITE(V6)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  fw = value;
}
BLYNK_WRITE(V7)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  bw = value;
}
BLYNK_WRITE(V8)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  left = value;
}
BLYNK_WRITE(V9)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  right = value;
}
// This function is called every time the device is connected to the Blynk.Cloud
// BLYNK_CONNECTED()
// {
//   // Change Web Link Button message to "Congratulations!"
//   Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
//   Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
//   Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
// }

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V5, pwm);
  Blynk.virtualWrite(V3, distance);
}

// **********************************************************************************************************************************************************
void setup(){
  Serial.begin(115200);
  Serial.println("setup...");
  // blink setup ***********
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 8080);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  // ***********************

  pinMode(dust, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  pinMode(t, OUTPUT);
  pinMode(e, INPUT);
  pinMode(led1, OUTPUT);

  lcdsetup();
    for (int i = 0; i < 5; i++) {
    digitalWrite(led1, HIGH);
    delay(250);
    digitalWrite(led1, LOW);
    delay(250);
    }

  Serial.println("setup...end");
}
void loop(){
  Serial.println("loop...");
  Blynk.run();
  Serial.println("Blynk.run()...end");
  Serial.println("wheel : "+String(wheel_spd));
  timer.run();

  // analogWrite(enA, wheel_spd);
  // digitalWrite(in1, 1);
  // digitalWrite(in2, 0);

  digitalWrite(t, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(t, HIGH);
  delayMicroseconds(10);
  digitalWrite(t, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(e, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);



  // function to real time update the distance ไม่ให้ตัวเลขทับกัน
  lcdrefresh(distance, 1);
  if(distance < 15){
    digitalWrite(led1, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
  }
  lcd.setCursor(0, 1);
  lcd.print("Dis:");
  lcd.print(distance);

  int p = analogRead(pot);
  pwm = map(p, 0, 4095, 0, 255);
  Serial.print("PWM:");
  Serial.println(pwm);
  lcdrefresh(wheel_spd, 0);
  lcd.setCursor(0, 0);
  lcd.print("SPD:");
  lcd.print(wheel_spd);

  Serial.println("move...");
  move(fw, bw, right, left);
  Serial.println("move...end");
  
  // digitalWrite(led1, HIGH);
  
  analogWrite(dust, pwm);

  // Serial.println("a:" + String((dust)));
  // ledcAttachPin(dust, pwm);
  Serial.println("loop...end"); 
}