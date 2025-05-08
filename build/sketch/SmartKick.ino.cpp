#line 1 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
#include <Arduino.h>       // Arduino-Basisbibliothek
#include <AltSoftSerial.h> // Serielle Kommunikation für Nextion

// Debug- und Nextion-Einstellungen
byte debugLevel = 1;        // 0=keine, 1=wichtig, 2=ausführlich
bool nextionEnabled = true; // true=aktiviert, false=deaktiviert

// Debug-Makros für verschiedene Ausgabelevels
#define DEBUG_0_PRINT(x) Serial.print(x) // Ausgabe immer (Level 0)
#define DEBUG_0_PRINTLN(x) Serial.println(x)
#define DEBUG_0_PRINT_F(x) Serial.print(F(x)) // Mit Flash-Speicher-Optimierung
#define DEBUG_0_PRINTLN_F(x) Serial.println(F(x))

#define DEBUG_1_PRINT(x) \
  if (debugLevel >= 1)   \
  Serial.print(x) // Wichtige Meldungen (Level 1)
#define DEBUG_1_PRINTLN(x) \
  if (debugLevel >= 1)     \
  Serial.println(x)
#define DEBUG_1_PRINT_F(x) \
  if (debugLevel >= 1)     \
  Serial.print(F(x))
#define DEBUG_1_PRINTLN_F(x) \
  if (debugLevel >= 1)       \
  Serial.println(F(x))

#define DEBUG_2_PRINT(x) \
  if (debugLevel >= 2)   \
  Serial.print(x) // Ausführliche Meldungen (Level 2)
#define DEBUG_2_PRINTLN(x) \
  if (debugLevel >= 2)     \
  Serial.println(x)
#define DEBUG_2_PRINT_F(x) \
  if (debugLevel >= 2)     \
  Serial.print(F(x))
#define DEBUG_2_PRINTLN_F(x) \
  if (debugLevel >= 2)       \
  Serial.println(F(x))

byte pinArrayInput[2]{
    // Array für Eingangs-Pins
    2, // Lichtschranke Tor 1 (Interrupt)
    3  // Lichtschranke Tor 2 (Interrupt)
};

byte pinArrayOutput[4]{
    // Array für RGB-LED-Pins
    4, // Rot-Pin für RGB-LED
    5, // Grün-Pin für RGB-LED
    6, // Blau-Pin für RGB-LED
    7  // GND-Pin für RGB-LED
};

volatile unsigned long interruptData[4]{
    // Array für Interrupt-Daten
    0,    // Letzte Interrupt-Zeit für Tor 1
    0,    // Letzte Interrupt-Zeit für Tor 2
    1000, // Entprellzeit in Millisekunden
    0     // Flag für Spielstandaktualisierung
};

unsigned long scoreArray[6]{
    // Array für Spielstand-Daten
    0, // Spielstand Team 1
    0, // Spielstand Team 2
    0, // Sieger-Team (0=unentschieden)
    0, // Spielendestatus (1=beendet)
    0, // Startzeitstempel in Millisek.
    0  // Spielzeit in Sekunden
};

byte configArray[4]{
    // Array für Spielkonfiguration
    5,  // Spielzeit in Minuten
    10, // Tore zum Sieg
    2,  // Spielmodus (0:Free,1:Time,2:Classic)
    0   // Control (1:Start,0:Stop,2:Reset)
};

String teamNames[2] = {
    // Array für Teamnamen
    "Team Blau", // Name Team 1 (blau)
    "Team Gelb"  // Name Team 2 (gelb)
};

const char *nextionTextFields[7] = {
    // Array für Nextion-Textfelder
    "t0.txt", // Überschrift
    "t1.txt", // Zeit
    "t2.txt", // Teamname 1
    "n0.val", // Spielstand 1
    "t3.txt", // Teamname 2
    "n1.val", // Spielstand 2
    "t4.txt"  // Spielmodus
};

class RGBLED
{ // Klasse zur Steuerung der RGB-LED
private:
  byte redPin;   // Pin für rote Farbe
  byte greenPin; // Pin für grüne Farbe
  byte bluePin;  // Pin für blaue Farbe
  byte gndPin;   // Gemeinsamer GND-Pin

public:
  RGBLED(byte redPin, byte greenPin, byte bluePin, byte gndPin)
  { // Konstruktor mit Pin-Konfiguration
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->gndPin = gndPin;
    pinMode(redPin, OUTPUT); // Pin-Modus auf Ausgang setzen
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(gndPin, OUTPUT);
    digitalWrite(gndPin, LOW); // GND-Pin auf LOW setzen
  }

  void on(byte red, byte green, byte blue)
  { // LED mit Farbwerten einschalten
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }

  void off()
  { // LED ausschalten
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(greenPin, 0); // Doppelter Aufruf (Fehler)
    analogWrite(bluePin, 0);
  }

  void blink(int times, int duration, byte red, byte green, byte blue)
  { // LED-Blinkfunktion
    for (int i = 0; i < times; i++)
    {                       // Wiederholt die Anzahl der Male
      on(red, green, blue); // LED einschalten
      delay(duration);      // Zeit warten
      off();                // LED ausschalten
      delay(duration);      // Zeit warten
    }
  }
};

RGBLED rgbLed(pinArrayOutput[0], pinArrayOutput[1], pinArrayOutput[2], pinArrayOutput[3]); // RGB-LED-Objekt erstellen
AltSoftSerial nextionSerial;                                                               // Nextion-Serielle-Verbindung

