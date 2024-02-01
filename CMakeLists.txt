include(FetchContent)

cmake_minimum_required(VERSION 3.28)
project(tinychords)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories("include")
set(SOURCES
  "src/api.cpp"
  "src/chord.cpp"
  "src/enums.cpp"
  "src/jsonconversions.cpp"
  "src/main.cpp"
  "src/note.cpp"
  "src/section.cpp"
  "src/sectionheader.cpp"
)

FetchContent_Declare(nlohmannjson
    URL      https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmannjson)
link_libraries(nlohmann_json::nlohmann_json)

add_executable(tinychords ${SOURCES})

if(EMSCRIPTEN)
  set_target_properties(tinychords PROPERTIES LINK_FLAGS "-s ENVIRONMENT=web -s WASM=0 -s MODULARIZE -s 'EXPORT_NAME=TinyChords' --bind")
endif()
