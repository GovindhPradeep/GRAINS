const int dirX = 10;
const int dirY = 7;
const int dirZ = 2;
const int stepX = 9;
const int stepY = 5;
const int stepZ = 3;                //step-y = 4cm, step-x = 3.4cm, step-z = 0.7cm
const int trigPin = 13;
const int echoPin = 12;
const int waterSig = 8;
const int seederSig = 11;
const int x_steps = 3200, y_steps = 3200, z_steps = 800;
const int pace_x = 40, pace_y = 34, pace_z = 7;
const double res_x = static_cast<double>(x_steps)/(pace_x) ;
const double res_y = static_cast<double>(y_steps)/(pace_y);
const double res_z = static_cast<double>(z_steps)/(pace_z);
bool seed = false, water = false;
int xrev, yrev = 0,zrev = 0;
int xlt = 30, ylt = 15, zlt = 50;
long duration;
int distance;
int min_distance = 30, max_distance = 31;


void setup() {
  // put your setup code here, to run once:
  pinMode(dirX, OUTPUT);
  pinMode(dirY, OUTPUT);
  pinMode(dirZ, OUTPUT);
  pinMode(stepX, OUTPUT);
  pinMode(stepY, OUTPUT);
  pinMode(stepZ, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(waterSig, OUTPUT);
  pinMode(seederSig, OUTPUT);
  digitalWrite(waterSig, LOW);
  digitalWrite(seederSig, LOW);
  Serial.begin(9600);
  Serial.println("Started");
}

void loop() {
  // put your main code here, to run repeatedly:

 cmdMove();

 
//  if (Serial.available()) {
//    String cmd = Serial.readStringUntil('\n');
//
//    if (cmd.startsWith("MOVE_Y")) {
//          int dy = cmd.substring(7).toInt();
//          moveY(dy);
//    }
//
//    else if (cmd.startsWith("MOVE_X")) {
//      int dx = cmd.substring(7).toInt();
//      moveX(dx);    
//    }
//   else if (cmd.startsWith("CENTERED")){
//      int x;
//   }
//  }
}


void seedProcess(){
  moveY(30);
}



void cmdMove(){
  while(!Serial.available());
  String input = Serial.readStringUntil('\n');  // read until newline
  input.trim(); 
  Serial.println(input);
  if (input.length() > 1) {
      if(input.startsWith("MOVE")){
      char axis = input.charAt(4);               // the letter (e.g., X, Y, Z)
      int value = input.substring(5).toInt();    // the signed number
      if(axis == 'x'){
        moveX(value);
      }
      else if(axis == 'y'){
        moveY(value);
      }
      else if(axis == 'z'){
        moveZ(value);
      }
      }
      else if (input.startsWith("S")){
        seed != seed;
        digitalWrite(seederSig, seed);
      }
      else if (input.startsWith("W")){
        water != water;
        digitalWrite(waterSig, water);
      }
  }
  
  Serial.println("Waiting for new input");
  Serial.flush();
  
}



void moveX(int dx){
  
  Serial.println(xrev);
  if (dx < 0){
    digitalWrite(dirX, 1);
  }
  else{
    digitalWrite(dirX, 0);
  }
  
  double steps = 1.0 * res_x * abs(dx);
  
  Serial.println(int(steps));
  for(int i = 0;i < int(steps) ; i++){
    digitalWrite(stepX, 1);
    delayMicroseconds(200);
    digitalWrite(stepX, 0);
    delayMicroseconds(200); 
      }
  Serial.println("moved in X");
}

void moveY(int dy){
  
  
  if (dy < 0){
    digitalWrite(dirY, 1);
  }
  else{
    digitalWrite(dirY, 0);
  }
  double steps = 1.0 * res_y * abs(dy);
  
  Serial.println(int(steps));
  for(int i = 0;i < int(steps) ; i++){
    digitalWrite(stepY, 1);
    delayMicroseconds(200);
    digitalWrite(stepY, 0);
    delayMicroseconds(200); 
      }
  Serial.println("moved in Y");
}
void moveZ(int dz){
  
  Serial.println(zrev);
  if (dz < 0){
    digitalWrite(dirZ, 0);
  }
  else{
    digitalWrite(dirZ, 1);
  }
  double steps = 1.0 * res_z * abs(dz);
  
  Serial.println(int(steps));
  for(int i = 0;i < int(steps) ; i++){
    digitalWrite(stepZ, 1);
    delayMicroseconds(200);
    digitalWrite(stepZ, 0);
    delayMicroseconds(200); 
      }

  Serial.println("moved in Z");
}

int distanceCheck(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    return distance;

}
