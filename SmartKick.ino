#include <Arduino.h>                                                                            // Include the Arduino core library
#include <SoftwareSerial.h>

#define NEXTION_RX_PIN 5                                                                        // Definiere die Pins fuer die Software-Serial-Verbindung zum Nextion-Display
#define NEXTION_TX_PIN 4

byte pinArrayInput[2]{                                                                          // Lichtschranken-Eingaenge
    2,        // Lichtschranke Tor 1 (Interrupt Pin 2)
    3         // Lichtschranke Tor 2 (Interrupt Pin 3)
};

byte pinArrayOutput[4]{                                                                         // RGB-LED-Ausgaenge
    9,        // Rot-Pin fuer RGB LED
    10,       // Gruen-Pin fuer RGB LED
    11,       // Blau-Pin fuer RGB LED
    12        // GND fuer RGB LED
};

volatile unsigned long interruptData[4]{                                                        // Interrupt-Daten
    0,        // Letzte Interrupt-Zeit fuer Lichtschranke Tor 1
    0,        // Letzte Interrupt-Zeit fuer Lichtschranke Tor 2
    1000,     // Entprellzeit in Millisekunden
    0         // Flagge, die anzeigt, dass der Spielstand aktualisiert wurde
};

unsigned long scoreArray[6]{                                                                    // Spielstand-Daten
    0,        // Spielstand Team 1
    0,        // Spielstand Team 2
    0,        // Sieger-Team (0 bedeutet kein Sieger)
    0,        // Spielende
    0,        // Startzeit
    0         // Spielzeit in Sekunden (nur fuer Zeitspiel)
};

unsigned long gameConfigArray[3]{                                                               // Spielkonfigurations-Daten
    5,        // Spielzeit in Minuten
    10,       // Tore zum Sieg
    2         // Spielmodus (0: FreePlay, 1: Zeitspiel, 2: Klassisches Spiel)
};

byte controlArray[4]{                                                                           // Steuerungs-Daten
    0,        // Start (1: Start, 0: Stop)
    0,        // Reset (1: Reset)
    0         // Debug (1: Aktiviert, 0: Deaktiviert)
};

String teamNames[2] = {                                                                         // Teamnamen
    "Team Blau",
    "Team Gelb"
};

const char* nextionTextFields[7] = {                                                            // Nextion-Textfeld-Adressen
    "NxtThead.txt",  // ueberschrift
    "t0.txt",        // Zeit
    "t1.txt",        // Teamname 1
    "n0.val",        // Spielstand 1
    "t2.txt",        // Teamname 2
    "n1.val",        // Spielstand 2
    "t5.txt"         // Spielmodus
};

class RGBLED {                                                                                  // Klasse fuer RGB-LEDs
  private:
    byte redPin;
    byte greenPin;
    byte bluePin;
    byte gndPin;

  public:
    RGBLED(byte redPin, byte greenPin, byte bluePin, byte gndPin) {
      this->redPin = redPin;
      this->greenPin = greenPin;
      this->bluePin = bluePin;
      this->gndPin = gndPin;
      pinMode(redPin, OUTPUT);
      pinMode(greenPin, OUTPUT);
      pinMode(bluePin, OUTPUT);
      pinMode(gndPin, OUTPUT);
      digitalWrite(gndPin, LOW);                                                                // GND-Pin auf LOW setzen
    }

    void on(byte red, byte green, byte blue) {
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);
    }

    void off() {
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    }

    void blink(int times, int duration, byte red, byte green, byte blue) {
      for (int i = 0; i < times; i++) {
        on(red, green, blue);
        delay(duration);
        off();
        delay(duration);
      }
    }
};

RGBLED rgbLed(pinArrayOutput[0], pinArrayOutput[1], pinArrayOutput[2], pinArrayOutput[3]);      // Instanz der Klasse RGBLED
SoftwareSerial nextionSerial(NEXTION_RX_PIN, NEXTION_TX_PIN);

