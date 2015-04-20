//left side ENBL: 2, PHASE: 4
//right side ENBL: 9, PHASE:8
 
#include <Servo.h>
 
//initialize variables
Servo servo;
int reading=0;           //we are initially not reading for our values
int index=0;             //index of our data array
int data[]={0,9,0,'F',0,0,0,'F',0,0,0};   //place for data to go, start character and 3 values
int olddata[]={0,9,0,'F',0,0,0,'F',0,0,0};//initialize old data variable
int incomingByte=0;      //current communication coming in from serial
int leftspeed=0;         //speed of left side of robot  (0-255)
int rightspeed=0;        //speed of right side of robot (0-255)
int LENBL=2;
int RENBL=9;
int LPH=4;
int RPH=8;

void setup(){
  servo.attach(11); //servo at 90 degrees, on pin 11
  servo.write(90);
  delay(20);
  Serial.begin(9600);//being serial communications
 
  pinMode(LENBL,OUTPUT);//initialize motor pins as ouputs
  pinMode(LPH,OUTPUT);
  pinMode(RENBL,OUTPUT);
  pinMode(RPH,OUTPUT);
  
  analogWrite(LENBL,0); //initialize robot as braked
  analogWrite(RENBL,0);

 
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
       data[index]=incomingByte-48;//get numerical, not ASCII value of numbers
       index=index+1;
     }
    
     if (index>10){
       //we've received everything, start actuating
       leftspeed=data[6]+(10*data[5])+(100*data[4]);  //translate three characters into 3 digit speed
       rightspeed=data[10]+(10*data[9])+(100*data[8]);//translate three characters into 3 digit speed
     
       if ((data[3]!=olddata[3]) || (data[7]!=olddata[7])){ 
         //if we're changing directions, brake first
         digitalWrite(LPH,LOW);
         digitalWrite(RPH,LOW);
         analogWrite(LENBL,0);
         analogWrite(RENBL,0);
         delay(1000);
       }
 
       if (data[3]=='F'){   //left side forward
         digitalWrite(LPH,LOW);
       }
       else if (data[3]=='B'){           //left backward
         digitalWrite(LPH,HIGH);
       }
       
       if (data[7]=='F'){  //right side forward
         digitalWrite(RPH,LOW);
       }
       else if (data[7]=='B'){             //right side backwards
         digitalWrite(RPH,HIGH);
       }
       
       analogWrite(LENBL,leftspeed);//set motor speeds
       analogWrite(RENBL,rightspeed);
       
       delay(1000);
       
       int m=0; //store old data to see if we changed direction
       for (m=0;m<11;m++){
         olddata[m]=data[m];
       }
       
       reading=0;//not reading anymore
       index=0;
 
     }
   }
   
   if (incomingByte=='*'){ //start character
     reading=1; //start reading if we see the start of a command
   }
 }
 }
