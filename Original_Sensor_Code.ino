int maxPeople = 5; // maximum number of people allowed before the alarm goes off
int sensitivity = 5; //lower values will make it more sensitive and higher values will make it less sensitive
//---------------------------------------------------


#include <TM1637.h>

int currentPeople = 0;

int buzzer = 8;

TM1637 tm(2,3);

int sensor1[] = {4,5};
int sensor2[] = {6,7};
int sensor1Initial;
int sensor2Initial;

String sequence = "";

int timeoutCounter = 0;

void setup() {
  //Setup code
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  tm.init();
  tm.set(2);

  delay(500);
  sensor1Initial = measureDistance(sensor1);
  sensor2Initial = measureDistance(sensor2);
}

void loop() {
  //Read ultrasonic sensors
  int sensor1Val = measureDistance(sensor1);
  int sensor2Val = measureDistance(sensor2);
  
  //Process the data
  if(sensor1Val < sensor1Initial - 30 && sequence.charAt(0) != '1'){
    sequence += "1";
  }else if(sensor2Val < sensor2Initial - 30 && sequence.charAt(0) != '2'){
    sequence += "2";
  }
  
  if(sequence.equals("12")){
    currentPeople++;  
    sequence="";
    delay(550);
  }else if(sequence.equals("21") && currentPeople > 0){
    currentPeople--;  
    sequence="";
    delay(550);
  }

  //Resets the sequence if it is invalid or timeouts
  if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200){
    sequence="";  
  }

  if(sequence.length() == 1){ //
    timeoutCounter++;
  }else{
    timeoutCounter=0;
  }

  //Print values to serial
  Serial.print("Seq: ");
  Serial.print(sequence);
  Serial.print(" S1: ");
  Serial.print(sensor1Val);
  Serial.print(" S2: ");
  Serial.println(sensor2Val);
  
  //Display current people count on 4-digit display
  tm.display(3, currentPeople % 10);
  int pos2 = currentPeople / 10;
  tm.display(2, pos2 % 10);
  int pos1 = pos2 / 10;
  tm.display(1, pos1 % 10);
  int pos0 = pos1 / 10;
  tm.display(0, pos0 % 10);

  //If the number of people is too high, trigger the buzzer
  if(currentPeople > maxPeople){
    tone(buzzer, 1700);  
  }else{
    noTone(buzzer);  
  }
}

//Returns the distance of the ultrasonic sensor that is passed in
//a[0] = echo, a[1] = trig
int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  digitalWrite(a[1], LOW);
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  pinMode(a[0], INPUT);
  long duration = pulseIn(a[0], HIGH, 100000);
  return duration / 29 / 2;
}
