#include "GPIOGroup.hpp"

GPIOGroup::~GPIOGroup() noexcept
{
  this->unregister();
}

bool GPIOGroup::gpio_used(const gpio_num_t &gpio_num) noexcept
{
  const auto it = allocated_gpios.find(gpio_num);
  return it != allocated_gpios.end() && std::get<0>(it->second);
}

bool GPIOGroup::can_be_replaced(const gpio_num_t &gpio_num, void *curr) noexcept
{
  const auto it = allocated_gpios.find(gpio_num);
  if (it == allocated_gpios.end()) return true;

  bool in_use = std::get<0>(it->second);
  bool is_strict = std::get<1>(it->second);
  void* assigner = std::get<2>(it->second);

  return (!in_use && !is_strict) || assigner == curr;
}

esp_err_t GPIOGroup::init() noexcept
{
  gpio_config_t io_conf = {};

  io_conf.pin_bit_mask = [&]() { 
    uint64_t bit_mask = 0;
    for (int i = 0; i < this->gpio_nums.get_size(); ++i) {
      if (can_be_replaced(this->gpio_nums[i], (void*)this)) {
        bit_mask |= (1ULL << this->gpio_nums[i]);
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


esp_err_t GPIOGroup::unregister() noexcept
{
  for (int i = 0; i < this->gpio_nums.get_size(); ++i) {
    if (can_be_replaced(this->gpio_nums[i], (void*)this)) {
      gpio_reset_pin(this->gpio_nums[i]);
      allocated_gpios[this->gpio_nums[i]] = tuple(false, strict, (void*)nullptr);
    }
  }

  return ESP_OK;
}
