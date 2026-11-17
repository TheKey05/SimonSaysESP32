#include <Arduino.h>

// Pin-Belegung exakt nach deinem Screenshot:
// Index 0 -> LED 4 (IO7) / Taster S4 (IO18 -> Start-Taster)
// Index 1 -> LED 3 (IO6) / Taster S3 (IO2)
// Index 2 -> LED 2 (IO5) / Taster S2 (IO1)
// Index 3 -> LED 1 (IO4) / Taster S1 (IO0)
const int ledPins[4]    = {7, 6, 5, 4};
const int buttonPins[4] = {18, 2, 1, 0};

// Bezeichnung der Taster für den Serial Monitor
const char* buttonLabels[4] = {
  "Taster S4 (GPIO 18 - Start)",
  "Taster S3 (GPIO 2)",
  "Taster S2 (GPIO 1)",
  "Taster S1 (GPIO 0)"
};

// Timing-Einstellungen (in Millisekunden)
const int LED_ON_TIME             = 500;   // Leuchtdauer pro LED
const int LED_PAUSE_TIME          = 300;   // Pause zwischen zwei LEDs
const unsigned long INPUT_TIMEOUT = 3000;  // 3 Sekunden Zeit pro Tastendruck

const int MAX_LEVEL = 50;
int sequence[MAX_LEVEL];
int currentLevel = 1;

// Funktions-Prototypen
void waitForStartButton();
void generateSequence();
void playSequence();
bool checkPlayerInput();
int waitForButtonPress();
void waitForButtonRelease(int btnIndex);
void startAnimation();
void gameOverAnimation();
void winAnimation();

void setup() {
  Serial.begin(115200);
  delay(1000); // Kurze Wartezeit für USB-Serial auf dem ESP32-C3

  // LEDs als Ausgang konfigurieren und ausschalten
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Taster mit internem Pullup konfigurieren (schalten gegen GND)
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Zufallsgenerator über Rauschen eines offenen Analog-Pins initialisieren
  randomSeed(analogRead(3));

  Serial.println("\n=== SIMON SAYS BEREIT ===");
  Serial.println("Druecke Taster S4 (GPIO 18) zum Starten!");
}

void loop() {
  // 1. Warten, bis der Start-Taster gedrückt wird (mit Lauflicht-Animation)
  waitForStartButton();

  // 2. Neues Spiel vorbereiten
  currentLevel = 1;
  generateSequence();
  startAnimation();
  delay(600);

  // 3. Spielschleife über alle Level
  bool gameRunning = true;
  while (gameRunning && currentLevel <= MAX_LEVEL) {
    Serial.print("--- Level ");
    Serial.print(currentLevel);
    Serial.println(" ---");

    // Aktuelle Sequenz abspielen
    playSequence();

    // Eingabe des Spielers prüfen
    if (checkPlayerInput()) {
      Serial.println("Richtig! Naechstes Level...");
      currentLevel++;
      delay(800);
    } else {
      Serial.println("Falscher Taster oder Zeit abgelaufen! GAME OVER.");
      gameOverAnimation();
      gameRunning = false;
    }
  }

  // Falls alle 50 Level geschafft wurden
  if (currentLevel > MAX_LEVEL) {
    Serial.println("GLUECKWUNSCH! Alle Level gemeistert!");
    winAnimation();
  }

  Serial.println("\nDruecke Taster S4 (GPIO 18) fuer eine neue Runde.");
}

// Wartet auf Taster S4 (Index 0 / GPIO 18) und zeigt ein sanftes Blinken
void waitForStartButton() {
  unsigned long lastBlink = 0;
  int animIndex = 0;

  while (digitalRead(buttonPins[0]) == HIGH) {
    if (millis() - lastBlink > 250) {
      lastBlink = millis();
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], (i == animIndex) ? HIGH : LOW);
      }
      animIndex = (animIndex + 1) % 4;
    }
  }

  // Alle LEDs aus und warten, bis Start-Taster losgelassen wird
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  waitForButtonRelease(0);
}

// Erzeugt eine zufällige Folge von 0 bis 3
void generateSequence() {
  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(0, 4);
  }
}

// Spielt die LED-Sequenz bis zum aktuellen Level ab
void playSequence() {
  for (int i = 0; i < currentLevel; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(LED_ON_TIME);
    digitalWrite(ledPins[led], LOW);
    delay(LED_PAUSE_TIME);
  }
}

// Prüft, ob der Spieler die komplette Sequenz korrekt nachdrückt
bool checkPlayerInput() {
  for (int step = 0; step < currentLevel; step++) {
    int pressedBtn = waitForButtonPress();

    // Timeout (-1) oder falscher Taster
    if (pressedBtn == -1 || pressedBtn != sequence[step]) {
      return false;
    }

    // Passende LED während des Tastendrucks aufleuchten lassen
    digitalWrite(ledPins[pressedBtn], HIGH);
    waitForButtonRelease(pressedBtn);
    digitalWrite(ledPins[pressedBtn], LOW);
    delay(100); // Kurze Entprellpause
  }
  return true;
}

// Wartet innerhalb von INPUT_TIMEOUT auf irgendeinen der 4 Taster
int waitForButtonPress() {
  unsigned long startTime = millis();

  while (millis() - startTime < INPUT_TIMEOUT) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(20); // Debounce (Entprellen)
        if (digitalRead(buttonPins[i]) == LOW) {
          Serial.print("Gedrueckt: ");
          Serial.println(buttonLabels[i]);
          return i;
        }
      }
    }
  }
  return -1; // Zeit abgelaufen
}

// Wartet, bis der gedrückte Taster wieder losgelassen wird
void waitForButtonRelease(int btnIndex) {
  while (digitalRead(buttonPins[btnIndex]) == LOW) {
    delay(10);
  }
  delay(20); // Debounce beim Loslassen
}

// Animation beim Spielstart
void startAnimation() {
  for (int repeat = 0; repeat < 2; repeat++) {
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], HIGH);
    delay(200);
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
    delay(200);
  }
}

// Animation bei Fehler / Game Over
void gameOverAnimation() {
  for (int repeat = 0; repeat < 4; repeat++) {
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], HIGH);
    delay(150);
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
    delay(150);
  }
}

// Animation bei Sieg (Level 50)
void winAnimation() {
  for (int repeat = 0; repeat < 5; repeat++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(80);
      digitalWrite(ledPins[i], LOW);
    }
  }
}