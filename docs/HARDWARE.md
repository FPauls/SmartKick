# Hardware-Architektur & Entwicklung

## 🏗️ **System-Überblick**

SmartKick basiert auf einer **modularen Hardware-Architektur**, die professionelle Komponenten mit kostengünstigen Maker-Elementen kombiniert. Das System wurde für **Langzeitbetrieb** und **einfache Wartung** konzipiert.

<div align="center">
<img src="../images/hardware/gehaeuse-render.png" alt="SmartKick 3D-Gehäuse Design" width="500">
<br>
<em>3D-CAD Konstruktion des SmartKick Gehäuses (Fusion 360)</em>
</div>

---

## 💰 **Komponentenauswahl & Kostenkalkulation**

Die Bauteilwahl folgte **industriellen Standards** bei **optimierten Kosten** für den Bildungsbereich:

| Kategorie | Komponente | Typ | Invest | Begründung |
|-----------|------------|-----|--------|------------|
| **🖥️ HMI** | Nextion Display | NX8048T070_11 (7") | 128,90€ | Industrieller Touch-Controller, keine PC nötig |
| **🧠 MCU** | Arduino Uno R3 | ATmega328P | 28,51€ | Bewährte Plattform, große Community |
| **⚡ Power** | Powerbank | 10.000mAh QC3.0 | 17,99€ | Autonomer Betrieb, Smartphone-kompatibel |
| **👁️ Sensorik** | IR-Lichtschranken | Adafruit ADA2168 (2x) | 31,80€ | Präzise 5mm-Sensoren, industrielle Qualität |
| **🔌 Interface** | Verbindungstechnik | DIN-4Pol + USB-C | 12,09€ | Professionelle Steckverbinder |
| **🛡️ Security** | Schlüsselschalter | Metall 12mm | 2,49€ | Zugangskontrolle in öffentlichen Bereichen |
| **🏠 Gehäuse** | 3D-Konstruktion | PLA (~100g) | 2,45€ | Maßgeschneiderte Lösung, recyceltes Material |
| **🔧 Integration** | Verkabelung & Montage | Diverse | 34,18€ | Klemmenblöcke, Adapter, Befestigungsmaterial |
| | **MATERIAL-GESAMT** | | **258,41€** | Einzelfertigung ohne Arbeitszeit |



---

## ⚡ **Elektronik-Design**

### **Systemarchitektur**

<div align="center">
<img src="../images/hardware/schaltplan.png" alt="SmartKick Schaltplan" width="600">
<br>
<em>Vollständige Schaltung mit Spannungsversorgung und Signalwegen</em>
</div>

**Arduino Uno Pin-Belegung:**
```
DIGITALE PINS:
Pin 0 (RX)  → Nextion Display TX
Pin 1 (TX)  → Nextion Display RX  
Pin 2 (INT) → IR-Sensor Tor 1 (Interrupt-fähig)
Pin 3 (INT) → IR-Sensor Tor 2 (Interrupt-fähig)
Pin 4 (PWM) → RGB-LED Rot  (optional)
Pin 5 (PWM) → RGB-LED Grün (optional)  
Pin 6 (PWM) → RGB-LED Blau (optional)
Pin 7       → RGB-LED GND  (optional)

STROMVERSORGUNG:
5V  → Nextion Display, IR-Sensoren
GND → Gemeinsame Masse aller Komponenten
VIN → Powerbank 5V Eingang
```

**Hardware-Kommunikation:**
- **UART-Verbindung** (Pin 0/1) für Display-Ansteuerung
- **Hardware-Interrupts** (Pin 2/3) für Echtzeit-Torerkennung
- **PWM-Ausgänge** (Pin 4-6) für RGB-LED Farbmischung
- **Spannungsverteilung** über Arduino 5V Rail

---

## 🏗️ **Mechanische Konstruktion**

### **3D-Gehäuse Engineering**

**Verfügbare CAD-Daten aus dem Projekt:**
```
📁 /3D-Daten/
├── Gehäuse v18.f3d    # Fusion 360 Projektdatei (editierbar)
├── Gehäuse.stl        # Hauptgehäuse (druckfertig)
└── Rückwand.stl       # Serviceklappe (separat druckbar)
```

**Design-Features der Version 18:**
- **Präzise Display-Aussparung** für 7" Nextion (154×85.8mm aktive Fläche)
- **Modulare Rückwand** für einfachen Service-Zugang
- **Integrierte Befestigungspunkte** für Arduino und Powerbank
- **Kabelführung** mit Zugentlastung
- **Materialoptimiert** für PLA-3D-Druck

### **3D-Druck Spezifikation**

**Empfohlene Parameter** (basierend auf Prototyp-Fertigung):
```
Material:       PLA (recycelt empfohlen)
Schichthöhe:    0.2mm für Oberflächenqualität
Infill:         20-25% für Stabilität vs. Materialverbrauch
Stützstrukturen: Minimal durch optimiertes Design
Nachbearbeitung: Entgraten der Display-Öffnung
```

**Gehäuse-Dimensionen:**
- **Außenmaße**: ~200×150×100mm (L×B×H)
- **Wandstärke**: 3-5mm für Stabilität
- **Display-Öffnung**: 154×85.8mm (exakt für Nextion)
- **Anschluss-Bohrungen**: USB-C, DIN-Buchsen, Schlüsselschalter

---

## 🔍 **Sensor-Technologie**

### **IR-Lichtschranken Spezifikation**

**Adafruit ADA2168 Eigenschaften:**
- **Erfassungsbereich**: 5mm (optimal für Tischkicker-Geometrie)
- **Wellenlänge**: 940nm Infrarot (unsichtbar)
- **Reaktionszeit**: <1ms (Hardware-Interrupt kompatibel)
- **Betriebsspannung**: 3.3V-5V (Arduino-kompatibel)
- **Stromverbrauch**: ~10mA pro Sensor
- **Ausgangssignal**: Digital HIGH/LOW

### **Sensor-Montage**
- **Position**: 5mm hinter Torlinie für optimale Erfassung
- **Befestigung**: Klebepads oder Schraubmontage je nach Tischkicker
- **Verkabelung**: 4-adrig über DIN-Stecker (VCC, GND, Signal, Reserve)
- **Austauschbarkeit**: Modulare Verbindung für einfachen Service

---

## 🔋 **Power-Management & Energieverbrauch**

### **Energiekonzept**

**Geschätzte Verbrauchswerte** (basierend auf Herstellerangaben):

```
ENERGIE-SCHÄTZUNG (Herstellerdaten):
┌─────────────────────────────────────┐
│ Arduino Uno:      ~200mA @ 5V       │
│ Nextion 7":       ~400mA @ 5V       │
│ IR-Sensoren:      ~20mA @ 5V        │
│ RGB-LED:          ~60mA @ 5V        │
│ Gesamt-System:    ~680mA @ 5V       │
│ Theoretische Zeit: 14.7h (10Ah)     │
│ Praxis-Schätzung:  ~4-6h (Verluste) │
└─────────────────────────────────────┘
```

*⚠️ Hinweis: Werte basieren auf Datenblättern, nicht auf Messung des echten Systems*

### **Powerbank-Integration**

**KB-Powerbank-M10000 Spezifikation:**
- **Kapazität**: 10.000mAh bei 3.7V (37Wh)
- **Ausgänge**: 2×USB-A, 1×USB-C 
- **Eingänge**: USB-C PD für Schnellladung
- **Schutzschaltungen**: Überstrom, Überspannung, Kurzschluss
- **Pass-Through**: Laden während Entladen möglich

**Integration im Gehäuse:**
- **Austauschbar** ohne Gehäuse-Demontage
- **USB-C Passthrough** für externe Geräte

---

## 🖥️ **Display-Integration**

### **Nextion 7" Touch-Spezifikation**

**NX8048T070_11 Eigenschaften:**
- **Bildschirmgröße**: 7" (178mm Diagonale)
- **Auflösung**: 800×480 Pixel
- **Touch-Technologie**: Resistiv (druckempfindlich)
- **Betriebsspannung**: 5V (Arduino-kompatibel)
- **Stromverbrauch**: ~400mA bei voller Helligkeit
- **Kommunikation**: UART (9600 Baud)
- **Programmierung**: Nextion Editor (GUI-Design)

**Mechanische Integration:**
- **Frontmontage** mit präziser Gehäuse-Aussparung
- **4-Punkt Befestigung** für vibrationsfreie Montage
- **Kabelführung** durch Gehäuse-Rückseite
- **Touch-Kalibrierung** werkseitig vorkonfiguriert

---

## 🔧 **Modulare Konstruktion**

### **Service-Design**

**Rückwand-Konzept** (`Rückwand.stl`):
- **Separater 3D-Druck** für einfache Demontage
- **Schraubverbindung** (4×M3 Schrauben) für werkzeuggestützten Service
- **Arduino-Zugang** für Hardware-Updates
- **Powerbank-Wechsel** ohne Hauptgehäuse-Öffnung
- **Kabelmanagement** mit Zugentlastung

### **Anschluss-Konzept**

**DIN-Steckverbinder für Sensoren:**
- **4-Pol Rundstecker** (VCC, GND, Signal, Reserve)
- **Professioneller Standard** für Industrie-Kompatibilität
- **Schnelle Demontage** bei Tischkicker-Wechsel
- **Modularer Austausch** defekter Sensoren
- **Farbkodierung** für eindeutige Zuordnung

**USB-C Integration:**
- **Einbaubuchse** mit Überspannungsschutz
- **Powerbank-Ladung** während des Betriebs
- **Smartphone-Ladefunktion** (Powerbank-Passthrough)
- **Zukunftssicherer Standard** (nicht Micro-USB)
- **Robuste Befestigung** gegen mechanische Belastung

### **Schlüsselschalter-Integration**

**Metall-Schlüsselschalter 12mm:**
- **Zugangskontrolle** für öffentliche Bereiche
- **2-Positionen**: EIN/AUS mit Rastfunktion
- **Hochwertige Mechanik** für Langzeitbetrieb
- **Frontmontage** für einfache Bedienung
- **Master-Schlüssel-System** möglich

---

**Navigation**: [← Projekt-Übersicht](../README.md) | **Software** → [Code-Architektur](SOFTWARE.md) | **Bedienung** → [Touch-Interface](USAGE.md) | **Development** → [Debug-Console](CONSOLE.md)