#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h> 

const char* ssid = "ESP32-Parkir";
const char* password = "12345678";

// Buat WebServer port 80
WebServer server(80);

// HTML kamu dimasukkan ke dalam variabel string (raw literal)
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>IOT smart park</title>
  <style>
    body {
      margin: 0;
      font-family: 'Segoe UI', sans-serif;
      background: #1e1e1e;
      color: white;
    }
    header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      background: linear-gradient(to right, #3a5aff, #bb40c6);
      padding: 10px 20px;
    }
    .content {
      padding: 20px;
    }
    .location {
      margin-top: 10px;
      font-size: 14px;
    }
    .info-box {
      background: linear-gradient(to right, #003cff, #a34bcf);
      border-radius: 12px;
      padding: 15px;
      margin: 20px 0;
      display: flex;
      justify-content: space-between;
      align-items: center;
    }
    .park-panel {
      background: white;
      border-radius: 16px;
      color: black;
      padding: 20px;
    }
    .panel-header {
      background: linear-gradient(to right, #2d66f0, #a057ff);
      color: white;
      padding: 10px;
      border-radius: 10px 10px 0 0;
    }
    .slot-status {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin: 10px 0;
    }
    .slot-btn {
      background: linear-gradient(to right, #2a60ff, #b455ff);
      padding: 10px 20px;
      border: none;
      border-radius: 12px;
      color: white;
      font-weight: bold;
    }
    .status {
      padding: 10px 15px;
      border-radius: 10px;
      font-weight: bold;
      color: white;
    }
    .status.available {
      background: linear-gradient(to bottom, #00d100, #00aa00);
    }
    .status.occupied {
      background: linear-gradient(to bottom, #ff3333, #cc0000);
    }
    .jumlah-parkir {
      margin: 10px 0;
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-weight: bold;
    }
    .jumlah-box {
      border: 2px solid #000;
      border-radius: 10px;
      padding: 5px 15px;
      background: #eee;
      font-size: 18px;
    }
  </style>
</head>
<body>
  <header>
    <div><strong>Smart sistem Park</strong></div>
    <div id="clock">07:00 AM</div>
  </header>
  <div class="content">
    <h3>Anda sekarang berada di gedung A</h3>
    <div class="location"> SMA Negeri 4 Bojonegoro Smart Park</div>

    <div class="info-box">
      <div style="font-size: 30px;">Web ini menunjukkan jumlah parkir yang tersedia secara realtime</div>
      <div><strong>🚗</strong></div>
    </div>

    <div class="park-panel">
      <div class="panel-header"><strong>Park A</strong></div>
      <div class="jumlah-parkir">
        <span>Jumlah Parkir yang tersedia saat ini</span>
        <div class="jumlah-box">2</div>
      </div>

      <div class="slot-status">
        <button class="slot-btn">Park A1</button>
        <div class="status occupied">❌</div>
      </div>
      <div class="slot-status">
        <button class="slot-btn">Park A2</button>
        <div class="status available">✅</div>
      </div>
      <div class="slot-status">
        <button class="slot-btn">Park A3</button>
        <div class="status occupied">❌</div>
      </div>
      <div class="slot-status">
        <button class="slot-btn">Park A4</button>
        <div class="status available">✅</div>
      </div>
    </div>
  </div>
<script>
  function updateClock() {
    const now = new Date();
    let h = now.getHours();
    let m = now.getMinutes();
    let ampm = h >= 12 ? "PM" : "AM";
    h = h % 12 || 12;
    m = m < 10 ? "0" + m : m;
    document.getElementById("clock").textContent = `${h}:${m} ${ampm}`;
  }
  updateClock();
  setInterval(updateClock, 1000);
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.println("Access Point dibuat!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.softAPIP()); // default: 192.168.4.1

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server aktif!");
}

void loop() {
  server.handleClient();
}
