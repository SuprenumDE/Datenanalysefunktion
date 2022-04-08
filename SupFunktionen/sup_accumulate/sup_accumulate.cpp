// sup_accumulate.cpp
// Suprenum ReadAndClassify (Daten Lesen, Klassifizieren und Akkumulieren, Entwickelt für den Kaggle-NY-Taxi-Datensatz)
// Studie zur Stream-Datenanalyse
// Entwickler: Guenter Faes, spv@faes.de
// --------------------------------------

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <vector>
#include "sup_functions.h"
using namespace std;


//---------------- Funktionen --------------------

/* Info-Ausgabe als Funktion */
void pgmInfo(string x)
{
    cout << "**********************************************\n";
    cout << "Studie Daten einlesen und verarbeiten\n";
    cout << "Einlesen des Datensatzes test.csv (Kaggle) als * stream *\n";
    cout << "Application-Info: " << x << "\n";
    cout << "**********************************************\n\n";
}
// ------------------------------------------------------------------------

/* Umrechnung der Geo-Koordinaten in die euklidische Distanz
* Input: String (zeichenkette)
* Output: String, Trennzeichen: ;
*/
string DistanzEuklid(vector<string> separierteZeile, int nachkommastellen)
{
    string zeichenkette{ "" };
    string temp{ "" };

    // Variablen deklarieren:
    double start_longitude{ 0.0 };  // X-Achse
    double start_latitude{ 0.0 };   // Y-Achse
    double ende_longitude{ 0.0 };
    double ende_latitude{ 0.0 };

    double geo_X{ 0.0 };
    double geo_Y{ 0.0 };
    double Distanz{ 0.0 };

    // Geo-Daten aus der separierten Zeile entnehmen:
    start_longitude = stod(separierteZeile[3]);
    start_latitude = stod(separierteZeile[4]);
    ende_longitude = stod(separierteZeile[5]);
    ende_latitude = stod(separierteZeile[6]);
    
    // Faktoren für den Breiten-/Längengrad New York (km):
    float longFaktor{ 85.0 };
    float latFaktor{ 111.0 }; 

    // Berechnung der Distanz:
    geo_X = longFaktor * (abs(start_longitude - ende_longitude));
    geo_Y = latFaktor * (abs(start_latitude - ende_latitude));
    Distanz = sqrt(geo_X * geo_X + geo_X * geo_X);

    // Startlongitude und -latitude auf die gewünschten Nachkommstellen fraktionieren:
    // (3 Stellen = 100, 4 stelle = 10 Genauigkeit)
    start_longitude = ZahlFraktionieren(start_longitude, nachkommastellen);
    start_latitude = ZahlFraktionieren(start_latitude, nachkommastellen);
    separierteZeile[3] = "";
    separierteZeile[4] = "";
    separierteZeile[3] = std::to_string(start_longitude);  // Angepasst Nachkommastellen zurück speichern
    separierteZeile[4] = std::to_string(start_latitude);

    // Ausgabezeichenkette basteln:
     zeichenkette = separierteZeile[0];          // key
    zeichenkette += ";";                        // Trennzeichen csv (Excel, deutsch)
    zeichenkette += ptoc(separierteZeile[1]);   // fare_amount
    zeichenkette += ";";
    zeichenkette += separierteZeile[2];         // pickup_datetime
    zeichenkette += ";";
    zeichenkette += ptoc(separierteZeile[3]);   // Startlongitude
    zeichenkette += ";";
    zeichenkette += ptoc(separierteZeile[4]);   // Startlatitude
    zeichenkette += ";";
    zeichenkette += ptoc(to_string(Distanz));   // Die berechnete Distanz
    zeichenkette += ";";
    zeichenkette += separierteZeile[7];         // passenger_count

    return zeichenkette;
}
// ------------------------------------------------------------------------

