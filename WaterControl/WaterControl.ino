int index = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void log(String message) {
  Serial.println(message + " " + index);
}

void loop() {
  // put your main code here, to run repeatedly:
  log("MOIN");
  delay(10000);
}


