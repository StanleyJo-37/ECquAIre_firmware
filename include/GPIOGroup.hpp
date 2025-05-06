#pragma once

#include <unordered_map>
#include <cstddef>
#include <tuple>
#include <algorithm>
#include <array>
#include <string>
#include "util/SafeVector/SafeVector.hpp"

extern "C" {
  #include "driver/gpio.h"
  #include "esp_log.h"
};


using std::unordered_map;
using std::tuple;
using std::copy;
using std::string;
using std::array;

using pin_status = tuple<bool, bool, void*>; // (in_use, strict, assigner)

constexpr unsigned short  GPIO_COUNT      = 39;
constexpr bool            DEFAULT_STRICT  = true;

unordered_map<gpio_num_t, pin_status> allocated_gpios;

struct GPIO {
  gpio_num_t gpio_num;
  pin_status gpio_pin_status;

  GPIO(gpio_num_t gpio_num, pin_status gpio_pin_status)
    : gpio_num(gpio_num), gpio_pin_status(gpio_pin_status) {};
};

namespace GPIO_UTIL {

    bool gpio_used(const gpio_num_t &gpio_num) noexcept {
      const auto it = allocated_gpios.find(gpio_num);
      return it != allocated_gpios.end() && std::get<0>(it->second);
    };

    bool gpio_can_be_replaced(const gpio_num_t &gpio_num, void *curr) noexcept {
      const auto it = allocated_gpios.find(gpio_num);
      if (it == allocated_gpios.end()) return true;
    
      bool in_use = std::get<0>(it->second);
      bool is_strict = std::get<1>(it->second);
      void* assigner = std::get<2>(it->second);
    
      return (!in_use && !is_strict) || assigner == curr || !assigner;
    };

    esp_err_t gpio_init(
      SafeVector<gpio_num_t> gpio_nums,
      gpio_mode_t gpio_mode,
      gpio_pullup_t pullup,
      gpio_pulldown_t pulldown,
      gpio_int_type_t intr_type,
      GPIO *assigner = nullptr,
      bool strict = DEFAULT_STRICT
    ) noexcept {

      gpio_config_t io_conf = {};

      io_conf.pin_bit_mask = [&]() { 
        uint64_t bit_mask = 0;
        for (int i = 0; i < gpio_nums.get_size(); ++i) {
          if (gpio_can_be_replaced(gpio_nums[i], (void*)assigner)) {
            bit_mask |= (1ULL << gpio_nums[i]);
            allocated_gpios[gpio_nums[i]] = tuple(true, strict, (void*)assigner);
            ESP_LOGW("GPIO_UTIL", "Pin %d successfully allocated.", gpio_nums[i]);
          } else {
            ESP_LOGW("GPIO_UTIL", "Pin %d already allocated, skipping.", gpio_nums[i]);
          }
        }
        return bit_mask;
      }();

      if (io_conf.pin_bit_mask == 0) {
        return ESP_ERR_INVALID_ARG;
      }

      io_conf.mode = gpio_mode;

      io_conf.pull_up_en = pullup;
      io_conf.pull_down_en = pulldown;

      io_conf.intr_type = intr_type;

      esp_err_t err = gpio_config(&io_conf);

      return err;
    };

    esp_err_t gpio_unregister(SafeVector<gpio_num_t> gpio_nums, GPIO *requester = nullptr) noexcept
    {
      for (int i = 0; i < gpio_nums.get_size(); ++i) {
        if (gpio_can_be_replaced(gpio_nums[i], (void*)requester)) {
          gpio_reset_pin(gpio_nums[i]);
          allocated_gpios[gpio_nums[i]] = tuple(false, false, (void*)nullptr);
        }
      }
    
      return ESP_OK;
    };

};