project(RiverCrossing)
cmake_minimum_required(VERSION 2.8)

find_package(Boost COMPONENTS thread REQUIRED)

SET(GCC_COVERAGE_LINK_FLAGS    "-ljsoncpp")

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS thread REQUIRED)


# get all sources
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/network SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/rivercrossing SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/rivercrossing/decision_network SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/rivercrossing/shunting_network SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/tools SUB_SOURCES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${SUB_SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    Boost::thread
  )

# link JSONCPP
target_link_libraries(${PROJECT_NAME} jsoncpp)

# for IDE purposes
FILE(GLOB_RECURSE NetworkInc "include/network/*.h")
FILE(GLOB_RECURSE RiverCrossingInc "include/rivercrossing/*.h")
FILE(GLOB_RECURSE ToolsInc "include/tools/*.h")
FILE(GLOB_RECURSE JSONInc "include/json/*.h")
FILE(GLOB_RECURSE DNInc "include/rivercrossing/decision_network/*.h")
FILE(GLOB_RECURSE SMInc "include/rivercrossing/shunting_network/*.h")

add_custom_target(Net_headers SOURCES ${NetworkInc})
add_custom_target(RC_headers SOURCES ${RiverCrossingInc})
add_custom_target(Tools_headers SOURCES ${ToolsInc})
add_custom_target(JSON_headers SOURCES ${JSONInc})
add_custom_target(DN_headers SOURCES ${DNInc})
add_custom_target(SM_headers SOURCES ${SMInc})
