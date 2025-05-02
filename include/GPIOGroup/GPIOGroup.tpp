#include "GPIOGroup.hpp"

template<size_t size>
GPIOGroup<size>::~GPIOGroup() noexcept
{
  this->unregister();
}

template <size_t size>
bool GPIOGroup<size>::gpio_used(const gpio_num_t &gpio_num) noexcept
{
  const auto it = allocated_gpios.find(gpio_num);
  return it != allocated_gpios.end() && std::get<0>(it->second);
}

template <size_t size>
bool GPIOGroup<size>::can_be_replaced(const gpio_num_t &gpio_num, void *curr) noexcept
{
  const auto it = allocated_gpios.find(gpio_num);
  if (it == allocated_gpios.end()) return true;

  bool in_use = std::get<0>(it->second);
  bool is_strict = std::get<1>(it->second);
  void* assigner = std::get<2>(it->second);

  return (!in_use && !is_strict) || assigner == curr;
}

template <size_t size>
esp_err_t GPIOGroup<size>::init() noexcept
{
  gpio_config_t io_conf = {};

  io_conf.pin_bit_mask = [&]() { 
    uint64_t bit_mask = 0;
    for (int i = 0; i < size; ++i) {
      if (can_be_replaced(gpio_nums[i], (void*)this)) {
        bit_mask |= (1ULL << gpio_nums[i]);
        allocated_gpios[gpio_nums[i]] = tuple(true, strict, (void*)this);
      }
    }
    return bit_mask;
  }();

  io_conf.mode = this->gpio_mode;

  io_conf.pull_up_en = this->pullup;
  io_conf.pull_down_en = this->pulldown;

  io_conf.intr_type = this->intr_type;

  esp_err_t err = gpio_config(&io_conf);

  return err;
}


template <size_t size>
esp_err_t GPIOGroup<size>::unregister() noexcept
{
  for (int i = 0; i < size; ++i) {
    if (can_be_replaced(gpio_nums[i], (void*)this)) {
      gpio_reset_pin(gpio_nums[i]);
      allocated_gpios[gpio_nums[i]] = tuple(false, strict, (void*)nullptr);
    }
  }

  return ESP_OK;
}
