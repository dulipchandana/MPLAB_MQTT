const int trigPin = 5;  //D1
const int neg = 16;  //D0


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  Serial.begin(9600); // Starts the serial communication
  pinMode(neg, OUTPUT);
  digitalWrite(neg, LOW);

}

void loop() {
  digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  delay(1000);
  Serial.println("testok: ");

}
