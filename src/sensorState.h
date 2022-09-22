#pragma once

struct SensorState
{
public:
    float piston_position;
    float water_temp;
    bool has_water;
    float water_flow;
    float water_pressure;
    float weight;
    float piston_speed;
    float motor_encoder;
};