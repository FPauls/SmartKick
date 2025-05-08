#line 1 "C:\\Projects\\Arduino\\SmartKick\\README.md"
# SmartKick
> Intelligente Torerfassung für Tischkicker

<img alt="Version" src="https://img.shields.io/badge/version-1.0-blue">
<img alt="License" src="https://img.shields.io/badge/license-MIT-green">
<img alt="Arduino" src="https://img.shields.io/badge/Arduino-Compatible-teal">

## 📋 Inhalt

- [📝 Projektbeschreibung](#-projektbeschreibung)
- [✨ Features](#-features)
- [🧰 Komponenten](#-komponenten)
- [🖥️ Konsolenschnittstelle](#️-konsolenschnittstelle)
  - [Allgemeine Hilfe](#allgemeine-hilfe)
  - [GET-Befehle](#get-befehle)
  - [SET-Befehle](#set-befehle)
  - [NXT-Befehle](#nxt-befehle)
  - [Beispiele](#beispiele)
- [🏫 Projektinformationen](#-projektinformationen)
- [📄 Lizenz](#-lizenz)

## 📝 Projektbeschreibung
Hey! Wir sind angehende Elektrotechniker und das hier ist unser Schulprojekt. Wir haben uns vorgenommen, einen vorhandenen Tischkicker mit einer digitalen Auswertung zu erweitern. Die Veröffentlichung dieses Projekts ist nur ein Bonus. Wenn jemand anderes es nützlich findet, ist das ein Gewinn für uns alle!

Wir entwickeln eine digitale Toranzeige als Nachrüstsatz für bestehende Tischkicker. Das System soll erzielte Tore automatisch erfassen, verschiedene Spielmodi unterstützen und das Spielerlebnis durch eine moderne Benutzeroberfläche verbessern.

Hierfür integrieren wir ein Touch-Display in ein 3D-gedrucktes Gehäuse. Die Tore werden durch Sensoren erfasst und die Steuerung erfolgt über einen Mikrocontroller.

## ✨ Features
Im Vergleich zu klassischen Anzeigen bietet das Touch-Display:

Dynamische Anzeige: Spielmodi, Animationen und visuelle Effekte
Touch-Bedienung: Direkte Steuerung über das Display
Erweiterte Funktionen: Darstellung von Teamnamen, Spielständen und Timern
Durch moderne Sensortechnik und interaktive Visualisierung schaffen wir eine innovative Lösung für Tischkicker-Enthusiasten.

## 🧰 Komponenten
- Arduino Mikrocontroller
- Nextion Touch-Display
- Lichtschranken zur Torerkennung
- RGB-LED für Visualisierung
- 3D-gedrucktes Gehäuse

## 🖥️ Konsolenschnittstelle
#### Allgemeine Hilfe
```
=== SMARTKICK HILFE ===
help          - Diese Hilfe
help get      - GET-Befehle anzeigen
help set      - SET-Befehle anzeigen
help nxt      - NXT-Befehle anzeigen
```

#### GET-Befehle
```
=== GET-BEFEHLE ===
get score     - Zeigt Spielstand an
get config    - Zeigt Konfiguration an
```

#### SET-Befehle
```
=== SET-BEFEHLE ===
set mode free|time|classic - Spielmodus setzen
set time INT               - Spielzeit in Minuten
set goals INT              - Tore zum Sieg
set start|stop|reset       - Spielsteuerung
set score1|score2 INT|+|-  - Spielstand setzen/ändern
set name1|name2 STRING     - Teamnamen setzen
set debug 0|1|2            - Debug-Level einstellen
set nextion on|off         - Nextion-Display aktivieren/deaktivieren
```

#### NXT-Befehle

```
=== NXT-BEFEHLE ===
nxt COMMAND   - Sendet Befehl an Nextion-Display
```

### Beispiele
#### Spielmodi einstellen
```
set mode free     # Freies Spiel ohne Zeitlimit/Siegbedingung
set mode time     # Zeitspiel mit festgelegter Spielzeit
set mode classic  # Klassisches Spiel mit definierter Torzahl zum Sieg
```
#### Spielkonfiguration
```
set time 10       # Spielzeit auf 10 Minuten setzen
set goals 5       # 5 Tore zum Sieg erforderlich
set name1 Blau    # Team 1 auf "Blau" umbenennen
set name2 Gelb    # Team 2 auf "Gelb" umbenennen
```
#### Spielsteuerung
```
set start         # Spiel starten (setzt Spielstand zurück)
set stop          # Spiel anhalten
set reset         # Spielstand zurücksetzen
```
#### Spielstand-Verwaltung
```
set score1 3      # Spielstand Team 1 auf 3 setzen
set score2 0      # Spielstand Team 2 auf 0 setzen
set score1 +      # Spielstand Team 1 erhöhen
set score2 -      # Spielstand Team 2 verringern
```
#### System-Einstellungen
```
set debug 0       # Keine Debug-Ausgaben
set debug 1       # Wichtige Debug-Meldungen
set debug 2       # Ausführliche Debug-Meldungen
set nextion on    # Nextion-Display aktivieren
set nextion off   # Nextion-Display deaktivieren
```
## 🏫 Projektinformationen
Dieses Projekt wurde mit Unterstützung und im Auftrag des Hans-Böckler-Berufskollegs erarbeitet.

Bitte beachte, dass wir das Projekt eventuell nach Abschluss der Technikerschule wieder offline nehmen können und keine Garantien stellen können.

## 📄 Lizenz
Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die LICENSE-Datei für weitere Details.

<p align="center"> <sub>Made with ❤️ by FSE2A-Team SmartKick | © 2025</sub> </p>