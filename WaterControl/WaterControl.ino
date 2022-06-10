#include <dht_nonblocking.h>

// pins
const int PUMPS[] = {22, 23};
const int HUMIDITIES[] = {0, 1};
const int environmentSensor = 8;
const int LEVEL_ECHO = 10;
const int LEVER_TRIGER = 11;

// sensor calibration
const int MAX_WETNES = 400;
const int SOIL_WETNES = 150;
const int TANK_HEIGHT = 18;

// watering parameters
const unsigned long POLL_DELAY = 10000;
const int WATERING_TIME = 5000;
const double SOIL_HUMIDITY_TARGET = 0.5;
const float MINIMUM_TEMPERATURE = 22.0;
const int MINIMUM_WATER_HIGH = 3;

// global variables
unsigned long routineTimestamp;
DHT_nonblocking dht_sensor( environmentSensor, DHT_TYPE_11 );

void setup() {
  routineTimestamp = millis();
  Serial.begin(9600);
  pinMode(PUMPS[0], OUTPUT);
  pinMode(PUMPS[1], OUTPUT);
  digitalWrite(PUMPS[0], LOW);
  digitalWrite(PUMPS[1], LOW);
  // TODO: Termometer, Ultraschall, Feuchtesensor
}

// gives the humidity of [index] flower-bed in a range of 0 - 1
bool measureHumidity(int index) {
  return 0.9;
}

// gives the Temperature and Humidity of the Environment
int measureEnvironmental (float *temperature, float *humidity) {
  // TODO: funktioniert noch nicht
  return dht_sensor.measure( temperature, humidity );
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
  if (millis() - routineTimestamp > POLL_DELAY) {
    routineTimestamp = millis();
    Serial.println("Routine started");
    float temperature = 0.0;
    float environmentHumidity = 0.0;
    measureEnvironmental(&temperature, &environmentHumidity);
    if (temperature >= MINIMUM_TEMPERATURE) {
      for (int i = 0; i < sizeof(PUMPS)/sizeof(int); i++) {
      double humidity = measureHumidity(i);
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
  }
}