/* Die Daten mit den transformierten Geo-Daten in km-Distanzen
* als Datei speichern.
* Input: FahrtenDistanz, Dateibezeichnung, verzeichnis (als filesystem-Objekt)
* Output: Datei
*/
void speicherDistanzen(string zeichenkette, string Dateibezeichnung, filesystem::path(verzeichnis))
{

    ofstream disout;

    // Verzeichnis und Dateiname zusammenbringen:
    verzeichnis.append(Dateibezeichnung);

    if (std::filesystem::exists(verzeichnis)) {
        // Datei ist vorhanden
        // Nun findet die eigentliche Speicherung an:
        disout.open(verzeichnis, ofstream::app);
        disout << zeichenkette << "\n";

    }
    else {     // Datei ist NICHT vorhanden, Kopfzeile wird vorangestellt!
        disout.open(verzeichnis, ofstream::app);

        string Kopfzeile{ "" };
        Kopfzeile = "key";
        Kopfzeile += ";";                 // Trennzeichen csv (Excel, deutsch)
        Kopfzeile += "fare_amount";
        Kopfzeile += ";";
        Kopfzeile += "pickup_datetime";
        Kopfzeile += ";";
        Kopfzeile += "Startlongitude";
        Kopfzeile += ";";
        Kopfzeile += "Startlatitude";
        Kopfzeile += ";";
        Kopfzeile += "Distance_km";
        Kopfzeile += ";";
        Kopfzeile += "passenger_count";


        disout << Kopfzeile << "\n";
        disout << zeichenkette << "\n";
    }

    disout.close();
}

//---------------- Funktionen Ende --------------------

