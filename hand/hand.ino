#define START digitalWrite(12, HIGH)
#define STOP digitalWrite(11, HIGH)

const int startPin = 12;
const int stopPin = 11;
const int dataPin[9] = {2,3,4,5,6,7,8,9,10}; // 10 - clock
unsigned long startTime;
boolean isRunning = false;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 9; i++) {
    pinMode(dataPin[i], INPUT);
  }
  pinMode(startPin, OUTPUT);
  pinMode(stopPin, OUTPUT);
  
  // Initial state - both pins HIGH
  digitalWrite(startPin, HIGH);
  digitalWrite(stopPin, HIGH);

  Serial.println("Type 'r' to run and 's' to stop.");

  startTime = millis();
}

void startStop(bool start) {
  digitalWrite(start ? startPin : stopPin, LOW);
  delay(20);
  digitalWrite(start ? startPin : stopPin, HIGH);
}

uint8_t readData(int bit) {
  uint8_t sensorValue = digitalRead(dataPin[bit]);
  return !sensorValue;
}

uint8_t readByte() {
  uint8_t val = 0;
  for (int i = 0; i < 8; i++) {
    val |= (readData(i) & 0x01) << (7 - i);
  }
  return val;
}

int clockPin = 0;
int oldClockPin = 0;

void loop() { 
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'r') {
      startStop(true);
      isRunning = true;
      startTime = millis();
      Serial.println("Started!");
    } else if (command == 's') {
      startStop(false);
      isRunning = false;
      Serial.println("Stopped!");
    }
  }

  if (isRunning) {
    Serial.print("Data bytes: ");
    for (int i = 0; i < 8; i++) {
      uint8_t data = readData(i);
      Serial.print(data ? "1" : "0");
      Serial.print(" ");
    }
    Serial.println();
    // Read clock pin separately without printing
    clockPin = readData(8);
    if (clockPin != oldClockPin) {
      oldClockPin = clockPin;
    }
    delay(10);
  }
  
}