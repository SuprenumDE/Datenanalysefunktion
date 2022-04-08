// Suprenum(sup_)-Suportfunktionen:

#include "sup_functions.h"
#include <string>
#include <vector>

// Dezimalpunkt gegen Dezimalkomma ersetzen:
std::string ptoc(std::string zeichenkette)
{

	std::string dezimalzeichen{ "." };
	std::string dezimal_neu{ "," };
	size_t position{ 0 };

	while ((position = zeichenkette.find(dezimalzeichen, position)) != std::string::npos)
	{

		zeichenkette.replace(position, dezimalzeichen.size(), dezimal_neu);

	}


	return zeichenkette;

}
// ------------------------------------------------------------------------

// Dezimalkomma gegen Dezimalpunkt ersetzen:
std::string pcot(std::string zeichenkette)
{

	std::string dezimalzeichen{ "," };
	std::string dezimal_neu{ "." };
	size_t position{ 0 };

	while ((position = zeichenkette.find(dezimalzeichen, position)) != std::string::npos)
	{

		zeichenkette.replace(position, dezimalzeichen.size(), dezimal_neu);

	}


	return zeichenkette;

}
// ------------------------------------------------------------------------

/* Dezimalzahl runden
   Weil die Rundungsfunktionen von mtah.h und der int-Trick unbefriedigend sind
   (es pfuscht immer die Rechnergenauigkeit rein) wurde diese Rundungsfunktion
   über einen String geschrieben.
 */
double ZahlFraktionieren(double zahl, int dezimalstellen)
{

	std::string zahl_string{ "" };
	int laenge{ 0 };
	int dezimalpos{ 0 };
	int neu_laenge{ 0 };

	double ergebnis{ 0.0 };

	zahl_string = std::to_string(zahl);
	laenge = zahl_string.size();
	dezimalpos = zahl_string.find(".", 0);


	// Berechnung der neuen Stringlänge zur Extraktion:
	neu_laenge = dezimalpos + dezimalstellen + 1;

	// Extraktion:
	zahl_string = zahl_string.std::string::substr(0, neu_laenge);

	ergebnis = std::stod(zahl_string);

	return ergebnis;

}
// ------------------------------------------------------------------------

/* Zerlegung der übergebenen Zeichenkette in einzelne Elemente
   Input: String (zeichenkette, trennzeichen)
   Output: Vector
*/
std::vector<std::string>ZeichenSeparieren(std::string zeichenkette, std::string trennzeichen)
{
    std::string::size_type start{ 0 };
    std::string::size_type pos{ 0 };
    std::vector<std::string> ergebnis;

    do {
        pos = zeichenkette.find_first_of(trennzeichen, start);
        ergebnis.push_back(zeichenkette.substr(start, pos - start));
        start = pos + 1;
    } while (pos != std::string::npos);

    return ergebnis;
}
// ------------------------------------------------------------------------
