#include <iostream>
#include "json/json.h"

/** \brief Write a Value object to a string.
 * Example Usage:
 * $g++ stringWrite.cpp -ljsoncpp -std=c++11 -o stringWrite
 * $./stringWrite
 * {
 *     "action" : "run",
 *     "data" :
 *     {
 *         "number" : 1
 *     }
 * }
 */
int main() {
    Json::Value    root;
    Json::Value    data;
    constexpr bool shouldUseOldWay = false;
    root["action"] = "run";
    data["number"] = 1;
    root["data"] = data;

    if (shouldUseOldWay) {
        Json::FastWriter  writer;
        const std::string json_file = writer.write(root);
        std::cout << json_file << std::endl;
    } else {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";  // for single line output
        const std::string         json_file = Json::writeString(builder, root);
        std::cout << json_file << std::endl;
    }
    return EXIT_SUCCESS;
}
