#pragma once

#include "sensors.h"
#include "telemetry.h"
#include <string>
#include "SafeVector/SafeVector.h"

using std::string;

class Sensor {
  public:
    Sensor(const string &name, const SensorType &type, const SafeVector<unsigned short> pin_in, const SafeVector<unsigned short> pin_out) : name(name), type(type), pin_in(pin_in), pin_out(pin_out) {};

    virtual ~Sensor() noexcept = default;
    virtual SensorStatus initialize() noexcept = 0;
    virtual SensorStatus turn_off() noexcept = 0;
    virtual TelemetryAssignmentStatus read(Telemetry& telemetry) noexcept = 0;

    string get_name() const noexcept { return this->name; };
    SensorType get_type() const noexcept { return this->type; };
  protected:
    string name;
    SensorType type;
    SafeVector<unsigned short> pin_in;
    SafeVector<unsigned short> pin_out;
  private:
};