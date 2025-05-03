#pragma once

#include "sensors.hpp"
#include "telemetry.hpp"
#include "GPIOGroup/GPIOGroup.hpp"
#include "util/SafeVector/SafeVector.hpp"
#include <string>

using std::string;

class Sensor {
  public:
    Sensor(const string &name, const SENSOR_TYPE &type, const SafeVector<gpio_num_t> pin_in, const SafeVector<gpio_num_t> pin_out) : name(name), type(type), pin_in(pin_in), pin_out(pin_out) {};

    virtual ~Sensor() noexcept = default;
    virtual SENSOR_STATUS initialize() noexcept = 0;
    virtual SENSOR_STATUS turn_off() noexcept = 0;
    virtual TELEMETRY_ASSIGNMENT_STATUS read(Telemetry& telemetry) noexcept = 0;

    string get_name() const noexcept { return this->name; };
    SENSOR_TYPE get_type() const noexcept { return this->type; };
    SafeVector<gpio_num_t> get_pin_in() const noexcept { return this->pin_in; };
    SafeVector<gpio_num_t> get_pin_out() const noexcept { return this->pin_out; };
  protected:
    string name;
    SENSOR_TYPE type;
    SafeVector<gpio_num_t> pin_in;
    SafeVector<gpio_num_t> pin_out;
  private:
};