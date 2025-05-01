#pragma once

enum class SensorType {
  NPK,
  PH,
  MOISTURE,
  LIGHT,
  ELECTRIC_CONDUCTIVITY,
};

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