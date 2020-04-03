# ----- PROJECT NAME & VERSION ----- #
project(bnk CXX)

# ----- CMake & Compiler ----- #
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----- Header Files ----- #
file(GLOB INCFILES source/bnk.hpp)
install(FILES ${INCFILES} DESTINATION include/${PROJECT_NAME})

# ----- Build & Install ----- #
file(GLOB SRCFILES source/bnk.cpp)
add_library(${PROJECT_NAME} SHARED ${SRCFILES})
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib
  INCLUDES DESTINATION include/${PROJECT_NAME} PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)
export(EXPORT ${PROJECT_NAME}-config FILE ${PROJECT_NAME}-export.cmake)
#EXPORT_LINK_INTERFACE_LIBRARIES)

# ----- Test Command ----- #
#add_executable(bnktest bnktest.c)
#target_link_libraries(bnktest PRIVATE ${PROJECT_NAME})

# ----- Uninstallation ----- #
#configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
#add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
