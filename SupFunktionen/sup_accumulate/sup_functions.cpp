// Suprenum(sup_)-Suportfunktionen:

#include "sup_functions.h"
#include <string>

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