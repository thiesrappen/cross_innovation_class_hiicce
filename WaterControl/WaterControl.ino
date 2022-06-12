//Belegung der Pins des Arduino. Damit spaeter leicht die Pin-Belgung angepasst werden kann, sollte
//die Software auf anderer Hardware ausgefuehrt werden, sind die Analogen und digitalen Pins, die
//zum Ansprechen der Hardware verwendet werden im folgenden als Konstanten definiert.

//Der Prototy wird durch Pumpen gesteuert. In einer spaeteren Version sind jedoch auch Tanks, welche
//auf den Tonnen montiert sind denkbar. Diese werden dann durch Ventile gesteuert, welche die
//Schwerkraft zur Verteilung des Wassers nutzen. Das Boolean-Array haelt fest, ob es sich bei einer
//Wasserquelle um eine Pumpe oder ein Ventil haelt. Pumpen oeffnen dabei durch die hoehere
//Durchlaufmenge nur fuer einen ANteil der Zeit eines Ventils.
const int SOURCES[] = {22, 23}; 
const bool IS_PUMP[] = {};
const float PUMP_RATIO = 0.15; //Wenn ein Ventil 10s oeffnet, soll eine Pumpe 1.5s pumpen.


const int HUMIDITIES[] = {0, 1};
const int environmentSensor = 8;
const int LEVEL_ECHO = 10;
const int LEVER_TRIGER = 11;

//Die bestellten Sensoren unterscheiden sich in ihren Messausgaben leider deutlich. Es lässt sich
//an dieser Stelle daher leider nicht vermeiden,spezifische Wert je Sensor festzulegen. In einer
//späteren Iteration sollte auf Kapazitive Feuchtesensoren umgestellt werden, die u. U.
//konsistentere Ergebnisse liefern und so globale Bereichsdefinitionen erlauben.
//In der Zwischenzeit wird ein Hilfsmethode die Werte der Sensoren auf einen globalen Richtwert
//übersetzen.
const int SESNOR_1_IDEAL = 180;
const int SESNOR_2_IDEAL = 250;
const float SOIL_HUMIDITY_TARGET = 0.25;

//Gibt die Tankhöhe in Centimetern an. Mit dem Ultraschallsensor lässt sich so die Füllhöhe des
//Tanks bestimmen und ein Überfüllen des Tanks oder Trockenlaufen der Pumpen vermeiden.
const int TANK_HEIGHT = 18;

//todo
const unsigned long POLL_DELAY = 10000;
const int WATERING_TIME = 5000;

//Um das Gießen bei Frost zu verhindern (Die Pumpe würde möglicherweise beim Versuch, Eis zu foerdern trockenlaufen)
const float MINIMUM_OPERATIONG_TEMPERATURE = 8.0;
const int MINIMUM_WATER_HIGH = 3;

void setup() {
  
  //Für Entwicklungszwecke wird eine serialle Schnittstelle mit dem Arduino eröffnet. Alle Ausgaben
  //auf dieser Schnittstelle lassen sich mit einem entsprechnden Tool auslesen.
  Serial.begin(9600);

  //Die Pins der Wasserquellen werden in den Output-Modus versetzt. Dies bedeutet, dass nun HIGH-
  //oder LOW-Signale über diese Ports gesendet werden können. Initial werden beide Quellen auf LOW
  //gesetzt.
  pinMode(SOURCES[0], OUTPUT);
  pinMode(SOURCES[1], OUTPUT);
  digitalWrite(SOURCES[0], LOW);
  digitalWrite(SOURCES[1], LOW);

  //Der Ultraschallabstandssensor benoetigt einen Ausgangskanal, um ein Ultraschallsignal zu senden
  //und einen Eingangskanal, um das Echo abzufangen. Ueber die Zeit zwischen den Signalen kann ein
  //Abstand errechnet werden.
  pinMode(LEVER_TRIGER, OUTPUT);
  pinMode(LEVEL_ECHO, INPUT);

  // TODO: Thermostat, Feuchtesensor
}

