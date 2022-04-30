################################################################################
## Geo-Daten mit R darstellen
##
## Dieses R-Skript ist eine mögliche Ergänzung zur Darstellung der mit dem Daten-
## analyse-Supportprogramm sup_rac2 klassifizierten NY-Taxi-Daten. Dieses Programm
## ist im Rahmen meiner C++-Lernreise "Datenanalysefunktionen" entstanden.
## Siehe Github:
## https://github.com/SuprenumDE/Datenanalysefunktion/tree/main/SupFunktionen/sup_rac2
##
## Skript zum Video "Geo-Daten mit R"
##
## Ad-Oculos-Projekt, https://www.faes.de/ad-oculos/
## Günter Faes
## Version 1.0, 27.04.2022
## R-Version: 4.2.0
## RStudio-Version: 2022.02.1 Build 461
##
## ggmap:
## D. Kahle and H. Wickham. ggmap: Spatial Visualization with ggplot2. The R Journal,
## 5(1), 144-161. URL http://journal.r-project.org/archive/2013-1/kahle-wickham.pdf
################################################################################

## Pakete:
library(ggplot2)
library(ggmap)

## Google-Registrierung:
register_google(key = "A----------------------------------M", write = TRUE)

## Ein paar Daten zum Datensatz:
n_Beobachtungen <- length(classified_data_3$key)
cat("Anzahl Beobachtungen: ", n_Beobachtungen, "\n")

# Beobachtungszeitraum
D_von <- as.character.Date(min(classified_data_3$pickup_Datum))
D_bis <- as.character.Date(max(classified_data_3$pickup_Datum))
cat("Selektierter Beobachtungszeitraum von ", D_von , "bis ", D_bis, "\n")

# Geo-Daten zum Anlegen der NY-Karte:
# New York: lon: -74.0059700, lat: 40.7142700 (Dezimal, Google Maps)
lat_von <- min(classified_data_3$Startlatitude)
lat_bis <- max(classified_data_3$Startlatitude)
cat("Latitude (Breitengrad) von ", lat_von, "bis ", lat_bis, "\n")

long_von <- min(classified_data_3$Startlongitude)
long_bis <- max(classified_data_3$Startlongitude)
cat("Longitude (Längengrad) von ", long_von, "bis ", long_bis, "\n")



## Karte von NY laden:
NY <- get_map(location = c(left = long_von, right = long_bis,
                           bottom = lat_von, top = lat_bis),
              zoom = 12)

# Karte & Beobachtungen für 3 km Distanzen ausgeben:
ggmap(NY) + geom_point(aes(x=Startlongitude, y = Startlatitude),
                       data = classified_data_3,
                       alpha = 0.5,
                       color = "darkred",
                       size = 1) +
  labs(title = "Startpunkte NY-Taxifahrten < 3 km",
       subtitle = "Datensatz: classified_data_3",
       x = "(Dezimal-) Längengrad",
       y = "(Dezimal-) Breitengrad")

# Karte & Beobachtungen für 5 km Distanzen ausgeben:
ggmap(NY) + geom_point(aes(x=Startlongitude, y = Startlatitude),
                       data = classified_data_5,
                       alpha = 0.5,
                       color = "blue",
                       size = 1) +
  labs(title = "Startpunkte NY-Taxifahrten < 5 km",
       subtitle = "Datensatz: classified_data_5",
       x = "(Dezimal-) Längengrad",
       y = "(Dezimal-) Breitengrad")

