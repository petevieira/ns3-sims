cmake_minimum_required(VERSION 2.6.3)
project(ns3-labs)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

FIND_PACKAGE(NS3 COMPONENTS
			   ns3-aodv                   ns3-applications
	           ns3-bridge			      ns3-config-store
			   ns3-core		              ns3-csma-layout
			   ns3-csma				      ns3-dsdv
			   ns3-emu				      ns3-energy
			   ns3-flow-monitor		      ns3-internet
      		   ns3-lte				      ns3-mesh
       	   	   ns3-mobility			      ns3-mpi
       	   	   ns3-netanim			      ns3-network
      		   ns3-nix-vector-routing     ns3-olsr
			   ns3-point-to-point-layout  ns3-point-to-point
       	   	   ns3-propagation			  ns3-spectrum
       	   	   ns3-stats				  ns3-tap-bridge
       	   	   ns3-test					  ns3-tools
     		   ns3-topology-read		  ns3-uan
			   ns3-virtual-net-device	  ns3-visualizer
       	   	   ns3-wimax                  ns3-wifi
              REQUIRED)
include_directories(${NS3_INCLUDE_DIRS})
link_directories(${NS3_LIBRARY_DIRS})
add_definitions(${NS3_DEFINITIONS})
if(NOT NS3_FOUND)
	message(ERROR " NS3 not found")
endif(NOT NS3_FOUND)

link_libraries(ns3-aodv                   ns3-applications
	           ns3-bridge			      ns3-config-store
			   ns3-core		              ns3-csma-layout
			   ns3-csma				      ns3-dsdv
			   ns3-emu				      ns3-energy
			   ns3-flow-monitor		      ns3-internet
       		   ns3-lte				      ns3-mesh
        	   ns3-mobility			      ns3-mpi
        	   ns3-netanim			      ns3-network
       		   ns3-nix-vector-routing     ns3-olsr
			   ns3-point-to-point-layout  ns3-point-to-point
        	   ns3-propagation			  ns3-spectrum
        	   ns3-stats				  ns3-tap-bridge
        	   ns3-test					  ns3-tools
      		   ns3-topology-read		  ns3-uan
 			   ns3-virtual-net-device	  ns3-visualizer
        	   ns3-wimax                  ns3-wifi
               )

file(GLOB simulations "src/*.cc" "src/*.cpp")
list(SORT simulations)

message(STATUS "\n -- SIMULATION: ")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
foreach(simulation_file ${simulations})
    get_filename_component(test_base ${simulation_file} NAME_WE)
    message(STATUS "Adding test ${test_base}")
    add_executable(${test_base} ${simulation_file})
#    target_link_libraries(${test_base} ${PROJECT_NAME})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(simulation_file)
