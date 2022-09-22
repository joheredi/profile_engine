#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

namespace Trigger
{
    enum TriggerKind
    {
        WaterTemperature,
        WaterFlow,
        WaterPressure,
        Weight,
        Wake,
        Timeout,
        PistonPosition,
        PistonSpeed,
        MotorEncoder,
        HasWater
    };

    enum TriggerOperator
    {
        LessThan,
        GreaterThan,
        LessOrEqual,
        GreaterOrEqual,
        Equal,
        Different
    };

    template <class T>
    static bool applyOperation(T a, T b, Trigger::TriggerOperator op)
    {
        switch (op)
        {
        case Trigger::TriggerOperator::Different:
            return a != b;
        case Trigger::TriggerOperator::Equal:
            return a == b;
        case Trigger::TriggerOperator::GreaterOrEqual:
            return a > b;
        case Trigger::TriggerOperator::GreaterThan:
            return a >= b;
        case Trigger::TriggerOperator::LessOrEqual:
            return a <= b;
        case Trigger::TriggerOperator::LessThan:
            return a < b;
        default:
            throw std::logic_error("Unknown operator");
        }
    }

    static std::map<std::string, TriggerOperator> operatorMap{
        {">", TriggerOperator::GreaterThan},
        {">=", TriggerOperator::GreaterOrEqual},
        {"<", TriggerOperator::LessThan},
        {"<=", TriggerOperator::LessOrEqual},
        {"==", TriggerOperator::Equal},
        {"!=", TriggerOperator::Different},
    };

    static std::map<std::string, TriggerKind> kindMap{
        {"has_water", TriggerKind::HasWater},
        {"motor_encoder", TriggerKind::MotorEncoder},
        {"piston_position", TriggerKind::PistonPosition},
        {"piston_speed", TriggerKind::PistonSpeed},
        {"timeout", TriggerKind::Timeout},
        {"wake", TriggerKind::Wake},
        {"water_flow", TriggerKind::WaterFlow},
        {"water_pressure", TriggerKind::WaterPressure},
        {"water_temperature", TriggerKind::WaterTemperature},
        {"weight", TriggerKind::Weight},
    };

    static bool evaluate(json *trigger, SensorState *state)
    {
        std::string kind = trigger->at("kind").get<std::string>();
        std::string op = trigger->at("operator").get<std::string>();
        json val = trigger->at("value");

        if (kindMap[kind] == TriggerKind::HasWater)
        {
            std::cout << "Trigger::evaluate - has_water" << std::endl;
            bool value = val.get<bool>();
            std::cout << value << op << state->has_water << std::endl;

            return applyOperation<bool>(value, state->has_water, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::MotorEncoder)
        {
            double value = val.get<double>();
            return applyOperation<double>(value, state->motor_encoder, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::PistonPosition)
        {
            std::cout << "Trigger::evaluate - piston_position" << std::endl;
            double value = val.get<double>();
            std::cout << value << op << state->piston_position << std::endl;

            return applyOperation<double>(value, state->piston_position, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::PistonSpeed)
        {
            double value = val.get<double>();
            return applyOperation<double>(value, state->piston_speed, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::Timeout)
        {
            double value = val.get<double>();
            // TODO: Implement timeout
            throw std::logic_error("NYI: Timeout trigger");
        }
        else if (kindMap[kind] == TriggerKind::Wake)
        {
            throw std::logic_error("NYI: Wake trigger");
        }
        else if (kindMap[kind] == TriggerKind::WaterFlow)
        {
            double value = val.get<double>();
            return applyOperation<double>(value, state->water_flow, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::WaterPressure)
        {
            double value = val.get<double>();
            return applyOperation<double>(value, state->water_pressure, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::WaterTemperature)
        {
            double value = val.get<double>();
            return applyOperation<double>(value, state->water_temp, operatorMap[op]);
        }
        else if (kindMap[kind] == TriggerKind::Weight)
        {

            double value = val.get<double>();
            return applyOperation<double>(value, state->weight, operatorMap[op]);
        }
        else
        {
            return false;
        }
    }
}
