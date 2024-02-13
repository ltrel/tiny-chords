include(FetchContent)

cmake_minimum_required(VERSION 3.28)
project(tinychords)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories("include")
set(COMMON_SOURCES
  "src/chord.cpp"
  "src/enums.cpp"
  "src/jsonconversions.cpp"
  "src/note.cpp"
  "src/section.cpp"
  "src/sectionheader.cpp"
)

FetchContent_Declare(nlohmannjson
    URL      https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmannjson)
link_libraries(nlohmann_json::nlohmann_json)

if(TRUE)
  set(EMSCRIPTEN_SOURCES ${COMMON_SOURCES} "src/api.cpp")
  set(EMCC_SETTINGS
    "-lembind"
    "-sENVIRONMENT=web,node"
    "-sMODULARIZE"
    "-sEXPORT_NAME=TinyChords"
  )
  set(WASM_SETTINGS
    ${EMCC_SETTINGS}
    "-sSINGLE_FILE=1"
    "--embind-emit-tsd interface.d.ts"
  )
  set(NOWASM_SETTINGS
    ${EMCC_SETTINGS} 
    "-sWASM=0"
  )
  list(JOIN WASM_SETTINGS " " WASM_SETTINGS)
  list(JOIN NOWASM_SETTINGS " " NOWASM_SETTINGS)

  add_executable(tinychords ${EMSCRIPTEN_SOURCES})
  set_target_properties(tinychords PROPERTIES LINK_FLAGS ${WASM_SETTINGS})

  add_executable(tinychords-nowasm ${EMSCRIPTEN_SOURCES})
  set_target_properties(tinychords-nowasm PROPERTIES LINK_FLAGS ${NOWASM_SETTINGS})
else()
  add_executable(tinychords ${COMMON_SOURCES} "src/main.cpp")
endif()
