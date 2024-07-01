#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <RF24.h>

// WiFi設定
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Google Apps ScriptのURL
const String scriptUrl = "https://script.google.com/macros/s/your_script_id/exec";

// ピン設定
#define CE_PIN 4   // D2 (GPIO4)
#define CSN_PIN 15 // D8 (GPIO15)

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001"; // 送受信で共通のアドレス

int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  SPI.begin(); // ハードウェアSPIの初期化
  radio.begin();
  radio.openReadingPipe(1, address); // 受信パイプを開く
  radio.setPALevel(RF24_PA_MAX); // PAレベルを最大に設定（PA + LNAモジュール用）
  radio.startListening(); // 受信モードに設定
}

void sendToGoogleSheets(int sensorValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"value1\":\"" + String(sensorValue) + "\"}";
    int httpCode = http.POST(jsonPayload);

    if (httpCode > 0) {
      Serial.printf("HTTP POST request sent, response code: %d\n", httpCode);
    } else {
      Serial.printf("HTTP POST request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  if (radio.available()) {
    radio.read(&sensorValue, sizeof(sensorValue)); // データを受信
    Serial.print("Received Water Sensor Value: ");
    Serial.println(sensorValue);

    sendToGoogleSheets(sensorValue);
  }

  delay(10000); // 10秒ごとにチェック（適宜変更）
}