int main()
{

    // Variablendeklaration:

    string pgm_version{ "Version 0.12.02, 08.04.2022" };

    const int contfig_row_n{ 20 };                  // Anzahl aller Configfile-Informationen

    string config_file{ "config.ini" };             // Muss unbedingt vorhanden sein!
    string logdatei{ "logdatei.txt" };              // Logbuchdatei
    string distancefile{ "" };                      // Analyseergebnis: Distanzdatei
    string dirDistanceFile{ "" };                   // Verzeichnis für die Distanzdateien
    string classifiedDataTemplate{ "" };            // Analysenergebnis: Über die Distanz klassifizierte Daten
    string classfiedData{ "" };                     // Arbeitsvariable von classifiedDataTemplate
    string config_content[contfig_row_n]{ "" };     // Nimmt den Inhalt der Configdatei auf
    string ProjektBezeichnung = "";
    string dateiname = "";                          // Bezeichnung der zuverarbeitende Datei
    string processing = "";                         // Soll die eingelesene Datei auf dem Screen gezeigt werden?
    string inZeile{ "" };                           // Eingeleseene Datenzeile
    string trennzeichen{ "" };                      // Trennzeichen zum Separieren des eingelesenen Zeileninhalts
    string trennzeichenClassifiers{ ";" };          // Trennzeichen zum Separieren der erzeugten Distanz-CSV-Datei
    string classifiers{ "" };                       // Zeichenkette mit den Klassifizierungsgrenzen
    string FahrtenDistanz{ "" };                    // Eingelesene Datenzeile (inZeile) in der die Geo-Daten als Distanzen in km abgebildet sind

    vector<string> separierteZeile;                 // Eingelesene Datenzeile in Elemente separiert
    vector<string> separierteFahrtenDistanz{ "" };  // FahrtenDistanz in Elemente zerlegt
    vector<string> separatedClassifiers;            // Eingelesene Klassifizierungsgrenze separriert

    long long read_n_rows{ 0 };                     // Stopp Ausfuehrung nach n Zeilen (n = 0: ohne Limitation)
    long long in_n{ 0 };                            // Anzahl der eingelesenen Datenzeilen

    size_t n_classifiers{ 0 };                      // Anzahl der eingelesenen Klassifizierungsgrenzen

    int geo_accu{ 0 };                              // Genauigkeit der klassifizierten Geo-Daten

    // Ausführungsteil:

    // Konfigurationsdatei einlesen:
    ifstream config_in(config_file);
    if (config_in.good()) {

        for (int i = 0; i < (contfig_row_n); i++) {

            getline(config_in, config_content[i]);

            if (i == 1) ProjektBezeichnung = config_content[i];
            if (i == 3) dateiname = config_content[i];
            if (i == 5) processing = config_content[i];
            if (i == 7) read_n_rows = stoi(config_content[i]);
            if (i == 9) trennzeichen = config_content[i];
            if (i == 11) distancefile = config_content[i];
            if (i == 13) classifiers = config_content[i];
            if (i == 15) classifiedDataTemplate = config_content[i];
            if (i == 17) dirDistanceFile = config_content[i];
            if (i == 19) geo_accu = stoi(config_content[i]);

        }

    }
    else {
        // Error-Log:
        ofstream log_out;
        log_out.open(logdatei);

        // Content:
        log_out << "------------ Logbook start -------------------" << "\n";
        log_out << config_file << " could not be read or is not present!" << "\n";
        log_out << "------------ Logbook end ---------------------" << "\n";

        log_out.close();
        // Ende Log-Datei schreiben

        // Programm verlassen:
        return EXIT_FAILURE;
    }

    // 1. Schritt nach dem Einlesen der ini-Datei:
        // Datenklassifikator separieren:
        separatedClassifiers = ZeichenSeparieren(classifiers, trennzeichen);
        n_classifiers = separatedClassifiers.size();

        // Verzeichnis für die klassifizierten Dateien einstellen:
        filesystem::path(klassVerzeichnis) = filesystem::current_path();        // Aktuelles Verzeichnis auslesen
        klassVerzeichnis.append(dirDistanceFile);                               // Das Wunschverzeichnis anfügen
        filesystem::create_directory(filesystem::path(klassVerzeichnis));       // Verzeichnis anlegen


    // Aufruf Programm-Info
    if (processing != "Yes") pgmInfo(pgm_version);


    // ------------------ Zu analysierende Daten einlesen -------------------

    // Zeit ab jetzt messen:
    clock_t Zeit;
    Zeit = clock();


    // Datei einlesen:
    ifstream datei_in(dateiname);
    // Prüfen, ob Datei vorhanden
    if (!datei_in.is_open()) {
        cerr << dateiname << " kann nicht geoeffnet werden!" << "\n";
        return EXIT_FAILURE;
    }

    if (processing != "Yes")
        cout << "Der Inhalt von Datei " << dateiname << " ist:\n\n";

    // Daten einlesen und analysieren:

    classfiedData = classifiedDataTemplate;  // Arbeitkopie anlegen


    while (!datei_in.eof()) 
    {

        // Kontinuierliches oder eine bestimmte Anzahl Datenzeilen einlesen?
        if (read_n_rows == 0) { // Kontinuierlich

            getline(datei_in, inZeile);

            // Sicherheitsabfrage:
            if (inZeile.length() == 0) {
                break;       // Abbruch, wenn eine leere Zeile eingelsenen wurde!
            }

            // Eingelesenen String zerlegen:
            separierteZeile = ZeichenSeparieren(inZeile, trennzeichen);

            /* Akkumulierungsfunktionen:
            Nur durchführen, wenn der Inhalt des ersten Geo-Feldes nicht leer ist!
            Das erste Geo-Feld ist in separierteZeile[3] abgelegt.
            1. Geo-Daten in km-Distanzen umrechnen
             */
            if ((in_n > 0) && (separierteZeile[3] != "0")) {

                FahrtenDistanz = DistanzEuklid(separierteZeile, geo_accu);

                // Die Geo-Daten-Transformationen (Distanz in km) als Datei speichern:
                speicherDistanzen(FahrtenDistanz, distancefile, klassVerzeichnis);

            }


            in_n += 1; // Anzahl der eingelesenen Zeilen hochzählen

        }
        else { // Einlesen bis zur vorgegebenen Anzahl Zeilen

            getline(datei_in, inZeile);

            // Sicherheitsabfrage:
            if (inZeile.length() == 0) {
                break;       // Abbruch, wenn eine leere Zeile eingelsenen wurde!
            }

            // Eingelesenen String zerlegen:
            separierteZeile = ZeichenSeparieren(inZeile, trennzeichen);

            /* Akkumulierungsfunktionen:
            Nur durchführen, wenn der Inhalt des ersten Geo-Feldes nicht leer ist!
            Das erste Geo-Feld ist in separierteZeile[3] abgelegt.
            1. Geo-Daten in km-Distanzen umrechnen
             */
            if ((in_n > 0) && stod(separierteZeile[3]) != 0)
            {

                FahrtenDistanz = DistanzEuklid(separierteZeile, geo_accu);

                // Die Geo-Daten-Transformationen (Distanz in km) als Datei speichern:
                speicherDistanzen(FahrtenDistanz, distancefile, klassVerzeichnis);

                // Daten auf Basis der Distanz klassifizieren:
                separierteFahrtenDistanz = ZeichenSeparieren(FahrtenDistanz, trennzeichenClassifiers);

                double sfd{ 0.0 };          // Hilfsvaribale für separierteFahrtenDistanz[3]
                double sepClass{ 0.0 };     // Hilfsvariable für separatedClassifiers[i]
                double sepClass_1{ 0.0 };   // Hilfsvariable für separatedClassifiers[i -1]
                double sepClass_last{ 0.0 };// Hilfsvariable für separatedClassifiers[letztes Element]

                sepClass_last = stod(separatedClassifiers[n_classifiers - 1]);

                for (size_t i = 0; i <= n_classifiers - 1; i++)
                {
                    // Distanz aus Vektor kopieren:
                    sfd = stod(pcot(separierteFahrtenDistanz[5])); // Auchtung! Das Komma mnuss gegen einen Punkt getauscht werden!
                    sepClass = stod(separatedClassifiers[i]);


                    if (i == 0) // Der Startbereich des 1. Klasse ist immer 0!
                    {
                        if (sfd <= sepClass) //  Vergleich mit dem 1. Klassifikator
                        {
                            classfiedData += separatedClassifiers[i];
                            classfiedData += ".csv";
                            speicherDistanzen(FahrtenDistanz, classfiedData, klassVerzeichnis);
                            classfiedData = classifiedDataTemplate;
                        }

                    }

                    if (i > 0) {

                        sepClass_1 = stod(separatedClassifiers[i - 1]);

                        if ((sfd > sepClass_1) && (sfd <= sepClass))

                            // Der i-1-Klassifikator ist die Untergrenze!            // Vergleiche ab dem 2. Klassifikator

                        {
                            classfiedData += separatedClassifiers[i];
                            classfiedData += ".csv";
                            speicherDistanzen(FahrtenDistanz, classfiedData, klassVerzeichnis);
                            classfiedData = classifiedDataTemplate;

                        }
                         

                    } // Ende if
                    
                    if (sfd > sepClass_last)
                    {   // Alles was größer ist als die größte Klassifizierung aufnehmen (der Restesammler):

                        classfiedData += separatedClassifiers[n_classifiers - 1];
                        classfiedData += "plus.csv";
                        speicherDistanzen(FahrtenDistanz, classfiedData, klassVerzeichnis);
                        classfiedData = classifiedDataTemplate;

                        // Gefunden, also raus aus der for-Schleife:
                        break;

                    }

                } // Ende for

            } // Ende Akkumulierungsfunktionen if

            // Abbruchbedingung prüfen:
            if (in_n == read_n_rows) {
                break;
                }

        }
            
        if (processing != "Yes") {

            // Eingelesene Zeile auf ausgeben:
            cout << inZeile << "\n";
        }

        in_n += 1; // Anzahl der eingelesenen Zeilen zählen

    } // Ende while

    datei_in.close();

    if (processing != "Yes")
        cout << "\nEs wurden " << in_n << " Zeilen eingelesen.\n";

    // Laufzeit ausgeben:
    Zeit = clock() - Zeit;
    if (processing != "Yes")
        printf("\nLaufzeit: %f Sekunden \n", ((float)Zeit) / CLOCKS_PER_SEC);



    // -------------------- Dateien schreiben ----------------------

    // Log-Datei schreiben:
    ofstream log_out;
    log_out.open(logdatei);

    // Content:
    log_out << "------------ Logbook start -------------------" << "\n";
    log_out << "Application-Info: " << pgm_version << "\n\n";
    log_out << "Project: " << ProjektBezeichnung << "\n";
    log_out << "File analyzed: " << dateiname << "\n\n";
    log_out << "Number of rows read: " << in_n << "\n\n";
    log_out << "The input file transformed into distances was saved as: " << distancefile << "\n";
    log_out << "The classified data was saved under the template " << classifiedDataTemplate << "\n";
    log_out << "The files were saved in the following directory: " << klassVerzeichnis << "\n\n";
    log_out << "Execution time in sec: " << ((float)Zeit) / CLOCKS_PER_SEC << "\n";
    log_out << "\n" << "In case of problems, please compare with the config.ini file!" << "\n";
    log_out << "------------ Logbook end ---------------------" << "\n";

    log_out.close();
    // Ende Log-Datei schreiben



    // -------------------------------------------------------------
    return 0;
}
