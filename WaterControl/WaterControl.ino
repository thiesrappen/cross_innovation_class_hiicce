// Belegung der Pins des Arduino. Damit spaeter leicht die Pin-Belgung angepasst werden kann, sollte
// die Software auf anderer Hardware ausgefuehrt werden, sind die Analogen und digitalen Pins, die
// zum Ansprechen der Hardware verwendet werden im folgenden als Konstanten definiert.

// Der Prototy wird durch Pumpen gesteuert. In einer spaeteren Version sind jedoch auch Tanks, welche
// auf den Tonnen montiert sind denkbar. Diese werden dann durch Ventile gesteuert, welche die
// Schwerkraft zur Verteilung des Wassers nutzen. Das Boolean-Array haelt fest, ob es sich bei einer
// Wasserquelle um eine Pumpe oder ein Ventil haelt. Pumpen oeffnen dabei durch die hoehere
// Durchlaufmenge nur fuer einen ANteil der Zeit eines Ventils.
const int SOURCES[] = {22, 23};
const bool IS_PUMP[] = {};
const float PUMP_RATIO = 0.15; // Wenn ein Ventil 10s oeffnet, soll eine Pumpe 1.5s pumpen.

const int HUMIDITIES[] = {0, 1};
const int environmentSensor = 8;
const int LEVEL_ECHO = 10;
const int LEVER_TRIGER = 11;

// Die bestellten Sensoren unterscheiden sich in ihren Messausgaben leider deutlich. Es lässt sich
// an dieser Stelle daher leider nicht vermeiden,spezifische Wert je Sensor festzulegen. In einer
// späteren Iteration sollte auf Kapazitive Feuchtesensoren umgestellt werden, die u. U.
// konsistentere Ergebnisse liefern und so globale Bereichsdefinitionen erlauben.
// In der Zwischenzeit wird ein Hilfsmethode die Werte der Sensoren auf einen globalen Richtwert
//übersetzen.
const int SESNOR_1_IDEAL = 180;
const int SESNOR_2_IDEAL = 250;
const float SOIL_HUMIDITY_TARGET = 0.25;

// Gibt die Tankhöhe in Centimetern an. Mit dem Ultraschallsensor lässt sich so die Füllhöhe des
// Tanks bestimmen und ein Überfüllen des Tanks oder Trockenlaufen der Pumpen vermeiden.
const int RESERVOIR_HIGHT = 18;

// Anzahl der Wiederholungen bei der Distanzmessung. Fuer Begruendung Methodenkommentar der
// Distanzmessung beachten.
const int DISTANCE_MESURMENT_REPETITIONS = 5;

// Verzoegerung zwischen den Durchlaeufen in ms.
const unsigned long POLL_DELAY = 20000;

// Oeffnungszeiten fuer Ventile, wenn bewaessert werden soll. Fuer Pumpen den Faktor beachten.
const int WATERING_TIME = 10000;

// Um das Gießen bei Frost zu verhindern (Die Pumpe würde möglicherweise beim Versuch, Eis zu
// foerdern, trockenlaufen).
const float MINIMUM_OPERATIONG_TEMPERATURE = 8.0;

// Um Schimmelbildung zu verhindern wird die Bewaesserung gestoppt, wenn keine Verdunstung mehr zu
// erwarten ist.
const float MAXIMUM_AIR_HUMIDITY = 99.9;

// Beschreibt die minimale Wasserhoehe, bei der Pumpen noch foerdern koennen/Ventileingaenge noch
// unterhalb der Wasseroberflaeche sind. Die Angabe ist in cm.
const int MINIMUM_WATER_LEVEL = 3;

// Beschreibt die maximale Wasserhoehe, bei der der Ultraschallsensor noch zuverlaessig Ergebnisse
// liefern kann und nicht unter Wasser steht.
const int MAXIMUM_WATER_LEVEL = 15;

void setup() {
  // Für Entwicklungszwecke wird eine serialle Schnittstelle mit dem Arduino eröffnet. Alle Ausgaben
  // auf dieser Schnittstelle lassen sich mit einem entsprechnden Tool auslesen.
  Serial.begin(9600);

  // Die Pins der Wasserquellen werden in den Output-Modus versetzt. Dies bedeutet, dass nun HIGH-
  // oder LOW-Signale über diese Ports gesendet werden können. Initial werden beide Quellen auf LOW
  // gesetzt.
  pinMode(SOURCES[0], OUTPUT);
  pinMode(SOURCES[1], OUTPUT);
  digitalWrite(SOURCES[0], LOW);
  digitalWrite(SOURCES[1], LOW);

  // Der Ultraschallabstandssensor benoetigt einen Ausgangskanal, um ein Ultraschallsignal zu senden
  // und einen Eingangskanal, um das Echo abzufangen. Ueber die Zeit zwischen den Signalen kann ein
  // Abstand errechnet werden.
  pinMode(LEVER_TRIGER, OUTPUT);
  pinMode(LEVEL_ECHO, INPUT);

  // TODO: Thermostat, Feuchtesensor
}

// Gibt den aktuellen Bodenfeuchtewert, abhängig vom angefragten Bettindex zurück. Der
float measureSoilHumidity(int index) {
  return 0.9;
}

