#pragma once

#include <iostream>
#include <vector>
#include "../game.h"
#include <sstream>

void WriteString(const std::string& data, std::stringstream& stream);

std::string create_world_packet(const std::vector<std::unique_ptr<GameObject>>& objects) {
    // TODO: Implement this

    std::stringstream stream;

    // Get the number of objects in the world

    uint32_t objectCount = objects.size();

    std::string objectCountS = std::to_string(objectCount);

    WriteString(objectCountS, stream);

    // Loop through all the objects

    int i;
    for (i = 0; i < objectCount; i++)
    {
        // The integer type of the object (4 bytes)
        uint32_t objectType = objects[i].get()->type;

        std::string objectTypeS = std::to_string(objectType);

        WriteString(objectTypeS, stream);

        // The objects serialized data

        std::string objectData = "";

        std::ostringstream dataStream;
        objects[i].get()->Serialize(dataStream);

        objectData = dataStream.str();

        WriteString(objectData, stream);
    }

    return stream.str();
}

void WriteString(const std::string& data, std::stringstream& stream)
{
    //Writing the string to binary
    int i;
    for (i = 0; i < data.size(); i++)
    {
        const char* messageChar = &data[i];
        stream.write(messageChar, sizeof(char));
    }
}

void deserialize_packet(const std::string& data)
{
    // TODO: Implement this

    //position
    //stream_read(is, position);

    //rotation
    //stream_read(is, rotation);

     //collision_radius
    //stream_read(is, collision_radius);

    //type
    //stream_read(is, type);
}