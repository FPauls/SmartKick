#include <Arduino.h>                                                                               // Standard-Arduino-Bibliothek

byte pinArrayInput[2]{
    2,        // Lichtschranke Tor 1 (Interrupt Pin 2)
    3         // Lichtschranke Tor 2 (Interrupt Pin 3)
};

byte pinArrayOutput[4]{
    9,        // Rot-Pin für RGB LED
    10,       // Grün-Pin für RGB LED
    11,       // Blau-Pin für RGB LED
    12        // GND für RGB LED
};

volatile unsigned long interruptData[4]{
    0,        // Letzte Interrupt-Zeit für Lichtschranke Tor 1
    0,        // Letzte Interrupt-Zeit für Lichtschranke Tor 2
    1000,     // Entprellzeit in Millisekunden
    0         // Flagge, die anzeigt, dass der Spielstand aktualisiert wurde
};

unsigned long scoreArray[6]{
    0,        // Spielstand Team 1
    0,        // Spielstand Team 2
    0,        // Sieger-Team (0 bedeutet kein Sieger)
    0,        // Spielende
    0,        // Startzeit
    0         // Spielzeit in Sekunden (nur für Zeitspiel)
};

unsigned long gameConfigArray[3]{
    5,        // Spielzeit in Minuten
    10,       // Tore zum Sieg
    2         // Spielmodus (0: FreePlay, 1: Zeitspiel, 2: Klassisches Spiel)
};

byte controlArray[4]{
    0,        // Start (1: Start, 0: Stop)
    0,        // Reset (1: Reset)
    0,        // Team 1 Spielstand ändern (1: +1, -1: -1)
    0         // Team 2 Spielstand ändern (1: +1, -1: -1)
};

class RGBLED {                                                                                     // Klasse für RGB-LEDs
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

bool debounce(volatile unsigned long &lastmillis) {                                             // Verwende interruptData[2] für die Entprellzeit
  if (millis() - lastmillis > interruptData[2]) {
    lastmillis = millis();
    return true;
  }
  return false;
}

void ISR_Goal1() {
  if (controlArray[0] == 1 && debounce(interruptData[0]) && scoreArray[3] == 0) {
    scoreArray[0]++;
    rgbLed.blink(1, 500, 0, 0, 255);                                                            // RGB LED für Team Blau (Blau) für 500ms aktivieren
    interruptData[3] = 1;
  }
}

void ISR_Goal2() {
  if (controlArray[0] == 1 && debounce(interruptData[1]) && scoreArray[3] == 0) {
    scoreArray[1]++;
    rgbLed.blink(1, 500, 255, 255, 0);                                                          // RGB LED für Team Gelb (Gelb) für 500ms aktivieren
    interruptData[3] = 1;
  }
}

char previousBuffer[64] = "";                                                                   // Puffer zum Speichern des vorherigen Spielstands

void printScore() {                                                                             // Funktion zur Ausgabe des Spielstands
  char buffer[64];                                                                              // Erhöht die Puffergröße, um sicherzustellen, dass genügend Platz für die Ausgabe vorhanden ist
  unsigned long elapsedTime = (millis() - scoreArray[4]) / 1000;
  unsigned long minutes = elapsedTime / 60;
  unsigned long seconds = elapsedTime % 60;
  snprintf(buffer, sizeof(buffer), "\n\n            %02lu:%02lu\nTeam Blau   %02lu:%02lu   Team Gelb\n", minutes, seconds, scoreArray[0], scoreArray[1]);

  if (strcmp(buffer, previousBuffer) != 0) {                                                    // Vergleiche den aktuellen Puffer mit dem vorherigen Puffer
    Serial.println(buffer);                                                                     // Nur ausgeben, wenn sich der Inhalt geändert hat
    strncpy(previousBuffer, buffer, sizeof(previousBuffer));                                    // Speichere den aktuellen Puffer als vorherigen Puffer
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
    case 0:
      Serial.println("FreePlay");
      break;
    case 1:
      Serial.println("TimePlay");
      break;
    case 2:
      Serial.println("ClassicPlay");
      break;
    default:
      Serial.println("Unbekannt");
      break;
  }
}

void initializeGameTime() {
  scoreArray[4] = millis();
  if (gameConfigArray[2] == 1) {                                                                // TIME_PLAY
    scoreArray[5] = gameConfigArray[0] * 60;                                                    // Spielzeit in Sekunden
  }
}

unsigned long getElapsedTime() {
  return (millis() - scoreArray[4]) / 1000;
}

unsigned long getRemainingTime() {
  unsigned long elapsedTime = getElapsedTime();
  return scoreArray[5] > elapsedTime ? scoreArray[5] - elapsedTime : 0;
}

void checkGameEnd() {
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
    default:
      break;
  }

