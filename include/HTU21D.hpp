#pragma once

#include "Sensor.hpp"
#include "driver/i2c.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <cmath>

constexpr uint8_t HTU21D_ADDRESS              = 0x40;
constexpr uint8_t HTU21D_READ_ACCESS          = 0x40 << 1 | 0x0;
constexpr uint8_t HTU21D_WRITE_ACCESS         = 0x40 << 1;

constexpr uint8_t HTU21D_TRIGGER_TEMP_HOLD    = 0xE3;
constexpr uint8_t HTU21D_TRIGGER_HUM_HOLD     = 0xE5;
constexpr uint8_t HTU21D_TRIGGER_TEMP_NO_HOLD = 0xF3;
constexpr uint8_t HTU21D_TRIGGER_HUM_NO_HOLD  = 0xF5;
constexpr uint8_t HTU21D_WRITE_USER_REGISTER  = 0xE6;
constexpr uint8_t HTU21D_READ_USER_REGISTER   = 0xE7;
constexpr uint8_t HTU21D_SOFT_RESET           = 0xFE;

constexpr uint8_t HTU21D_CRC_INIT             = 0x00;

constexpr uint8_t HTU21D_MIN_RH_OUTPUT        = 0x00;
constexpr uint8_t HTU21D_MAX_RH_OUTPUT        = 0xFF;

constexpr float   A                           = 8.1332;
constexpr float   B                           = 1762.39;
constexpr float   C                           = 235.66;


class HTU21D : public Sensor {
  public:
    HTU21D(const string &name, const SENSOR_TYPE &type, const SafeVector<gpio_num_t> pin_in, const SafeVector<gpio_num_t> pin_out) : Sensor(name, type, pin_in, pin_out) { };
    SENSOR_STATUS initialize() noexcept;
    SENSOR_STATUS turn_off() noexcept;
    TELEMETRY_ASSIGNMENT_STATUS read(Telemetry &telemetry) noexcept;
  private:
    static float get_relative_humidity(const float& sig_hum) noexcept {
      return -6.0f + 125.0f * (sig_hum / 65536.0f);
    }

    static float get_relative_temperature(const float& sig_temp) noexcept {
      return -46.85f + 175.72f * (sig_temp / 65536.0f);
    }

    static float get_partial_pressure(const float& amb_temp) noexcept {
      return pow(10.0f, (A - (B / (amb_temp + C))));
    }

    static float get_dew_point_temperatue(const float& relative_humidity, const float& partial_pressure) {
      return -((B / (log10(relative_humidity * (partial_pressure / 100)) - A)) + C);
    }
};