bool debounce(volatile unsigned long &lastmillis) {                                             // Verwende interruptData[2] fuer die Entprellzeit
  if (millis() - lastmillis > interruptData[2]) {
    lastmillis = millis();
    return true;
  }
  return false;
}

void ISR_Goal1() {
  if (controlArray[0] == 1 && debounce(interruptData[0]) && scoreArray[3] == 0) {
    scoreArray[0]++;
    rgbLed.blink(1, 500, 0, 0, 255);                                                            // RGB LED fuer Team 1 (Blau) fuer 500ms aktivieren
    interruptData[3] = 1;
  }
}

void ISR_Goal2() {
  if (controlArray[0] == 1 && debounce(interruptData[1]) && scoreArray[3] == 0) {
    scoreArray[1]++;
    rgbLed.blink(1, 500, 255, 255, 0);                                                          // RGB LED fuer Team 2 (Gelb) fuer 500ms aktivieren
    interruptData[3] = 1;
  }
}

char previousBuffer[64] = "";                                                                   // Puffer zum Speichern des vorherigen Spielstands

void printScore() {                                                                             // Funktion zur Ausgabe des Spielstands
  char buffer[64];
  unsigned long minutes, seconds;
  unsigned long elapsedTime = calculateElapsedTime();

  minutes = elapsedTime / 60;
  seconds = elapsedTime % 60;

  snprintf(buffer, sizeof(buffer), "\nZeit: %02lu:%02lu\n%s: %lu\n%s: %lu\n", minutes, seconds, teamNames[0].c_str(), scoreArray[0], teamNames[1].c_str(), scoreArray[1]);

  if (strcmp(buffer, previousBuffer) != 0 || interruptData[3] == 1) {                           // Aktualisiere den Spielstand bei jeder Änderung
    Serial.println(buffer);
    strncpy(previousBuffer, buffer, sizeof(previousBuffer));
    interruptData[3] = 0;                                                                       // Setze die Flagge zurück
    updateNextion();                                                                            // Aktualisiere das Nextion-Display
  }
}

void printConfig() {                                                                            // Funktion zur Ausgabe der Spielkonfiguration
  Serial.print("Spiel Zeit: ");
  Serial.print(gameConfigArray[0]);
  Serial.println(" minuten");
  Serial.print("Tore zum Sieg: ");
  Serial.println(gameConfigArray[1]);
  Serial.print("Spiel Modus: ");
  switch (gameConfigArray[2]) {
    case 0:                                                                                     // FreePlay-Modus
      Serial.println("FreePlay");
      break;
    case 1:                                                                                     // TimePlay-Modus
      Serial.println("TimePlay");
      break;
    case 2:                                                                                     // ClassicPlay-Modus
      Serial.println("ClassicPlay");
      break;
    default:                                                                                    // Unbekannter Modus
      Serial.println("Unbekannt");
      break;
  }
  Serial.print("Debug Modus: ");
  if (controlArray[2] == 1) {
    Serial.println("Aktiviert");
  } else {
    Serial.println("Deaktiviert");
  }
}

void initializeGameTime() {                                                                     // Funktion zur Initialisierung der Spielzeit
  scoreArray[4] = millis();
  if (gameConfigArray[2] == 1 || gameConfigArray[2] == 2) {                                     // TIME_PLAY und CLASSIC_PLAY
    scoreArray[5] = gameConfigArray[0] * 60;                                                    // Spielzeit in Sekunden
  }
}

unsigned long getElapsedTime() {                                                                // Funktion zur Berechnung der vergangenen Zeit
  return (millis() - scoreArray[4]) / 1000;
}

unsigned long getRemainingTime() {                                                              // Funktion zur Berechnung der verbleibenden Zeit
  unsigned long elapsedTime = getElapsedTime();
  return scoreArray[5] > elapsedTime ? scoreArray[5] - elapsedTime : 0;
}