#line 149 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
bool debounce(volatile unsigned long &lastmillis);
#line 159 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void ISR_Goal1();
#line 169 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void ISR_Goal2();
#line 179 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void printScore();
#line 243 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void printConfig();
#line 280 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void initializeGameTime();
#line 289 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
unsigned long getElapsedTime();
#line 294 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
unsigned long getRemainingTime();
#line 300 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
unsigned long calculateElapsedTime();
#line 323 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void checkGameEnd();
#line 390 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void resetGame();
#line 402 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void startGame();
#line 455 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void stopGame();
#line 468 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void processCommand(String input);
#line 528 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleGetCommand(String params);
#line 561 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetCommand(String params);
#line 638 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleNxtCommand(String params);
#line 652 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleHelpCommand(String params);
#line 685 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetMode(String value);
#line 700 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetTime(String value);
#line 711 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetGoals(String value);
#line 719 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetScore(int teamIndex, String value);
#line 760 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetName(int teamIndex, String value);
#line 769 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void handleSetFlag(int flagIndex, String value);
#line 819 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void sendCommandToNextion(const String &command);
#line 834 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
String readNextionResponse();
#line 890 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void updateNextion();
#line 981 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void setup();
#line 1001 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
void loop();
#line 149 "C:\\Projects\\Arduino\\SmartKick\\SmartKick.ino"
bool debounce(volatile unsigned long &lastmillis)
{ // Entprellfunktion für Interrupts
  if (millis() - lastmillis > interruptData[2])
  {                        // Prüft, ob Entprellzeit verstrichen ist
    lastmillis = millis(); // Aktualisiert Zeitstempel
    return true;           // Signal entprellt
  }
  return false; // Signal ignorieren (Prellen)
}

void ISR_Goal1()
{ // Interrupt-Service-Routine für Tor 1
  if (configArray[3] == 1 && debounce(interruptData[0]) && scoreArray[3] == 0)
  {                                  // Prüft Spielstatus und Entprellung
    scoreArray[0]++;                 // Zählt Tor für Team 1
    rgbLed.blink(1, 500, 0, 0, 255); // Blaue LED für Team 1 blinken lassen
    interruptData[3] = 1;            // Anzeige-Update-Flag setzen
  }
}

void ISR_Goal2()
{ // Interrupt-Service-Routine für Tor 2
  if (configArray[3] == 1 && debounce(interruptData[1]) && scoreArray[3] == 0)
  {                                    // Prüft Spielstatus und Entprellung
    scoreArray[1]++;                   // Zählt Tor für Team 2
    rgbLed.blink(1, 500, 255, 255, 0); // Gelbe LED für Team 2 blinken lassen
    interruptData[3] = 1;              // Anzeige-Update-Flag setzen
  }
}

void printScore()
{                                          // Funktion zur Ausgabe des Spielstands
  static String previousScoreString = "";  // Speichert letzten Spielstand
  static unsigned long lastTimeUpdate = 0; // Speichert letzten Zeitwert

  String currentScoreString = String(scoreArray[0]) + ":" + String(scoreArray[1]); // Aktueller Spielstand als String

  unsigned long currentTime = calculateElapsedTime(); // Aktuelle Spielzeit berechnen

  if (currentScoreString != previousScoreString ||
      currentTime != lastTimeUpdate ||
      interruptData[3] == 1)
  {                                          // Prüft, ob Update nötig ist
    DEBUG_1_PRINTLN_F("=== SPIELSTAND ==="); // Überschrift ausgeben
    DEBUG_1_PRINT(teamNames[0]);
    DEBUG_1_PRINT_F(": ");
    DEBUG_1_PRINTLN(scoreArray[0]); // Team 1 Punktestand
    DEBUG_1_PRINT(teamNames[1]);
    DEBUG_1_PRINT_F(": ");
    DEBUG_1_PRINTLN(scoreArray[1]); // Team 2 Punktestand

    // Spielmodus anzeigen
    DEBUG_1_PRINT_F("Modus: ");
    switch (configArray[2])
    {
    case 0:
      DEBUG_1_PRINTLN_F("Freies Spiel"); // Modus 0: Freies Spiel
      break;
    case 1:
      DEBUG_1_PRINTLN_F("Zeitspiel"); // Modus 1: Zeitspiel
      break;
    case 2:
      DEBUG_1_PRINTLN_F("Klassisches Spiel"); // Modus 2: Klassisches Spiel
      break;
    default:
      DEBUG_2_PRINTLN_F("Unbekannt"); // Unbekannter Modus
      break;
    }

    // Zeit formatiert anzeigen
    unsigned long minutes, seconds;
    minutes = currentTime / 60; // Minuten berechnen
    seconds = currentTime % 60; // Sekunden berechnen

    if (configArray[2] == 1)
    { // Bei Zeitspiel verbleibende Zeit
      DEBUG_1_PRINT_F("Verbleibende Zeit: ");
    }
    else
    { // Sonst verstrichene Zeit
      DEBUG_1_PRINT_F("Verstrichene Zeit: ");
    }
    DEBUG_1_PRINT(minutes); // Minuten ausgeben
    DEBUG_1_PRINT_F(":");   // Doppelpunkt ausgeben
    if (seconds < 10)
      DEBUG_1_PRINT_F("0");   // Führende Null bei einstelligen Sekunden
    DEBUG_1_PRINTLN(seconds); // Sekunden ausgeben

    previousScoreString = currentScoreString; // Werte für nächsten Vergleich speichern
    lastTimeUpdate = currentTime;
    interruptData[3] = 0; // Update-Flag zurücksetzen
  }
}

