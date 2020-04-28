# ----- PROJECT NAME & VERSION ----- #
project(com_cli CXX)

# ----- CMake & Compiler ----- #
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# ----- Header Files ----- #
file(GLOB INCFILES source/com_cli.hpp)
install(FILES ${INCFILES} DESTINATION include/${PROJECT_NAME})

# ----- Build & Install ----- #
file(GLOB SRCFILES source/com_cli_1.cpp source/com_cli_2.cpp source/com_cli_3.cpp source/com_cli_private.cpp)
add_library(${PROJECT_NAME} SHARED ${SRCFILES})
target_link_libraries(${PROJECT_NAME} PRIVATE -lreadline)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config DESTINATION lib
  INCLUDES DESTINATION include/${PROJECT_NAME} PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/${PROJECT_NAME} EXPORT_LINK_INTERFACE_LIBRARIES)
export(EXPORT ${PROJECT_NAME}-config FILE ${PROJECT_NAME}-export.cmake)
# ----- Test Command ----- #
add_executable(clitest source/clitest.cpp)
target_link_libraries(clitest PRIVATE ${PROJECT_NAME} -lreadline)

# ----- Uninstallation ----- #
#configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
#add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)
