#include <iostream>
#include <thread>
#include <tuple>

#include "Sensor.hpp"
#include "telemetry.hpp"

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_log.h"
  #include "esp_http_client.h"
  #include "esp_http_server.h"
}

using std::tuple;

volatile Telemetry telemetry = Telemetry();
SemaphoreHandle_t telemetry_semaphore;

SafeVector<Sensor*> sensors;

volatile bool do_read_sensors = false;
volatile unsigned int sensor_reading_interval = 1;
volatile bool do_capture_frames = false;
volatile unsigned int frames_capturing_interval = 1;

extern "C" void app_main() {
  ESP_LOGI("Main", "Program Starting...");
}