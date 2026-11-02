#define SIMULATION 1   // 1: Wokwi (Serial Monitor) | 0: ESP32 thật (Bluetooth)

#if !SIMULATION
  #include "BluetoothSerial.h"
  BluetoothSerial SerialBT;
  #define LINK SerialBT
#else
  #define LINK Serial
#endif

const int LED1_PIN = 26;
const int LED2_PIN = 27;

bool led1State = false;
bool led2State = false;

void sendStatus() {
  LINK.printf("STATUS: LED1=%s | LED2=%s\n",
              led1State ? "ON" : "OFF",
              led2State ? "ON" : "OFF");
}

void handleCommand(String cmd) {
  if (cmd == "1ON")        { led1State = true;  digitalWrite(LED1_PIN, HIGH); }
  else if (cmd == "1OFF")  { led1State = false; digitalWrite(LED1_PIN, LOW);  }
  else if (cmd == "2ON")   { led2State = true;  digitalWrite(LED2_PIN, HIGH); }
  else if (cmd == "2OFF")  { led2State = false; digitalWrite(LED2_PIN, LOW);  }
  else if (cmd == "ALLON") {
    led1State = led2State = true;
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
  }
  else if (cmd == "ALLOFF") {
    led1State = led2State = false;
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  }
  else if (cmd == "STATUS") { /* chi gui trang thai */ }
  else {
    LINK.println("ERR: lenh khong hop le");
    return;
  }
  sendStatus();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

#if !SIMULATION
  SerialBT.begin("ESP32_LED");
#endif

  LINK.println("San sang. Lenh: 1ON 1OFF 2ON 2OFF ALLON ALLOFF STATUS");
}

void loop() {
  if (LINK.available()) {
    String cmd = LINK.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    if (cmd.length() > 0) handleCommand(cmd);
  }
}