void printConfig()
{                                             // Konfiguration ausgeben
  DEBUG_0_PRINTLN_F("=== KONFIGURATION ==="); // Überschrift

  DEBUG_0_PRINT_F("Spielmodus: "); // Spielmodus anzeigen
  switch (configArray[2])
  {
  case 0:
    DEBUG_0_PRINTLN_F("Freies Spiel");
    break;
  case 1:
    DEBUG_0_PRINTLN_F("Zeitspiel");
    break;
  case 2:
    DEBUG_0_PRINTLN_F("Klassisches Spiel");
    break;
  default:
    DEBUG_2_PRINTLN_F("Unbekannt");
    break;
  }

  DEBUG_0_PRINT_F("Spielzeit: "); // Spielzeit in Minuten
  DEBUG_0_PRINT(configArray[0]);
  DEBUG_0_PRINTLN_F(" Minuten");

  DEBUG_0_PRINT_F("Tore zum Sieg: "); // Benötigte Tore zum Sieg
  DEBUG_0_PRINTLN(configArray[1]);

  DEBUG_0_PRINT_F("Debug-Level: "); // Aktueller Debug-Level
  DEBUG_0_PRINTLN(debugLevel);

  DEBUG_0_PRINT_F("Nextion-Display: "); // Nextion-Display-Status
  DEBUG_0_PRINTLN(nextionEnabled ? "Aktiviert" : "Deaktiviert");

  DEBUG_0_PRINTLN_F("-------------------"); // Abschlusszeile
}

void initializeGameTime()
{                           // Spielzeit initialisieren
  scoreArray[4] = millis(); // Aktueller Zeitstempel speichern
  if (configArray[2] == 1 || configArray[2] == 2)
  {                                      // Bei Zeit- oder Klassischem Spiel
    scoreArray[5] = configArray[0] * 60; // Minuten in Sekunden umrechnen
  }
}

unsigned long getElapsedTime()
{                                           // Verstrichene Zeit berechnen
  return (millis() - scoreArray[4]) / 1000; // Zeit in Sekunden seit Spielbeginn
}

unsigned long getRemainingTime()
{                                                                       // Verbleibende Zeit berechnen
  unsigned long elapsedTime = getElapsedTime();                         // Verstrichene Zeit
  return scoreArray[5] > elapsedTime ? scoreArray[5] - elapsedTime : 0; // Verbleibende Zeit oder 0
}

unsigned long calculateElapsedTime()
{ // Spielzeit je nach Modus berechnen
  if (scoreArray[3] == 1)
  {                       // Bei Spielende
    return scoreArray[5]; // Endzeit zurückgeben
  }
  else if (configArray[3] == 1)
  { // Bei laufendem Spiel
    if (configArray[2] == 1)
    {                            // Bei Zeitspiel-Modus
      return getRemainingTime(); // Verbleibende Zeit
    }
    else
    {                          // Bei anderen Modi
      return getElapsedTime(); // Verstrichene Zeit
    }
  }
  else
  {           // Bei gestopptem Spiel
    return 0; // Zeit ist 0
  }
}

void checkGameEnd()
{ // Prüft, ob Spielende erreicht ist
  if (scoreArray[3] == 1)
    return; // Wenn Spiel bereits beendet, abbrechen

  switch (configArray[2])
  { // Je nach Spielmodus
  case 1:
  {                                                   // Zeit-Modus
    unsigned long remainingTime = getRemainingTime(); // Verbleibende Zeit berechnen
    if (remainingTime == 0)
    {                    // Wenn keine Zeit mehr übrig
      scoreArray[3] = 1; // Spiel ist beendet
    }
    break;
  }
  case 2:
  { // Klassischer Modus
    if (scoreArray[0] >= configArray[1] || scoreArray[1] >= configArray[1])
    {                    // Wenn Siegtore erreicht
      scoreArray[3] = 1; // Spiel ist beendet
    }
    break;
  }
  default: // Andere Modi
    break; // Keine spezielle Endebedingung
  }

  if (scoreArray[3] == 1)
  {                                   // Wenn Spielende erreicht
    stopGame();                       // Spiel stoppen
    scoreArray[5] = getElapsedTime(); // Gesamtspielzeit speichern
    switch (scoreArray[0] > scoreArray[1] ? 1 : (scoreArray[1] > scoreArray[0] ? 2 : 0))
    {       // Gewinner ermitteln
    case 1: // Team 1 hat gewonnen
      DEBUG_1_PRINT(teamNames[0]);
      DEBUG_1_PRINTLN_F(" gewinnt!");
      scoreArray[2] = 1;    // Sieger-ID speichern
      rgbLed.on(0, 0, 255); // Blaues Siegerlicht
      if (nextionEnabled)
      {
        sendCommandToNextion(String(nextionTextFields[6]) + "=\"" + teamNames[0] + " gewinnt!\"");
      }
      break;
    case 2: // Team 2 hat gewonnen
      DEBUG_1_PRINT(teamNames[1]);
      DEBUG_1_PRINTLN_F(" gewinnt!");
      scoreArray[2] = 2;      // Sieger-ID speichern
      rgbLed.on(255, 255, 0); // Gelbes Siegerlicht
      if (nextionEnabled)
      {
        sendCommandToNextion(String(nextionTextFields[6]) + "=\"" + teamNames[1] + " gewinnt!\"");
      }
      break;
    case 0: // Unentschieden
      DEBUG_1_PRINTLN_F("Unentschieden!");
      scoreArray[2] = 0;    // Unentschieden speichern
      rgbLed.on(255, 0, 0); // Rotes Licht bei Unentschieden
      if (nextionEnabled)
      {
        sendCommandToNextion(String(nextionTextFields[6]) + "=\"Unentschieden!\"");
      }
      break;
    }
  }
}