// Misst die Distanz bis zur Wasseroberflaeche und errechnet damit den Wasserstand im Tank. Da der
// Sensor bei schnell wechselnden Abstaenden teilweise kurz den Maximalwert ausgibt, bevor ein neuer
// Abstand gemessen werden kann, wird fuenffach gemessen und der stochastische Median ausgegeben.
// Auch wenn diese Methode keine Fehlerhafte Messungen ausschließen kann, ist sie fuer die
// verwendete Hardware und dessen Messfehlerqueote mehr als ausreichend.
float measureWaterLevel() {
  long duration;

  // Bei den standartmaeßigen
  // fuenf Messungen mit einer Verzoegerung von 100 Millisekunden zwischen den Messungen wird ueber
  // 0,5 Sekunden hinweg gemessen. Selbst bei heftigen Aenderungen der Distanz benoetigte der Sensor
  // nie mehr als zwei Iterationen, um wieder korrekt zu messen.
  float measurements[DISTANCE_MESURMENT_REPETITIONS];
  for (short int i = 0; i < sizeof(measurements) / sizeof(*measurements); i++) {
    // Zur Distanzmessung wird ein 10 Mikrosekunden langes Signal ueber den Lautsprecher emmittiert.
    digitalWrite(LEVER_TRIGER, LOW);
    delayMicroseconds(2);
    digitalWrite(LEVER_TRIGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(LEVER_TRIGER, LOW);
    duration = pulseIn(LEVEL_ECHO, HIGH);
    measurements[i] = duration * 0.034 / 2;
    delay(100);
  }

  // Da die Anzahl der erhobenen Messwerte nur Sensorbedings ueberhaupt groeßer als eins ist, wird
  // zum sortieren hier nur ein einfacher Bubble-Sort Algorythmus verwendet. Komplexitaeten und
  // Laufzeitverhalten spielen bei dieser Datenmenge noch keine Rolle.
  for (short int i = 0; i < sizeof(measurements) / sizeof(*measurements) - 1; i++) {
    for (short int j = 0; j < sizeof(measurements) / sizeof(*measurements) - i - 1; i++) {
      if (measurements[j] > measurements[j + 1]) {
        float temp = measurements[j];
        measurements[j] = measurements[j + 1];
        measurements[j + 1] = temp;
      }
    }
  }

  // Nun wird der Median ermittelt und von der Reservoirhoehe abgezogen. Je nach Messungsanzahl
  // (gerade/ungerade) ist die Methodik zur Bestimmung des Medians leicht anders. So wird entweder
  // das mittlere Elemnt gewaehlt, oder der durchschnitt der beiden mittleren Elemente, wenn es
  // eine gerade Anzahl Elemente gibt.
  if (DISTANCE_MESURMENT_REPETITIONS % 2 == 0) {
    return RESERVOIR_HIGHT - (measurements[DISTANCE_MESURMENT_REPETITIONS / 2] + measurements[(DISTANCE_MESURMENT_REPETITIONS + 1) / 2]) / 2;
  } else {
    return RESERVOIR_HIGHT - measurements[DISTANCE_MESURMENT_REPETITIONS / 2];
  }
}

int measureEnvironmental(float *temperature, float *humidity) {
  return 1;
}

// Routine zum Bewaessern eines Beetes
void watering(int index) {
  if (index < sizeof(SOURCES) / sizeof(int)) {
    digitalWrite(SOURCES[index], HIGH);
    delay(WATERING_TIME);
    digitalWrite(SOURCES[index], LOW);
  }
}

float measureAirTemperature() {
  return 21.6;
}

float measureAirHumidity() {
  return 55.2;
}

void openInlet() {
  //todo
}

void closeInlet() {
  //todo
}

void loop() {
  Serial.println("Routine gestartet");

  // Als erstes wird die vorhandene Wassermenge geprueft. Ist kein Wasser vorhanden, kann der Rest
  // der Routine uebersprungen werden. Dies muss unabhaengig von der Temeperaturpruefung immer
  // geschehen, da eventuelle Zulaufventile trozdem gesteuert werden sollen.
  float waterLevel = measureWaterLevel();
  if (waterLevel < MINIMUM_WATER_LEVEL) {
    openInlet();
    return;
  }
  else if (waterLevel >= MAXIMUM_WATER_LEVEL) {
    closeInlet();
  }

  // Nachfolgend wird die Umgebungstemperatur geprueft
  float airTemperature = measureAirTemperature();
  if (airTemperature < MINIMUM_OPERATIONG_TEMPERATURE) {
    Serial.print("Temperatur fuer Bewaesserung zu niedrig: °C ");
    Serial.println(airTemperature);
    return;
  }

  if (measureAirHumidity() > MAXIMUM_AIR_HUMIDITY) {
    return;
  }

  for (int i = 0; i < sizeof(SOURCES) / sizeof(int); i++) {
    float tempHumidity = measureSoilHumidity(i);
    Serial.print("Beet ");
    Serial.print(i);
    if (tempHumidity < SOIL_HUMIDITY_TARGET) {
      Serial.print(" zu trocken: ");
      watering(i);
    } else {
      Serial.print(" nass genug: ");
    }
    Serial.println(tempHumidity);
  }

  // Da die Anwendung sequentiell ausgeführt wird und es keine parallelen Aufgaben gibt, die
  // ausgeführt werden könnten, wird an dieser Stelle ein delay verwendet, um einem eventuell
  // frisch gewässerten Beet die Zeit zu geben, das erhaltene Wasser zu verteilen und am
  // Feuchtigkeitssensor eine Veränderung zu erzeugen.
  delay(POLL_DELAY);
  Serial.println();
}