unsigned long calculateElapsedTime() {                                                          // Funktion zur Berechnung der Spielzeit
  if (scoreArray[3] == 1) {
    return scoreArray[5];
  } else if (controlArray[0] == 1) {                                                            // Nur wenn das Spiel laeuft
    if (gameConfigArray[2] == 1) {                                                              // TimePlay-Modus
      return getRemainingTime();
    } else {
      return getElapsedTime();
    }
  } else {
    return 0;
  }
}

void checkGameEnd() {                                                                           // Funktion zur Ueberpruefung des Spielendes
  if (scoreArray[3] == 1) return;                                                               // Wenn das Spiel bereits beendet ist, nichts tun

  switch (gameConfigArray[2]) {
    case 1: { // TIME_PLAY
      unsigned long remainingTime = getRemainingTime();
      if (remainingTime == 0) {
        scoreArray[3] = 1;
      }
      break;
    }
    case 2: { // CLASSIC_PLAY
      if (scoreArray[0] >= gameConfigArray[1] || scoreArray[1] >= gameConfigArray[1]) {
        scoreArray[3] = 1;
      }
      break;
    }
    default:                                                                                    // Unbekannter Modus
      break;
  }

  if (scoreArray[3] == 1) {
    stopGame();                                                                                 // Spiel stoppen 
    scoreArray[5] = getElapsedTime();                                                           // Spielzeit speichern
    switch (scoreArray[0] > scoreArray[1] ? 1 : (scoreArray[1] > scoreArray[0] ? 2 : 0)) {      
      case 1:                                                                                   // Team 1 gewinnt
        Serial.print(teamNames[0]);
        Serial.println(" gewinnt!");
        scoreArray[2] = 1;                                                                      // Sieger-Team auf 1 setzen (Team 1)
        rgbLed.on(0, 0, 255);                                                                   // RGB LED fuer Team 1 (Blau) anlassen
        sendCommandToNextion(String(nextionTextFields[1]) + "=\"" + teamNames[0] + " gewinnt!\"");
        break;
      case 2:                                                                                   // Team 2 gewinnt
        Serial.print(teamNames[1]);
        Serial.println(" gewinnt!");
        scoreArray[2] = 2;                                                                      // Sieger-Team auf 2 setzen (Team 2)
        rgbLed.on(255, 255, 0);                                                                 // RGB LED fuer Team 2 (Gelb) anlassen
        sendCommandToNextion(String(nextionTextFields[1]) + "=\"" + teamNames[1] + " gewinnt!\"");
        break;
      case 0:                                                                                   // Unentschieden
        Serial.println("Unentschieden!");
        scoreArray[2] = 0;                                                                      // Unentschieden
        rgbLed.on(255, 0, 0);                                                                   // RGB LED fuer Unentschieden (Rot) anlassen
        sendCommandToNextion(String(nextionTextFields[1]) + "=\"Unentschieden!\"");
        break;
    }
  }
}

void resetGame() {                                                                              // Funktion zum Zuruecksetzen des Spiels
  scoreArray[0] = 0;
  scoreArray[1] = 0;
  scoreArray[2] = 0;
  scoreArray[3] = 0;
  controlArray[1] = 0;
  rgbLed.off();                                                                                 // RGB LED ausschalten
  Serial.println("Spiel zurueckgesetzt");
}

void startGame() {                                                                              // Funktion zum Starten des Spiels
  resetGame();
  controlArray[0] = 1;
  initializeGameTime();                                                                         // Initialisiere die Spielzeit nur hier
  rgbLed.blink(2, 500, 0, 255, 0);                                                              // Zwei gruene Impulse beim Start
  Serial.println("Spiel gestartet");
}

void stopGame() {                                                                               // Funktion zum Stoppen des Spiels
  controlArray[0] = 0;
  scoreArray[3] = 1;                                                                            // Setze das Spielende
  Serial.println("Spiel gestoppt");
}

