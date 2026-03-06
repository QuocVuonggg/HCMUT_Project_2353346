/*
 * Code nâng cấp v5: Phân tích Gait Phase (Stance, Swing, Return)
 * Sensor: MPU-6050 (Cổ chân) + FSR (Gót/Lòng bàn chân)
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// --- ĐỊNH NGHĨA CHÂN ---
#define FSR_PIN A0
#define LED_GREEN  6  // Báo hiệu Stance
#define LED_YELLOW 7  // Báo hiệu Return
#define LED_RED    8  // Báo hiệu Swing (hoặc lỗi)
#define BUTTON_PIN 5
#define BT_RX_PIN 2
#define BT_TX_PIN 3
#define SD_CS_PIN 10

Adafruit_MPU6050 mpu;
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);
File dataFile; 

// --- QUẢN LÝ TRẠNG THÁI HỆ THỐNG ---
enum SystemState { OFF, RUNNING, PAUSED, SHUTTING_DOWN };
SystemState currentState = OFF;

// --- QUẢN LÝ GAIT PHASE (MỚI) ---
enum GaitPhase { PHASE_UNKNOWN, PHASE_STANCE, PHASE_SWING, PHASE_RETURN };
GaitPhase currentGaitPhase = PHASE_UNKNOWN;
String phaseString = "UNKNOWN";

unsigned long redLedOffTime = 0;
unsigned long lastSensorReadTime = 0;
const long sensorReadInterval = 50; // Giảm xuống 50ms để bắt pha nhạy hơn (20Hz)

// --- BIẾN NÚT NHẤN ---
int buttonState=HIGH;
int lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long pressTime = 0;
unsigned long longPressDuration = 1000;
bool isPressed = false;
bool longPressTriggered = false;

// --- BIẾN ĐẾM BƯỚC & LOGIC ---
long stepCount = 0;
bool footWasOnGround = false;
bool swingDetected = false; 

// !!! CẤU HÌNH NGƯỠNG (Cần tinh chỉnh thực tế) !!!
const int FSR_THRESHOLD = 200;          // Ngưỡng lực FSR
const float MPU_SWING_THRESHOLD = 13.0; // Ngưỡng gia tốc để coi là đang Vung (Swing)
const float GRAVITY = 9.81;             // Gia tốc trọng trường chuẩn

void setup(void) {
  Serial.begin(9600);
  btSerial.begin(9600); 
  
  if (!mpu.begin()) {
    Serial.println(F("Lỗi MPU-6050!"));
    while (1) { digitalWrite(LED_RED, HIGH); delay(100); digitalWrite(LED_RED, LOW); delay(100); }
  }

  // Cấu hình MPU
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SD_CS_PIN, OUTPUT); 

  // Khởi tạo SD
  if (SD.begin(SD_CS_PIN)) {
    if (SD.exists("gait_log.txt")) SD.remove("gait_log.txt"); // Xóa file cũ cho gọn
    File headerFile = SD.open("gait_log.txt", FILE_WRITE);
    if (headerFile) {
      headerFile.println(F("Timestamp\tStepCount\tPhase\tFSR\tAccelVM"));
      headerFile.close();
    }
  }

  Serial.println(F("He thong san sang."));
}

void loop(void) {
  if (currentState != SHUTTING_DOWN) handleButton();

  if (currentState == RUNNING) {
    unsigned long currentTime = millis();
    if (currentTime - lastSensorReadTime >= sensorReadInterval) {
      lastSensorReadTime = currentTime;
      readAndAnalyzeGait(); 
    }
  } 
  else if (currentState == SHUTTING_DOWN) {
    if (millis() >= redLedOffTime) {
      currentState = OFF;
      stepCount = 0; 
      swingDetected = false;
      updateSystemLed();
    }
  }
}

// --- HÀM XỬ LÝ CHÍNH: PHÂN TÍCH DÁNG ĐI ---
void readAndAnalyzeGait() {
  int fsrValue = analogRead(FSR_PIN);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  unsigned long currentTime = millis();

  // 1. Tính toán gia tốc tổng hợp (Vector Magnitude)
  float totalAccel = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));

  // 2. Xác định trạng thái chân chạm đất
  bool footIsOnGround = (fsrValue > FSR_THRESHOLD);

  // ==========================================================
  // PHẦN 1: PHÂN LOẠI GAIT PHASE (Stance / Swing / Return)
  // ==========================================================
  
  if (footIsOnGround) {
    // Nếu FSR có áp lực -> Chắc chắn là STANCE PHASE
    currentGaitPhase = PHASE_STANCE;
    phaseString = "STANCE";
    
    // LED xanh sáng khi chạm đất
    digitalWrite(LED_GREEN, HIGH); 
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    
  } else {
    // Nếu chân ở trên không (FSR = 0) -> Có thể là SWING hoặc RETURN
    
    if (totalAccel > MPU_SWING_THRESHOLD) {
      // Gia tốc lớn -> Đang vung chân mạnh -> SWING PHASE
      currentGaitPhase = PHASE_SWING;
      phaseString = "SWING";
      swingDetected = true; // Đánh dấu đã có vung chân để đếm bước
      
      // LED Đỏ sáng khi vung mạnh
      digitalWrite(LED_GREEN, LOW); 
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, LOW);

    } else {
      // Gia tốc nhỏ (gần bằng trọng trường hoặc thấp hơn ngưỡng Swing) -> RETURN CYCLE
      // Đây là giai đoạn chuyển tiếp: vừa nhấc chân lên hoặc đang thả trôi chân
      currentGaitPhase = PHASE_RETURN;
      phaseString = "RETURN";
      
      // LED Vàng sáng khi ở pha hồi/trôi
      digitalWrite(LED_GREEN, LOW); 
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, HIGH);
    }
  }

  // ==========================================================
  // PHẦN 2: ĐẾM BƯỚC (Logic cũ vẫn giữ để đếm tổng quát)
  // ==========================================================
  // Logic: Chuyển từ (Đã Swing) -> (Chạm đất Stance) = 1 Bước
  if (footIsOnGround && !footWasOnGround) {
    if (swingDetected) {
      stepCount++;
      swingDetected = false; // Reset cờ
      Serial.print(F(">>> BUOC CHAN THU: ")); Serial.println(stepCount);
    }
  }
  footWasOnGround = footIsOnGround;

  // ==========================================================
  // PHẦN 3: GHI VÀ GỬI DỮ LIỆU
  // ==========================================================
  
  // In ra Serial Monitor (Dùng Serial Plotter để xem đồ thị rất đẹp)
  // Format: FSR, VM, PhaseCode(để vẽ đồ thị), Step
  Serial.print("FSR:"); Serial.print(fsrValue);
  Serial.print(",VM:"); Serial.print(totalAccel);
  
  // Mã hóa Phase thành số để vẽ đồ thị: Stance=0, Return=10, Swing=20
  int phaseCode = 0;
  if(currentGaitPhase == PHASE_RETURN) phaseCode = 10;
  if(currentGaitPhase == PHASE_SWING) phaseCode = 20;
  
  Serial.print(",Phase:"); Serial.print(phaseCode); 
  Serial.print(",Step:"); Serial.println(stepCount);

  // Gửi qua Bluetooth
  btSerial.print("P:"); btSerial.print(phaseString);
  btSerial.print(",S:"); btSerial.println(stepCount);

  // Ghi SD
  dataFile = SD.open("gait_log.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(currentTime);
    dataFile.print("\t");
    dataFile.print(stepCount);
    dataFile.print("\t");
    dataFile.print(phaseString); // Ghi rõ tên pha
    dataFile.print("\t");
    dataFile.print(fsrValue);
    dataFile.print("\t");
    dataFile.println(totalAccel);
    dataFile.close();
  }
}

// --- CÁC HÀM PHỤ TRỢ (BUTTON, LED HỆ THỐNG) ---
// (Giữ nguyên logic nút nhấn như cũ nhưng rút gọn lại cho bài viết)
void handleButton() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { isPressed = true; pressTime = millis(); longPressTriggered = false; }
      else { isPressed = false; if (!longPressTriggered) handleShortPress(); }
    }
  }
  if (isPressed && !longPressTriggered && (millis() - pressTime > longPressDuration)) {
    longPressTriggered = true; handleLongPress();
  }
  lastButtonState = reading;
}

void handleShortPress() {
  if(currentState == OFF) currentState = RUNNING;
  else if(currentState == RUNNING) currentState = PAUSED;
  else if(currentState == PAUSED) currentState = RUNNING;
  updateSystemLed();
}

void handleLongPress() {
  if(currentState == RUNNING || currentState == PAUSED) { currentState = SHUTTING_DOWN; updateSystemLed(); }
}

void updateSystemLed() {
  // Chỉ dùng khi đổi trạng thái hệ thống, còn trong khi chạy thì LED được dùng để báo Phase
  if(currentState == OFF) { digitalWrite(LED_GREEN, LOW); digitalWrite(LED_YELLOW, LOW); digitalWrite(LED_RED, LOW); }
  else if(currentState == SHUTTING_DOWN) { digitalWrite(LED_RED, HIGH); redLedOffTime = millis() + 1000; }
}