const int ltXStepper = 8;
const int ltXPulley = 11;
const int ltYStepper = 12;
const int ltYPulley = 13;
const int stepPinX = 9; 
const int dirPinX = 10; 
const int stepPinY = 6; 
const int dirPinY = 7;
const int stepPinZ = 3; 
const int dirPinZ = 2; 
int directionX = 1;
int prevLimit = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(ltXStepper, INPUT_PULLUP);
  pinMode(ltXPulley, INPUT_PULLUP);
  pinMode(ltYStepper, INPUT_PULLUP);
  pinMode(ltYPulley, INPUT_PULLUP);
  pinMode(stepPinX,OUTPUT); 
  pinMode(dirPinX,OUTPUT);
  pinMode(stepPinY,OUTPUT); 
  pinMode(dirPinY,OUTPUT);
  pinMode(stepPinZ,OUTPUT); 
  pinMode(dirPinZ,OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // put your main code here, to run repeatedly:a
  digitalWrite(dirPinX, directionX);
  
  for(int x = 0; x < 32000; x++){
    
    digitalWrite(stepPinX,HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPinX,LOW);
    delayMicroseconds(100);
    
}  
}



int checkLimits(){
  if(digitalRead(ltXStepper) == 0){
    Serial.println("X axis 0 point");
    return 1;
  }
  else if(digitalRead(ltXPulley) == 0){
    Serial.println("X axis max point");
    return 1;
  }
  else if(digitalRead(ltYStepper) == 0){
    Serial.println("Y axis 0 point");
    return 2;
  }
  else if(digitalRead(ltYPulley) == 0){
    Serial.println("Y axis max point");
    return 2;
  }
  return 0;

}