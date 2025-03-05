#include <Arduino.h>                                                                            // Standard-Arduino-Bibliothek

byte pinArrayInput[2]{
    2,                                                                                          // Lichtschranke Tor 1 (Interrupt Pin 2)
    3                                                                                           // Lichtschranke Tor 2 (Interrupt Pin 3)
};

byte pinArrayOutput[3]{
    9,                                                                                          // LED 1 für Tor Team 1
    10,                                                                                         // LED 2 für Tor Team 2
    11                                                                                          // LED 3 für Spielende
};

volatile unsigned long interruptData[3]{
  0,                                                                                            // Letzte Interrupt-Zeit für Lichtschranke Tor 1
  0,                                                                                            // Letzte Interrupt-Zeit für Lichtschranke Tor 2    
  1000                                                                                          // Entprellzeit in Millisekunden
};

byte scoreArray[4]{
    0,                                                                                          // Spielstand Team 1
    0,                                                                                          // Spielstand Team 2
    0,                                                                                          // Sieger-Team (0 bedeutet kein Sieger)
    false                                                                                       // Spielende 
};

byte gameConfigArray[2]{
    5,                                                                                          // Spielzeit in Minuten
    10                                                                                          // Tore zum Sieg
};

class LED {                                                                                     // Klasse für die LEDs
  private:
    byte pin;

  public:                                                                                       // Konstruktor
    LED(byte pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
    }

    void activate(int duration) {                                                               // Methode zur Aktivierung der LED
      digitalWrite(pin, HIGH);                                                                  // LED einschalten
      delay(duration);
      digitalWrite(pin, LOW);                                                                   // LED ausschalten
    }
};

LED led1(pinArrayOutput[0]);                                                                    // Instanzen der Klasse LED
LED led2(pinArrayOutput[1]);
LED led3(pinArrayOutput[2]);

bool debounce(volatile unsigned long &lastmillis) {                                             // Verwende interruptData[2] für die Entprellzeit
  if (millis() - lastmillis > interruptData[2]) {
    lastmillis = millis();
    return true;
  }
  return false;
}

void ISR_Goal1() {                                                                              // Interrupt-Handler für die Lichtschranken
  if (debounce(interruptData[0])) {                                                             // Entprellung
    scoreArray[0]++;                                                                            // Spielstand für Team 1 erhöhen
    led1.activate(100);                                                                         // LED für Team 1 für 100ms aktivieren
    printScore();                                                                               // Spielstand ausgeben
  }
}

void ISR_Goal2() {
  if (debounce(interruptData[1])) {                                                             // Entprellung
    scoreArray[1]++;                                                                            // Spielstand für Team 2 erhöhen
    led2.activate(100);                                                                         // LED für Team 2 für 100ms aktivieren
    printScore();                                                                               // Spielstand ausgeben
  }
}

void printScore() {                                                                             // Funktion zur Ausgabe des Spielstands
  char buffer[6];
  snprintf(buffer, sizeof(buffer), "%2d:%2d", scoreArray[0], scoreArray[1]);
  Serial.println(buffer);
}

void setup() {                                                                                  // Setup-Funktion
  Serial.begin(115200);                                                                         // Initialisierung der seriellen Schnittstelle
  Serial.println("Setup started");

  pinMode(pinArrayInput[0], INPUT_PULLUP);                                                      // Aktiviere interne Pull-up-Widerstände für die Eingänge
  pinMode(pinArrayInput[1], INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinArrayInput[0]), ISR_Goal1, FALLING);                 // Interrupts für Lichtschranken
  attachInterrupt(digitalPinToInterrupt(pinArrayInput[1]), ISR_Goal2, FALLING);
  Serial.println("Setup completed");
}

void loop() {                                                                                   // Loop-Funktion
  if (scoreArray[3] == false) {                                                                 // Überprüfen, ob das Spiel beendet ist
    if (scoreArray[0] >= gameConfigArray[1]) {                                                  // Überprüfen, ob ein Team gewonnen hat
      Serial.println("Team 1 wins!");
      scoreArray[3] = true;                                                                     // Spielende auf true setzen
      scoreArray[2] = 1;                                                                        // Sieger-Team auf 1 setzen (Team 1)
    } else if (scoreArray[1] >= gameConfigArray[1]) {
      Serial.println("Team 2 wins!");
      scoreArray[3] = true;                                                                     // Spielende auf true setzen
      scoreArray[2] = 2;                                                                        // Sieger-Team auf 2 setzen (Team 2)
    }
  } else {
      if (scoreArray[2] == 1) {                                                                 // Team 1 hat gewonnen
        led1.activate(500);
        led3.activate(500);
      } else if (scoreArray[2] == 2) {                                                          // Team 2 hat gewonnen
        led2.activate(500);
        led3.activate(500);
      }
    }
}