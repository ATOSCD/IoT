#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin = 14;  // IR LED 연결 핀 (GPIO 4)
IRsend irsend(kIrLedPin);

void setup() {
  Serial.begin(115200);
  irsend.begin();
  Serial.println("시리얼 입력으로 명령 전송 준비됨");
  Serial.println("입력 예시: power / ch+ / ch- / vol+ / vol-");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "power") {
      sendRepeated(0x4FB4AB5);
      Serial.println("전원 버튼 전송 (5회 반복)");
    } else if (command == "ch+") {
      sendRepeated(0x4FB40BF);
      Serial.println("채널 업 전송 (5회 반복)");
    } else if (command == "ch-") {
      sendRepeated(0x4FB906F);
      Serial.println("채널 다운 전송 (5회 반복)");
    } else if (command == "vol+") {
      sendRepeated(0x4FBC03F);
      Serial.println("볼륨 업 전송 (5회 반복)");
    } else if (command == "vol-") {
      sendRepeated(0x4FB827D);
      Serial.println("볼륨 다운 전송 (5회 반복)");
    } else {
      Serial.println("알 수 없는 명령입니다.");
    }
  }
}


void sendRepeated(uint32_t data) {
  int count = 1;
  for (int i = 0; i < count; i++) {
    irsend.sendNEC(data);
    delay(20);  // 수신기 수용을 위해 약간의 딜레이 (30~50ms 권장)
  }
}

