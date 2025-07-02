#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Fen";
const char* password = "12345678";

WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Ganti alamat kalau perlu

String pesan = "Selamat Datang!";

// HTML UI tampilan modern
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>IOT SIMPLY</title>
  <style>
    body {
      background-color: #808080;
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      margin: 0;
    }
    h1 {
      color: black;
      font-size: 3rem;
    }
    #inputContainer {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 20px;
    }
    input {
      padding: 10px;
      font-size: 1.2rem;
      border-radius: 5px;
      border: none;
      margin-bottom: 15px;
      width: 250px;
    }
    .button {
      background-color: #FFD700;
      border-radius: 5px;
      padding: 15px 30px;
      color: black;
      font-weight: bold;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <h1>IOT SIMPLY</h1>
  <div id="inputContainer">
    <input type="text" id="inputTeks" placeholder="Tulis teks di sini" />
    <div class="button" onclick="kirim()">ENTER</div>
  </div>

  <script>
    function kirim() {
      const teks = document.getElementById("inputTeks").value;
      fetch("/tulis", {
        method: "POST",
        headers: { "Content-Type": "text/plain" },
        body: teks
      }).then(() => {
        alert("Teks terkirim ke ESP32!");
      });
    }
  </script>
</body>
</html>
)rawliteral";

// Handle halaman utama
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handle kiriman teks
void handleTulis() {
  if (server.hasArg("plain")) {
    pesan = server.arg("plain");
    Serial.println("Teks baru: " + pesan);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();

  server.on("/", handleRoot);
  server.on("/tulis", HTTP_POST, handleTulis);
  server.begin();
}

void loop() {
  server.handleClient();

  String teksPanjang = "                " + pesan; // 16 spasi di depan
  for (int i = 0; i <= teksPanjang.length() - 16; i++) {
    String tampil = teksPanjang.substring(i, i + 16);
    lcd.setCursor(0, 0);
    lcd.print(tampil);
    delay(300);
  }
}
