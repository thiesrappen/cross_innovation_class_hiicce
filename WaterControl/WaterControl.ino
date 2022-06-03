const int PUMPS[] = {22, 23};
const int POLL_DELAY = 10000;
const double SOIL_HUMIDITY_TARGET = 0.5;
const double MINIMUM_RESERVOIR_LEVEL = 0.2;
const int MINIMUM_TEMPERATURE = 4;
const int WATERING_TIME = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(PUMPS[0], OUTPUT);
  pinMode(PUMPS[1], OUTPUT);
  digitalWrite(PUMPS[0], LOW);
  digitalWrite(PUMPS[1], LOW);
}

double measureHumidities(int index) {
  return 0.0;
}

int measureTemperature() {
  return 15;
}

// watering the flower-beds
void watering(int index){
  if (index < sizeof(PUMPS)) {
    digitalWrite(PUMPS[index], HIGH);
    delay(WATERING_TIME);
    digitalWrite(PUMPS[index], LOW); 
  }
}

// update the screen
void updateScreen(double temperature){
  
}

void loop() {
  Serial.println("Routine started");
  int temperature = measureTemperature();
  Serial.print("Temperatur: ");
  Serial.println(temperature);
  if (temperature >= MINIMUM_TEMPERATUR) {
    for (int i = 0; i < sizeof(PUMPS)/sizeof(int); i++) {
    double humidity = measureHumidities(i);
    Serial.print(i);
    if (humidity < SOIL_HUMIDITY_TARGET){
      Serial.print(" zu trocken: ");
      watering(i);
    } else {
      Serial.print(" nass genug: ");
    }
    Serial.println(humidity);
    }
  }
  updateScreening(temperature);
  delay(POLL_DELAY);
}
