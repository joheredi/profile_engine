#pragma once

#include "json.hpp"
#include "driver.h"
#include <string>

using nlohmann::json;

namespace Controller
{
    enum ControllerKind
    {
        SetWaterTemperature,
        SetWaterFlow,
        SetWaterPressure,
        SetWeight,
        Timer,
        MovePiston,
        Tare,
        PositionReference,
        Log
    };

    std::map<std::string, ControllerKind> kindMap{
        {"set_water_temperature", ControllerKind::SetWaterTemperature},
        {"set_water_flow", ControllerKind::SetWaterFlow},
        {"set_watter_pressure", ControllerKind::SetWaterPressure},
        {"set_weight", ControllerKind::SetWeight},
        {"timer", ControllerKind::Timer},
        {"move_piston", ControllerKind::MovePiston},
        {"tare", ControllerKind::Tare},
        {"position_reference", ControllerKind::PositionReference},
        {"log", ControllerKind::Log}};

    void executeController(json *controller, Driver *driver)
    {
        std::string str_kind = controller->at("kind").get<std::string>();
        ControllerKind kind = kindMap[str_kind];

        std::cout << "Controller::executeController - kind: " << str_kind << std::endl;
        json params = controller->at("parameters");

        std::cout << params.dump() << std::endl;

        if (kind == ControllerKind::SetWaterTemperature)
        {
            
            std::string mode = params.at("mode").get<std::string>();
            std::cout << "Controller::executeController - mode: " << mode << std::endl;
            double x = params.at("x").get<double>();
            double y = params.at("y").get<double>();
            std::cout << "Controller::executeController - mode: " << mode << " x: " << x << " y: " << y << std::endl;
            driver->setWaterTemperature(mode, x, y);
        }
        else if (kind == ControllerKind::Log)
        {
            std::string message = params.at("message").get<std::string>();
            std::string level = params.at("level").get<std::string>();
            driver->log(level, message);
        }
        else if (kind == ControllerKind::MovePiston)
        {
            std::string mode = params.at("mode").get<std::string>();
            double speed = params.at("speed").get<double>();
            double value = params.at("value").get<double>();
            driver->movePiston(mode, value, speed);
        }
        else if (kind == ControllerKind::PositionReference)
        {
            double position = params.at("value").get<double>();
            driver->positionReference(position);
        }
        else if (kind == ControllerKind::SetWaterFlow)
        {
            double flow = params.at("value").get<double>();
            driver->setWaterFlow(flow);
        }
        else if (kind == ControllerKind::SetWaterPressure)
        {
            double pressure = params.at("value").get<double>();
            driver->setWaterPressure(pressure);
        }
        else if (kind == ControllerKind::SetWeight)
        {
            double weight = params.at("value").get<double>();
            driver->setWeight(weight);
        }
        else if (kind == ControllerKind::Tare)
        {
            double value = params.at("value").get<double>();
            driver->tare(value);
        }
    }
}