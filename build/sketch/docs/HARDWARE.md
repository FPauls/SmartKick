#line 1 "C:\\Projects\\Arduino\\SmartKick\\docs\\HARDWARE.md"
# Hardware Setup

## 📦 Einkaufsliste (Basierend auf Projektspezifikation)

### Hauptkomponenten (Exakte Modelle verwendet)

| Komponente | **Verwendetes Modell** | Spezifikation | **Tatsächliche Kosten** | Bezugsquelle |
|------------|------------------------|---------------|-------------------------|--------------|
| **Mikrocontroller** | **Arduino Uno Rev 3** | ATmega328P, 16MHz | **28,51€** | Arduino Official |
| **Display** | **Nextion NX8048T070_11** | **7"**, 800x480, Touch | **128,90€** | Nextion Official |
| **Lichtschranke 1** | **Adafruit ADA2168** | IR Break Beam Sensor | **15,90€** | Adafruit |
| **Lichtschranke 2** | **Adafruit ADA2168** | IR Break Beam Sensor | **15,90€** | Adafruit |
| **Powerbank** | **KB-Powerband-M10000** | 10.000mAh, QC3.0, USB-C | **17,99€** | Amazon |

**Hauptkomponenten Gesamt: 207,20€**

### Zusätzliche Bauteile (Projekt-spezifisch)

| Bauteil | **Verwendetes Modell** | Anzahl | Verwendung | **Kosten** |
|---------|------------------------|--------|------------|------------|
| **Arduino Terminal Block** | **Treedix Arduino Klemme Hut Block** | 1x | Sichere Verkabelung | **15,99€** |
| **DIN-Stecker** | **RUNCCI-YUN DIN 4-Pol** | 2x | Modulare Sensorverbindung | **2,10€** |
| **USB-C Buchse** | **MxyVrix USB C PD 60W** | 1x | Einbau-Ladebuchse | **9,99€** |
| **Schlüsselschalter** | **Gebildet Metall 12mm** | 1x | Unbefugte Nutzung verhindern | **2,49€** |
| **USB Adapter** | **DuHeSin 180° USB3.1** | 1x | Platzsparende Verkabelung | **2,20€** |
| **USB-C Adapter** | **MOGOOD 90° Rechts** | 1x | Winkel-Verbindung | **3,99€** |
| **Kabel/Schrauben** | **Pauschale** | 1x | Verkabelung & Montage | **10,00€** |

**Zusatzkomponenten Gesamt: 46,76€**

### 3D-Druck & Gehäuse

| Material | Spezifikation | Menge | Verwendung | **Kosten** |
|----------|---------------|-------|------------|------------|
| **Filament** | **100% Recyceltes PLA** | ~100g | Nachhaltiges Gehäuse | **2,45€** |
| **Gehäuse-Features** | - | - | **5mm Wandstärke, Serviceklappe** | - |

**Mechanik Gesamt: 2,45€**

## **💰 Gesamtkosten: 256,41€ (Materialkosten netto)**
*Entspricht dem Pflichtenheft: 326,41€ inkl. Kleinteile-/Energiepauschale*

### 3D-Druck & Montage

| Material | Typ | Menge | Verwendung | Kosten |
|----------|-----|-------|------------|--------|
| **3D-Druck Filament** | PLA/PETG | ~100g | Gehäuse + Rückwand | ~3€ |
| **3D-Druck Service** | - | - | Falls kein eigener Drucker | ~15€ |
| **Schrauben** | M3x8mm | 4-6x | Gehäusemontage | ~2€ |
| **Muttern** | M3 | 4-6x | Gehäusemontage | ~1€ |

**Mechanik Gesamt: ~5-20€ (je nach 3D-Druck)**

## 🔌 Pin-Belegung

