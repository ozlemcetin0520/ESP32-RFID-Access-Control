# 🏭 Industrial Smart Shelf & Secure Access Control System (IoT)

## 📌 Project Overview
This project demonstrates a robust, two-node Internet of Things (IoT) architecture designed for industrial warehouses, pharmacies, or secure storage facilities. It features an HTTP Client-Server communication model between two ESP32 microcontrollers, combining hardware-level security with live data monitoring.

## 🚀 Key Features
* **Secure Access Control (RFID):** MFRC522 RFID-based authorization. Only predefined 'Admin' or 'Guest' credentials can trigger the logic-converted relay (simulating an electronic shelf lock).
* **Real-Time Inventory Tracking:** An HC-SR04 Ultrasonic sensor continuously monitors shelf capacity and triggers physical/web alarms for critical stock levels.
* **HTTP Client-Server Network Architecture:** The Sensor Node (Client) measures data and continuously pushes it to the Central Security Node (Server) over a local Wi-Fi network using HTTP GET requests.
* **Live Web Dashboard:** A responsive HTML/CSS local web server hosted directly on the ESP32 to monitor access logs and live stock capacity simultaneously.

## 📸 System Dashboard & Live Web Interface
<img width="947" height="472" alt="deney_anaekranı" src="https://github.com/user-attachments/assets/21ae8940-0fc2-43b3-9df3-4fb2eff7b7a7" />


## ⚙️ Hardware Components & Topology
**Node 1 (Central Web Server & Access Control):**
* ESP32 Development Board
* MFRC522 RFID Reader (SPI Communication)
* Relay Module (with Logic Level Converter)

**Node 2 (Sensor Client & Alarm System):**
* ESP32 Development Board
* HC-SR04 Ultrasonic Sensor
* Active Buzzer & Warning LEDs

## 🧑‍💻 Author & Developer
**Özlem Çetin** *Internet and Network Technologies*
