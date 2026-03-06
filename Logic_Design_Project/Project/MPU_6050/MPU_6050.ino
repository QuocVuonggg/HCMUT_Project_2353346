/*
 * Code nâng cấp v2: MPU-6050 + FSR + 3 LEDs + 1 Button
 * Logic: OFF -> RUNNING (Xanh) -> PAUSED (Vàng)
 * Nhấn giữ: Chuyển sang SHUTTING_DOWN (Đỏ 1s) -> rồi về OFF.
 *
 * * SƠ ĐỒ DÂY:
 * MPU-6050: A4(SDA), A5(SCL)
 * FSR: A0 (với mạch chia áp 10k)
 * LED Xanh: D6
 * LED Đỏ:   D7
 * LED Vàng: D8
 * Button: D5 (Nối D5 với GND, dùng INPUT_PULLUP)
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Định nghĩa chân cảm biến
#define FSR_PIN A0

// Định nghĩa chân linh kiện mới
#define LED_GREEN   6
#define LED_RED     8
#define LED_YELLOW  7
#define BUTTON_PIN  5

// Khởi tạo đối tượng MPU
Adafruit_MPU6050 mpu;

// 1. Biến cho Máy trạng thái (State Machine)
// *** THAY ĐỔI 1: Thêm trạng thái SHUTTING_DOWN ***
enum State { OFF, RUNNING, PAUSED, SHUTTING_DOWN };
State currentState = OFF; // Trạng thái ban đầu là TẮT

// Biến timer cho trạng thái tắt
unsigned long redLedOffTime = 0;

// 2. Biến cho việc đọc cảm biến (Non-blocking)
unsigned long lastSensorReadTime = 0;
const long sensorReadInterval = 100; // Đọc cảm biến mỗi 100ms

// 3. Biến cho xử lý Nút nhấn (Chống dội & Nhấn giữ)
int buttonState=HIGH;
int lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long pressTime = 0;
unsigned long longPressDuration = 1000; // 1 giây để coi là nhấn giữ
bool isPressed = false;
bool longPressTriggered = false;


void setup(void) {
  Serial.begin(9600);

  // Khởi tạo MPU-6050
  if (!mpu.begin()) {
    Serial.println("Không tìm thấy MPU-6050!");
    // Bật LED đỏ để báo lỗi nếu MPU hỏng
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);
    while (1) { delay(10); }
  }
  Serial.println("MPU-6050 đã sẵn sàng!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Cài đặt pin cho LED
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Cài đặt pin cho Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Đảm bảo tất cả LED đều tắt khi bắt đầu
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println("Hệ thống đã khởi động. Đang ở trạng thái TẮT.");
  Serial.println("Nhấn nút để BẬT.");
}

void loop(void) {
  // Luôn luôn kiểm tra nút nhấn, trừ khi đang trong quá trình tắt
  if (currentState != SHUTTING_DOWN) {
    handleButton();
  }

  // *** THAY ĐỔI 2: Thêm logic kiểm tra timer cho trạng thái SHUTTING_DOWN ***
  if (currentState == RUNNING) {
    // Kiểm tra xem đã đến lúc đọc cảm biến chưa
    unsigned long currentTime = millis();
    if (currentTime - lastSensorReadTime >= sensorReadInterval) {
      lastSensorReadTime = currentTime; // Đặt lại thời gian đọc
      readAndPrintSensorData();
    }
  } 
  else if (currentState == SHUTTING_DOWN) {
    // Khi đang ở trạng thái tắt, liên tục kiểm tra xem 1 giây đã trôi qua chưa
    if (millis() >= redLedOffTime) {
      // Đã hết 1 giây, chuyển hoàn toàn sang OFF
      currentState = OFF;
      updateLedState(); // Cập nhật đèn (tắt hết)
    }
  }
}

/**
 * Hàm này xử lý logic nút nhấn (chống dội, nhấn thả, nhấn giữ)
 */
void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) { // Nút vừa được NHẤN XUỐNG
        isPressed = true;
        pressTime = millis();
        longPressTriggered = false;
        
      } else { // Nút vừa được THẢ RA
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

/**
 * Xử lý khi có một cú NHẤN NGẮN (Click)
 */
void handleShortPress() {
  Serial.print("Sự kiện: Nhấn ngắn! ");
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
    // (Không làm gì nếu đang ở trạng thái SHUTTING_DOWN)
  }
  updateLedState(); 
}

/**
 * Xử lý khi có NHẤN GIỮ
 */
void handleLongPress() {
  Serial.print("Sự kiện: Nhấn giữ! ");
  if (currentState == RUNNING || currentState == PAUSED) {
    // *** THAY ĐỔI 3: Chuyển sang SHUTTING_DOWN thay vì OFF ***
    currentState = SHUTTING_DOWN;
    updateLedState(); // Cập nhật đèn LED
  }
}

/**
 * Cập nhật trạng thái đèn LED dựa trên biến currentState
 */
void updateLedState() {
  switch (currentState) {
    case OFF:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      Serial.println(" -> Chuyển sang TẮT.");
      break;
    case RUNNING:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      Serial.println(" -> Chuyển sang CHẠY (Xanh).");
      break;
    case PAUSED:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      Serial.println(" -> Chuyển sang TẠM DỪNG (Vàng).");
      break;
    
    // *** THAY ĐỔI 4: Thêm logic cho SHUTTING_DOWN ***
    case SHUTTING_DOWN:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, HIGH); // Bật LED đỏ
      // Đặt hẹn giờ 1 giây (1000ms) kể từ bây giờ
      redLedOffTime = millis() + 1000; 
      Serial.println(" -> Đang TẮT... (Đèn đỏ 1s)");
      break;
  }
}

/**
 * Hàm đọc và in dữ liệu cảm biến
 */
void readAndPrintSensorData() {
  int fsrValue = analogRead(FSR_PIN);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("FSR: ");
  Serial.print(fsrValue);
  Serial.print("\t | Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print("  Y: ");
  Serial.print(a.acceleration.y);
  Serial.print("  Z: ");
  Serial.print(a.acceleration.z);
  Serial.println();
}