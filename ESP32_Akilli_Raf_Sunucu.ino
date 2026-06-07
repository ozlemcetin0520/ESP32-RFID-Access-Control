#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "WIFI_ADI";
const char* password = "WIFI_SIFRESI";

String adminUID = "B5 FD 79 AE"; // Yönetici
String guestUID = "63 2E 43 39"; // Misafir

#define RST_PIN 22
#define SS_PIN 5
#define RELAY_PIN 26

MFRC522 mfrc522(SS_PIN, RST_PIN);
WebServer server(80);

// Sistem Değişkenleri
bool kilitAcik = false;
int rafMesafe = 0; 
int bosRafMesafesi = 30; // 30 cm = Raf Boş

// Log Sistemi
String loglar[5] = {"Sistem Başlatıldı...", "-", "-", "-", "-"};

void logEkle(String mesaj) {
  for(int i=4; i>0; i--) { loglar[i] = loglar[i-1]; }
  loglar[0] = String(millis()/1000) + "sn: " + mesaj; 
}

// Web Arayüzü
void handleRoot() {
  int dolulukYuzdesi = map(rafMesafe, bosRafMesafesi, 5, 0, 100);
  if(dolulukYuzdesi > 100) dolulukYuzdesi = 100;
  if(dolulukYuzdesi < 0) dolulukYuzdesi = 0;
  
  String barRenk = (dolulukYuzdesi < 20) ? "#dc3545" : "#28a745"; 

  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>Akıllı Raf Paneli</title>";
  html += "<style>body{font-family:Arial; text-align:center; background:#f4f4f9;} ";
  html += ".box{background:white; padding:20px; margin:20px auto; width:80%; border-radius:10px; box-shadow:0px 4px 10px rgba(0,0,0,0.1);} ";
  html += ".bar-bg{background:#e0e0e0; border-radius:10px; width:100%; height:30px;} ";
  html += ".bar-fill{background:" + barRenk + "; height:100%; border-radius:10px; width:" + String(dolulukYuzdesi) + "%; transition: 0.5s;} ";
  html += "table{width:100%; border-collapse:collapse;} th,td{border-bottom:1px solid #ddd; padding:10px;}</style>";
  html += "<script>setInterval(function(){location.reload();}, 2000);</script></head><body>"; 
  
  html += "<h2>Endüstriyel Akıllı Raf ve Güvenlik Sistemi</h2>";
  
  html += "<div class='box'><h3>📦 Stok Doluluk Oranı: %" + String(dolulukYuzdesi) + "</h3>";
  html += "<div class='bar-bg'><div class='bar-fill'></div></div></div>";
  
  html += "<div class='box'><h3>🔒 Raf Kilit Durumu: " + String(kilitAcik ? "<span style='color:green'>AÇIK</span>" : "<span style='color:red'>KAPALI</span>") + "</h3></div>";

  html += "<div class='box'><h3>📋 Son Erişim Kayıtları</h3><table><tr><th>Zaman</th><th>Olay</th></tr>";
  for(int i=0; i<5; i++) {
    html += "<tr><td>" + loglar[i].substring(0, loglar[i].indexOf(":")) + " sn</td><td>" + loglar[i].substring(loglar[i].indexOf(":")+2) + "</td></tr>";
  }
  html += "</table></div></body></html>";
  
  server.send(200, "text/html", html);
}

// Node 2 (Arkadaşın) Veri Gönderme Rotası
void handleStokGuncelle() {
  if (server.hasArg("mesafe")) {
    rafMesafe = server.arg("mesafe").toInt();
    server.send(200, "text/plain", "Veri Alindi");
  } else {
    server.send(400, "text/plain", "Hata");
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nSunucu IP: " + WiFi.localIP().toString());
  
  server.on("/", handleRoot);
  server.on("/stokGuncelle", handleStokGuncelle);
  server.begin();
}

void loop() {
  server.handleClient();
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      tagUID += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      tagUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    tagUID.trim(); tagUID.toUpperCase();
    
    if (tagUID == adminUID || tagUID == guestUID) {
      kilitAcik = !kilitAcik; 
      digitalWrite(RELAY_PIN, kilitAcik ? LOW : HIGH);
      String kim = (tagUID == adminUID) ? "YÖNETİCİ" : "MİSAFİR";
      logEkle(kim + (kilitAcik ? " kilidi AÇTI." : " kilidi KAPATTI."));
    } else {
      logEkle("YETKİSİZ KART DENEMESİ!");
    }
    mfrc522.PICC_HaltA();
  }
}