void processCommand(String input) {                                                             // Funktion zur Verarbeitung von Befehlen
  input.trim();                                                                                 // Entfernt fuehrende und nachfolgende Leerzeichen

  if (controlArray[2] == 1) {                                                                   // Debug-Modus aktivieren
    Serial.print("Verarbeite Befehl: ");
    Serial.println(input);
  }

  String commandArray[3] = {"set ", "get ", "nxt "};                                            // Befehlsarray
  int commandType = 0;

  for (int i = 0; i < 3; i++) {                    // Schleife ueber das Befehlsarray
    if (input.startsWith(commandArray[i])) {
      commandType = i + 1;
      input = input.substring(commandArray[i].length());
      break;
    }
  }

  if (controlArray[2] == 1) {                                                                   // Debug-Modus aktivieren
    Serial.print("Befehlstyp: ");
    Serial.println(commandType);
  }

  switch (commandType) {
    case 1: { // CMD_SET
      String command = input.substring(0, input.indexOf(' '));
      String value = input.substring(input.indexOf(' ') + 1);
      String setCommandArray[11] = {"mode", "time", "goals", "start", "stop", "reset", "score1", "score2", "name1", "name2", "debug"};
      int setCommand = 0;                                                                       // Set-Befehlsindex

      for (int i = 0; i < 11; i++) {                                                            // Schleife ueber das Set-Befehlsarray
        if (command == setCommandArray[i]) {
          setCommand = i + 1;
          break;
        }
      }

      if (controlArray[2] == 1) {                                                               // Debug-Modus aktivieren
        Serial.print("Set-Befehl: ");
        Serial.println(setCommand);
      }

      switch (setCommand) {
        case 1: { // SET_MODE
          String gameModeArray[3] = {"free", "time", "classic"};
          for (int i = 0; i < 3; i++) {
            if (value == gameModeArray[i]) {
              gameConfigArray[2] = i;
              printConfig();                                                                     // Ausgabe der aktuellen Spielkonfiguration
              startGame();                                                                       // Spiel starten
              break;
            }
          }
          break;
        }
        case 2: { // SET_TIME
          int time = value.toInt();
          gameConfigArray[0] = time;
          initializeGameTime();                                                                 // Initialisiere die Spielzeit neu
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Spielzeit auf ");
            Serial.print(time);
            Serial.println(" Minuten gesetzt");
          }
          break;
        }
        case 3: { // SET_GOALS
          int goals = value.toInt();
          gameConfigArray[1] = goals;
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Tore zum Sieg auf ");
            Serial.println(goals);
          }
          break;
        }
        case 4: // SET_START
          startGame();
          break;
        case 5: // SET_STOP
          stopGame();
          break;
        case 6: // SET_RESET
          resetGame();
          break;
        case 7: { // SET_SCORE1
          if (value == "+") {
            scoreArray[0]++;
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[0]);
              Serial.println(" um 1 erhoeht: ");
            }
          } else if (value == "-") {
            scoreArray[0]--;
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[0]);
              Serial.println(" um 1 verringert: ");
            }
          } else {
            int score1 = value.toInt();
            scoreArray[0] = score1;
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[0]);
              Serial.print(" auf ");
            }
          }
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.println(scoreArray[0]);
          }
          interruptData[3] = 1;                                                                 // Setze die Flagge, dass der Spielstand aktualisiert wurde
          break;
        }
        case 8: { // SET_SCORE2
          if (value == "+") {
            scoreArray[1]++;
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[1]);
              Serial.println(" um 1 erhoeht: ");
            }
          } else if (value == "-") {
            scoreArray[1]--;
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[1]);
              Serial.println(" um 1 verringert: ");
            }
          } else {
            scoreArray[1] = value.toInt();
            if (controlArray[2] == 1) {                                                         // Debug-Modus aktivieren
              Serial.print("Spielstand fuer ");
              Serial.print(teamNames[1]);
              Serial.print(" auf ");
            }
          }
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.println(scoreArray[1]);
          }
          interruptData[3] = 1;                                                                 // Setze die Flagge, dass der Spielstand aktualisiert wurde
          break;
        }
        case 9: { // SET_NAME1
          teamNames[0] = value;
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Teamname 1 auf ");
            Serial.print(teamNames[0]);
            Serial.println(" gesetzt");
          }
          break;
        }
        case 10: { // SET_NAME2
          teamNames[1] = value;
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Teamname 2 auf ");
            Serial.print(teamNames[1]);
            Serial.println(" gesetzt");
          }
          break;
        }
        case 11: { // SET_DEBUG
          if (value == "on") {
            controlArray[2] = 1;
            Serial.println("Debug-Modus aktiviert");
          } else if (value == "off") {
            controlArray[2] = 0;
            Serial.println("Debug-Modus deaktiviert");
          } else {
            Serial.println("Ungueltiger Wert fuer Debug-Modus");
          }
          break;
        }
        default:                                                                                // Ungueltiger Befehl
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Ungueltiger Befehl: ");
            Serial.println(input);
          }
          break;
      }
      break;
    }
    case 2: { // CMD_GET
      String getCommand = input;

      String getCommandArray[2] = {"score", "config"};                                          // Get-Befehlsarray
      int getCommandIndex = 0;

      for (int i = 0; i < sizeof(getCommandArray) / sizeof(getCommandArray[0]); i++) {          // Schleife ueber das Get-Befehlsarray
        if (getCommand == getCommandArray[i]) {
          getCommandIndex = i + 1;
          break;
        }
      }

      if (controlArray[2] == 1) {                                                               // Debug-Modus aktivieren
        Serial.print("Get-Befehl: ");
        Serial.println(getCommandIndex);
      }

      switch (getCommandIndex) {
        case 1: // GET_SCORE
          printScore();                                                                         // Ausgabe des aktuellen Spielstands
          break;
        case 2: // GET_CONFIG
          printConfig();                                                                        // Ausgabe der aktuellen Spielkonfiguration
          break;
        default:                                                                                // Ungueltiges Befehlsformat
          if (controlArray[2] == 1) {                                                           // Debug-Modus aktivieren
            Serial.print("Ungueltiges Befehlsformat: ");
            Serial.println(input);
          }
          break;
      }
      break;
    }
    case 3: { // CMD_NEXTION
      sendCommandToNextion(input);
      if (controlArray[2] == 1) {                                                               // Debug-Modus aktivieren
        Serial.print("Gesendet an Nextion: ");
        Serial.println(input);
      }
      break;
    }
    default:                                                                                    // Ungueltiges Befehlsformat
      if (controlArray[2] == 1) {                                                               // Debug-Modus aktivieren
        Serial.print("Ungueltiges Befehlsformat: ");
        Serial.println(input);
      }
      break;
  }
}

