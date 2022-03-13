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

/* Zelegung der übergebenen Zeichenkette in einzelne Elemente
   Input: String (zeichenkette, trennzeichen)
   Output: Vector
*/
vector<string>ZeichenSeparieren(string zeichenkette, string trennzeichen)
{
    string::size_type start{ 0 };
    string::size_type pos{ 0 };
    vector<string> ergebnis;

    do {
        pos = zeichenkette.find_first_of(trennzeichen, start);
        ergebnis.push_back(zeichenkette.substr(start, pos - start));
        start = pos + 1;
    } while (pos != string::npos);

    return ergebnis;
}
// ------------------------------------------------------------------------

/* Zeile bezüglich eines Merkmals klassifizieren und in der
   entsprechenden Datei ablegen.
   Diese Funktion ist datenanalystisch gesehen, ein Klassifikator
   basierend auf einem Merkmal
   Input: String, Vektor-Element, resdatei-Template
*/
void klassifikator(string zeichenkette, string merkmal, string resfile, string header_row_save)
{
    // Resultatdatei-Template mit Merkmal-Ausprägung verknüpfen:
    resfile += merkmal;
    resfile += ".csv";

    ofstream resout;

    if (std::filesystem::exists(resfile)) {         // Datei ist vorhanden
        // Nun findet die eigentliche Speicherung an:
        resout.open(resfile, ofstream::app);
        resout << zeichenkette << "\n";
    }
    else {                                          // Datei ist nicht vorhanden, Kopfzeile wird eingefügt!
        resout.open(resfile, ofstream::app);
        resout << header_row_save << "\n";
        resout << zeichenkette << "\n";
    }

    resout.close();
}
// ------------------------------------------------------------------------


//---------------- Funktionen Ende --------------------

int main()
{

    // Variablendeklaration:

    string pgm_version{ "Version 0.0.01, 13.03.2022" };

    const int contfig_row_n{ 14 };               // Anzahl aller Configfile-Informationen

    string config_file{ "config.ini" };          // Muss unbedingt vorhanden sein!
    string logdatei{ "logdatei.txt" };           // Logbuchdatei
    string resdatei{ "" };                       // Ergebnisdatei-Template
    string config_content[contfig_row_n]{ "" };  // Nimmt den Ihnahlt der Configdatei auf
    string ProjektBezeichnung = "";
    string dateiname = "";                       // Bezeichnung der zuverarbeitende Datei
    string processing = "";                      // Soll die eingelesene Datei auf dem Screen gezeigt werden?
    string inZeile{ "" };                        // Eingeleseene Datenzeile
    string trennzeichen{ ";" };                  // Trennzeichen zum separieren des eingelesenen Zeileninhalts
    string header_row{ "" };                     // Ist eine Kopfzeile (Header row) vorhanden?
    string header_row_save{ "" };                // Sicherung der Kopfzeile als String

    vector<string> separierteZeile;              // Eingelesene Datenzeile in Elemente separiert
    vector<string> HeaderInfo;                   // Nimmt die Kopfzeile (z. B. die Merkmalsbezeichnungen) auf

    long long read_n_rows{ 0 };                  // Stopp Ausfuehrung nach n Zeilen (n = 0: ohne Limitation)
    long long in_n{ 0 };                         // Anzahl der eingelesenen Datenzeilen




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
            if (i == 11) header_row = config_content[i];
            if (i == 13) resdatei = config_content[i];

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
    while (!datei_in.eof()) {

        // Kontinuierliches oder eine bestimmte Anzahl Datenzeilen einlesen?
        if (read_n_rows == 0) { // Kontinuierlich

            getline(datei_in, inZeile);

            // Sicherheitsabfrage:
            if (inZeile.length() == 0) {
                break;       // Abbruch, wenn eine leere Zeile eingelsenen wurde!
            }

            // Eingelesenen String zerlegen:
            separierteZeile = ZeichenSeparieren(inZeile, trennzeichen);

            // Kopfzeile der eingelesenen Datei sichern:
            if ((in_n == 0) && (header_row == "Yes")) {

                header_row_save = inZeile;      // Kopfzeile als string sichern
                HeaderInfo = separierteZeile;   // Kopfzeile separiert als Vektor 

            }

            // Eingelesene Daten klassifizieren und spreichern:
            if (in_n > 0) klassifikator(inZeile, separierteZeile[12], resdatei, header_row_save);

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

            // Kopfzeile der eingelsenen Datei sichern:
            if ((in_n == 0) && (header_row == "Yes")) {

                header_row_save = inZeile;       // Kopfzeile als string sichern
                HeaderInfo = separierteZeile;    // Kopfzeile separiert als Vektor 

            }

            // Eingelesene Daten klassifizieren und spreichern:
            if (in_n > 0) klassifikator(inZeile, separierteZeile[12], resdatei, header_row_save);

            // Abbruchbedingung prüfen:
            if (in_n == read_n_rows) {
                break;
            }

            in_n += 1; // Anzahl der eingelesenen Zeilen zählen

        }




        if (processing != "Yes") {

            // Eingelesene Zeile auf ausgeben:
            cout << inZeile << "\n";
        }

    }

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
    log_out << "Number of rows read: " << in_n << "\n";

    // Header Information (How many Cols):
    if (HeaderInfo.empty()) {
        log_out << "No header available or read!\n";
    }
    else {
        log_out << "Haeder-Info:\n";
        for (size_t i = 0; i < HeaderInfo.size(); ++i)
        {
            log_out << i << ".  " << HeaderInfo[i] << "\n";
        }
        log_out << "\n";
    }

    log_out << "Results reported as file(s) with the following template: " << resdatei << "*.csv\n";
    log_out << "The files were saved in the following directory: " << std::filesystem::current_path() << "\n\n";
    log_out << "Execution time in sec: " << ((float)Zeit) / CLOCKS_PER_SEC << "\n";
    log_out << "\n" << "In case of problems, please compare with the config.txt file!" << "\n";
    log_out << "------------ Logbook end ---------------------" << "\n";

    log_out.close();
    // Ende Log-Datei schreiben



    // -------------------------------------------------------------
    return 0;
}


m dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
