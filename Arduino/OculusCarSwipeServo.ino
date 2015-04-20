//read in speeds and directions for all 4 motors, angle for servo via serial
//      front of robot
//left      1      3    right
//          2      4
 
 
//motor 1: pin 2 enable, pin 4 phase
//motor 2: pin 3 enable, pin 7 phase
//motor 3: pin 5 enable, pin 6 phase
//motor 4: pin 9 enable, pin 8 phase
 
#include <Servo.h>
 
//initialize variables
Servo servo;
int reading=0;           //we are initially not reading for our values
int index=0;             //index of our data array
int data[]={0,9,0,'F',0,0,0,'F',0,0,0};             //place for data to go, start character and 3 values
int olddata[]={0,9,0,'F',0,0,0,'F',0,0,0};
int pos=90;
int incomingByte=0;      //current thing coming in from serial
int leftspeed=0;
int rightspeed=0;
int enables[]={2,3,6,9};  //these are analog (pwm)
int phases[]={4,7,5,8};   //these are digital
int oldpos;
 
void setup(){
  servo.attach(11);
  oldpos=servo.read();
  if (pos>oldpos){
         int i=oldpos;
         for (i=oldpos;i<pos;i++){
           servo.write(i);
           delay(20);
         }
       }
       
        if (pos<oldpos){
         int j=oldpos;
         for (j=oldpos;j>pos;j--){
           servo.write(j);
           delay(20);
         }
       }
  Serial.begin(9600);
 
  for (int currpin=0; currpin<4;currpin++){
    pinMode(enables[currpin], OUTPUT);
    pinMode(phases[currpin],OUTPUT);
   
    analogWrite(enables[currpin],0); //initialize robot as braked
  }
 
}
 
void loop()
{
 
 if (Serial.available()>0){     //if there's data
   incomingByte=Serial.read();  //read first byte
   if (reading==1){             //reading data that corresponds to a state
     if (index==3 || index==7){  //this is a direction, so not a number
       data[index]=incomingByte;
       index=index+1;
     }
     else{
       data[index]=incomingByte-48;
       index=index+1;
     }
     
     if (index>10){
       //we've received everything, start actuating
       oldpos=pos;
       pos=data[2]+(10*data[1])+(100*data[0]);
       leftspeed=data[6]+(10*data[5])+(100*data[4]);
       rightspeed=data[10]+(10*data[9])+(100*data[8]);
       //Serial.println(leftspeed);
      // Serial.println(rightspeed);
       if (pos==180){
         int i=90;
         for (i=90;i<pos;i++){
           servo.write(i);
           delay(20);
         }
         int j=180;
         for (j=180;j>90;j--){
           servo.write(j);
           delay(20);
         }}
         
       if (pos==0){
           int j=90;
           for (j=90;j>0;j--){
           servo.write(j);
           delay(20);
         }
          int i=0;
          for (i=0;i<90;i++){
           servo.write(i);
           delay(20);
         }
         }
      
       if ((data[3]!=olddata[3]) || (data[7]!=olddata[7])){ //if we're changing directions
         digitalWrite(4,LOW);
         digitalWrite(7,LOW);
         digitalWrite(5,LOW);
         digitalWrite(8,LOW);
         analogWrite(2,0);
         analogWrite(3,0);
         analogWrite(6,0);
         analogWrite(7,0);
         delay(1000);
         //Serial.println("Dir changed");
       }
 
       if (data[3]=='F'){   //left side forward
         digitalWrite(4,LOW);
         //Serial.println("Forward!!");
         digitalWrite(7,LOW);
       }
       else if (data[3]=='B'){           //left backward
         digitalWrite(4,HIGH);
         digitalWrite(7,HIGH);
       }
       
       if (data[7]=='F'){  //right side forward
         digitalWrite(5,LOW);
         digitalWrite(8,LOW);
       }
       else if (data[7]=='B'){             //right side backwards
         digitalWrite(5,HIGH);
         digitalWrite(8,HIGH);
         //Serial.println("hallo");
       }
       
       analogWrite(2,leftspeed);
       //Serial.println(leftspeed);
       //Serial.println("hi");
       analogWrite(3,leftspeed);
       analogWrite(6,rightspeed);
       analogWrite(9,rightspeed);
       
       delay(1000);
       
       int m=0;
       for (m=0;m<11;m++){
         olddata[m]=data[m];
       }
       reading=0;//not reading anymore
       index=0;
 
     }
   }
   
   if (incomingByte=='*'){ //start character
     reading=1;
   }
 }
 }
