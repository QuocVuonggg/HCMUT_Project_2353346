/*
* Code nâng cấp v4.2: MPU-6050 + FSR + Chống dậm chân tại chỗ
* Logic: (MPU "Swing") AND (FSR "Impact") = 1 Bước
* THAY ĐỔI: TẮT hoàn toàn chức năng SD Card
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>
// #include <SD.h> // Ghi chú thư viện SD
#include <SoftwareSerial.h>

// Định nghĩa chân cảm biến
#define FSR_PIN A0

// Định nghĩa chân linh kiện
#define LED_GREEN  6
#define LED_RED   8
#define LED_YELLOW 7
#define BUTTON_PIN 5

#define BT_RX_PIN 2
#define BT_TX_PIN 3
// #define SD_CS_PIN 10 // Không cần thiết nếu không dùng SD


Adafruit_MPU6050 mpu;
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);
// File dataFile; // Không cần thiết


// 1. Biến cho Máy trạng thái
enum State { OFF, RUNNING, PAUSED, SHUTTING_DOWN };
State currentState = OFF;
unsigned long redLedOffTime = 0;

// 2. Biến cho việc đọc cảm biến
unsigned long lastSensorReadTime = 0;
const long sensorReadInterval = 100; 

// 3. Biến cho Nút nhấn
int buttonState=HIGH;
int lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long pressTime = 0;
unsigned long longPressDuration = 1000;
bool isPressed = false;
bool longPressTriggered = false;

// 4. Biến cho Bộ đếm bước logic kép
long stepCount = 0;
bool footWasOnGround = false; 
bool swingDetected = false;  

// !!! QUAN TRỌNG: Bạn cần điều chỉnh 2 ngưỡng này
const int FSR_THRESHOLD = 200;    
const float MPU_SWING_THRESHOLD = 13.0; 


void setup(void) {
 Serial.begin(9600);
 btSerial.begin(9600); 
 btSerial.println(F("Bluetooth da san sang!"));
 Serial.println(F("Bluetooth da san sang!"));

 // 1. Khởi tạo MPU-6050
 if (!mpu.begin()) {
  Serial.println(F("Không tìm thấy MPU-6050!"));
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH); 
  while (1) { delay(10); }
 }

 Serial.println(F("MPU-6050 đã sẵn sàng!"));
 mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
 mpu.setGyroRange(MPU6050_RANGE_500_DEG);
 mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

 // 2. Khởi tạo chân ngoại vi
 pinMode(LED_GREEN, OUTPUT);
 pinMode(LED_RED, OUTPUT);
 pinMode(LED_YELLOW, OUTPUT);
 pinMode(BUTTON_PIN, INPUT_PULLUP);

 digitalWrite(LED_GREEN, LOW);
 digitalWrite(LED_RED, LOW);
 digitalWrite(LED_YELLOW, LOW);

 // 3. Khởi tạo SD Card (ĐÃ GHI CHÚ)
 /*
 Serial.print(F("Dang khoi tao SD card..."));
 pinMode(SD_CS_PIN, OUTPUT); 
 digitalWrite(SD_CS_PIN, HIGH); 
 delay(100);

 if (!SD.begin(SD_CS_PIN)) {
  Serial.println(F("Khoi tao that bai!"));
  digitalWrite(LED_RED, HIGH); 
 } else {
  Serial.println(F("SD card san sang."));

    Serial.println(F("Kiem tra file cu..."));
    if (SD.exists("datalog.txt")) {
      Serial.println(F("Phat hien file cu. Dang xoa..."));
      if (SD.remove("datalog.txt")) {
        Serial.println(F("Xoa file cu thanh cong."));
      } else {
        Serial.println(F("Loi: Khong the xoa file cu!"));
      }
    } else {
      Serial.println(F("Khong co file cu."));
    }
    
    File headerFile = SD.open("datalog.txt", FILE_WRITE);
    if (headerFile) {
        headerFile.println(F("--- Bat dau phien ghi moi ---"));
        headerFile.println(F("Timestamp\tStepCount\tFSR\tAccelX\tAccelY\tAccelZ\tAccelVM"));
        headerFile.close();
        Serial.println(F("Da tao file datalog.txt thanh cong."));
    } else {
        Serial.println(F("Khong the mo file datalog.txt"));
        digitalWrite(LED_RED, HIGH); 
    }
 }
 */

 // 4. Kết thúc Setup
 Serial.println(F("Hệ thống đã khởi động. Đang ở trạng thái TẮT."));
}

void loop(void) {
 if (currentState != SHUTTING_DOWN) {
  handleButton();
 }

 if (currentState == RUNNING) {
  unsigned long currentTime = millis();
  if (currentTime - lastSensorReadTime >= sensorReadInterval) {
   lastSensorReadTime = currentTime;
   readAndPrintSensorData(); 
  }
 } 
 else if (currentState == SHUTTING_DOWN) {
  if (millis() >= redLedOffTime) {
   currentState = OFF;
   
   stepCount = 0; 
   swingDetected = false; 
   Serial.println(F("Bo dem buoc da reset."));
   
   updateLedState();
  }
 }
}

// *** CÁC HÀM XỬ LÝ NÚT NHẤN VÀ LED (GIỮ NGUYÊN) ***
void handleButton() {
 int reading = digitalRead(BUTTON_PIN);
 if (reading != lastButtonState) {
  lastDebounceTime = millis();
 }
 if ((millis() - lastDebounceTime) > debounceDelay) {
  if (reading != buttonState) {
   buttonState = reading;
   if (buttonState == LOW) {
    isPressed = true;
    pressTime = millis();
    longPressTriggered = false;
   } else {
    isPressed = false;
    if (longPressTriggered == false) {
     handleShortPress();
    }
   }
  }
 }
 if (isPressed && !longPressTriggered) {
  if ((millis() - pressTime) > longPressDuration) {
   longPressTriggered = true;
   handleLongPress();
  }
 }
 lastButtonState = reading; 
}

