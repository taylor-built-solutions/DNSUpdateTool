cmake_minimum_required(VERSION 4.0.2)

project(
  DNSUpdateTool
  VERSION 1.0
  LANGUAGES CXX)

find_package(cpr)
find_package(nlohmann_json)

add_executable(${PROJECT_NAME} DNSUpdateTool.cpp)
target_link_libraries(${PROJECT_NAME} cpr::cpr nlohmann_json::nlohmann_json)
