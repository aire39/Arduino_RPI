#include <SPI.h>
#include <WiFi.h>

const byte CW =  0;
const byte CCW = 1;
#define MOTOR_A 0
#define MOTOR_B 1

const byte PWMA = 5;
const byte PWMB = 3;
const byte DIRA = 2;
const byte DIRB = 4;

byte m_mode = 1;

char movement = 's';

byte leftWheel = 0;
byte rightWheel = 0;
byte leftWheelDir = CCW;
byte rightWheelDir = CCW;

byte val_speed = 255;

int status = WL_IDLE_STATUS;
boolean clientConnected = false;
WiFiServer server(23);
IPAddress ip(192, 168, 2, 171);

char ssid[]     = "belkin.0f4";
//char ssid[]     = "CSE541";
char password[] = "";

void setup() {
  
  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  digitalWrite(PWMA,LOW);
  digitalWrite(PWMB,LOW);
  digitalWrite(DIRA,LOW);
  digitalWrite(DIRB,LOW);

  printNetworks();
  Serial.println("");
  
  Serial.print("Firmware Version:");
  Serial.print(WiFi.firmwareVersion());
  Serial.println("");
  
  WiFi.config(ip);
  connectClosedNetwork(ssid, password);
  //connectOpenNetwork(ssid);

  server.begin();
}

void loop() {
  
  WiFiClient client = server.available();
  if(client) {
  
    if(!clientConnected) {
      client.flush();
      Serial.println("New Client");
      Serial.println("Hi");
      clientConnected = true;
    }
  
    if(client.available() > 0) {
      char temp = client.read();
      
      if(temp == 'm') {
          m_mode = client.read();
      }
      
      if(m_mode == 2) {
        if(temp == 'a') {
          leftWheel = client.read();
          leftWheelDir = CCW;
        } else if(temp == 'b') {
          rightWheel = client.read();
          rightWheelDir = CCW;      
        } else if(temp == 'c') {
          leftWheel = client.read();
          leftWheelDir = CW;      
        } else if(temp == 'd') {
          rightWheel = client.read();
          rightWheelDir = CW;      
        }
        
        if(leftWheel > 1)
          driveArdumoto(MOTOR_A, leftWheelDir, leftWheel);
        else
          driveArdumoto(MOTOR_A, leftWheelDir, 0);
          
        if(rightWheel > 1)
          driveArdumoto(MOTOR_B, rightWheelDir, rightWheel);
        else
          driveArdumoto(MOTOR_B, rightWheelDir, 0);
      }
      
      if(m_mode == 1) {
        
        if(temp == 'k') {
            val_speed = client.read();
            val_speed += 50;
        }
        
        if( temp == 'f' || temp == 'b' || temp == 'l' || temp == 'r' || temp == 's') {
          movement = temp;
        }
        bot_control(movement, val_speed);
      }
      
    }
  
  }

}

void printNetworks() {

  Serial.println("Scanning Networks");
  byte nNetworks = WiFi.scanNetworks();
  
  for(int network=0; network<nNetworks; network++) {
  
     Serial.print(network);
     Serial.print(")");
     Serial.print(WiFi.SSID(network));
     Serial.print("  Strength:  ");
     Serial.print(WiFi.RSSI(network));
     Serial.print(" dbm  Security: ");
     Serial.println(WiFi.encryptionType(network));
  
  }
}

void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

void Forward(byte val_speed) {
  driveArdumoto(MOTOR_A, CCW, val_speed);
  driveArdumoto(MOTOR_B, CCW, val_speed);
}

void Backward(byte val_speed) {
  driveArdumoto(MOTOR_A, CW, val_speed);
  driveArdumoto(MOTOR_B, CW, val_speed);
}

void turnLeft(byte val_speed) {
  digitalWrite(DIRA,CCW);
  digitalWrite(DIRB,CCW);
  analogWrite(PWMA,val_speed);
  analogWrite(PWMB,val_speed-125);
}

void turnRight(byte val_speed) {
  digitalWrite(DIRA,CCW);
  digitalWrite(DIRB,CCW);
  analogWrite(PWMA,val_speed-125);
  analogWrite(PWMB,val_speed);
}

void Stop() {
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
}

void bot_control(char sig, byte val_speed) {

  if(sig == 'f'){
    Forward(val_speed);
  }else if(sig == 'b'){
    Backward(val_speed);
  }else if(sig == 'r'){
    turnRight(val_speed);
  }else if(sig == 'l'){
    turnLeft(val_speed);
  }else if(sig == 's'){
    Stop();
  }
  
}

void connectOpenNetwork(char ssid[]) {

  Serial.print("Connecting to ");
  Serial.println(ssid);

  status = WiFi.begin(ssid);
  delay(10000);
  
  while(status != WL_CONNECTED);
  
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());
  
}

void connectClosedNetwork(char ssid[], const char password[]) {

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  status = WiFi.begin(ssid, password);
  delay(10000);
  
  while(status != WL_CONNECTED) {
    Serial.println("Trying to connect!");
    delay(10000);
  }
  
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());
  
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

