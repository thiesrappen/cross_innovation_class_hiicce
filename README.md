# Cross Innovation Class, HIICCE

---

## Designentscheidungen

### Offen

- Genereller Programmablauf, Soll das ein terminiertes Skript ablaufen, dass von einer Aufgabenverwaltung, wie Crontab gestartet wird, oder eine while(True)-Schleife einen Prozess mit Warteanweisungen dauerhaft am Leben halten?
  - Je nach Entscheidung: Wenn Dauerloop: Speichern, wann ein Beet zuletzt bewässert wurde. Damit Bett nicht dreimal nacheinander bewässert wird und dann überläuft, bevor die Erde um den Sensor feucht geworden ist. Bei Aufruf via Scheduler müssten diese Daten in einem Speicherstand abgelegt werden.
  - Dauerloop erlaubt leichter, eine ansprechbare Weboberfläche aufzubauen, Scheduler erlaubt komplettes abschalten des Geräts.
- Wie sollen Konfigurationen auf die Geräte übertragen werden? Es bietet sich beispielsweise an, dass mithilfe einer digitalen Signatur eine Datei mit einem definierten Namen, zum Beispiel cic_hiiccee_config.txt automatisch geladen und über eine bestehende Konfiguration geschrieben wird (korrekte Signatur vorausgesetzt).
  - Beispiel für Libary zum Parsen von Konfigurationsdateien: "configparser".

### Entschieden (mit Begründung)

---

## Teammitglieder

- Amely Hesse (HCU, Stadtplanung)
- Malte Anton Koch (HCU, Stadtplanung)
- Johanna Simon (HCU, Stadtplanung)
- Sascha Witzel (AMD, Produkt Design)
- Dominik Utz (AMD, Produkt Design)
- This Frederik Rappen (FH-Wedel, Wirtschaftsinformatik)
- Jannes Wegner (IT-Engineering)

## Partnerunternehmen

- Hiicce
- Stadtreinigung Hamburg
- Innovationsladen Hamburg und EU //todo