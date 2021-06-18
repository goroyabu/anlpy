# ----- PROJECT NAME ----- #
project(anlpy CXX)

# ----- CMake ----- #
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----- Pybind11 ----- #
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++1z")

# ----- Sub Project ----- #
include_directories(com_cli/source)
add_subdirectory(com_cli)

include_directories(bnk/source)
add_subdirectory(bnk)

include_directories(evs/source)
add_subdirectory(evs)

set(ANL_CXX_LIB_NAME anl)

# ----- Header Files ----- #
file(GLOB INCFILES anl/source/ANLStatus.hpp anl/source/VANL_Module.hpp anl/source/VANL_Parameters.hpp anl/source/ANLmanager.hpp anl/source/ANLmoduleStacker.hpp)
#file(GLOB INCFILES anl/source/ANLStatus.hpp anl/source/VANL_Module.hpp anl/source/VANL_Parameters.hpp anl/source/ANL.hpp anl/source/ANLmanager.hpp anl/source/ANLmoduleStacker.hpp)
install(FILES ${INCFILES} DESTINATION include/${ANL_CXX_LIB_NAME})

# ----- Build & Install ----- #
#file(GLOB SRCFILES anl/source/VANL_Module.cpp anl/source/ANLmanager.cpp anl/source/ANLmoduleStacker.cpp)
#file(GLOB SRCFILES anl/source/VANL_Module.cpp anl/source/ANL_0.cpp anl/source/ANL_1.cpp anl/source/ANL_2.cpp anl/source/ANLmanager.cpp anl/source/ANLmoduleStacker.cpp)
add_library(${ANL_CXX_LIB_NAME} SHARED anl/source/VANL_Module.cpp anl/source/ANLmanager.cpp anl/source/ANLmoduleStacker.cpp)
#set_target_properties(${ANL_CXX_LIB_NAME} PROPERTIES PUBLIC_HEADER anl/source/ANL.hpp)
target_link_libraries(${ANL_CXX_LIB_NAME} bnk evs com_cli)
#install(TARGETS ${ANL_CXX_LIB_NAME} EXPORT ${ANL_CXX_LIB_NAME}-config DESTINATION lib/${ANL_CXX_LIB_NAME}
install(TARGETS ${ANL_CXX_LIB_NAME} EXPORT ${ANL_CXX_LIB_NAME}-config DESTINATION lib/${ANL_CXX_LIB_NAME}
  INCLUDES DESTINATION include/${ANL_CXX_LIB_NAME} PUBLIC_HEADER DESTINATION include/${ANL_CXX_LIB_NAME})
install(EXPORT ${ANL_CXX_LIB_NAME}-config DESTINATION lib/${ANL_CXX_LIB_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# ----- Build Python Package ----- #
file(GLOB SRCFILES anl/source/pyfunc_anl.cpp)
pybind11_add_module(${PROJECT_NAME} ${SRCFILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ANL_CXX_LIB_NAME})
# bnk evs com_cli)
# install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME})
# install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)


# ----- Uninstall ----- #
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
