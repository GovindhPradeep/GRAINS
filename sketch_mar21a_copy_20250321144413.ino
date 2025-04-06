const int stepPinY = 6; 
const int dirPinY = 7; 
const int stepPinX = 9; 
const int dirPinX = 8; 
const int stepPinZ = 3; 
const int dirPinZ = 2; 
int direction = -1;

void setup() {
  pinMode(stepPinX,OUTPUT); 
  pinMode(dirPinX,OUTPUT);
   pinMode(stepPinY,OUTPUT); 
  pinMode(dirPinY,OUTPUT);
   pinMode(stepPinZ,OUTPUT); 
  pinMode(dirPinZ,OUTPUT);
  Serial.begin(9600);
  
  
}

void loop() {
  digitalWrite(dirPinX,0);
  for(int x = 0; x < 3200; x++) {
    
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(200);

}

  delay(1000);
  digitalWrite(dirPinX,1);
  for(int x = 0; x < 3200; x++) {
    
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(200);

}

}