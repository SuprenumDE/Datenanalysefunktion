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
