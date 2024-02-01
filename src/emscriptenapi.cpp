#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include <emscripten/bind.h>
#include "nlohmann/json.hpp"
#include "section.hpp"
#include "jsonconversions.hpp"
using json = nlohmann::ordered_json;
using namespace emscripten;

std::string binToJson(std::vector<uint8_t> binaryData)
{
    std::stringstream dataStream{};
    dataStream.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    Section section{Section::read(dataStream)};
    json j(section);
    return j.dump();
}

std::vector<uint8_t> jsonToBin(std::string jsonStr)
{
    json j{json::parse(jsonStr)};
    Section section{j.template get<Section>()};

    std::stringstream outStream{};
    section.write(outStream);

    std::streamoff dataSize{outStream.tellp()};
    std::vector<uint8_t> vec(dataSize);
    outStream.read(reinterpret_cast<char*>(vec.data()), dataSize);

    return vec;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("binToJson", &binToJson);
    function("jsonToBin", &jsonToBin);
    register_vector<uint8_t>("uint8vec");
}
