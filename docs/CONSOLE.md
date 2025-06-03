# Serielle Konsole - Erweiterte Steuerung

## 🖥️ Übersicht

Die serielle Konsole bietet vollständige Kontrolle über SmartKick für Entwickler, Techniker und fortgeschrittene Nutzer. Während das Touch-Display für den normalen Spielbetrieb ausreicht, ermöglicht die Konsole individuelle Anpassungen und Debug-Funktionen.

## 🔌 Verbindung herstellen

### Hardware-Anschluss
- **USB-Kabel** zwischen Arduino und Computer
- **Baudrate**: 115200
- **Line Ending**: Both NL & CR (Newline + Carriage Return)

### Software-Optionen
- **Arduino IDE**: Serieller Monitor (Strg+Shift+M)
- **PuTTY**: Serial Connection, COM-Port ermitteln
- **VS Code**: Mit Serial Monitor Extension
- **Terminal**: `screen /dev/ttyUSB0 115200` (Linux/Mac)

### Erste Verbindung
```
SmartKick System gestartet
Debug Level: 1
Nextion Display: Aktiviert
Bereit für Befehle. 'help' für Hilfe.
```

## 📖 Befehlsübersicht

### Hilfe-System
```bash
help          # Allgemeine Hilfe
help get      # GET-Befehle auflisten  
help set      # SET-Befehle auflisten
help nxt      # NXT-Befehle auflisten
```

## 📊 GET-Befehle (Status abfragen)

### Spielstand anzeigen
```bash
get score
```
**Ausgabe:**
```
=== SPIELSTAND ===
Team Blau: 3
Team Gelb: 1  
Modus: Zeitspiel
Verbleibende Zeit: 8:45
Status: Spiel läuft
```

### System-Konfiguration
```bash
get config
```
**Ausgabe:**
```
=== KONFIGURATION ===
Spielmodus: Zeitspiel (1)
Spielzeit: 10 Minuten
Tore zum Sieg: 5
Team 1: Team Blau
Team 2: Team Gelb
Debug-Level: 1
Nextion-Display: Aktiviert
```

## ⚙️ SET-Befehle (Konfiguration ändern)

### Spielmodi einstellen
```bash
set mode free      # Freies Spiel (0)
set mode time      # Zeitspiel (1)  
set mode classic   # Klassisches Spiel (2)
```

### Spielparameter anpassen
```bash
set time 15        # Spielzeit auf 15 Minuten
set time 3         # Oder 3 Minuten für schnelle Tests
set goals 7        # 7 Tore zum Sieg
set goals 20       # Oder 20 für lange Matches
```

**Bereiche:**
- **Zeit**: 1-99 Minuten
- **Tore**: 1-99 Tore

### Spielsteuerung
```bash
set start          # Spiel starten (automatischer Reset)
set stop           # Spiel pausieren
set reset          # Spielstand auf 0:0 zurücksetzen
```

### Punktestand manipulieren
```bash
# Absolute Werte setzen
set score1 5       # Team 1 auf 5 Punkte
set score2 3       # Team 2 auf 3 Punkte

# Relative Änderungen
set score1 +       # Team 1 +1 Punkt
set score1 -       # Team 1 -1 Punkt
set score2 +       # Team 2 +1 Punkt  
set score2 -       # Team 2 -1 Punkt
```

### Teamnamen anpassen
```bash
set name1 "Heim"                 # Kurze Namen
set name2 "Gast"                 # Für bessere Display-Darstellung
```

### Debug-System
```bash
set debug 0        # Nur kritische Fehler
set debug 1        # Standard: Wichtige Events (empfohlen)
set debug 2        # Ausführlich: Alle Befehle und Aktionen
```

**Debug-Level Beispiele:**

**Level 1 (Standard):**
```
Tor für Team 1
Spielstand: 1:0
Spiel gestartet
Zeitspiel beendet - Zeit abgelaufen
```

**Level 2 (Entwicklung):**
```
Verarbeite Befehl: 'set mode time'
→ HandleSetCommand mit Parameter: mode time
→ Spielmodus auf Zeitspiel gesetzt
→ Nextion Update: t4.txt="Zeitspiel"
Interrupt Tor 1 - Zeit seit letztem: 1250ms
→ Entprellung erfolgreich, Tor akzeptiert
```

### Display-Steuerung
```bash
set nextion on     # Display aktivieren
set nextion off    # Display deaktivieren (Debug/Test)
```

## 📱 NXT-Befehle (Direkte Display-Steuerung)

### Grundlagen
Nextion-Befehle werden direkt an das Display weitergeleitet. Format: `nxt <command>`

### Seitenwechsel
```bash
nxt page 0         # Zur Hauptseite
nxt page 1         # Zur Menüseite (falls vorhanden)
```

### Textfelder setzen
```bash
nxt t0.txt="SmartKick Pro"        # Titel ändern
nxt t1.txt="15:30"                # Zeit manuell setzen
nxt t2.txt="Heimmannschaft"       # Team 1 Name
nxt t3.txt="Gäste"               # Team 2 Name
nxt t4.txt="Turniermodus"        # Modus-Anzeige
```

