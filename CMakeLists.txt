cmake_minimum_required(VERSION 2.6.3)
project(ns3-labs)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

 FIND_PACKAGE(NS3 COMPONENTS ns3-core ns3-wifi ns3-internet ns3-point-to-point ns3-applications ns3-network REQUIRED)
 include_directories(${NS3_INCLUDE_DIRS})
 link_directories(${NS3_LIBRARY_DIRS})
 add_definitions(${NS3_DEFINITIONS})
 if(NOT NS3_FOUND)
	message(ERROR " NS3 not found")
 endif(NOT NS3_FOUND)

link_libraries(ns3-core ns3-wifi ns3-internet ns3-point-to-point ns3-applications ns3-network)
add_executable(main "myfirst.cc")
