# SmartKick
> Intelligente Torerfassung für Tischkicker

<img alt="Version" src="https://img.shields.io/badge/version-1.0-blue">
<img alt="License" src="https://img.shields.io/badge/license-MIT-green">
<img alt="Arduino" src="https://img.shields.io/badge/Arduino-Compatible-teal">

## 🎯 Was ist SmartKick?

Verwandle deinen Tischkicker in eine moderne Spielarena! SmartKick erfasst automatisch Tore mit Lichtschranken und zeigt alles auf einem Touch-Display an.

![SmartKick Gehäuse](./images/gehaeuse-render.png)
*3D-gedrucktes Gehäuse mit integriertem Nextion Touch-Display*

**Ein Schulprojekt der FSE2A** - Wir sind angehende Elektrotechniker am Hans-Böckler-Berufskolleg und haben einen vorhandenen Tischkicker digital erweitert. Open Source für alle!

## ✨ Features auf einen Blick

- ⚽ **Automatische Torerkennung** mit Lichtschranken-Sensoren
- 📱 **7" Touch-Display** für Steuerung und Anzeige  
- 🎮 **3 Spielmodi**: Freies Spiel, Zeitspiel, Klassisches Spiel
- 🏆 **Individuell anpassbar**: Teamnamen, Spielzeit, Torziele
- 🔧 **Einfacher Nachbau** mit 3D-Gehäuse und Anleitung
- ⚡ **3+ Stunden Akkulaufzeit** mit USB-C Powerbank

## 🚀 Schnellstart

### 1. 📦 Hardware beschaffen
**[→ Zur detaillierten Teileliste](./docs/HARDWARE.md)**

| Was du brauchst | Ungefähre Kosten |
|----------------|------------------|
| Arduino Uno R3 | ~29€ |
| Nextion 7" Display | ~129€ |
| 2x IR-Lichtschranken | ~32€ |
| 10.000mAh Powerbank | ~18€ |
| 3D-Druck + Kleinteile | ~25€ |
| **Gesamt** | **~233€** |

### 2. 🔧 Zusammenbauen
**[→ Zur Hardware-Setup Anleitung](./docs/HARDWARE.md#installation)**

- Pin-Belegung folgen
- 3D-Gehäuse drucken
- Sensoren am Tischkicker montieren

### 3. 💻 Software installieren
**[→ Zur Software-Anleitung](./docs/SOFTWARE.md)**

- Arduino Code uploaden
- Nextion Display programmieren
- Erste Inbetriebnahme

### 4. 🎮 Loslegen!
**[→ Zur Bedienungsanleitung](./docs/USAGE.md)**

```bash
set mode time     # Zeitspiel aktivieren
set time 5        # 5 Minuten Spielzeit
set name1 Heim    # Team 1 benennen
set name2 Gast    # Team 2 benennen
set start         # Spiel starten
```

## 🎮 So funktioniert's

### Spielmodi im Überblick
- **🆓 Freies Spiel**: Unbegrenzt spielen ohne Zeitlimit
- **⏰ Zeitspiel**: Feste Spielzeit (1-99 Minuten)
- **🏆 Klassisches Spiel**: Erstes Team mit X Toren gewinnt

### Bedienung
- **Touch-Display**: Direkte Steuerung über das 7" Display
- **Serielle Konsole**: Vollständige Kontrolle per Terminal (115200 Baud)
- **Automatisch**: Tore werden per IR-Lichtschranke erkannt

![Nextion Display](./images/nextion-main-screen.png)
*Touch-Display zeigt Spielstand, Zeit und Teamnamen*

## 🏫 Projekt-Hintergrund

**Entwicklerteam**: Philipp Klein, Felix Pauls, Florian Groß-Hartmann  
**Schule**: Hans-Böckler-Berufskolleg, Münster  
**Fachrichtung**: Elektrotechnik (FSE2A)  
**Projektjahr**: 2025  

Dieses Projekt ist unser Schulprojekt im Rahmen der Elektrotechniker-Ausbildung. Wir veröffentlichen es als Open Source, damit andere davon profitieren können!

**Hinweis**: Das Projekt wird eventuell nach Abschluss der Technikerschule archiviert.

## 📚 Vollständige Dokumentation

- 🛠️ **[Hardware-Setup](./docs/HARDWARE.md)** - Teileliste, Schaltplan, 3D-Druck, Installation
- 💻 **[Software-Details](./docs/SOFTWARE.md)** - Code-Architektur, Installation, Entwicklung
- 🎮 **[Bedienungsanleitung](./docs/USAGE.md)** - Spielmodi, Befehle, FAQ, Troubleshooting

## 📁 Projektdateien

```
SmartKick/
├── 📄 README.md              # Diese Datei - Projekt-Überblick
├── 📄 SmartKick.ino          # Arduino Hauptprogramm
├── 📁 docs/                  # Detaillierte Dokumentation
│   ├── HARDWARE.md          # Teileliste, Schaltplan, 3D-Druck
│   ├── SOFTWARE.md          # Code-Dokumentation, Architektur
│   └── USAGE.md             # Bedienung, Spielmodi, Befehle
├── 📁 3D-Daten/             # STL-Dateien für 3D-Druck
├── 📁 Nextion HMI/          # Display-Interface Projekt
└── 📁 images/               # Screenshots und Bilder
```

## 📄 Lizenz

MIT-Lizenz - Siehe [LICENSE](LICENSE) für Details.

---

<p align="center">
  <strong>🎮 Bereit für das nächste Level Tischkicker? 🎮</strong><br>
  <sub>Made with ❤️ by FSE2A-Team SmartKick | © 2025 Hans-Böckler-Berufskolleg</sub>
</p>