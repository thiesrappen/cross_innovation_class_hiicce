// pins
const int PUMPS[] = {22, 23};
const int HUMIDITIES[] = {0, 1};
const int TERMOMETER = 8;
const int LEVEL_ECHO = 10;
const int LEVER_TRIGER = 11;

// sensor calibration
const int MAX_WETNES = 400;
const int SOIL_WETNES = 150;
const int TANK_HEIGHT = 18;

// watering parameters
const int POLL_DELAY = 10000;
const int WATERING_TIME = 5000;
const double SOIL_HUMIDITY_TARGET = 0.5;
const int MINIMUM_TEMPERATURE = 4;
const int MINIMUM_WATER_HIGH = 3;

void setup() {
  Serial.begin(9600);
  pinMode(PUMPS[0], OUTPUT);
  pinMode(PUMPS[1], OUTPUT);
  digitalWrite(PUMPS[0], LOW);
  digitalWrite(PUMPS[1], LOW);
  // TODO: Termometer, Ultraschall, Feuchtesensor
}

// gives the humidity of [index] flower-bed in a range of 0 - 1
double measureHumidities(int index) {
  // TODO: measure Humidities
  return 0.4;
}

// gives the Temperature
int measureTemperature() {
  // TODO: measure Temperature
  return 15;
}

// gives true, if the tank ist to empty for watering
bool emptyTank() {
  // TODO: read Ultrasonic-Sensor
  return true;
}

// watering the flower-beds
void watering(int index){
  if (index < sizeof(PUMPS)/sizeof(int)) {
    digitalWrite(PUMPS[index], HIGH);
    delay(WATERING_TIME);
    digitalWrite(PUMPS[index], LOW); 
  }
}

void loop() {
  Serial.println("Routine started");
  int temperature = measureTemperature();
  if (temperature >= MINIMUM_TEMPERATURE) {
    for (int i = 0; i < sizeof(PUMPS)/sizeof(int); i++) {
    double humidity = measureHumidities(i);
    Serial.print("Beet "); 
    Serial.print(i);
    if (emptyTank()) {
      Serial.print(" kann wegen leerem Tank nicht gewässert werden: ");
    } else if (humidity < SOIL_HUMIDITY_TARGET){
      Serial.print(" zu trocken: ");
      watering(i);
    } else {
      Serial.print(" nass genug: ");
    }
    Serial.println(humidity);
    }
  } else {
    Serial.print("Temperatur für Bewässerung zu niedrig: ");
    Serial.println(temperature);
  }
  delay(POLL_DELAY);
}
