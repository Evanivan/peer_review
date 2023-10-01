cmake_minimum_required(VERSION 3.24)
project(peer_review3)

set(CMAKE_CXX_STANDARD 17)

add_executable(peer_review3 LoadPerson.cpp CheckDateTimeValidity.cpp CheckDateTimeValidity.h ParseCitySubjson.cpp ParseCitySubjson.h)
