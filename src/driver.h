#pragma once

#include <string>
#include <vector>
#include "sensorState.h"

class Driver
{
    public:
        virtual SensorState* setWaterTemperature(std::string mode, double x, double y) = 0;
        virtual SensorState* setWaterFlow(double flowInMlPerSecond) = 0;
        virtual SensorState* setWaterPressure(double pressureInBars)= 0;
        virtual SensorState* setWeight(double weightInGrams)= 0;
        virtual SensorState* movePiston(std::string mode, double value, double speed)= 0;
        virtual SensorState* tare(double value)= 0;
        virtual SensorState* positionReference(double position)= 0;
        virtual SensorState* log(std::string level, std::string message)= 0;
        virtual SensorState* getSensorData()= 0;
};