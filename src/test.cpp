#include "json.hpp"
#include <iostream>
#include <fstream>
#include "engine.h"
#include <string>
#include <unistd.h>
#include <iterator>
#include <cstddef>

class MockDriver : public Driver
{
private:
    SensorState *state;

public:
    MockDriver()
    {
        // Set mock initial state
        this->state = new SensorState();
        this->state->has_water = true;
        this->state->motor_encoder = 0;
        this->state->piston_position = 0;
        this->state->piston_speed = 0;
        this->state->water_flow = 0;
        this->state->water_pressure = 0;
        this->state->water_temp = 0;
        this->state->weight = 0;
    }

    virtual SensorState *setWaterTemperature(std::string mode, double x, double y)
    {
        std::cout << "Driver::setWaterTemperature - start" << std::endl;
        double current_temp = this->state->water_temp;
        double target_temp = y;
        std::cout << "Driver::setWaterTemperature - current temperature: " << current_temp << std::endl;

        if (current_temp >= target_temp)
        {
            std::cout << "Driver::setWaterTemperature - water is already at target temp" << std::endl;
            return this->state;
        }
        else
        {
            std::cout << "Driver::setWaterTemperature - heating water to " << target_temp << std::endl;
            this->state->water_temp = target_temp;
            return this->state;
        }
    }
    virtual SensorState *setWaterFlow(double flowInMlPerSecond)
    {
        std::cout << "Driver::setWaterFlow - start" << std::endl;
        double current_flow = this->state->water_flow;
        std::cout << "Driver::setWaterFlow - current flow: " << current_flow << std::endl;

        if (current_flow >= flowInMlPerSecond)
        {
            std::cout << "Driver::setWaterFlow - water is already flowing at target flow" << std::endl;
            return this->state;
        }
        else
        {
            std::cout << "Driver::setWaterFlow - increasing flow to " << flowInMlPerSecond << std::endl;
            this->state->water_flow = flowInMlPerSecond;
            return this->state;
        }
    }
    virtual SensorState *setWaterPressure(double pressureInBars)
    {
        std::cout << "Driver::setWaterPressure - start" << std::endl;
        double current_pressure = this->state->water_pressure;
        std::cout << "Driver::setWaterPressure - current pressure: " << current_pressure << std::endl;
        if (current_pressure >= pressureInBars)
        {
            std::cout << "Driver::setWaterPressure - water is already at target pressure" << std::endl;
            return this->state;
        }
        else
        {
            std::cout << "Driver::setWaterPressure - increasing pressure to " << pressureInBars << std::endl;
            this->state->water_pressure = pressureInBars;
            return this->state;
        }
    }
    virtual SensorState *setWeight(double weightInGrams)
    {
        std::cout << "Driver::setWeight - start" << std::endl;
        double current_weight = this->state->weight;
        std::cout << "Driver::setWeight - current weight: " << current_weight << std::endl;
        if (current_weight >= weightInGrams)
        {
            std::cout << "Driver::setWeight - weight is already at target weight" << std::endl;
            return this->state;
        }
        else
        {
            std::cout << "Driver::setWeight - increasing weight to " << weightInGrams << std::endl;
            this->state->weight = weightInGrams;
            return this->state;
        }
    }
    virtual SensorState *movePiston(std::string mode, double value, double speed)
    {
        std::cout << "Driver::movePiston - start" << std::endl;
        double current_position = this->state->piston_position;

        if (current_position < value)
        {
            std::cout << "Driver::movePiston - moving piston forward at " << speed << "mm/s." << std::endl;
            this->state->piston_position = value;
            double waitFor = (value - current_position) / speed;
            std::cout << "Driver::movePiston - moving piston will take " << waitFor << "seconds" << std::endl;
            sleep(waitFor);
            return this->state;
        }
        else
        {
            std::cout << "Driver::movePiston - moving piston backwards at " << speed << "mm/s." << std::endl;
            this->state->piston_position = value;
            double waitFor = (current_position - value) / speed;
            std::cout << "Driver::movePiston - moving piston will take " << waitFor << "seconds" << std::endl;
            sleep(waitFor);
            return this->state;
        }
    }
    virtual SensorState *tare(double value)
    {
        std::cout << "tare" << std::endl;
        throw std::logic_error("NYI");
    }
    virtual SensorState *positionReference(double position)
    {
        std::cout << "positionReference" << std::endl;
        throw std::logic_error("NYI");
    }
    virtual SensorState *log(std::string level, std::string message)
    {
        std::cout << "log" << std::endl;
        throw std::logic_error("NYI");
    }
    virtual SensorState *getSensorData()
    {
        std::cout << "getSensorData" << std::endl;
        return this->state;
    }
};

int main()
{
    std::ifstream f("example.json");
    nlohmann::json data = nlohmann::json::parse(f);
    MockDriver d;

    ProfileEngine *engine = new ProfileEngine(&data, &d);

    while (engine->current_node->at("id").get<std::string>() != "end")
    {
        engine->step();
    }
}