```
Arduino Pin | Komponente           | Beschreibung
------------|---------------------|------------------------------------------
Pin 2       | Lichtschranke Tor 1 | Interrupt-fähig, INPUT_PULLUP
Pin 3       | Lichtschranke Tor 2 | Interrupt-fähig, INPUT_PULLUP
Pin 4       | RGB-LED Rot         | Optional, nur für Debug-Zwecke
Pin 5       | RGB-LED Grün        | Optional, nur für Debug-Zwecke
Pin 6       | RGB-LED Blau        | Optional, nur für Debug-Zwecke
Pin 7       | RGB-LED GND         | Optional, gemeinsame Kathode
Pin 8       | Nextion RX          | AltSoftSerial, zu Display TX
Pin 9       | Nextion TX          | AltSoftSerial, zu Display RX
5V          | VCC                 | Stromversorgung für Display
GND         | GND                 | Gemeinsame Masse
```

## 🔧 Schaltungsaufbau

### 1. Lichtschranken (Torsensoren)
```
Lichtschranke 1 (Tor Team 1):
├── VCC → Arduino 5V
├── GND → Arduino GND  
└── OUT → Arduino Pin 2 (mit INPUT_PULLUP)

Lichtschranke 2 (Tor Team 2):
├── VCC → Arduino 5V
├── GND → Arduino GND
└── OUT → Arduino Pin 3 (mit INPUT_PULLUP)
```

**Funktionsweise**: 
- Sensor triggert bei FALLING Edge (HIGH → LOW)
- Interne Pullup-Widerstände aktiviert
- Ball unterbricht Lichtschranke → Tor erkannt

### 2. Nextion Display
```
Nextion Display:
├── 5V → Arduino 5V (oder externes 5V Netzteil)
├── GND → Arduino GND
├── RX → Arduino Pin 8 (AltSoftSerial TX)
└── TX → Arduino Pin 9 (AltSoftSerial RX)
```

**Kommunikation**:
- Baudrate: 9600
- Protokoll: Nextion-spezifisch
- Library: AltSoftSerial

### 3. RGB-LED (Optional - nur Debug)
```
RGB-LED (Common Cathode):
├── Kathode (-) → Arduino Pin 7 (GND)
├── Rot → 220Ω Widerstand → Arduino Pin 4
├── Grün → 220Ω Widerstand → Arduino Pin 5
└── Blau → 220Ω Widerstand → Arduino Pin 6
```

**Debug-Funktionen**:
- Blau: Tor Team 1
- Rot: Tor Team 2  
- Grün: Spielende
- Aus: Normaler Betrieb

### 4. Stromversorgung
```
Empfohlen: Externes 5V/2A Netzteil
├── Arduino: ~100mA
├── Nextion Display: ~300-500mA
├── Sensoren: ~20mA
└── LED: ~20mA
Gesamt: ~650mA (2A Netzteil ausreichend)
```

## 📐 3D-Gehäuse

### Verfügbare Dateien
📁 **[3D-Daten Ordner](../3D-Daten/)**
- `Gehäuse v18.f3d` - Fusion 360 Projektdatei (editierbar)
- `Gehäuse.stl` - Hauptgehäuse für 3D-Druck
- `Rückwand.stl` - Rückwand für 3D-Druck

### Druckeinstellungen
```
Material:       PLA oder PETG
Layer-Höhe:     0.2mm (0.15mm für feinere Details)
Infill:         20% (ausreichend stabil)
Druckgeschw.:   50mm/s
Supports:       Ja (für Überhänge am Display-Ausschnitt)
Bett-Haftung:   Brim empfohlen
```

### Nachbearbeitung
1. **Supports entfernen** - Vorsichtig mit Seitenschneider
2. **Display-Ausschnitt prüfen** - Nextion muss exakt passen
3. **Schraubenlöcher nachbohren** - Falls zu eng gedruckt
4. **Oberfläche glätten** - Optional mit Schleifpapier (220er Körnung)

### Montage-Anleitung
1. **Display einsetzen** - Von vorne in den Ausschnitt
2. **Verkabelung durchführen** - Kabel durch Rückwand
3. **Arduino befestigen** - Mit Schrauben oder Klebepads
4. **Rückwand montieren** - Mit M3 Schrauben befestigen
5. **Funktionstest** - Vor endgültiger Montage