### Numerische Werte
```bash
nxt n0.val=7       # Team 1 Punkte auf 7
nxt n1.val=4       # Team 2 Punkte auf 4
```

### Display-Eigenschaften
```bash
nxt dim=50         # Helligkeit auf 50% (0-100)
nxt sleep=1        # Display in Standby
nxt sleep=0        # Display aktivieren
```

## 🎯 Praktische Anwendungsfälle

### 🏆 Turnier-Setup
```bash
# Turnier-Konfiguration in einem Zug
set mode time
set time 8
set name1 "Team Alpha"
set name2 "Team Beta"  
set debug 1
set start
```

### 🔧 System-Test
```bash
# Vollständiger Funktionstest
set debug 2                    # Vollständige Ausgabe
get config                     # Status prüfen
set mode classic               # Klassischen Modus testen
set goals 3                    # Kurzes Spiel für Test
set name1 "Test1"             # Testnamen
set name2 "Test2"
set start                      # Test starten
set score1 +                   # Manueller Punkt
set score2 +                   # Manueller Punkt
```

### 🎮 Individuelle Spielmodi
```bash
# 3-Minuten Blitz-Turnier
set mode time
set time 3
set name1 "Speed1"
set name2 "Speed2"
set start

# Marathon-Match bis 15 Tore
set mode classic  
set goals 15
set name1 "Marathon A"
set name2 "Marathon B"
set start
```

### 🐛 Debug & Entwicklung
```bash
# Debug-Session starten
set debug 2                    # Alles protokollieren
set nextion off               # Display ausschalten für Konsole-Fokus
get config                    # Basis-Status
set score1 5                  # Testdaten setzen
set score2 3
nxt t0.txt="DEBUG MODE"       # Display-Kennzeichnung
set nextion on                # Display wieder einschalten
```

## 🔍 Fehlerbehebung

### Verbindungsprobleme
```bash
# Kein Output im seriellen Monitor
1. Baudrate auf 115200 prüfen
2. Line Ending auf "Both NL & CR" stellen
3. USB-Kabel und Port prüfen
4. Arduino Reset-Button drücken

# "Command not found" Fehler
→ 'help' eingeben für verfügbare Befehle
→ Syntax prüfen: Leerzeichen zwischen Befehl und Parameter
```

### Display-Probleme
```bash
# Display reagiert nicht auf nxt-Befehle
set nextion off    # Display reset
set nextion on     # Wieder aktivieren

# Display zeigt falsche Werte
get config         # Aktuelle Werte prüfen
nxt t1.txt="--:--" # Zeit manuell zurücksetzen
set reset          # Komplett neu starten
```

### Sensor-Probleme
```bash
# Tore werden nicht erkannt
set debug 2        # Detaillierte Sensor-Ausgabe
→ Hand vor Sensor halten und Debug-Output beobachten
→ Bei "Interrupt detected" aber "Debounce failed": Sensor-Position prüfen

# Doppelte Tore
→ Debounce-Zeit im Code erhöhen (Standard: 500ms)
→ Sensor-Abstand zum Tor vergrößern
```

## 💡 Experten-Tipps

### Effizienter Workflow
```bash
# Favoriten-Konfiguration speichern (manuell notieren)
set mode time && set time 10 && set debug 1 && set start

# Schneller Neustart zwischen Tests
set reset && set start

# Batch-Befehle mit Copy-Paste
set mode classic
set goals 5  
set name1 "Team1"
set name2 "Team2"
set start
```

### Erweiterte Display-Kontrolle
```bash
# Custom Anzeigetexte
nxt t4.txt="FINALE!"          # Besondere Matches kennzeichnen
nxt t0.txt="TURNIER 2025"     # Event-Branding

# Farben ändern (falls im HMI definiert)
nxt t2.pco=31                 # Team 1 blau (RGB565)
nxt t3.pco=63488              # Team 2 rot (RGB565)
```

### Performance-Optimierung
```bash
# Minimal-Setup für beste Performance
set debug 0        # Keine Debug-Ausgaben
set nextion on     # Display aktiv aber sparsam updaten

# Test-Setup für Entwicklung
set debug 2        # Maximale Informationen
set nextion off    # Display ausschalten für reine Konsole
```

## 🚨 Wichtige Hinweise

### ⚠️ Sicherheit
- **Keine gefährlichen Befehle**: Alle SET-Befehle sind sicher
- **Kein Datenverlust**: Änderungen sind nur temporär (bis Neustart)
- **Fail-Safe**: Ungültige Werte werden automatisch korrigiert

### 🔧 Grenzen beachten
- **Teamnamen**: Maximal 20 Zeichen (Display-Begrenzung)
- **Zeit**: 1-99 Minuten (Byte-Bereich)
- **Tore**: 1-99 Tore (Byte-Bereich)
- **Debug**: Level 0-2 (höhere Werte = Level 2)

### 💾 Persistenz
- **Alle Einstellungen** gehen bei Neustart verloren
- **Standard-Werte** werden automatisch geladen
- **Dauerhafte Änderungen** nur durch Code-Anpassung

---

**Navigation**: [← Zurück zur Hauptbedienung](USAGE.md) | [Hardware →](HARDWARE.md) | [Software →](SOFTWARE.md)