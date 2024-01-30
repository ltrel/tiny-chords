#include <iostream>
#include <format>
#include <string_view>
#include <fstream>
#include <nlohmann/json.hpp>
#include "section.hpp"
#include "jsonconversions.hpp"
using json = nlohmann::ordered_json;

static constexpr std::string_view argumentErrStr{"Please specify a .json or .tc file\n"};

// int main(int argc, char *argv[])
int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << argumentErrStr;
        return 0;
    }

    std::string_view fileArg{argv[1]};
    // Conversion to binary format
    if (fileArg.ends_with(".json"))
    {
        std::ifstream inStream{argv[1]};
        json data(json::parse(inStream));
        Section section{data.template get<Section>()};
        inStream.close();

        std::string outName{fileArg.substr(0, fileArg.find_last_of('.'))};
        outName.append(".tc");
        std::ofstream outStream{outName};
        section.write(outStream);
        std::streamoff byteCount{outStream.tellp()};
        outStream.close();
        std::cout << std::format("Wrote {} bytes to file: {}\n", byteCount, outName);
        return 0;
    }

    // Conversion to JSON
    if (fileArg.ends_with(".tc"))
    {
        std::ifstream inStream{argv[1]};
        Section section{Section::read(inStream)};
        json j(section);
        std::cout << j.dump(2) << '\n';
        inStream.close();
        return 0;
    }

    // File extension matched neither case
    std::cout << argumentErrStr;
    return 0;
}
