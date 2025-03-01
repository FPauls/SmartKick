#include <Arduino.h>

byte pinArrayInput[2]{
    2,                                              // Lichtschranke Tor 1 (Interrupt Pin 2)
    3                                               // Lichtschranke Tor 2 (Interrupt Pin 3)
};

byte pinArrayOutput[1]{
    11                                              // Piezo Summer
};

byte scoreArray[2]{
    0,                                              // Spielstand Team 1
    0                                               // Spielstand Team 2
};

byte gameConfigArray[2]{
    5,                                              // Spielzeit in Minuten
    10                                              // Tore zum Sieg
};

const byte debounce = 1000;                         // Entprellzeit in Millisekunden

volatile bool goal1Triggered = false;               // Globale Variablen für die Interrupts
volatile bool goal2Triggered = false;

class LightBarrier {                                // Klasse für Lichtschranken
  private:
    byte pin;
    unsigned long lastMillis;

  public:                                           // Konstruktor
    LightBarrier(byte pin) {
      this->pin = pin;
      pinMode(pin, INPUT_PULLUP);                    // Interner Pull-up-Widerstand aktiviert
      lastMillis = 0;
    }
                
    bool isTriggered() {                            // Methode zur Abfrage der Lichtschranke
      if (millis() - lastMillis >= debounce) {
        lastMillis = millis();
        return digitalRead(pin) == LOW;             // Invertiertes Signal
      }
      return false;
    }
};

class Buzzer {                                      // Klasse für den Piezo Summer
  private:
    byte pin;

  public:                                           // Konstruktor
    Buzzer(byte pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
    }

    void activate(int duration) {                   // Methode zur Aktivierung des Piezo Summers
      digitalWrite(pin, HIGH);
      delay(duration);
      digitalWrite(pin, LOW);
    }
};

LightBarrier goal1(pinArrayInput[0]);               // Instanzen der Klasse Lichtschranke
LightBarrier goal2(pinArrayInput[1]);
Buzzer buzzer(pinArrayOutput[0]);                   // Instanz der Klasse Buzzer

void handleInterruptGoal1();                        // Vorwärtsdeklaration der Interrupt-Handler
void handleInterruptGoal2();

void setup() {                                      // Setup-Funktion
  Serial.begin(115200);                             // Initialisierung der seriellen Schnittstelle
  Serial.println("Setup started");

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), handleInterruptGoal1, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), handleInterruptGoal2, FALLING);
  Serial.println("Setup completed");
}

void handleInterruptGoal1() {                       // Interrupt-Handler für die Lichtschranken
  if (goal1.isTriggered()) {
    goal1Triggered = true;
    Serial.println("Interrupt Goal 1 triggered");
  }
}

void handleInterruptGoal2() {
  if (goal2.isTriggered()) {
    goal2Triggered = true;
    Serial.println("Interrupt Goal 2 triggered");
  }
}

void loop() {                                       // Loop-Funktion
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();

  // Überprüfen, ob ein Tor erzielt wurde
  if (goal1Triggered) {
    goal1Triggered = false;
    scoreArray[0]++;
    buzzer.activate(100);                           // Piezo Summer für 100ms aktivieren
    Serial.print("Goal for Team 1! Score: ");
    Serial.println(scoreArray[0]);
    // Code, um den Spielstand auf dem Nextion-Display zu aktualisieren
  }

  if (goal2Triggered) {
    goal2Triggered = false;
    scoreArray[1]++;
    buzzer.activate(100);                           // Piezo Summer für 100ms aktivieren
    Serial.print("Goal for Team 2! Score: ");
    Serial.println(scoreArray[1]);
    // Code, um den Spielstand auf dem Nextion-Display zu aktualisieren
  }

  // Überprüfen, ob ein Team gewonnen hat
  if (scoreArray[0] >= gameConfigArray[1]) {
    Serial.println("Team 1 wins!");
    // Code, um den Sieg auf dem Nextion-Display anzuzeigen
  } else if (scoreArray[1] >= gameConfigArray[1]) {
    Serial.println("Team 2 wins!");
    // Code, um den Sieg auf dem Nextion-Display anzuzeigen
  }
}