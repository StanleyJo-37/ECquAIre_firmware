#include <iostream>

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_log.h"
  #include "esp_http_client.h"
  #include "esp_http_server.h"
}

void read_sensors(void *params) {
  while (true) {
    ESP_LOGI("Main", "[Automated]: Reading Sensors...");

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

extern "C" void app_main() {
  ESP_LOGI("Main", "Program Starting...");

  xTaskCreate(read_sensors, "Read Sensor", 4096, nullptr, 1, nullptr);
}