void resetGame()
{                                                                        // Spiel zurücksetzen
  scoreArray[0] = 0;                                                     // Spielstand Team 1 auf 0
  scoreArray[1] = 0;                                                     // Spielstand Team 2 auf 0
  scoreArray[2] = 0;                                                     // Sieger zurücksetzen
  scoreArray[3] = 0;                                                     // Spielstatus zurücksetzen
  configArray[3] = 2;                                                    // Steuerung auf Reset
  rgbLed.off();                                                          // LED ausschalten
  DEBUG_1_PRINTLN_F("Spiel zurueckgesetzt");                             // Status melden
  sendCommandToNextion(String(nextionTextFields[0]) + "=\"SmartKick\""); // Titel anzeigen
}

void startGame()
{                       // Spiel starten
  resetGame();          // Spiel zurücksetzen
  configArray[3] = 1;   // Steuerung auf Start
  initializeGameTime(); // Spielzeit initialisieren

  // Nextion-Display initialisieren
  if (nextionEnabled)
  { // Wenn Nextion aktiviert
    // Teamnamen senden
    sendCommandToNextion(String(nextionTextFields[2]) + "=\"" + teamNames[0] + "\""); // Teamname 1
    sendCommandToNextion(String(nextionTextFields[4]) + "=\"" + teamNames[1] + "\""); // Teamname 2

    // Spielmodus senden
    String currentGameMode;
    switch (configArray[2])
    { // Je nach Modus
    case 0:
      currentGameMode = "FreePlay";
      break;
    case 1:
      currentGameMode = "TimePlay";
      break;
    case 2:
      currentGameMode = "ClassicPlay";
      break;
    default:
      currentGameMode = "Unknown";
      break;
    }
    if (currentGameMode == "ClassicPlay")
    {
      sendCommandToNextion(nextionTextFields[6] + String("=\"") + currentGameMode + "  (" + configArray[1] + ")  \""); // Neuen Modus senden + Toranzahl
    }
    else
    {
      sendCommandToNextion(String(nextionTextFields[6]) + "=\"" + currentGameMode + "\""); // Modus anzeigen
    }

    // Spielstand initialisieren
    sendCommandToNextion(String(nextionTextFields[3]) + "=0"); // Team 1 Score auf 0
    sendCommandToNextion(String(nextionTextFields[5]) + "=0"); // Team 2 Score auf 0

    // Zeit initialisieren
    sendCommandToNextion(String(nextionTextFields[1]) + "=\"00:00\""); // Zeit zurücksetzen
  }

  DEBUG_1_PRINTLN_F("Spiel gestartet"); // Status melden

  // LED-Signal zum Spielstart
  rgbLed.blink(2, 250, 0, 255, 0); // Grünes Doppelblinken
}

void stopGame()
{                                      // Spiel stoppen
  configArray[3] = 0;                  // Steuerung auf Stop
  scoreArray[3] = 1;                   // Spielende markieren
  DEBUG_1_PRINTLN_F("Spiel gestoppt"); // Status melden
}

// Arrays für Befehlsverarbeitung
const char *COMMAND_TYPES[] = {"get", "set", "nxt", "help"};                                                                                // Befehlstypen
const char *GET_COMMANDS[] = {"score", "config"};                                                                                           // GET-Befehle
const char *SET_COMMANDS[] = {"mode", "time", "goals", "start", "stop", "reset", "score1", "score2", "name1", "name2", "debug", "nextion"}; // SET-Befehle
const char *MODE_VALUES[] = {"free", "time", "classic"};                                                                                    // Spielmodi-Namen

void processCommand(String input)
{               // Befehl verarbeiten
  input.trim(); // Führende/nachfolgende Leerzeichen entfernen

  DEBUG_2_PRINT_F("Verarbeite Befehl: '"); // Debug-Ausgabe
  DEBUG_2_PRINT(input);
  DEBUG_2_PRINTLN_F("'");

  // Befehlstyp und Parameter extrahieren
  int firstSpace = input.indexOf(' '); // Position des ersten Leerzeichens
  String cmdType, params;

  if (firstSpace == -1)
  {                  // Wenn kein Leerzeichen gefunden
    cmdType = input; // Ganzer Input ist Befehlstyp
    params = "";     // Keine Parameter
  }
  else
  {                                           // Sonst
    cmdType = input.substring(0, firstSpace); // Befehlstyp bis zum Leerzeichen
    params = input.substring(firstSpace + 1); // Parameter nach dem Leerzeichen
  }

  DEBUG_2_PRINT_F("Befehlstyp: "); // Debug-Ausgabe des Befehlstyps
  DEBUG_2_PRINTLN(cmdType);
  DEBUG_2_PRINT_F("Parameter: "); // Debug-Ausgabe der Parameter
  DEBUG_2_PRINTLN(params);

  // Befehlstyp im Array suchen
  int cmdTypeIndex = -1; // Index für Befehlstyp
  for (int i = 0; i < 4; i++)
  { // Alle bekannten Befehlstypen durchsuchen
    if (cmdType == COMMAND_TYPES[i])
    {                   // Wenn Typ gefunden
      cmdTypeIndex = i; // Index speichern
      break;            // Schleife beenden
    }
  }

  // Befehl je nach Typ ausführen
  switch (cmdTypeIndex)
  {
  case 0: // GET-Befehl
    handleGetCommand(params);
    break;
  case 1: // SET-Befehl
    handleSetCommand(params);
    break;
  case 2: // NXT-Befehl
    handleNxtCommand(params);
    break;
  case 3: // HELP-Befehl
    handleHelpCommand(params);
    break;
  default: // Unbekannter Befehlstyp
    DEBUG_2_PRINTLN("Unbekannter Befehlstyp: " + cmdType);
    break;
  }
}

