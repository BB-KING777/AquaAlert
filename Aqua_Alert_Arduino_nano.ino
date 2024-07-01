#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSNピンの設定

const byte address[6] = "00001"; // 送受信で共通のアドレス

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address); // 送信パイプを開く
  radio.setPALevel(RF24_PA_MAX); // PAレベルを最大に設定（PA + LNAモジュール用）
  radio.stopListening(); // 送信モードに設定
}

void loop() {
  int sensorValue = analogRead(A0); // 水センサーの値を読み取る
  Serial.print("Water Sensor Value: ");
  Serial.println(sensorValue);
  radio.write(&sensorValue, sizeof(sensorValue)); // データを送信
  delay(1000);
}