void sendCommandToNextion(const String &command) {
  nextionSerial.print(command);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  ignoreNextionResponse();                                                                      // Ignoriere die Bestaetigungsnachricht
}

void ignoreNextionResponse() {
  while (nextionSerial.available()) {
    char c = nextionSerial.read();
    if (c == 0x1A) {
      if (nextionSerial.read() == 0xFF && nextionSerial.read() == 0xFF && nextionSerial.read() == 0xFF) {
        break;                                                                                  // Ignoriere die Bestaetigungsnachricht
      }
    }
  }
}

String readNextionResponse() {
  String response = "";
  while (nextionSerial.available()) {
    char c = nextionSerial.read();
    response += c;
    if (controlArray[2] == 1) {                                                                 // Debug-Modus aktivieren
      Serial.print("Empfangenes Zeichen: ");
      Serial.println(c, HEX);
    }
    if (response.endsWith("\xFF\xFF\xFF")) {
      response = response.substring(0, response.length() - 3);                                  // Entferne die Endzeichen
      break;
    }
  }
  return response;
}

void updateNextion() {
  char buffer[64];
  unsigned long minutes, seconds;
  unsigned long elapsedTime = calculateElapsedTime();

  if (controlArray[0] == 1) {                                                            // Nur aktualisieren, wenn das Spiel laeuft
    minutes = elapsedTime / 60;
    seconds = elapsedTime % 60;

    snprintf(buffer, sizeof(buffer), "%s=\"%02lu:%02lu\"", nextionTextFields[1], minutes, seconds);
    sendCommandToNextion(buffer);
  } else {                                                                                      // Wenn das Spiel nicht laeuft, setze die Zeit auf 00:00
    snprintf(buffer, sizeof(buffer), "%s=\"00:00\"", nextionTextFields[1]);
    sendCommandToNextion(buffer);
  }

  snprintf(buffer, sizeof(buffer), "%s=\"%s\"", nextionTextFields[2], teamNames[0].c_str());
  sendCommandToNextion(buffer);

  snprintf(buffer, sizeof(buffer), "%s=%lu", nextionTextFields[3], scoreArray[0]);
  sendCommandToNextion(buffer);

  snprintf(buffer, sizeof(buffer), "%s=\"%s\"", nextionTextFields[4], teamNames[1].c_str());
  sendCommandToNextion(buffer);

  snprintf(buffer, sizeof(buffer), "%s=%lu", nextionTextFields[5], scoreArray[1]);
  sendCommandToNextion(buffer);

  const char* gameModeText;
  switch (gameConfigArray[2]) {
    case 0:
      gameModeText = "FreePlay";
      break;
    case 1:
      gameModeText = "TimePlay";
      break;
    case 2:
      gameModeText = "ClassicPlay";
      break;
    default:
      gameModeText = "Unbekannt";
      break;
  }

  snprintf(buffer, sizeof(buffer), "%s=\"%s\"", nextionTextFields[6], gameModeText);
  sendCommandToNextion(buffer);
}