void handleGetCommand(String params)
{ // GET-Befehle verarbeiten
  DEBUG_2_PRINT_F("HandleGetCommand mit Parameter: ");
  DEBUG_2_PRINTLN(params);

  // GET-Befehl im Array suchen
  int cmdIndex = -1; // Index für GET-Befehl
  for (int i = 0; i < 2; i++)
  { // Alle bekannten GET-Befehle durchsuchen
    if (params == GET_COMMANDS[i])
    {               // Wenn Befehl gefunden
      cmdIndex = i; // Index speichern
      break;        // Schleife beenden
    }
  }

  // GET-Befehl ausführen
  switch (cmdIndex)
  {
  case 0:                 // score - Spielstand anzeigen
    interruptData[3] = 1; // Ausgabe erzwingen
    printScore();
    break;
  case 1: // config - Konfiguration anzeigen
    printConfig();
    break;
  default: // Unbekannter GET-Befehl
    DEBUG_1_PRINT_F("Ungueltiger GET-Befehl: ");
    DEBUG_1_PRINTLN(params);
    break;
  }
}

void handleSetCommand(String params)
{ // SET-Befehle verarbeiten
  DEBUG_2_PRINT_F("HandleSetCommand mit Parameter: ");
  DEBUG_2_PRINTLN(params);

  // Befehl und Wert trennen
  int space = params.indexOf(' '); // Position des Leerzeichens
  String command, value;

  if (space != -1)
  {                                       // Wenn Leerzeichen gefunden
    command = params.substring(0, space); // Befehl bis zum Leerzeichen
    value = params.substring(space + 1);  // Wert nach dem Leerzeichen
  }
  else
  {                   // Sonst
    command = params; // Ganzer Input ist Befehl
    value = "";       // Kein Wert
  }

  DEBUG_2_PRINT("Befehl: "); // Debug-Ausgabe des Befehls
  DEBUG_2_PRINTLN(command);
  DEBUG_2_PRINT("Wert: "); // Debug-Ausgabe des Werts
  DEBUG_2_PRINTLN(value);

  // SET-Befehl im Array suchen
  int cmdIndex = -1; // Index für SET-Befehl
  for (int i = 0; i < 12; i++)
  { // Alle bekannten SET-Befehle durchsuchen
    if (command == SET_COMMANDS[i])
    {               // Wenn Befehl gefunden
      cmdIndex = i; // Index speichern
      break;        // Schleife beenden
    }
  }

  // SET-Befehl ausführen
  switch (cmdIndex)
  {
  case 0: // mode - Spielmodus setzen
    handleSetMode(value);
    break;
  case 1: // time - Spielzeit setzen
    handleSetTime(value);
    break;
  case 2: // goals - Tore zum Sieg setzen
    handleSetGoals(value);
    break;
  case 3: // start - Spiel starten
    startGame();
    break;
  case 4: // stop - Spiel stoppen
    stopGame();
    break;
  case 5: // reset - Spiel zurücksetzen
    resetGame();
    break;
  case 6: // score1 - Punktestand Team 1 setzen
  case 7: // score2 - Punktestand Team 2 setzen
    handleSetScore(cmdIndex - 6, value);
    break;
  case 8: // name1 - Name Team 1 setzen
  case 9: // name2 - Name Team 2 setzen
    handleSetName(cmdIndex - 8, value);
    break;
  case 10: // debug - Debug-Level setzen
    handleSetFlag(10, value);
    break;
  case 11: // nextion - Nextion-Status setzen
    handleSetFlag(11, value);
    break;
  default: // Unbekannter SET-Befehl
    DEBUG_1_PRINTLN("Ungueltiger SET-Befehl: " + command);
    break;
  }
}

void handleNxtCommand(String params)
{ // NXT-Befehle verarbeiten
  if (nextionEnabled)
  {                               // Wenn Nextion aktiviert
    sendCommandToNextion(params); // Befehl senden
    DEBUG_1_PRINT_F("Gesendet an Nextion: ");
    DEBUG_1_PRINTLN(params);
  }
  else
  {                                               // Sonst
    DEBUG_1_PRINTLN_F("Nextion ist deaktiviert"); // Fehlermeldung ausgeben
  }
}

