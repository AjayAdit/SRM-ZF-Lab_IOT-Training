const int trig = 5;
const int echo = 18;
int motorPin1 = 27;
int motorPin2 = 26;
int duration = 0; 
int distance = 0;
void setup()
{
  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);
  pinMode(motorPin1 , OUTPUT);
  pinMode(motorPin2 , OUTPUT);
  Serial.begin(9600);

}

void loop() {
  digitalWrite(trig , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig , LOW);

  duration = pulseIn(echo , HIGH);
  distance = (duration / 2) / 28.5 ;// calculate distance in CM
  //Serial.println(distance);

  if ( distance < 20 ) {
    Serial.print(distance);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2 , LOW);
    Serial.println("cm   MOTOR STOP");
    delay(1000);
  }
  else{
    Serial.print(distance);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2 , LOW);
    Serial.println("cm   MOTOR START");
    delay(1000);
  }
}