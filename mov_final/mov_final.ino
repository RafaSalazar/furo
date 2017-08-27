#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 8); //RX pin = 11, TX pin = 8

//definition of variables
#define DEBUG true 

int L1=4;//pin 1 for direction left control
bool valL1=false;//L1 value
int L2=5;//pin 2 for direction left control
bool valL2=false;//L2 value
int R1=6;//pin 1 for direction rigth control
bool valR1=false;//R1 value
int R2=7;//pin 2 for direction rigth control
bool valR2=false;//R2 value

int LVel=10;//pin for speed control of left motor
int RVel=9;//pin for speed control of rigth motor
int SpeedR=100;//value of LVel
int SpeedL=100;//Value of RVel

const int L=30;//length of furo in milimeters

String msg;//String obtained for ESP
char command[4];//String decodified for msg
int parameter1=100;
int parameter2=100;
bool newData=false; //info provide from wifi flag

double tMicrosR=0;
double tAntR=0;
double tMicrosL=0;
double tAntL=0;

double millisant=0;

double ticL=0;
double ticR=0;

double temp;

void setup() {
  pinMode (L1,OUTPUT);
  pinMode (L2,OUTPUT);
  pinMode (R1,OUTPUT);
  pinMode (R2,OUTPUT);
  pinMode (LVel,OUTPUT);
  pinMode (RVel,OUTPUT); 
  setupPWM10_Bits ( ) ; 
  
  //start communication
  Serial.begin(57600);
  esp8266.begin(115200);
  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CIOBAUD=57600\r\n", 2000, DEBUG);
  esp8266.begin(19200);
  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CIOBAUD=57600\r\n", 2000, DEBUG);
  esp8266.begin(57600);
  //sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CWMODE=3\r\n", 1000, DEBUG); //set station mode
  sendData("AT+CWJAP=\"RAFA\",\"lulolila\"\r\n", 2000, DEBUG); //connect wi-fi network
  delay(10000); 
  sendData("AT+CIFSR\r\n", 1000, DEBUG); //show IP address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); //allow multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // start web server on port 80
  sendData("AT+CIPSTO=1\r\n", 2000, DEBUG);
  Serial.println("Server Ready");

  attachInterrupt( 0, intR, CHANGE);
  attachInterrupt( 1, intL, CHANGE);
}

void loop() {
  //obtain command 
  if (esp8266.available())  //verify incoming data
  {
    newData=true;
    if (esp8266.find("+IPD,")) //if there is a message
    {      
      esp8266.find("?"); //look for the message
      msg = esp8266.readStringUntil(' '); //read whole message don't find space
      //int a1 = msg.indexOf("?@?"); //take the main mensage
      msg.substring(0,2).toCharArray(command,3);
      Serial.println(msg.substring(0,2));
      parameter1=command[0];
      parameter2=command[1];
      }    
  }     
  if(newData==true){
      straight((int)191,(int)191);
      newData=false;
  }
  if (millis()>=millisant+100){
    Serial.print(millis());
    Serial.print(" , ");
    Serial.print(velocityL());
    Serial.print(" , ");
    Serial.println(velocityR());
    millisant=millis();
  }
}

void straight(int velL, int velR){ //Movment
  velL-=100;
  velR-=100;
    if(velL>=0){
    valL1=true;
    valL2=false;
    SpeedL=velL*10;
    }
    else{
    valL1=false;
    valL2=true;
    SpeedL=velL*-10;
    }
    if(velR>=0){
    valR1=true;
    valR2=false;
    SpeedR=velR*10;
    }
    else{
    valR1=false;
    valR2=true;
    SpeedR=velR*-10; 
    }    
     
    Serial.println("###################### info #################### ");
    Serial.print("Parametro 1 y 2 :");
    Serial.print(velL);
    Serial.print(" , ");
    Serial.println(velR);
    Serial.print("rueda L ");
    Serial.print(SpeedL);
    Serial.print(" con direccion: ");
    Serial.print(valL1);
    Serial.print("-");
    Serial.println(valL2);
    Serial.print("rueda R ");
    Serial.print(SpeedR);
    Serial.print("  con direccion: ");
    Serial.print(valR1);
    Serial.print("-");
    Serial.println(valR2); 
    
  digitalWrite(L1, valL1);
  digitalWrite(L2, valL2);
  digitalWrite(R1, valR1);
  digitalWrite(R2, valR2);
  analogWrite10_Bits(LVel,(SpeedL)); 
  analogWrite10_Bits(RVel,(SpeedR));
}

String sendData(String command, const int timeout, boolean debug)//configuration of ESP
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

void setupPWM10_Bits( ) {//Establishment of pwm 10 bits ports
  DDRB |= _BV(PB1) | _BV(PB2 ) ;
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)
  | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12)
  | _BV(CS10);
  ICR1 = 0x03ff;
}

void analogWrite10_Bits ( uint8_t pin , uint16_t val )//Establishment of pwm 10 bits value
{
  switch ( pin ) {
  case 9: OCR1A = val ; break ;
  case 10: OCR1B = val ; break ;
  }
}

void intR(){
  ticR++;
  //Serial.print(ticR);
  //Serial.println("pepita");
}
void intL(){
  ticL++;
  //Serial.print(ticL);
  //Serial.println("juanito");
}
double velocityR(){
  tMicrosR=micros();
  temp=((562869*ticR)/(tMicrosR-tAntR));
  tAntR=tMicrosR;
  ticR=0;  
  return(temp);
}
double velocityL(){
  tMicrosL=micros();
  temp=((562869*ticL)/(tMicrosL-tAntL));
  tAntL=tMicrosL;
  ticL=0;
  return(temp);
}
