 /*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials:   http://www.blynk.cc
    Sketch generator:             http://examples.blynk.cc
    Blynk community:              http://community.blynk.cc
    Follow us:                    http://www.fb.com/blynkapp
                                  http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 ****************************************************************************

  Blynk using a LED widget on your phone!

  App project setup:
    LED widget on V1
 ****************************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL33xWRplnj"
#define BLYNK_TEMPLATE_NAME "ROAD DIVIDER"
#define BLYNK_AUTH_TOKEN "nWd0mJ2Oz-ulLVJfXK50QV1FdK_9Uah0"

#include <Servo.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <LiquidCrystal.h>    // include the library
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

BlynkTimer timer;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "wifi_name";        // type your wifi name
char pass[] = "password";  // type your wifi password

#define EspSerial Serial
ESP8266 wifi(&EspSerial);

Servo servo1;
#define  DD0  8
#define  Ser  9
#define  IR1  10
#define  IR2  11
#define  Buz  12
#define  Led  13

int pos1   = 0;
int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

int valor;
bool ledStatus = false;
unsigned long t = 0;

bool isFirstConnect = true;

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

//********************************************************************************************
void setup()
 {
  // Debug console
  EspSerial.begin(115200); 
  lcd.begin(16, 2);               // 16 characters, 2 rows
  lcd.clear();  lcd.home ();
  servo1.attach(Ser); servo1.write(pos1);
  pinMode(IR1,INPUT);  pinMode(IR2,INPUT);
  pinMode(Buz,OUTPUT); pinMode(Led,OUTPUT);
  digitalWrite(Led,HIGH);
  
  lcd.setCursor(0, 0);  lcd.print("   Adaptive Urban Lane   ");   
  lcd.setCursor(0, 1);  lcd.print(" Management system.");
  delay(3000);
  lcd.clear();

 // Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(1000L, send_IR12);
  timer.setInterval(2000L, send_Ambu);
 }
//********************************************************************************************
//********************************************************************************************
BLYNK_CONNECTED() 
 {
  if (isFirstConnect) { Blynk.syncAll();  isFirstConnect = false; }    
 }
 
//*******************************************************************************************
//*************************************************************************************
void send_IR12() 
 {
  if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH)  
    { 
     lcd.setCursor(0, 1);  lcd.print("Normal traffic  ");
     if(state1 == 1)  { move_2(); state1 = 0; }
    }
  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == HIGH) 
    { 
     lcd.setCursor(0, 1);  lcd.print("Medium traffic  ");
     if(state1 == 1)  { move_2(); state1 = 0; }
    }
   
  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW && state1 == 0) 
    { 
     lcd.setCursor(0, 1);  lcd.print("Heavy traffic   ");
     delay(2000);
     move_1();
     Blynk.logEvent("Alert", "Heavy traffic at ringroad");
     state1 = 1;
    }
 }
//*************************************************************************************
void send_Ambu() 
 {
  if(digitalRead(DD0) == LOW) 
    { 
     lcd.setCursor(0, 0);  lcd.print("Ambulance detect");
     lcd.setCursor(0, 1);  lcd.print("keep left side..");
     delay(2000);
     if(state2 == 0)
      {
       move_1();
       state2 = 1;
      }
     }
//......................................................................................
  else if(digitalRead(DD0) == HIGH && state2 == 1)
    {
     move_2();
     state2 = 0;
    }
 }
//********************************************************************************************
void move_1()
  {
   digitalWrite(Buz,HIGH); digitalWrite(Led,LOW);
   lcd.setCursor(0, 0);  lcd.print("Road divider is ");
   lcd.setCursor(0, 1);  lcd.print("moving...       ");
   delay(2000);
   for(pos1 = 0; pos1 <= 110; pos1 += 1) { servo1.write(pos1); delay(15); }
   delay(2000); lcd.clear(); digitalWrite(Buz,LOW); digitalWrite(Led,HIGH);
  }
//********************************************************************************************
void move_2()
  {
   digitalWrite(Buz,HIGH); digitalWrite(Led,LOW);
   lcd.setCursor(0, 0);  lcd.print("Road divider is ");
   lcd.setCursor(0, 1);  lcd.print("moving...       ");
   delay(2000);
   for(pos1 = 110; pos1 >= 0; pos1 -= 1) {  servo1.write(pos1); delay(15);  }
   delay(2000); lcd.clear(); digitalWrite(Buz,LOW); digitalWrite(Led,HIGH);
  }
//********************************************************************************************
//********************************************************************************************
void loop()
 {    
  //Blynk.run();  
  timer.run();
 }
  
//********************************************************************************************
//********************************************************************************************




















 
/* Blynk.logEvent("Alert", "Fire Alert, Take action");
  if  (digitalRead(door) == HIGH)
    {
      while ( digitalRead(door) == HIGH )
      { delay(500);}
      Blynk.notify("ALERT...! Door Vibration Detected");

      lcd.setCursor(0, 0);
      lcd.print("Alert...!!!");       // Print Something on LCD
      lcd.setCursor(0, 1);// Clearing LCD
      lcd.print("Door Vibration..");
      delay(3000);
   }
   ************MONOSTABIL push button android*****************
BLYNK_WRITE(V1)
{
  if (param.asInt())
  {
    led1.off();
    //led1.setColor(BLYNK_RED);
    //delay(1000);
    L_CD.print(0, 0, "Door: CLOSE     ");
  }
  else
  {
    led1.on();
    //led1.setColor(BLYNK_GREEN);
    //delay(1000);
    L_CD.print(0, 0, "Door: Open!    ");
  }
}

char auth[] = "fa19b9343bf645b2bc6df7b69b93db16";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Shreedevi";
char pass[] = "Shree@2011";
   */
