# Überblickartige Beschreibung des Programms sup_rac.cpp
## Suprenum ReadAndClassify (Daten Lesen und Klassifizieren, Entwickelt für den Weindatensatz)
## Studie zur Stream-Datenanalyse

*Version 0.03.00, 03.04.2022*

Diese ist das 1. Programm zum Einlesen und Verarbeiten von Daten, die kontinuierlich anfallen. Das ist
für dieses Programm natürlich noch simuliert. Die verwendeten Beispieldaten könnten *locker* von dem
System/Programm eingelesen werden, aber dass ist nicht das Ziel dieser Simulation. Jede Datenzeile wird
einzeln eingelesen, verarbeitet und als verarbeitete Information in eine entsprechenden csv-Datei gespeichert.

Die Verarbeitung, d. h. die Klassifizierung, erfolgt über die Qualitsbeurteilung. Sie ist ein *Merkmal* im
Datensatz *Wein*.

Das Programm wird über die config.ini-Datei konfiguriert und nach getaner Arbeit wird eine Log-Datei
geschrieben.

Die folgende Grafik gibt einen Überblick:

![sup_rac-Flow](https://github.com/SuprenumDE/Datenanalysefunktion/blob/main/images/sup_rac_Flow.jpg)

### Aus C++-Sicht wesentliche Merkmale des Programms:
- Standard C++ 17
- Alle Anweisungen und Funktionen in einem Skript
- Variablendeklaration
  - string
  - vector<string>
  - long long
  - const int
- Funktion, Übergabe als Wert
- Einlesen und Schreiben von CSV-Dateien (#include *fstream*, #include *filesystem*, ifstream / ofstream /getline)
- Kontrollstrukturen (if / while)
- Zeichenkettenbearbeitung (#include *string*)
- Vektoren (#include *vector*)
- Zeiterfassung (#include *ctime*)

### Video-Beschreibung des sup_rac2-Skripts
  
  *In Arbeit ...*
 
