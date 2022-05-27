# Cross Innovation Class, HIICCE

## Designentscheidungen

### Offen

- Genereller Programmablauf, Soll das ein terminiertes Skript ablaufen, dass von einer Aufgabenverwaltung, wie Crontab gestartet wird, oder eine while(True)-Schleife einen Prozess mit Warteanweisungen dauerhaft am Leben halten?
- Wie sollen Konfigurationen auf die Geräte übertragen werden? Es bietet sich beispielsweise an, dass mithilfe einer digitalen Signatur eine Datei mit einem definierten Namen, zum Beispiel cic_hiiccee_config.txt automatisch geladen und über eine bestehende Konfiguration geschrieben wird (korrekte Signatur vorausgesetzt)

### Entschieden (mit Begründung)

