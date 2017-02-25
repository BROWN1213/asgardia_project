//asgardia v 2//



//Library
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


//Digital pin mapping
//*********** Heimadal **************
const int trigPin1 = 11;
const int echoPin1 = 12;
int motor = 6;
int door_led = 10;

int pinDHT11 = 4; //DHT

int cds = 1;
int led1 = 2;
int led2 = 3;
int led3 = 4;
//***********************************

  // You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ca9e5ac677444263a1993ee5273dd9c8";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "cchamchi1";
char pass[] = "1234asdf";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);


// Objects
Servo myservo; //Heimdal
LiquidCrystal_I2C lcd(0x27,16,2); //poseidon
// set the LCD address to 0x27 for a 16 chars and 2 line display
SimpleDHT11 dht11; //poseidon

//variables
long duration1, distance1; // Heimdal

//*************** Poseidon *****************
void DHT_run()
{
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
  Serial.print("Read DHT11 failed.");
  return;
  }
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, ");
    Serial.print((int)humidity); Serial.println(" %");
    
    // write a temperature
    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.setCursor(2,0);
    lcd.print((int)temperature);
    lcd.setCursor(4,0);
    lcd.print("C");
    
    // write a humidity
    lcd.setCursor(7,0);
    lcd.print("H:");
    lcd.setCursor(9,0);
    lcd.print((int)humidity);
    lcd.setCursor(11,0);
    lcd.print("%");
    }
//**********************************

//*********************************

void setup_blynk()
{
  // Set console baud rate
Serial.begin(115200);
delay(10);
// Set ESP8266 baud rate
EspSerial.begin(ESP8266_BAUD);
delay(10);
}

void setup_Poseidon()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(0,0); //첫줄
  //lcd.print("Hello, world!");
  //lcd.setCursor(0,1);// 둘째줄
  //lcd.print("Asgardia Arduino");
  }

void Poseidon_run()
{
  //lcd.setCursor(0,0); //첫줄
  //lcd.print("Poseidon running");
  DHT_run();
  }

void setup_Horus()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  }

void setup_Heimdal()
{
  myservo.attach(6);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(door_led,OUTPUT);
  }
void gate_open() 
{
  digitalWrite(door_led,HIGH);
  Serial.println("GATE OPEN");
  lcd.setCursor(0,1); //첫줄
  lcd.print("GATE   OPEN");
  delay(1000);

  myservo.write(0);
  delay(10);
  myservo.write(80);
  delay(3000);
  digitalWrite(door_led,LOW);
  }

void gate_close()
{
  digitalWrite(motor, LOW);
  Serial.println("GATE CLOSED");
  lcd.setCursor(0,1); //첫줄
  lcd.print("GATE CLOSED");
  digitalWrite(door_led,LOW);
  }

void Heimdal_run()
{
  long duration1, distance1;
  digitalWrite(trigPin1,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
  if ((distance1 < 5) && (distance1 > 0))
  {
    gate_open();
    }
    else
    {
      gate_close();
      }
      myservo.write(0);
      //delay(1000); // moved to main loop
       }

void Horus_run()
{
  int cdsValue = analogRead(cds);
  Serial.println(cdsValue);
  if (cdsValue >900 ) 
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    Serial.println("LED1 ON");
    }else if (720 < cdsValue < 900)
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      Serial.println("LED12 ON");
      }else if( cdsValue < 720) 
      {
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        Serial.println("LED123 ON");
        }
        delay(2000);
         }



//******************************************

void setup() 
{
  setup_blynk();
  // put your setup code here, to run once:
  // Set console baud rate
  setup_Heimdal();
  setup_Poseidon();
  setup_kim();
  Blynk.begin(auth, wifi, ssid, pass);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Heimdal_run();
  Poseidon_run();
  Horus_run();
  delay(1000);
  Blynk.run();
}

