int Relay = 2;

void setup() {
  pinMode(Relay,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()){
    String Bulb = Serial.readString();
    if(Bulb == "1"){
      digitalWrite(Relay,HIGH);
    }else if(Bulb == "0"){
      digitalWrite(Relay,LOW);
    }
  }
  // put your main code here, to run repeatedly:

}