void handleShortPress() {
 Serial.print(F("Sự kiện: Nhấn ngắn! "));
 switch (currentState) {
  case OFF:
   currentState = RUNNING;
   break;
  case RUNNING:
   currentState = PAUSED;
   break;
  case PAUSED:
   currentState = RUNNING;
   break;
 }
 updateLedState(); 
}

void handleLongPress() {
 Serial.print(F("Sự kiện: Nhấn giữ! "));
 if (currentState == RUNNING || currentState == PAUSED) {
  currentState = SHUTTING_DOWN;
  updateLedState();
 }
}

void updateLedState() {
 switch (currentState) {
  case OFF:
   digitalWrite(LED_GREEN, LOW);
   digitalWrite(LED_YELLOW, LOW);
   digitalWrite(LED_RED, LOW); 
   Serial.println(F(" -> Chuyển sang TẮT."));
   break;
  case RUNNING:
   digitalWrite(LED_GREEN, HIGH);
   digitalWrite(LED_YELLOW, LOW);
   digitalWrite(LED_RED, LOW);
   Serial.println(F(" -> Chuyển sang CHẠY (Xanh)."));
   break;
  case PAUSED:
   digitalWrite(LED_GREEN, LOW);
   digitalWrite(LED_YELLOW, HIGH);
   digitalWrite(LED_RED, LOW);
   Serial.println(F(" -> Chuyển sang TẠM DỪNG (Vàng)."));
   break;
  case SHUTTING_DOWN:
   digitalWrite(LED_GREEN, LOW);
   digitalWrite(LED_YELLOW, LOW);
   digitalWrite(LED_RED, HIGH);
   redLedOffTime = millis() + 1000; 
   Serial.println(F(" -> Đang TẮT... "));
   break;
 }
}


// **********************************************************
// *** HÀM ĐỌC CẢM BIẾN VÀ ĐẾM BƯỚC ***
// **********************************************************
void readAndPrintSensorData() {
 int fsrValue = analogRead(FSR_PIN);
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 unsigned long currentTime = millis();

 // 1. Tính toán gia tốc tổng hợp (Vector Magnitude - VM)
 float totalAccel = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));

 // 2. Kiểm tra trạng thái chân (Đang trên không hay chạm đất)
 bool footIsOnGround = (fsrValue > FSR_THRESHOLD);

 // 3. ĐIỀU KIỆN 1 (MPU): Phát hiện "Swing"
 if (!footIsOnGround && totalAccel > MPU_SWING_THRESHOLD) {
  swingDetected = true; 
 }

 // 4. ĐIỀU KIỆN 2 (FSR) & ĐẾM BƯỚC: Phát hiện "Impact"
 if (footIsOnGround && !footWasOnGround) {
   if (swingDetected) {
     stepCount++; 
     swingDetected = false; 
     
     Serial.print(F("--- BUOC DI (MPU+FSR): "));
     Serial.println(stepCount);
     btSerial.print(F("STEP:"));
     btSerial.println(stepCount);
   } else {
     Serial.println(F("--- Nham chan tai cho (Bo qua) ---"));
   }
 }

 // 5. Cập nhật trạng thái FSR cho lần đọc tiếp theo
 footWasOnGround = footIsOnGround;
 // --- KẾT THÚC LOGIC ĐẾM BƯỚC ---


 // 1. Gửi ra Serial Monitor (Debug)
 Serial.print(F("Time: "));
 Serial.print(currentTime);
 Serial.print(F(", Step: "));
 Serial.print(stepCount);
 Serial.print(F(", FSR: "));
 Serial.print(fsrValue);
 Serial.print(F(", VM: ")); 
 Serial.print(totalAccel);
 Serial.print(F(", Ax: "));
 Serial.println(a.acceleration.x); 
 
 // 2. Gửi qua Bluetooth (format Tab-Separated)
 btSerial.print(currentTime);
 btSerial.print(F("\t")); 
 btSerial.print(stepCount);
 btSerial.print(F("\t")); 
 btSerial.print(fsrValue);
 btSerial.print(F("\t")); 
 btSerial.print(a.acceleration.x);
 btSerial.print(F("\t")); 
 btSerial.print(a.acceleration.y);
 btSerial.print(F("\t")); 
 btSerial.print(a.acceleration.z); 
 btSerial.print(F("\t")); 
 btSerial.println(totalAccel); 

 // 3. Ghi vào SD Card (ĐÃ GHI CHÚ)
 /*
 dataFile = SD.open("datalog.txt", FILE_WRITE);
 if (dataFile) {
  dataFile.print(currentTime);
  dataFile.print(F("\t")); 
  dataFile.print(stepCount);
  dataFile.print(F("\t")); 
  dataFile.print(fsrValue);
  dataFile.print(F("\t")); 
  dataFile.print(a.acceleration.x);
  dataFile.print(F("\t")); 
  dataFile.print(a.acceleration.y);
  dataFile.print(F("\t")); 
  dataFile.print(a.acceleration.z);
  dataFile.print(F("\t")); 
  dataFile.println(totalAccel); 
  dataFile.close();
 } else {
  Serial.println(F("Loi ghi file SD!"));
  digitalWrite(LED_RED, HIGH);
  delay(50);
  digitalWrite(LED_RED, LOW);
 }
 */
}