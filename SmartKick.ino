#include <Arduino.h>

byte pinArrayInput[2]{
    2,                                                    // Lichtschranke Tor 1 (Interrupt Pin 2)
    3                                                     // Lichtschranke Tor 2 (Interrupt Pin 3)
};

byte pinArrayOutput[3]{
    9,                                                    // LED 1 für Tor Team 1
    10,                                                   // LED 2 für Tor Team 2
    11                                                    // LED 3 für Spielende
};

byte scoreArray[4]{
    0,                                                    // Spielstand Team 1
    0,                                                    // Spielstand Team 2
    255,                                                   // Sieger-ID (255 bedeutet kein Sieger)
    false                                                  // Spielende 
};

byte gameConfigArray[2]{
    5,                                                    // Spielzeit in Minuten
    10                                                    // Tore zum Sieg
};

const byte debounce = 1000;                               // Entprellzeit in Millisekunden

volatile bool goal1Triggered = false;                     // Globale Variablen für die Interrupts
volatile bool goal2Triggered = false;

class LightBarrier {                                      // Klasse für Lichtschranken
  private:
    byte pin;
    unsigned long lastMillis;

  public:                                                 // Konstruktor
    LightBarrier(byte pin) {
      this->pin = pin;
      pinMode(pin, INPUT_PULLUP);                         // Interner Pull-up-Widerstand aktiviert
      lastMillis = 0;
    }
                
    bool isTriggered() {                                  // Methode zur Abfrage der Lichtschranke
      if (millis() - lastMillis >= debounce) {
        lastMillis = millis();
        return digitalRead(pin) == LOW;                   // Invertiertes Signal
      }
      return false;
    }
};

class LED {                                               // Klasse für die LEDs
  private:
    byte pin;

  public:                                                 // Konstruktor
    LED(byte pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
    }

    void activate(int duration) {                         // Methode zur Aktivierung der LED
      digitalWrite(pin, HIGH);                            // LED einschalten
      delay(duration);
      digitalWrite(pin, LOW);                             // LED ausschalten
    }

    void turnOn() {                                       // Methode zum Einschalten der LED
      digitalWrite(pin, HIGH);
    }

    void turnOff() {                                      // Methode zum Ausschalten der LED
      digitalWrite(pin, LOW);
    }
};

LightBarrier goal1(pinArrayInput[0]);                     // Instanzen der Klasse Lichtschranke
LightBarrier goal2(pinArrayInput[1]);
LED led1(pinArrayOutput[0]);                              // Instanz der Klasse LED für Pin 9
LED led2(pinArrayOutput[1]);                              // Instanz der Klasse LED für Pin 10
LED led3(pinArrayOutput[2]);                              // Instanz der Klasse LED für Pin 11

void handleInterruptGoal1();                              // Vorwärtsdeklaration der Interrupt-Handler
void handleInterruptGoal2();

void setup() {                                            // Setup-Funktion
  Serial.begin(115200);                                   // Initialisierung der seriellen Schnittstelle
  Serial.println("Setup started");

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), handleInterruptGoal1, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), handleInterruptGoal2, FALLING);
  Serial.println("Setup completed");
}

void handleInterruptGoal1() {                             // Interrupt-Handler für die Lichtschranken
  if (goal1.isTriggered()) {
    goal1Triggered = true;
    scoreArray[0]++;
      }
}

void handleInterruptGoal2() {
  if (goal2.isTriggered()) {
    goal2Triggered = true;
    scoreArray[1]++;
      }
}

void printScore() {
  char buffer[6];
  snprintf(buffer, sizeof(buffer), "%2d:%2d", scoreArray[0], scoreArray[1]);
  Serial.println(buffer);
}

void loop() {                                             // Loop-Funktion
  static unsigned long lastPrintTime = 0;
  static unsigned long lastBlinkTime = 0;
  unsigned long currentTime = millis();

  if (goal1Triggered) {                                   // Überprüfen, ob ein Tor erzielt wurde
    goal1Triggered = false;
    led1.activate(100);                                   // LED für Team 1 für 100ms aktivieren
    printScore();                                         // Spielstand ausgeben
  }

  if (goal2Triggered) {
    goal2Triggered = false;
    led2.activate(100);                                   // LED für Team 2 für 100ms aktivieren
    printScore();                                         // Spielstand ausgeben
  }

  if (scoreArray[3] == false) {                           // Überprüfen, ob das Spiel beendet ist
    if (scoreArray[0] >= gameConfigArray[1]) {            // Überprüfen, ob ein Team gewonnen hat
      Serial.println("Team 1 wins!");
      scoreArray[3] = true;                               // Spielende auf true setzen
      scoreArray[2] = 1;                                  // Sieger-ID auf 1 setzen (Team 1)
    } else if (scoreArray[1] >= gameConfigArray[1]) {
      Serial.println("Team 2 wins!");
      scoreArray[3] = true;                               // Spielende auf true setzen
      scoreArray[2] = 2;                                  // Sieger-ID auf 2 setzen (Team 2)
    }
  } else {
    if (currentTime - lastBlinkTime >= 500) {             // Blinken alle 500ms
      lastBlinkTime = currentTime;
      if (scoreArray[2] == 1) {                           // Team 1 hat gewonnen
        led1.activate(500);
        led3.activate(500);
      } else if (scoreArray[2] == 2) {                    // Team 2 hat gewonnen
        led2.activate(500);
        led3.activate(500);
      }
    }
  }
}