# Überblickartige Beschreibung des Programms sup_rac2.cpp
## Suprenum ReadAndClassify 2 (Daten Lesen und Klassifizieren, Entwickelt für den NY-Taxi-Datensatz)
## Studie zur Stream-Datenanalyse

*Version 0.15.00, 24.04.2022*

Diese ist das 2. Programm zum Einlesen und Verarbeiten von Daten, die kontinuierlich anfallen. Das ist wie
für das erste Programm auch noch simuliert. Die verwendeten NY-Taxi-Beispieldaten wurden von der Plattform Kaggle
heruntergeladen und haben eine Größe von über 5.5 GB und können also nicht mehr ohne Aufwand geladen werden.

Link zum New York City Taxi Fare Prediction3-Datensatz: https://www.kaggle.com/c/new-york-city-taxi-fare-prediction/data

Jede Datenzeile wird einzeln eingelesen, verarbeitet und als verarbeitete Information in eine entsprechenden
csv-Datei gespeichert.

Die Verarbeitung, d. h. die Klassifizierung, erfolgt über die im Programm berechnete Distanz. Die Distanz (eklidische)
ist ein nues *Merkmal* und wird neben weiteren transformierten Merkmalen in den Ergebnisdateien abgelegt.

Das Programm wird über die config.ini-Datei konfiguriert und nach getaner Arbeit wird eine Log-Datei
geschrieben.

Die folgende Grafik gibt einen Überblick:

![sup_rac2-Flow Bild 1](https://github.com/SuprenumDE/Datenanalysefunktion/blob/main/images/sup_rac2_1.jpg)
![sup_rac2-Flow Bild 2](https://github.com/SuprenumDE/Datenanalysefunktion/blob/main/images/sup_rac2_2.jpg)

### Aus C++-Sicht wesentliche Merkmale des Programms:
- Standard C++ 17
- Einbinden eines eigenen Headers #include "sup_functions.h"
- Variablendeklaration
  - string
  - vector<string>
  - double
  - float
  - long long
  - size_t (int)
  - const int
- Funktion, Übergabe als Wert
- Einlesen und Schreiben von CSV-Dateien (#include *fstream*, #include *filesystem*, ifstream / ofstream /getline)
- komplexe Kontrollstrukturen (if / while)
- Zeichenkettenbearbeitung (#include *string*)
- Vektoren (#include *vector*)
- Zeiterfassung (#include *ctime*)
