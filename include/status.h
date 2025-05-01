#pragma once

enum class SensorStatus {
  NOT_FOUND,
  TIMEOUT,
  ERROR,
  OK,
};

enum class TelemetrySendStatus {
  NOT_FOUND,
  TIMEOUT,
  EMPTY,
  SUCCESS,
};

enum class ActuatorStatus {
  NOT_FOUND,
  TIMEOUT,
  ERROR,
  OK,
};