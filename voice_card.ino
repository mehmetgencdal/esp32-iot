int sensorPin = 5;
boolean val = 0;

void setup(){
  pinMode(sensorPin, INPUT);
  Serial.begin (115200);
}
  
void loop (){
  val = digitalRead(sensorPin);
  Serial.println (val);
}
