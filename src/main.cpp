#include <iostream>
#include "Sensor.h"
#include "telemetry.h"

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_log.h"
  #include "esp_http_client.h"
  #include "esp_http_server.h"
}

volatile Telemetry telemetry = Telemetry();

volatile bool do_read_sensors = false;
volatile unsigned int sensor_reading_interval = 1;
volatile bool do_capture_frames = false;
volatile unsigned int frames_capturing_interval = 1;

void read_sensors(void *params) {
  while (do_read_sensors) {
    ESP_LOGI("Main", "[Automated]: Reading Sensors...");

    vTaskDelay(pdMS_TO_TICKS(sensor_reading_interval * 1000));
  }
}

void capture_frames(void *params) {
  while (do_capture_frames) {
    ESP_LOGI("Main", "[Automated]: Capturing frames...");

    vTaskDelay(pdMS_TO_TICKS(frames_capturing_interval * 1000));
  }
}

extern "C" void app_main() {
  ESP_LOGI("Main", "Program Starting...");

  xTaskCreate(read_sensors, "Read Sensor", 4096, nullptr, 1, nullptr);
}