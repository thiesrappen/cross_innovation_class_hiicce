#define PUMP1 = 22
#define PUMP2 = 23

const int POLL_RATE_SECONDS = 10;
const int FLOWER_BEDS = 2;
const double SOIL_HUMIDITY_TARGET = 0.5;
const double MINIMUM_RESERVOIR_LEVEL = 0.2;
const int VALVE_OPEN_TIME = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  //digitalWrite(PUMP1, LOW);
  //digitalWrite(PUMP1, LOW);
}

double measureHumidities(int index) {
  // measures individual himudity levels and stores them in an array
  return 0.0;
}

void watering(int index){
  Serial.println("kommt an");
  digitalWrite(22, HIGH);
  delay(1000);
  digitalWrite(22, LOW);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Routine started");
  for (int i = 0; i<FLOWER_BEDS; i++) {
    double humidity = measureHumidities(i);
    if (humidity<SOIL_HUMIDITY_TARGET){
      Serial.println("Zu trocken");
      watering(i);
    }
  }
  
  //delay(5000);
}


