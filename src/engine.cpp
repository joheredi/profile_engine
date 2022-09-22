#include <iostream>
#include "engine.h"
#include "controller.h"
#include "trigger.h"

using nlohmann::json;

#define stringify(name) #name

const std::string NODE_START_ID = "start";
const std::string NODE_END_ID = "end";
const std::string NODE_UNDEFINED = "";

ProfileEngine::ProfileEngine(json *profile, Driver *driver)
{
    this->driver = driver;
    this->state = driver->getSensorData();
    this->nodes = this->getAllNodes(profile);
    std::cout << "ProfileEngine::ProfileEngine" << std::endl;
    this->current_node = &this->nodes["start"];
}

void ProfileEngine::step()
{
    if (this->current_node->at("id") == NODE_END_ID)
    {
        std::cout << "Reached end of profile";
        return;
    }

    std::string next_node = this->evaluateTriggers();
    std::cout << "Next Node:" << next_node << std::endl;

    // A trigger has been met, move to the next state
    if (next_node != NODE_UNDEFINED)
    {
        this->current_node = &this->nodes[next_node];
        std::cout << "ProfileEngine::step - A trigger has been met, move to the next state:" << next_node << std::endl;
        return;
    }

    std::cout << "!!!Execute actions" << std::endl;
    // No trigger has been met, execute the current node's controllers
    this->executeControllers();
}

json ProfileEngine::getStartNode()
{
    return this->nodes[NODE_START_ID];
}

std::map<std::string, json> ProfileEngine::getAllNodes(json *profile)
{
    std::map<std::string, json> nodes;

    for (json stage : profile->at("stages"))
    {
        for (json node : stage["nodes"])
        {
            nodes.insert({{node["id"].get<std::string>(), node}});
        }
    }
    return nodes;
}

std::string ProfileEngine::evaluateTriggers()
{
    std::vector<json> triggers = this->current_node->at("triggers").get<std::vector<json>>();

    std::cout << "===========================" << std::endl;
    for (json item : triggers)
    {
        std::string type = item.at("kind");
        std::cout << "Trigger type:" << type << std::endl;
    }
    std::cout << "===========================" << std::endl;

    for (json item : triggers)
    {
        if (item["kind"] == "wake")
        {
            return item["target"];
        }

        std::cout << "ProfileEngine::evaluateTriggers - Evaluating trigger " << item.at("kind") << std::endl;

        SensorState *sensorState = this->driver->getSensorData();
        if (Trigger::evaluate(&item, sensorState))
        {
            return item["target"];
        }
    }

    return "";
}

SensorState *ProfileEngine::getSensorState()
{
    return this->driver->getSensorData();
}

void ProfileEngine::executeControllers()
{
    std::vector<json> controllers = this->current_node->at("controllers");
    std::cout << "ProfileEngine::executeControllers - Executing " << controllers.size() << " controllers" << std::endl;
    for (json controller : controllers)
    {
        std::cout << "ProfileEngine::executeControllers - Executing controller " << controller.at("kind") << std::endl;
        Controller::executeController(&controller, this->driver);
    }
}
