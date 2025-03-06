#line 1 "C:\\Users\\felix\\OneDrive - Hans-Böckler-Berufskolleg\\Unterlagen\\FSE2A\\FSE2A_Zwischenprojekt\\SmartKick\\README.md"
# SmartKick
Intelligente Torerfassung für Tischkicker

## Arrays

### controlArray
Das `controlArray` wird verwendet, um Befehle wie Start, Stopp, Reset sowie die Möglichkeit, den Spielstand manuell zu erhöhen oder zu senken, zu steuern.

- `[0]`: Start  1: Start,
                0: Stop
- `[1]`: Reset  1: Reset
- `[2]`: Team 1 Spielstand ändern    1: +1,
                                    -1: -1
- `[3]`: Team 2 Spielstand ändern    1: +1,
                                    -1: -1

### gameConfigArray
Das `gameConfigArray` wird verwendet, um die Spielkonfiguration zu speichern, wie z.B. die Spielzeit, die Anzahl der Tore zum Sieg und den Spielmodus.

- `[0]`: Spielzeit in Minuten
- `[1]`: Tore zum Sieg
- `[2]`: Spielmodus 0: FreePlay, 
                    1: Zeitspiel, 
                    2: Klassisches Spiel

## Konsolenschnittstelle

### Format

`<key> <command> <value>`
- `<key>`       : Das Keyword, unterscheidet zwischen `set` und `get`.
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


### Daten abrufen

- **Spielstand abrufen:** Sende `get score`, um den aktuellen Spielstand abzurufen.
- **Konfiguration abrufen:** Sende `get config`, um die aktuelle Konfiguration abzurufen.