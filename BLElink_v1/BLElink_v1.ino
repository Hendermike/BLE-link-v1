#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 9); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
//---------------------
// Const. and variables
//---------------------
const String DEF = "DEFAULT";
String msg_rcv;
int suggestion = 0;
char c;
//---------------------
// Functions
//---------------------
// Sets suggested acc. (setSuggestion) and retrieve relevant data (retrieveData)
void dataAndSuggestionExchange(String msg) {
  if (msg != DEF) { 
  setSuggestion(msg);
  }
  retrieveData();
  Serial.println(msg);
  msg_rcv = DEF;
  }
// Sets suggested acc.
void setSuggestion(String msg) {
  if (msg == "+") {suggestion = 1;}
  if (msg == "0") {suggestion = 0;}
  if (msg == "-") {suggestion = -1;}
  }
// Retrieves formated data throug BLE link.
void retrieveData(){
  String msg2Send = "{V:" + getSpeedString()+",A:" + getSuggestion() + "}";
  
  char __dataFile[msg2Send.length() + 1];
  msg2Send.toCharArray(__dataFile, sizeof(__dataFile));

  BTserial.write(__dataFile);
  BTserial.write('\n');
  }
// Converts current speed value into formated String
String getSpeedString() {
  float spd = getSpeed();
  char str[5];
  
  dtostrf(spd, 4, 2, str );
  String speedString = String(str);
  
  return speedString;
  }
// Gets current acc. suggestion  
String getSuggestion(){
  String  suggestionString;
  if (suggestion == 1) {suggestionString = "+";}
  if (suggestion == 0) {suggestionString = "0";}
  if (suggestion == -1) {suggestionString = "-";}
  return suggestionString;
  }
// Gets current speed value.
float getSpeed() {
  float spd = 1.463;
  return spd;
  }
  
//---------------------
// Setup & Loop
//---------------------

void setup()
{
 Serial.begin(9600);
 Serial.println("Arduino is ready");
 Serial.println("Remember to select Both NL & CR in the serial monitor");
 // HC-05 default serial speed for AT mode is 38400
 BTserial.begin(9600);
}

void loop()
{
 // Keep reading from HC-05 and send to Arduino Serial Monitor
 while(BTserial.available()){
    c = BTserial.read();
    if (char(c) == '{'){
      c = BTserial.read();
      msg_rcv = String(c);
    }
 }
 dataAndSuggestionExchange(msg_rcv);
 delay(1000);
}