//Gibt den aktuellen Bodenfeuchtewert, abhängig vom angefragten Bettindex zurück. Der 
float measureHumidity(int index) {
  return 0.9;
}

//Misst die Distanz bis zur Wasseroberflaeche und errechnet damit den Wasserstand im Tank. Da der
//Sensor bei schnell wechselnden Abstaenden teilweise kurz den Maximalwert ausgibt, bevor ein neuer
//Abstand gemessen werden kann, wird fuenffach gemessen und der stochastische Median ausgegeben. 
//Auch wenn diese Methode keine Fehlerhafte Messungen ausschließen kann, ist sie fuer die
//verwendete Hardware und dessen Messfehlerqueote mehr als ausreichend. 
float mesureWaterLevel(){
  long duration;

  //Bei den standartmaeßigen
  //fuenf Messungen mit einer Verzoegerung von 500 Millisekunden zwischen den Messungen wird ueber
  //2,5 Sekunden hinweg gemessen. Selbst bei heftigen Aenderungen der Distanz benoetigte der Sensor
  //nie mehr als 0,7 Sekunden, um wieder korrekt zu messen.
  int measurements[5];
  for (short int i = 0; i < sizeof(measurements)/sizeof(*measurements); i++){
    //Zur Distanzmessung wird ein 10 Mikrosekunden langes Signal ueber den Lautsprecher emmittiert.
    digitalWrite(LEVER_TRIGER, LOW);
    delayMicroseconds(2);
    digitalWrite(LEVER_TRIGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(LEVER_TRIGER, LOW);
    duration = pulseIn(LEVEL_ECHO, HIGH);
    measurements[i] = duration * 0.034 / 2; 

      Serial.print("Distance: ");
      Serial.print(measurements[i]);
      Serial.println(" cm");

    delay(500);
  }



} 

// gives the Temperature and Humidity of the Environment
int measureEnvironmental (float *temperature, float *humidity) {
  // TODO: funktioniert noch nicht
  //return dht_sensor.measure( temperature, humidity );
  return 1;
}

// gives true, if the tank ist to empty for watering
bool emptyTank() {
  // TODO: read Ultrasonic-Sensor
  return true;
}

// watering the flower-beds
void watering(int index){
  if (index < sizeof(SOURCES)/sizeof(int)) {
    digitalWrite(SOURCES[index], HIGH);
    delay(WATERING_TIME);
    digitalWrite(SOURCES[index], LOW); 
  }
}

void loop() {
    Serial.println("Routine gestartet");
    mesureWaterLevel();
    //Zu Beginn der Schleife werden Umgebungswerte geladen
    float enviromentAirTemperature = 0.0;
    float environmentAirHumidity = 0.0;
    measureEnvironmental(&enviromentAirTemperature, &environmentAirHumidity);

    if (enviromentAirTemperature >= MINIMUM_OPERATIONG_TEMPERATURE) {
      for (int i = 0; i < sizeof(SOURCES)/sizeof(int); i++) {
      float humidity = measureHumidity(i);
      Serial.print("Beet "); 
      Serial.print(i);
      if (emptyTank()) {
        Serial.print(" kann wegen leerem Tank nicht gewaessert werden: ");
      } else if (humidity < SOIL_HUMIDITY_TARGET){
        Serial.print(" zu trocken: ");
        watering(i);
      } else {
        Serial.print(" nass genug: ");
      }
      Serial.println(humidity);
      }
    } else {
      Serial.print("Temperatur fuer Bewaesserung zu niedrig: °C ");
      Serial.println(enviromentAirTemperature);
    }
  
  //Da die Anwendung sequentiell ausgeführt wird und es keine parallelen Aufgaben gibt, die
  //ausgeführt werden könnten, wird an dieser Stelle ein delay verwendet, um einem eventuell
  //frisch gewässerten Beet die Zeit zu geben, das erhaltene Wasser zu verteilen und am
  //Feuchtigkeitssensor eine Veränderung zu erzeugen.
  delay(POLL_DELAY);
}
