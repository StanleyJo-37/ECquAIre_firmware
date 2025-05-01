#pragma once

#include <string>
#include <nlohmann/json.hpp>

using std::string;
using JSON = nlohmann::json;

struct Telemetry {
  string stringify() {
    JSON json;

    json["nitrogen"] = this->nitrogen;
    json["phosphorus"] = this->phosphorus;
    json["kalium"] = this->kalium;
    json["pH"] = this->pH;
    json["moisture"] = this->moisture;
    json["light"] = this->light;
    json["temperature"] = this->temperature;
    
    return json.dump();
  };

  float nitrogen;
  float phosphorus;
  float kalium;
  float pH;
  float moisture;
  float light;
  float temperature;
};