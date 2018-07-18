// defines pins numbers

const int trigPin = 5;  //D1
const int echoPin = 4;  //D2

// defines variables
long duration;
long distance;


void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

void loop() {
int distance = sample_distance();
Serial.println("Distance: ");
Serial.println(distance);
delay(2000);
}


int sample_distance() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.println("Duration: ");
  Serial.println(duration);
  distance = int((duration / 2) / 29.155);
  return distance;
}