void handleHelpCommand(String params)
{ // HELP-Befehle verarbeiten
  if (params == "get")
  { // Hilfe für GET-Befehle
    DEBUG_0_PRINTLN_F("=== GET-BEFEHLE ===");
    DEBUG_0_PRINTLN_F("get score   - Zeigt Spielstand an");
    DEBUG_0_PRINTLN_F("get config  - Zeigt Konfiguration an");
  }
  else if (params == "set")
  { // Hilfe für SET-Befehle
    DEBUG_0_PRINTLN_F("=== SET-BEFEHLE ===");
    DEBUG_0_PRINTLN_F("set mode free|time|classic");
    DEBUG_0_PRINTLN_F("set time INT - Minuten");
    DEBUG_0_PRINTLN_F("set goals INT - Tore zum Sieg");
    DEBUG_0_PRINTLN_F("set start|stop|reset");
    DEBUG_0_PRINTLN_F("set score1|score2 INT|+|-");
    DEBUG_0_PRINTLN_F("set name1|name2 STRING");
    DEBUG_0_PRINTLN_F("set debug 0|1|2 - Debug-Level");
    DEBUG_0_PRINTLN_F("set nextion on|off");
  }
  else if (params == "nxt")
  { // Hilfe für NXT-Befehle
    DEBUG_0_PRINTLN_F("=== NXT-BEFEHLE ===");
    DEBUG_0_PRINTLN_F("nxt COMMAND - Sendet an Display");
  }
  else
  { // Allgemeine Hilfe
    DEBUG_0_PRINTLN_F("=== SMARTKICK HILFE ===");
    DEBUG_0_PRINTLN_F("help - Diese Hilfe");
    DEBUG_0_PRINTLN_F("help get|set|nxt - Details");
  }
}

void handleSetMode(String value)
{ // Spielmodus setzen
  for (int i = 0; i < 3; i++)
  { // Alle bekannten Modi durchsuchen
    if (value == MODE_VALUES[i])
    {                     // Wenn Modus gefunden
      configArray[2] = i; // Modus setzen
      printConfig();      // Konfiguration anzeigen
      startGame();        // Spiel starten
      return;             // Funktion beenden
    }
  }
  DEBUG_1_PRINTLN("Ungueltiger Spielmodus: " + value); // Fehlermeldung bei ungültigem Modus
}

void handleSetTime(String value)
{                           // Spielzeit setzen
  int time = value.toInt(); // String in Zahl umwandeln
  configArray[0] = time;    // Spielzeit speichern
  initializeGameTime();     // Spielzeit initialisieren
  DEBUG_1_PRINT_F("Spielzeit auf ");
  DEBUG_1_PRINT(time);
  DEBUG_1_PRINTLN_F(" Minuten gesetzt");
  interruptData[3] = 1; // Anzeige-Update-Flag setzen
}

void handleSetGoals(String value)
{                            // Tore zum Sieg setzen
  int goals = value.toInt(); // String in Zahl umwandeln
  configArray[1] = goals;    // Tore speichern
  DEBUG_1_PRINT("Tore zum Sieg auf ");
  DEBUG_1_PRINTLN(goals);
}

void handleSetScore(int teamIndex, String value)
{ // Spielstand setzen
  if (scoreArray[3] == 1)
  {
    return; // Wenn Spiel bereits beendet, abbrechen
  }

  if (value == "+")
  { // Wenn "+" als Wert
    if (scoreArray[teamIndex] < 99)
    {                          // Nur erhöhen, wenn unter 99
      scoreArray[teamIndex]++; // Spielstand erhöhen
    }
  }
  else if (value == "-")
  { // Wenn "-" als Wert
    if (scoreArray[teamIndex] > 0)
    {                          // Nur verringern, wenn über 0
      scoreArray[teamIndex]--; // Spielstand verringern
    }
  }
  else
  {                               // Sonst (direkter Wert)
    int newScore = value.toInt(); // String in Zahl umwandeln
    if (newScore < 0)
    {               // Wenn Wert negativ
      newScore = 0; // Auf Minimum setzen
    }
    else if (newScore > 99)
    {                // Wenn Wert zu groß
      newScore = 99; // Auf Maximum setzen
    }
    scoreArray[teamIndex] = newScore; // Begrenzten Wert setzen
  }
  DEBUG_1_PRINT("Spielstand Team ");
  DEBUG_1_PRINT(teamIndex + 1);
  DEBUG_1_PRINT(" auf ");
  DEBUG_1_PRINTLN(scoreArray[teamIndex]);
  interruptData[3] = 1; // Anzeige-Update-Flag setzen
}

void handleSetName(int teamIndex, String value)
{                               // Teamnamen setzen
  teamNames[teamIndex] = value; // Teamname speichern
  DEBUG_1_PRINT("Teamname ");
  DEBUG_1_PRINT(teamIndex + 1);
  DEBUG_1_PRINT(" auf ");
  DEBUG_1_PRINTLN(teamNames[teamIndex]);
}

