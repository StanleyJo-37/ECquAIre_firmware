#pragma once

#include "driver/gpio.h"
#include <unordered_map>
#include <cstddef>
#include <tuple>
#include <algorithm>
#include <array>
#include <string>
#include "SafeVector.hpp"

using std::unordered_map;
using std::tuple;
using std::copy;
using std::string;
using std::array;

using pin_status = tuple<bool, bool, void*>; // (in_use, strict, assigner)
constexpr unsigned short GPIO_COUNT = 39;
unordered_map<gpio_num_t, pin_status> allocated_gpios;

template<size_t size>
class GPIOGroup {
  public:
    GPIOGroup() noexcept = default;
    GPIOGroup(string name, SafeVector<gpio_num_t> gpio_nums, gpio_mode_t gpio_mode, gpio_pullup_t pullup, gpio_pulldown_t pulldown, gpio_int_type_t intr_type, bool strict) noexcept
      : name(name), gpio_nums(gpio_nums), gpio_mode(gpio_mode), pullup(pullup), pulldown(pulldown), intr_type(intr_type), strict(strict) {};

    ~GPIOGroup() noexcept;

    static bool gpio_used(const gpio_num_t &gpio_num) noexcept;
    static bool can_be_replaced(const gpio_num_t &gpio_num, void *curr) noexcept;

    esp_err_t init() noexcept;
    esp_err_t unregister() noexcept;

    string get_name() const noexcept { return this->name; };
    SafeVector<gpio_num_t> get_gpio_nums() const noexcept { return this->gpio_nums; };
    gpio_mode_t get_gpio_mode() const noexcept { return this->gpio_mode; };
    gpio_pullup_t get_pullup_mode() const noexcept { return this->pullup; };
    gpio_pulldown_t get_pulldown_mode() const noexcept { return this->pulldown; };
    gpio_int_type_t get_gpio_interrupt_type() const noexcept { return this->intr_type; };
    bool is_strict() const noexcept { return this->strict; };

  private:
    string name;
    SafeVector<gpio_num_t> gpio_nums;
    gpio_mode_t gpio_mode;
    gpio_pullup_t pullup;
    gpio_pulldown_t pulldown;
    gpio_int_type_t intr_type;
    bool strict;
};

#include "GPIOGroup.tpp"