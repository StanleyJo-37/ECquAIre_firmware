#include "status.h"
#include "telemetry.h"

class Sensor {
  public:
    virtual SensorStatus intialize() = 0;
    virtual Telemetry read() = 0;

  protected:
  private:
};