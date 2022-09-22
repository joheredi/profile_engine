#pragma once

#include "json.hpp"
#include <map>
#include "sensorState.h"
#include "driver.h"

using nlohmann::json;

class ProfileEngine
{
public:
    ProfileEngine(json *profile, Driver *driver);
    void setSensorData(SensorState state);
    SensorState *getSensorState();
    json *current_node;
    void step();

private:
    SensorState *state;
    Driver *driver;
    std::map<std::string, json> nodes;
    json getStartNode();
    std::map<std::string, json> getAllNodes(json *profile);
    void updateState(SensorState state);
    std::string evaluateTriggers();
    void executeControllers();
};