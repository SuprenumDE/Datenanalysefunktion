#pragma once

// Header zur Unterstützung der Suprenum (sup_)-Programme

// Vermeidung von mehrfacher Inkludierung:
#ifndef SUP_FUNCTIONS_H		// Include Guard!
#define SUP_FUNCTIONS_H

#include <string>
#include <vector>
#include <filesystem>

// Funktionen:
// 
// Tausche Punkt gegen Komma (Dezimaltrennzeichen):
std::string ptoc(std::string zeichenkette);

// Tausche Komma gegen Punkt (Dezimaltrennzeichen):
std::string pcot(std::string zeichenkette);

// Zahl runden:
double ZahlFraktionieren(double zahl, int dezimalstellen);

// Separieren einer Zeichenkette auf Basis eines definierten Trennzeichens
std::vector<std::string>ZeichenSeparieren(std::string zeichenkette, std::string trennzeichen);



#endif // Ende Header-Definition
