# SmartKick
Intelligente Torerfassung für Tischkicker

## Projektbeschreibung

Hey! Wir sind angehende Elektrotechniker und das hier ist unser Schulprojekt. Wir haben uns vorgenommen, einen vorhandenen Tischkicker mit einer digitalen Auswertung zu erweitern. Die Veröffentlichung dieses Projekts ist nur ein Bonus. Wenn jemand anderes es nützlich findet, ist das ein Gewinn für uns alle!

Wir entwickeln eine digitale Toranzeige als Nachrüstsatz für bestehende Tischkicker. Das System soll erzielte Tore automatisch erfassen, verschiedene Spielmodi unterstützen und das Spielerlebnis durch eine moderne Benutzeroberfläche verbessern.

Hierfür integrieren wir ein Touch-Display in ein 3D-gedrucktes Gehäuse. Die Tore werden durch Sensoren erfasst und die Steuerung erfolgt über einen Mikrocontroller.

Im Vergleich zu klassischen Anzeigen bietet das Touch-Display:

- **Dynamische Anzeige:** Spielmodi, Animationen und visuelle Effekte
- **Touch-Bedienung:** Direkte Steuerung über das Display
- **Erweiterte Funktionen:** Darstellung von Teamnamen, Spielständen und Timern

Durch moderne Sensortechnik und interaktive Visualisierung schaffen wir eine innovative Lösung für Tischkicker-Enthusiasten.

Dieses Projekt wurde mit Unterstützung und im Auftrag des [Hans-Böckler-Berufskollegs](https://www.hbbk-muenster.de/bildungsgaenge/fst/projekte/) erarbeitet.

Bitte beachte, dass wir das Projekt eventuell nach Abschluss der Technikerschule wieder offline nehmen können und keine Garantien stellen können.

## Konsolenschnittstelle

### Format

`<key> <command> <value>`
- `<key>`       : Das Keyword, unterscheidet zwischen `set`, `get` und `nxt`.
- `<command>`   : Der Befehl, der ausgeführt werden soll.
- `<value>`     : Der Wert, der dem Befehl zugewiesen werden soll.

### Beispiele

| Befehl             | Beschreibung                                      |
|--------------------|---------------------------------------------------|
| `set mode free`    | Setzt den Spielmodus auf FreePlay                 |
| `set mode time`    | Setzt den Spielmodus auf Zeitspiel                |
| `set mode classic` | Setzt den Spielmodus auf Klassisches Spiel        |
| `set time 10`      | Setzt die Spielzeit auf 10 Minuten                |
| `set goals 5`      | Setzt die Tore zum Sieg auf 5                     |
| `set start`        | Startet das Spiel                                 |
| `set stop`         | Stoppt das Spiel                                  |
| `set reset`        | Setzt das Spiel zurück                            |
| `set score1 0`     | Setzt den Spielstand für Team 1 auf 0             |
| `set score2 3`     | Setzt den Spielstand für Team 2 auf 3             |
| `nxt score2 +`     | Erhöht den Spielstand für das aktuelle Team       |
| `nxt score1 -`     | Verringert den Spielstand für das aktuelle Team   |
|                    |                                                   |
| `get score`        | Gibt den aktuellen Spielstand aus                 |
| `get config`       | Gibt die aktuelle Spielkonfiguration aus          |

### Hinweise
- Die Eingaben müssen über die serielle Konsole gesendet werden.
- Die Werte werden sofort aktualisiert und die Änderungen werden in der Konsole bestätigt.

### Spiel starten, stoppen und zurücksetzen

- **Spiel starten:** Sende `set start`, um das Spiel zu starten.
- **Spiel stoppen:** Sende `set stop`, um das Spiel zu stoppen.
- **Spiel zurücksetzen:** Sende `set reset`, um das Spiel zurückzusetzen.

### Zeiten einstellen

- **Spielzeit auf 10 Minuten setzen:** Sende `set time 10`, um die Spielzeit auf 10 Minuten zu setzen.
- **Spielzeit auf 5 Minuten setzen:** Sende `set time 5`, um die Spielzeit auf 5 Minuten zu setzen.

### Modi umschalten

- **Spielmodus auf FreePlay setzen:** Sende `set mode free`, um den Spielmodus auf FreePlay zu setzen.
- **Spielmodus auf Zeitspiel setzen:** Sende `set mode time`, um den Spielmodus auf Zeitspiel zu setzen.
- **Spielmodus auf Klassisches Spiel setzen:** Sende `set mode classic`, um den Spielmodus auf Klassisches Spiel zu setzen.

### Spielstand korrigieren

- **Spielstand für Team 1 auf 3 setzen:** Sende `set score1 3`, um den Spielstand für Team 1 auf 3 setzen.
- **Spielstand für Team 2 auf 0 setzen:** Sende `set score2 0`, um den Spielstand für Team 2 auf 0 setzen.
- **Spielstand für Team 1 um 1 zu erhöhen:** Sende `set score1 +`, um den Spielstand für Team 1 um 1 zu erhöhen.
- **Spielstand für Team 2 um 1 zu senken:** Sende `set score2 -`, um den Spielstand für Team 2 um 1 zu senken.

### Daten abrufen

- **Spielstand abrufen:** Sende `get score`, um den aktuellen Spielstand abzurufen.
- **Konfiguration abrufen:** Sende `get config`, um die aktuelle Konfiguration abzurufen.

### Befehle an Nextion senden

- **Nextion Befehl senden:** Sende `nxt <command>`, um einen befehl an das Nextion Display zu senden.

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die [LICENSE](LICENSE)-Datei für weitere Details.

Bitte beachte, dass wir das Projekt eventuell nach Abschluss der Technikerschule wieder offline nehmen können und keine Garantien stellen können.
