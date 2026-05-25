#include <SoftwareSerial.h>

#define FLAME_SENSOR_PIN 2
#define BUZZER_PIN 8
#define SIM800_TX 10  // Arduino RX
#define SIM800_RX 11  // Arduino TX

SoftwareSerial sim800l(SIM800_TX, SIM800_RX); // RX, TX

bool alertSent = false;

void setup() {
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  sim800l.begin(9600);
  delay(1000);
  
  Serial.println("Initializing SIM800L...");
  sendCommand("AT");      // Basic check
  sendCommand("AT+CMGF=1"); // Set SMS mode
}

void loop() {
  int flameDetected = digitalRead(FLAME_SENSOR_PIN);

  if (flameDetected == LOW && !alertSent) {
    Serial.println("🔥 Flame Detected! Sending SMS...");
    digitalWrite(BUZZER_PIN, HIGH);

    sendSMS("+91XXXXXXXXXX", "Alert! 🔥 Flame detected at your system! Take immediate action.");
    alertSent = true;
  }
  else if (flameDetected == HIGH) {
    Serial.println("No Flame.");
    digitalWrite(BUZZER_PIN, LOW);
    alertSent = false;
  }

  delay(1000);
}

void sendSMS(String number, String message) {
  sim800l.println("AT+CMGS=\"" + number + "\"");
  delay(500);
  sim800l.print(message);
  delay(500);
  sim800l.write(26); // Ctrl+Z to send
  delay(2000);
}

void sendCommand(String cmd) {
  sim800l.println(cmd);
  delay(500);
}
