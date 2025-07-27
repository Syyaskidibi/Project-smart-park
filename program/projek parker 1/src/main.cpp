#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define SENSOR1 13
#define SENSOR2 14
#define SENSOR3 22
#define SENSOR4 23

const char* ssid = "ESP32-Parkir";
const char* password = "12345678";
WebServer server(80);

// Status parkir (0 = kosong, 1 = isi)
int slot[4] = {0, 0, 0, 0};

// Halaman HTML dengan tampilan sesuai permintaanmu
String htmlPage = R"rawliteral(
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
    .content { padding: 20px; }
    .location { margin-top: 10px; font-size: 14px; }
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
        <div id="available" class="jumlah-box">0</div>
      </div>

      <div class="slot-status"><button class="slot-btn">Park A1</button><div id="s1" class="status">❌</div></div>
      <div class="slot-status"><button class="slot-btn">Park A2</button><div id="s2" class="status">❌</div></div>
      <div class="slot-status"><button class="slot-btn">Park A3</button><div id="s3" class="status">❌</div></div>
      <div class="slot-status"><button class="slot-btn">Park A4</button><div id="s4" class="status">❌</div></div>
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
  setInterval(updateClock, 1000);
  updateClock();

  async function fetchData() {
    const res = await fetch('/data');
    const data = await res.json();
    let available = 0;
    for (let i = 0; i < 4; i++) {
      const el = document.getElementById('s' + (i + 1));
      if (data.slots[i] == 0) {
        el.textContent = '✅';
        el.className = 'status available';
        available++;
      } else {
        el.textContent = '❌';
        el.className = 'status occupied';
      }
    }
    document.getElementById("available").textContent = available;
  }
  setInterval(fetchData, 1000);
  fetchData();
</script>
</body>
</html>
)rawliteral";

// Halaman utama
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Endpoint JSON status sensor
void handleData() {
  String json = "{\"slots\":[";
  for (int i = 0; i < 4; i++) {
    json += String(slot[i]);
    if (i < 3) json += ",";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);

  WiFi.softAP(ssid, password);
  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {

  int ir1 = !digitalRead(SENSOR1);
  int ir2 = !digitalRead(SENSOR2);
  int ir3 = !digitalRead(SENSOR3);
  int ir4 = !digitalRead(SENSOR4);

  Serial.print("IR1: ");
  Serial.print(ir1);
  Serial.print(" | IR2: ");
  Serial.print(ir2);
  Serial.print(" | IR3: ");
  Serial.print(ir3);
  Serial.print(" | IR4: ");
  Serial.println(ir4);

  Serial.print("Sensor aktif: ");
  bool adaAktif = false;
  if (ir1) {
    Serial.print("IR1 ");
    adaAktif = true;
  }
  if (ir2) {
    Serial.print("IR2 ");
    adaAktif = true;
  }
  if (ir3) {
    Serial.print("IR3 ");
    adaAktif = true;
  }
  if (ir4) {
    Serial.print("IR4 ");
    adaAktif = true;
  }
  if (!adaAktif) {
    Serial.print("Tidak ada sensor yang aktif");
  }
  Serial.println();
  // Update data sensor
  slot[0] = !digitalRead(SENSOR1);
  slot[1] = !digitalRead(SENSOR2);
  slot[2] = !digitalRead(SENSOR3);
  slot[3] = !digitalRead(SENSOR4);

  server.handleClient();
  delay(300); // Jangan terlalu cepat polling sensor
}