void handleSetFlag(int flagIndex, String value)
{ // Flag-Werte setzen
  if (flagIndex == 10)
  {                            // Debug-Level
    int level = value.toInt(); // String in Zahl umwandeln
    if (level >= 0 && level <= 2)
    {                     // Wenn Level gültig
      debugLevel = level; // Level setzen
      DEBUG_0_PRINT_F("Debug-Level auf ");
      DEBUG_0_PRINT(level);
      DEBUG_0_PRINTLN_F(" gesetzt");

      // Beschreibung des Debug-Levels
      switch (level)
      {
      case 0:
        DEBUG_0_PRINTLN_F("Debug-Modus: Keine Ausgaben"); // Level 0: Keine Debug-Ausgaben
        break;
      case 1:
        DEBUG_0_PRINTLN_F("Debug-Modus: Wichtige Meldungen"); // Level 1: Wichtige Debug-Ausgaben
        break;
      case 2:
        DEBUG_0_PRINTLN_F("Debug-Modus: Ausführliche Meldungen"); // Level 2: Ausführliche Debug-Ausgaben
        break;
      }
    }
    else
    {                                                                      // Sonst
      DEBUG_0_PRINTLN_F("Ungültiger Debug-Level. Gültige Werte: 0, 1, 2"); // Fehlermeldung ausgeben
    }
  }
  else if (flagIndex == 11)
  { // Nextion-Status
    if (value == "on")
    {                        // Wenn "on"
      nextionEnabled = true; // Nextion aktivieren
      DEBUG_0_PRINTLN_F("Nextion aktiviert");
    }
    else if (value == "off")
    {                         // Wenn "off"
      nextionEnabled = false; // Nextion deaktivieren
      DEBUG_0_PRINTLN_F("Nextion deaktiviert");
    }
    else
    {                                                                                 // Sonst
      DEBUG_0_PRINTLN_F("Ungueltiger Wert für Nextion: Nur 'on' oder 'off' erlaubt"); // Fehlermeldung ausgeben
    }
  }
}

void sendCommandToNextion(const String &command)
{ // Befehl an Nextion senden
  DEBUG_2_PRINT_F("Nextion: ");
  DEBUG_2_PRINTLN(command);

  if (nextionEnabled)
  {                               // Wenn Nextion aktiviert
    nextionSerial.print(command); // Befehl senden
    for (uint8_t i = 0; i < 3; i++)
    { // 3x 0xFF als Abschluss senden
      nextionSerial.write(0xFF);
    }
  }
}

String readNextionResponse()
{                                     // Antwort vom Nextion lesen
  String response = "";               // Antwort-String
  String hexResponse = "";            // Hex-Darstellung für Debug
  unsigned long startTime = millis(); // Startzeitstempel

  // Warte kurz auf weitere Daten (max. 100ms)
  while ((millis() - startTime) < 100)
  { // Timeout von 100ms
    if (nextionSerial.available())
    {                       // Wenn Daten verfügbar
      startTime = millis(); // Timer zurücksetzen

      char c = nextionSerial.read();             // Zeichen lesen
      response += c;                             // Zum Antwort-String hinzufügen
      hexResponse += String((byte)c, HEX) + " "; // Hex-Darstellung für Debug

      if (response.endsWith("\xFF\xFF\xFF"))
      {        // Wenn Ende-Marker gefunden
        break; // Schleife beenden
      }

      delay(5); // Kurze Pause für weitere Daten
    }

    // Nach 50ms ohne neue Daten abbrechen
    if ((millis() - startTime) > 50 && response.length() > 0)
    { // Timeout für Antwort
      break;
    }
  }

  // Ignoriere Antworten mit Command-Codes
  if (response.length() >= 4 && response[0] >= 0x00 && response[0] <= 0x23 && response.endsWith("\xFF\xFF\xFF"))
  { // System-Antworten ignorieren
    return "";
  }

  // Entferne die Endsequenz "\xFF\xFF\xFF"
  if (response.endsWith("\xFF\xFF\xFF"))
  {                                                          // Wenn Ende-Marker vorhanden
    response = response.substring(0, response.length() - 3); // Marker entfernen
  }

  // Debug-Ausgabe der empfangenen Daten
  if (response.length() > 0)
  { // Wenn Antwort nicht leer
    DEBUG_2_PRINT_F("Empfangene Rohdaten: ");
    DEBUG_2_PRINTLN(hexResponse);
    DEBUG_2_PRINT_F("Verarbeitete Antwort: ");
    DEBUG_2_PRINTLN(response);
  }

  return response; // Verarbeitete Antwort zurückgeben
}

