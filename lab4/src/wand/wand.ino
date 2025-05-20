#include <Arduino.h>
#include <TECHIN515_lab4_inferencing.h> // Edge Impulse 模型头文件
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// 引脚定义
#define RED_LED    D10
#define GREEN_LED  D9
#define BLUE_LED   D8
#define BUTTON_PIN D3

// 传感器对象
Adafruit_MPU6050 mpu;

// 数据采样参数
#define SAMPLE_RATE_MS 10
#define CAPTURE_DURATION_MS 1000

// 特征数据数组
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// 状态变量
bool capturing = false;
bool buttonPressed = false;
unsigned long last_sample_time = 0;
unsigned long capture_start_time = 0;
int sample_count = 0;

// 给模型读取数据的接口
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
  memcpy(out_ptr, features + offset, length * sizeof(float));
  return 0;
}

// 控制 LED 灯亮灭
void lightLED(const char* label) {
  if (strcmp(label, "O") == 0) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  } else if (strcmp(label, "V") == 0) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  } else if (strcmp(label, "Z") == 0) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  } else {
    // 未识别/熄灭所有灯
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }
}

// 初始化
void setup() {
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 内部上拉，按钮接地有效

  // 初始化 MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Setup complete. Press button to recognize gesture.");
}

// 数据采集和推理函数
void capture_accelerometer_data() {
  if (millis() - last_sample_time >= SAMPLE_RATE_MS) {
    last_sample_time = millis();

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    if (sample_count < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE / 3) {
      int idx = sample_count * 3;
      features[idx] = a.acceleration.x;
      features[idx + 1] = a.acceleration.y;
      features[idx + 2] = a.acceleration.z;
      sample_count++;
    }

    if (millis() - capture_start_time >= CAPTURE_DURATION_MS) {
      capturing = false;

      ei_impulse_result_t result = { 0 };
      signal_t signal;
      signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
      signal.get_data = &raw_feature_get_data;

      EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
      if (res != EI_IMPULSE_OK) {
        Serial.println("Failed to run classifier.");
        return;
      }

      // 找到预测概率最高的类别
      float max_val = 0;
      int max_idx = -1;
      for (int i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_val) {
          max_val = result.classification[i].value;
          max_idx = i;
        }
      }

      if (max_idx >= 0) {
        const char* label = ei_classifier_inferencing_categories[max_idx];
        Serial.print("Detected gesture: ");
        Serial.print(label);
        Serial.print(" (");
        Serial.print(max_val * 100);
        Serial.println("%)");

        lightLED(label);  // 点亮对应灯光
        delay(1000);      // 保持1秒
        lightLED("off");  // 熄灭
      }
    }
  }
}

// 主循环
void loop() {
  // 检测按钮是否按下（下降沿）
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    capturing = true;
    sample_count = 0;
    capture_start_time = millis();
    last_sample_time = millis();
    Serial.println("Button pressed! Start capturing...");
  }

  // 按钮松开后，允许下一次触发
  if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }

  // 如果正在采集，就持续读取数据
  if (capturing) {
    capture_accelerometer_data();
  }
}