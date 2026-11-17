# Rein Raus – Intelligente Parkplatzerkennung

**Projekt im Modul Mikrocontroller | Gruppe 16**  
**Autoren:** Daniel Rosiak & Tim Lintzen  
**Institution:** Hochschule Niederrhein (Ingenieurwissenschaften und Informatik)  
**Microcontroller:** ESP32-C3-DevKitC-02  

---

## Inhaltsverzeichnis

1. [Projektübersicht & Motivation](#projektübersicht--motivation)
2. [Kernfunktionen & Mehrwert](#kernfunktionen--mehrwert)
3. [Hardware & Stückliste (BOM)](#hardware--stückliste-bom)
4. [PCB Layout & Pin-Belegung](#pcb-layout--pin-belegung)
5. [Systemarchitektur & Zustandsautomat (FSM)](#systemarchitektur--zustandsautomat-fsm)
6. [Software & Installation](#software--installation)
7. [Inbetriebnahme & Test](#inbetriebnahme--test)
8. [Projekt-Team](#projekt-team)

---

## Projektübersicht & Motivation

In modernen Parkhäusern führt die Suche nach freien Parkplätzen ohne automatisierte Anzeige oft zu Frust, Zeitverlust und unnötigem Suchverkehr. Weder Parkgäste noch Betreiber besitzen ohne ein intelligentes Überwachungssystem einen klaren Überblick über die reale Auslastung.

Das Projekt **„Rein Raus – Intelligente Parkplatzerkennung“** löst dieses Problem durch eine automatisierte, mikrocontrollergesteuerte Zufahrts- und Ausfahrtskontrolle. Auf Basis eines **ESP32-C3 DevKitC-02** steuert das System zwei Schranken (Servomotoren), verifiziert Fahrzeuge über eine zweistufige Optosensorik und visualisiert Fahrerhinweise sowie freie Stellplätze in Echtzeit auf zwei OLED-Displays.

---

## Kernfunktionen & Mehrwert

Das System wurde gezielt entwickelt, um typische Fehlauslösungen herkömmlicher Parkplatzschranken zu vermeiden:

* **Zweistufige Verifikationslogik:** Durch den Einsatz einer Doppel-Lichtschranke ($S_1$ und $S_2$) wird die genaue Fahrtrichtung ausgewertet.
* **Ausschluss von Nicht-KFZ:** Fußgänger oder Radfahrer, die nur einen einzelnen Sensor abdecken, lösen die Schranke nicht aus.
* **Trennlogik bei dichtem Auffahren:** Aufeinanderfolgende Fahrzeuge werden sauber getrennt und einzeln gezählt.
* **Schutz vor Fehlzählungen bei Rangierbewegungen:** Rückwärtsfahrende Fahrzeuge führen zu keinem falschen Zählerimpuls; bei Timeout kehrt das System sauber in den Bereitschaftszustand zurück.
* **Echtzeit-Feedback über Dual-OLED:**
  * **Display 1 (Fahrer-Info):** Handlungsanweisung („Fahren für das nächste Auto“ / „Warte...“ / „BESETZT“).
  * **Display 2 (Kapazitäts-Zähler):** Verbleibende freie Parkplätze in Großschrift.

---

## Hardware & Stückliste (BOM)

Die Gesamtkosten der benötigten Elektronik- und Platinenkomponenten belaufen sich auf **33,43 €** (Stand Oktober 2026).

### Sensorik, Aktorik & Displays

| Position | Komponente | Stückzahl | Einzelpreis / Summe | Funktion |
| :--- | :--- | :---: | :---: | :--- |
| Servomotor | SG90 9g Micro-Servo | 2x | 3,30 € | Schrankenantrieb Ein- & Ausfahrt |
| Lichtschranke | SEN0503 IR-Sensor | 4x | 15,64 € | Doppel-Lichtschranken S1/S2 |
| Ultraschallsensor | HC-SR04P Entfernungsmesser | 2x | 3,78 € | Durchfahrtsprüfung Innenbereich |
| OLED-Display | 0,96" I2C SSD1306 (128x64) | 2x | 5,98 € | Status & Zähleranzeige |
| Steckkabel | Jumper Wire Set | 1x | 1,95 € | Signal- & Stromverbindungen |
| **Summe Sensorik & Aktorik** | | | **30,65 €** | |

### Trägerplatine (PCB Buchsenleisten)

| Position | Steckbuchsenleiste | Stückzahl | Preis | Funktion auf Platine |
| :--- | :--- | :---: | :---: | :--- |
| Buchse 1x14 Pin | SSK02-FPH-254-A14 | 1x | 0,46 € | GND-Schiene (J1) |
| Buchse 1x10 Pin | SSK02-FPH-254-A10 | 1x | 0,41 € | 5V-Spannungsschiene (J2) |
| Buchse 1x6 Pin | SSK02-FPH-254-A6 | 1x | 0,31 € | LeftSideGPIO Header (J4) |
| Buchse 1x8 Pin | SSK02-FPH-254-A8 | 1x | 0,34 € | RightSideGPIO Header (J5) |
| Buchse 1x4 Pin | SSK02-FPH-254-A4 | 1x | 0,28 € | 3.3V-Spannungsschiene (J3) |
| Buchse 1x15 Pin | SSK02-FPH-254-A15 | 2x | 0,98 € | ESP32-C3 DevKit Socket (U2) |
| **Summe Platine** | | | **2,78 €** | |

### Modellbau & Stromversorgung

| Position | Beschreibung | Stückzahl | Kosten |
| :--- | :--- | :---: | :---: |
| Grundplatte | Modellbau-Untergrund | 1x | 0,00 € (Eigenbestand) |
| Modellautos | Testfahrzeuge | 2x | 0,00 € (Eigenbestand) |
| Stromversorgung | Powerbank / 5V Netzstecker | 1x | 0,00 € (Eigenbestand) |
| Gehäuse | 3D-Drucke Schrankengehäuse | Nx | Restbudget |
| **Gesamtsumme Projekt** | | | **33,43 €** |

---

## PCB Layout & Pin-Belegung

Das maßgeschneiderte Trägerboard erweitert das **ESP32-C3-DevKitC-02** (`U2`) um zentrale Stromschienen sowie gut erreichbare Steckbuchsen.

### Versorgungsleisten
* **`J1` (GND):** 14-polige Buchsenleiste für alle Masse-Verbindungen.
* **`J2` (5V):** 10-polige Buchsenleiste für Servomotoren und Stromversorgung.
* **`J3` (3V3):** 4-polige Buchsenleiste für Logikspannung und OLED-Displays.

### GPIO-Pinbelegung (Anschluss an `J4` & `J5`)

| Baugruppe | Modul-Signal | Header | Header-Pin | ESP32-C3 GPIO | Funktion / Betriebsspannung |
| :--- | :--- | :--- | :---: | :---: | :--- |
| **Displays (I2C)** | SCL | **J4 (Left)** | Pin 1 | **GPIO 9** | I2C Clock (Display 1 & 2) |
| | SDA | **J4 (Left)** | Pin 2 | **GPIO 8** | I2C Data (Display 1 & 2) |
| **Ausfahrt (OUT)** | Servo OUT | **J4 (Left)** | Pin 4 | **GPIO 6** | PWM Schranke Ausfahrt (5V) |
| | LS2_OUT (Außen) | **J4 (Left)** | Pin 5 | **GPIO 5** | Lichtschranke Außen (S2_OUT) |
| | LS1_OUT (Innen) | **J4 (Left)** | Pin 6 | **GPIO 4** | Lichtschranke Innen (S1_OUT) |
| **Einfahrt (IN)** | LS1_IN (Außen) | **J5 (Right)**| Pin 8 | **GPIO 0** | Lichtschranke Außen (S1_IN) |
| | LS2_IN (Innen) | **J5 (Right)**| Pin 7 | **GPIO 1** | Lichtschranke Innen (S2_IN) |
| | Servo IN | **J5 (Right)**| Pin 6 | **GPIO 2** | PWM Schranke Einfahrt (5V) |
| | US TRIG IN | **J5 (Right)**| Pin 5 | **GPIO 3** | Ultraschall Trigger Einfahrt |
| | US ECHO IN | **J5 (Right)**| Pin 4 | **GPIO 10**| Ultraschall Echo Einfahrt |
| | US TRIG OUT | **J5 (Right)**| Pin 3 | **GPIO 20**| Ultraschall Trigger Ausfahrt |
| | US ECHO OUT | **J5 (Right)**| Pin 2 | **GPIO 18**| Ultraschall Echo Ausfahrt |

*Hinweis:* Bei Display 2 muss der Lötjumper `SA0/ADDR` auf der Rückseite mit **3,3V** verbunden werden, um die I2C-Adresse von `0x3C` auf `0x3D` zu ändern.

---

## Systemarchitektur & Zustandsautomat (FSM)

Die Firmware basiert auf einer **nicht-blockierenden Zustandsmaschine (Finite State Machine / FSM)**, die Einfahrt und Ausfahrt über `millis()`-Zeitstempel unabhängig voneinander steuert.

### Zustandsdiagramm

```
           +---------------------------------------------+
           |                 STATE_IDLE                  |
           | Schranke ZU | Display 1: "Warte..."         |
           +----------------------+----------------------+
                                  | S1 getriggert & Plätze > 0
                                  v
           +---------------------------------------------+
           |               STATE_VERIFYING               |
           | Warten auf S2 (Timeout: 4000 ms)            |
           +----------------------+----------------------+
                                  | S2 getriggert (Richtige Sequenz)
                                  v
           +---------------------------------------------+
           |                 STATE_OPEN                  |
           | Servo auf 90° | Display 1: "Fahren..."      |
           +----------------------+----------------------+
                                  | Ultraschall < 12 cm
                                  v
           +---------------------------------------------+
           |                STATE_PASSING                |
           | Fahrzeug im Schranken-Innenbereich          |
           +----------------------+----------------------+
                                  | Ultraschall >= 12 cm
                                  v
           +---------------------------------------------+
           |                STATE_CLOSING                |
           | Servo auf 0° | Zählerupdate | Display 2 Auto |
           +----------------------+----------------------+
                                  | Rückkehr nach 1s Delay
                                  v
                           ( STATE_IDLE )
```

---

## Software & Installation

### Voraussetzungen
1. **Arduino IDE** (Version 2.x empfohlen)
2. **ESP32 Board Support** in der Arduino IDE installieren (über den ESP32 Boardverwalter).
3. Board auswählen: **ESP32C3 Dev Module** oder **ESP32-C3 DevKitC-02**

### Benötigte Bibliotheken
Installieren Sie folgende Bibliotheken über den Arduino Bibliotheksverwalter:
* `Adafruit SSD1306` (OLED Displaytreiber)
* `Adafruit GFX Library` (Grafikbibliothek)
* `ESP32Servo` (PWM Servo-Steuerung für ESP32)

### Flashen des Mikrocontrollers
1. Öffnen Sie die Datei `parkplatz_steuerung_esp32c3-v2.ino`.
2. Verbinden Sie den ESP32-C3 über USB-C mit dem PC.
3. Wählen Sie den passenden COM-Port und das Board `ESP32C3 Dev Module`.
4. Klicken Sie auf **Hochladen**.

---

## Inbetriebnahme & Test

1. **Serieller Monitor:** Öffnen Sie den Seriellen Monitor bei **115200 Baud**. Beim Start erscheint die Meldung:
   `>>> Systemstart: Rein Raus Parkplatzerkennung (ESP32-C3) <<<`
2. **Display-Test:** Display 1 zeigt `FAHRER-INFO: Warte...`, Display 2 zeigt `Frei: 10/10 STATUS: FREI`.
3. **Einfahrtstest:**
   * Führen Sie ein Modellauto über Lichtschranke S1 (Außen) und anschließend über S2 (Innen).
   * Die Schranke öffnet sich auf 90° und Display 1 schaltet auf `Fahren...`.
   * Passieren Sie den Ultraschallsensor. Nach dem Verlassen schließt die Schranke und der Zähler sinkt auf `9/10`.
4. **Fehlflankentest:** Unterbrechen Sie nur Sensor S1 und warten Sie 4 Sekunden. Das System bricht die Verifikation ab und kehrt ohne Zählung in den Zustand `IDLE` zurück.

---

## Projekt-Team

**Gruppe 16 – Modul Mikrocontroller**  
* **Daniel Rosiak** (Gelernter Mechatroniker)
* **Tim Lintzen** (Gelernter Mechatroniker)

**Hochschule Niederrhein**  
Fachbereich Ingenieurwissenschaften und Informatik  
*Stand: November 2026*
