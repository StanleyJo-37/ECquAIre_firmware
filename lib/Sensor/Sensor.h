#pragma once

#include "sensors.h"
#include "telemetry.h"
#include <string>
#include "SafeVector/SafeVector.h"

using std::string;

class Sensor {
  public:
    Sensor(const string &name, const SensorType &type, const SafeVector<unsigned short> pin_in, const SafeVector<unsigned short> pin_out) : name(name), type(type), pin_in(pin_in), pin_out(pin_out) {};

    virtual ~Sensor() = default;
    virtual SensorStatus initialize() = 0;
    virtual SensorStatus turn_off() = 0;
    virtual TelemetryAssignmentStatus read(Telemetry& telemetry) = 0;

    string get_name() const noexcept { return this->name; };
    SensorType get_type() const noexcept { return this->type; };
  protected:
    string name;
    SensorType type;
    SafeVector<unsigned short> pin_in;
    SafeVector<unsigned short> pin_out;
  private:
};