cmake_minimum_required(VERSION 3.16)
project(cpp2js)
set(CMAKE_CXX_STANDARD 20)
add_executable(cpp2js main.cpp)
find_package(Threads)
target_link_libraries(cpp2js Threads::Threads)

message(info ": Compiling with toolchain for ${CMAKE_SYSTEM_NAME}")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(WRAPPER_OPTIONS "-s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")
    set(EXPORTED_FUNCTIONS "-s EXPORTED_FUNCTIONS='[\"_get_42\",\"_hello_world_string\",\"_is_prime\"]'")
    set(EXTRA_OPTIONS "-s ASSERTIONS=1 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=8 -s INITIAL_MEMORY=39321600")

    set_target_properties(cpp2js PROPERTIES LINK_FLAGS "${WRAPPER_OPTIONS} ${EXPORTED_FUNCTIONS} ${EXTRA_OPTIONS}")
    set_target_properties(cpp2js PROPERTIES COMPILE_FLAGS "${WRAPPER_OPTIONS} ${EXPORTED_FUNCTIONS} ${EXTRA_OPTIONS}")
    target_compile_definitions(cpp2js PUBLIC IS_EMSCRIPTEN)
endif ()