## 🔍 Schaltplan

*Hier kann später ein Fritzing-Diagramm oder gezeichneter Schaltplan eingefügt werden*

![Schaltplan Platzhalter](../images/schaltplan.png)
*Vollständiger Schaltplan des SmartKick Systems*

## ⚡ Inbetriebnahme

### Erste Schritte
1. **Verkabelung prüfen** - Alle Verbindungen nach Pin-Belegung
2. **Stromversorgung anschließen** - 5V für Arduino und Display
3. **Seriellen Monitor öffnen** - 115200 Baud
4. **Test-Befehle senden**:
   ```
   get config          # Systemkonfiguration anzeigen
   set debug 2         # Vollständige Debug-Informationen
   ```

### Sensor-Test
1. **Lichtschranken prüfen**:
   - Hand vor Sensor 1 → Team 1 Punkt müsste steigen
   - Hand vor Sensor 2 → Team 2 Punkt müsste steigen
2. **Entprellung testen** - Mehrfache schnelle Unterbrechungen
3. **LED-Feedback** (falls verbaut) - Farbwechsel bei Tor

### Display-Test
1. **Nextion-Verbindung**:
   ```
   set nextion on      # Display aktivieren
   nxt page 0          # Hauptseite anzeigen
   ```
2. **Anzeige-Updates**:
   ```
   set name1 Test1     # Team 1 Name setzen
   set name2 Test2     # Team 2 Name setzen
   set score1 +        # Team 1 Punkt hinzufügen
   ```

## 🔧 Troubleshooting

### Häufige Probleme

| Problem | Mögliche Ursache | Lösung |
|---------|------------------|--------|
| **Keine Tore erkannt** | Sensor-Verkabelung | Pin-Belegung prüfen, INPUT_PULLUP |
| **Doppelte Erkennung** | Entprellung zu kurz | Debounce-Zeit in Code erhöhen |
| **Display zeigt nichts** | Nextion-Verbindung | RX/TX vertauscht? Baudrate 9600? |
| **Arduino reagiert nicht** | Stromversorgung | USB-Kabel oder externes Netzteil |
| **LED funktioniert nicht** | Falsche Polung | Common Cathode? Vorwiderstände? |

### Debug-Hilfen
```
set debug 2             # Maximale Debug-Ausgabe
get config              # Aktuelle Konfiguration anzeigen
nxt get "sleep"         # Nextion Status abfragen
```

### Kontakte & Support
- **Issues**: [GitHub Issues](../../issues) für Probleme und Fragen
- **Diskussionen**: [GitHub Discussions](../../discussions) für allgemeine Fragen
- **E-Mail**: *Wird eventuell später ergänzt*

---

**Weiter zu**: [Software Installation →](SOFTWARE.md)

## 🔌 Technische Anforderungen (Projektspezifikation)

### Stromversorgung
- **Betriebsspannung**: 5V via USB-C (VDE-konform)
- **Stromaufnahme**: Max. 2A (spezifiziert)
- **Mindest-Betriebsdauer**: **3 Stunden** mit 10.000mAh Powerbank
- **Energieeffizienz**: Typisch 0.5A im Normalbetrieb

### Umgebungsbedingungen
- **Betriebstemperatur**: **0°C bis +40°C** (Gastronomie-tauglich)
- **Zuverlässigkeit**: **99%+ Torerkennung** (Abnahmekriterium)
- **Langzeittest**: **100+ Stunden** Dauerbetrieb validiert
- **Schutzklasse**: CE-Norm + VDE 0701-0702 konform

### Mechanische Eigenschaften
- **Gehäusematerial**: 100% recyceltes Filament (Nachhaltigkeit)
- **Wandstärke**: Mindestens 5mm (Robustheit)
- **Serviceklappe**: Rückwand für Komponentenwechsel
- **Befestigung**: Klettverschluss oder Schraubmontage