  if (scoreArray[3] == 1) {
    stopGame();                                                                                 // Spiel stoppen 
    switch (scoreArray[0] > scoreArray[1] ? 1 : (scoreArray[1] > scoreArray[0] ? 2 : 0)) {      
      case 1:
        Serial.println("Team Blau gewinnt!");
        scoreArray[2] = 1;                                                                      // Sieger-Team auf 1 setzen (Team Blau)
        rgbLed.on(0, 0, 255);                                                                   // RGB LED für Team Blau (Blau) anlassen
        break;
      case 2:
        Serial.println("Team Gelb gewinnt!");
        scoreArray[2] = 2;                                                                      // Sieger-Team auf 2 setzen (Team Gelb)
        rgbLed.on(255, 255, 0);                                                                 // RGB LED für Team Gelb (Gelb) anlassen
        break;
      case 0:
        Serial.println("Unentschieden!");
        scoreArray[2] = 0;                                                                      // Unentschieden
        rgbLed.on(255, 0, 0);                                                                   // RGB LED für Unentschieden (Rot) anlassen
        break;
    }
  }
}

void setGameMode(int mode) {
  gameConfigArray[2] = mode;
  switch (mode) {
    case 0:
      Serial.println("Spielmodus auf FreePlay gesetzt");
      break;
    case 1:
      Serial.println("Spielmodus auf TimePlay gesetzt");
      break;
    case 2:
      Serial.println("Spielmodus auf ClassicPlay gesetzt");
      break;
    default:
      Serial.println("Ungültiger Spielmodus");
      break;
  }
}

void resetGame() {
  scoreArray[0] = 0;
  scoreArray[1] = 0;
  scoreArray[2] = 0;
  scoreArray[3] = 0;
  controlArray[1] = 0;
  rgbLed.off();                                                                                 // RGB LED ausschalten
  initializeGameTime();                                                                         // Initialisiere die Spielzeit
  Serial.println("Spiel zurückgesetzt");
}

void startGame() {
  resetGame();
  controlArray[0] = 1;
  rgbLed.blink(2, 500, 0, 255, 0);                                                              // Zwei grüne Impulse beim Start
  Serial.println("Spiel gestartet");
}

void stopGame() {
  controlArray[0] = 0;
  Serial.println("Spiel gestoppt");
}