void updateNextion()
{ // Nextion-Display aktualisieren
  static String lastValues[6] = {
      // Zuletzt gesendete Werte
      "", // 0: Zeit
      "", // 1: Spielstand Team 1
      "", // 2: Spielstand Team 2
      "", // 3: Teamname 1
      "", // 4: Teamname 2
      ""  // 5: Spielmodus
  };

  unsigned long minutes, seconds;
  unsigned long elapsedTime = calculateElapsedTime(); // Aktuelle Zeit berechnen

  // Zeit aktualisieren
  if (configArray[3] == 1 && scoreArray[3] == 0)
  {                                                                                           // Wenn Spiel läuft
    minutes = elapsedTime / 60;                                                               // Minuten berechnen
    seconds = elapsedTime % 60;                                                               // Sekunden berechnen
    String currentTime = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds); // Zeit formatieren
    if (currentTime != lastValues[0])
    {                                                                                  // Wenn Zeit geändert
      sendCommandToNextion(nextionTextFields[1] + String("=\"") + currentTime + "\""); // Zeit senden
      lastValues[0] = currentTime;                                                     // Neue Zeit speichern
    }
  }
  else
  {                                                                  // Wenn Spiel nicht läuft
    sendCommandToNextion(nextionTextFields[1] + String("=\"END\"")); // Zeit auf 00:00 setzen
  }

  // Spielstand Team 1 aktualisieren
  if (String(scoreArray[0]) != lastValues[1])
  {                                                                                   // Wenn Spielstand geändert
    sendCommandToNextion(nextionTextFields[3] + String("=") + String(scoreArray[0])); // Neuen Spielstand senden
    lastValues[1] = String(scoreArray[0]);                                            // Neuen Spielstand speichern
  }

  // Spielstand Team 2 aktualisieren
  if (String(scoreArray[1]) != lastValues[2])
  {                                                                                   // Wenn Spielstand geändert
    sendCommandToNextion(nextionTextFields[5] + String("=") + String(scoreArray[1])); // Neuen Spielstand senden
    lastValues[2] = String(scoreArray[1]);                                            // Neuen Spielstand speichern
  }

  // Teamname 1 aktualisieren
  if (teamNames[0] != lastValues[3])
  {                                                                                   // Wenn Teamname geändert
    sendCommandToNextion(nextionTextFields[2] + String("=\"") + teamNames[0] + "\""); // Neuen Teamnamen senden
    lastValues[3] = teamNames[0];                                                     // Neuen Teamnamen speichern
  }

  // Teamname 2 aktualisieren
  if (teamNames[1] != lastValues[4])
  {                                                                                   // Wenn Teamname geändert
    sendCommandToNextion(nextionTextFields[4] + String("=\"") + teamNames[1] + "\""); // Neuen Teamnamen senden
    lastValues[4] = teamNames[1];                                                     // Neuen Teamnamen speichern
  }

  // Spielmodus aktualisieren
  String currentGameMode;
  switch (configArray[2])
  { // Je nach Spielmodus
  case 0:
    currentGameMode = "FreePlay";
    break;
  case 1:
    currentGameMode = "TimePlay";
    break;
  case 2:
    currentGameMode = "ClassicPlay";
    break;
  default:
    currentGameMode = "Unknown";
    break;
  }
  if (currentGameMode != lastValues[5])
  { // Wenn Spielmodus geändert
    if (currentGameMode == "ClassicPlay")
    {
      sendCommandToNextion(nextionTextFields[6] + String("=\"") + currentGameMode + "  (" + configArray[1] + ")  \""); // Neuen Modus senden + Toranzahl
    }
    else
    {
      sendCommandToNextion(nextionTextFields[6] + String("=\"") + currentGameMode + "\""); // Neuen Modus senden
    }
    lastValues[5] = currentGameMode; // Neuen Modus speichern
  }
}

void setup()
{                                 // Setup-Funktion (wird einmal ausgeführt)
  Serial.begin(115200);           // Serielle Verbindung starten
  DEBUG_1_PRINTLN_F("SmartKick"); // Programmname ausgeben

  if (nextionEnabled)
  {                                                                        // Wenn Nextion aktiviert
    nextionSerial.begin(9600);                                             // Nextion-Verbindung starten
    delay(100);                                                            // Warte auf Initialisierung
    sendCommandToNextion("page 0");                                        // Zur Startseite wechseln
    sendCommandToNextion(String(nextionTextFields[0]) + "=\"SmartKick\""); // Titel setzen
  }

  pinMode(pinArrayInput[0], INPUT_PULLUP); // Lichtschranke 1 als Eingang
  pinMode(pinArrayInput[1], INPUT_PULLUP); // Lichtschranke 2 als Eingang

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), ISR_Goal1, FALLING); // Interrupt für Tor 1
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), ISR_Goal2, FALLING); // Interrupt für Tor 2
}

void loop()
{ // Hauptschleife (wird wiederholt ausgeführt)
  if (Serial.available() > 0)
  {            // Wenn Daten von serieller Schnittstelle
    delay(50); // Kurze Verzögerung für vollständige Daten

    String input = ""; // Alle verfügbaren Zeichen lesen
    while (Serial.available() > 0)
    {                         // Solange Daten verfügbar
      char c = Serial.read(); // Zeichen lesen
      if ((c == '\r' || c == '\n') && input.length() == 0)
      { // CR/LF am Anfang ignorieren
        continue;
      }
      if (c == '\r' || c == '\n')
      { // CR/LF beendet Eingabe
        break;
      }
      input += c; // Zeichen zum Input hinzufügen
    }

    input.trim(); // Leerzeichen entfernen
    if (input.length() > 0)
    { // Wenn Input nicht leer
      DEBUG_2_PRINT_F("Eingabe von Serial: ");
      DEBUG_2_PRINTLN(input);

      DEBUG_2_PRINT_F("HEX: "); // Hex-Darstellung für Debug
      for (unsigned int i = 0; i < input.length(); i++)
      {
        DEBUG_2_PRINT(String((byte)input.charAt(i), HEX));
        DEBUG_2_PRINT_F(" ");
      }
      DEBUG_2_PRINTLN_F("");

      processCommand(input); // Befehl verarbeiten
    }
  }

  if (nextionEnabled)
  { // Nextion Kommunikation
    if (nextionSerial.available() > 0)
    {                                       // Wenn Daten vom Display
      String input = readNextionResponse(); // Antwort lesen
      DEBUG_2_PRINT("Eingabe von Nextion: ");
      DEBUG_2_PRINTLN(input);

      processCommand(input); // Befehl verarbeiten
    }

    updateNextion(); // Display aktualisieren
  }

  printScore();   // Spielstand anzeigen
  checkGameEnd(); // Spielende prüfen
}
