# ----- PROJECT NAME & VERSION ----- #
project(anltest CXX)
set(USER_ANL_LIB_NAME ${PROJECT_NAME}c)

# ----- CMake & Compiler ----- #
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----- Pybind11 ----- #
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++1z")

# ----- Header Files ----- #
#file(GLOB INCFILES source/ANLStatus.hpp source/ANLModuleBase.hpp)
#install(FILES ${INCFILES} DESTINATION include/${PROJECT_NAME})
find_package(bnk)
find_package(evs)
find_package(com_cli)
find_package(anl)

# ----- Build & Install ----- #
file(GLOB SRCFILES source/TimeGetter.cpp source/LoggerText.cpp)
add_library(${USER_ANL_LIB_NAME} STATIC ${SRCFILES})
target_link_libraries(${USER_ANL_LIB_NAME} PRIVATE anl)
install(TARGETS ${USER_ANL_LIB_NAME} EXPORT ${USER_ANL_LIB_NAME}-config DESTINATION lib/${USER_ANL_LIB_NAME})
install(EXPORT ${USER_ANL_LIB_NAME}-config DESTINATION lib/${USER_ANL_LIB_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# ----- Build Python Package ----- #
file(GLOB SRCFILES source/pyfunc.cpp)
pybind11_add_module(${PROJECT_NAME} ${SRCFILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${USER_ANL_LIB_NAME} anl)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib)
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)

# ----- Test Command ----- #
#add_executable(bnktest bnktest.c)
#target_link_libraries(bnktest PRIVATE ${PROJECT_NAME})

# ----- Uninstallation ----- #
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