void updateArray() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();                                                                               // Entfernt führende und nachfolgende Leerzeichen

    String commandArray[3] = {"set ", "get ", ""};                                              // Befehlsarray
    int commandType = 0;

    for (int i = 0; i < sizeof(commandArray) / sizeof(commandArray[0]); i++) {                  // Schleife über das Befehlsarray
      if (input.startsWith(commandArray[i])) {
        commandType = i + 1;
        input = input.substring(commandArray[i].length());
        break;
      }
    }

    switch (commandType) {
      case 1: { // CMD_SET
        String command = input.substring(0, input.indexOf(' '));
        String value = input.substring(input.indexOf(' ') + 1);
        String setCommandArray[8] = {"mode", "time", "goals", "start", "stop", "reset", "score1", "score2"}; 
        int setCommand = 0;                                                                     // Set-Befehlsindex                  

        for (int i = 0; i < sizeof(setCommandArray) / sizeof(setCommandArray[0]); i++) {        // Schleife über das Set-Befehlsarray
          if (command == setCommandArray[i]) {
            setCommand = i + 1;
            break;
          }
        }

        switch (setCommand) {
          case 1: { // SET_MODE
            String gameModeArray[] = {"free", "time", "classic"};
            int gameMode = -1;                                                                  // Initialisiere mit -1, um ungültigen Modus anzuzeigen
            for (int i = 0; i < sizeof(gameModeArray) / sizeof(gameModeArray[0]); i++) {
              if (value == gameModeArray[i]) {
                gameMode = i;
                break;
              }
            }
            if (gameMode != -1) {
              setGameMode(gameMode);
              startGame();
            }
            break;
          }
          case 2: { // SET_TIME
            int time = value.toInt();
            gameConfigArray[0] = time;
            Serial.print("Spielzeit auf ");
            Serial.print(time);
            Serial.println(" Minuten gesetzt");
            break;
          }
          case 3: { // SET_GOALS
            int goals = value.toInt();
            gameConfigArray[1] = goals;
            Serial.print("Tore zum Sieg auf ");
            Serial.println(goals);
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
            int score1 = value.toInt();
            scoreArray[0] = score1;
            Serial.print("Spielstand für Team Blau auf ");
            Serial.println(score1);
            interruptData[3] = 1;                                                               // Setze die Flagge, dass der Spielstand aktualisiert wurde
            break;
          }
          case 8: { // SET_SCORE2
            int score2 = value.toInt();
            scoreArray[1] = score2;
            Serial.print("Spielstand für Team Gelb auf ");
            Serial.println(score2);
            interruptData[3] = 1;                                                               // Setze die Flagge, dass der Spielstand aktualisiert wurde
            break;
          }
          default:
            Serial.println("Ungültiger Befehl");
            break;
        }
        break;
      }
      case 2: { // CMD_GET
        String getCommand = input;

        String getCommandArray[2] = {"score", "config"};                                        // Get-Befehlsarray
        int getCommandIndex = 0;

        for (int i = 0; i < sizeof(getCommandArray) / sizeof(getCommandArray[0]); i++) {        // Schleife über das Get-Befehlsarray
          if (getCommand == getCommandArray[i]) {
            getCommandIndex = i + 1;
            break;
          }
        }

        switch (getCommandIndex) {
          case 1: // GET_SCORE
            printScore();                                                                       // Ausgabe des aktuellen Spielstands
            break;
          case 2: // GET_CONFIG
            printConfig();                                                                      // Ausgabe der aktuellen Spielkonfiguration
            break;
          default:
            Serial.println("Ungültiges Befehlsformat");
            break;
        }
        break;
      }
      default:
        Serial.println("Ungültiges Befehlsformat");
        break;
    }
  }
}

void handleControlArray() {
  for (int i = 0; i < 4; i++) {
    switch (i) {
      case 0: // Start/Stop
        if (controlArray[i] == 1) {
          checkGameEnd();                                                                       // Überprüfen, ob die Zeit abgelaufen ist und das Spiel beenden
          printScore();                                                                         // Spielstand und Zeit ausgeben
        }
        break;
      case 1:                                                                                                                                                                     // Reset
        if (controlArray[i] == 1) {
          resetGame();
        }
        break;
      case 2: // Team Blau Spielstand ändern
        if (controlArray[i] == 1) {
          scoreArray[0]++;
          controlArray[i] = 0;
          printScore();
        } else if (controlArray[i] == -1) {
          scoreArray[0]--;
          controlArray[i] = 0;
          printScore();
        }
        break;
      case 3: // Team Gelb Spielstand ändern
        if (controlArray[i] == 1) {
          scoreArray[1]++;
          controlArray[i] = 0;
          printScore();
        } else if (controlArray[i] == -1) {
          scoreArray[1]--;
          controlArray[i] = 0;
          printScore();
        }
        break;
      default:
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);                                                                         // Initialisierung der seriellen Schnittstelle

  pinMode(pinArrayInput[0], INPUT_PULLUP);                                                      // Aktiviere interne Pull-up-Widerstände für die Eingänge
  pinMode(pinArrayInput[1], INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), ISR_Goal1, FALLING);                 // Interrupts für Lichtschranken
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), ISR_Goal2, FALLING);

  setGameMode(0);                                                                               // Setze den Spielmodus auf FreePlay
  startGame();                                                                                  // Starte das Spiel
}

void loop() {                                                                                   
  updateArray();                                                                                // Update arrays based on serial input

  if (interruptData[3] == 1) {                                                                  // Überprüfen, ob der Spielstand aktualisiert wurde
    printScore();                                                                               // Spielstand ausgeben
    interruptData[3] = 0;                                                                       // Flagge zurücksetzen
  }

  if (gameConfigArray[2] == 2) {                                                                // CLASSIC_PLAY
    scoreArray[5] = getElapsedTime();                                                           // Spielzeit hochzählen
  }

  handleControlArray();                                                                         // Handling des controlArray
}