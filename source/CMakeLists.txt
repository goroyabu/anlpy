# /**
#     @date 2020/03/08
# **/
project(anlpy CXX)

# CMake
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# Pybind11
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++11")

# Uninstallation
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)

# --------------------

add_subdirectory(core)
add_subdirectory(bnk)
add_subdirectory(evs)
add_subdirectory(com_cli)
file(GLOB SOURCE core/*.cpp bnk/*.cpp evs/*.cpp com_cli/*.cpp)

# --------------------

# Build
file(GLOB PYSOURCE anlpy/*cpp)
pybind11_add_module(${PROJECT_NAME} hello_py.cpp ${PYSOURCE})
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Config
  DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  )
install(
  EXPORT ${PROJECT_NAME}Config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/${PROJECT_NAME}
  EXPORT_LINK_INTERFACE_LIBRARIES
  )

add_library(anlcross SHARED main.cpp ${SOURCE})
target_link_libraries(anlcross -lreadline)
install(
  TARGETS anlcross
  EXPORT anlcrossConfig
  DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/anlcross
  )
install(
  EXPORT anlcrossConfig
  NAMESPACE anlcross::
  DESTINATION lib/anlcross
  EXPORT_LINK_INTERFACE_LIBRARIES
  )
add_library(anlcross::anlcross ALIAS anlcross)

# add_executable(hi main.cpp)
# install(
#   TARGETS hi
#   EXPORT hiConfig
#   DESTINATION bin
#   LIBRARY DESTINATION lib
#   INCLUDES DESTINATION include
#   PUBLIC_HEADER DESTINATION include/anlcross
#   )

target_link_libraries(${PROJECT_NAME} PRIVATE anlcross)