void setup() {
  Serial.begin(115200);                                                                         // Initialisierung der seriellen Schnittstelle
  nextionSerial.begin(9600);                                                                    // Initialisierung der Software-Serial-Schnittstelle fuer das Nextion-Display
  sendCommandToNextion("baud=115200");
  delay(100);                                                                                   // Warte kurz, damit das Nextion-Display die Aenderung uebernehmen kann
  nextionSerial.end();                                                                          // Beende die 9600 Baud-Verbindung
  nextionSerial.begin(115200);                                                                  // Aendere die Baudrate der Software-Serial-Schnittstelle auf 115200

  pinMode(pinArrayInput[0], INPUT_PULLUP);                                                      // Aktiviere interne Pull-up-Widerstaende fuer die Eingaenge
  pinMode(pinArrayInput[1], INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), ISR_Goal1, FALLING);                 // Interrupts fuer Lichtschranken
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), ISR_Goal2, FALLING);

  Serial.println("SmartKick");                                                                  // Ausgabe des Starttextes
  sendCommandToNextion(String(nextionTextFields[0]) + "=\"SmartKick\"");                        // Sende Begrueßungsnachricht an das Nextion-Display
  sendCommandToNextion("page Start");                                                           // Wechsle zur Startseite des Nextion-Displays
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (controlArray[2] == 1) {                                                                 // Debug-Modus aktivieren
      Serial.print("Eingabe von Serial: ");
      Serial.println(input);
    }
    processCommand(input);                                                                      // Verarbeite Befehle von der seriellen Schnittstelle
  }

  if (nextionSerial.available() > 0) {
    String input = readNextionResponse();
    if (controlArray[2] == 1) {                                                                 // Debug-Modus aktivieren
      Serial.print("Eingabe von Nextion: ");
      Serial.println(input);
    }
    processCommand(input);                                                                      // Verarbeite Befehle vom Nextion-Display
  }

  printScore();                                                                                 // Spielstand ausgeben und Nextion-Display aktualisieren
  checkGameEnd();                                                                               // Ueberpruefen, ob das Spiel beendet ist
}