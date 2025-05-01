#pragma once

#include <iostream>
#include <vector>
#include "../game.h"
#include <sstream>

/// <summary>
/// Used to write a string into a stream
/// </summary>
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
