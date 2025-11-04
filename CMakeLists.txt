cmake_minimum_required(VERSION 3.16)

project(Buffer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(Buffer
  main.cpp
  res.qrc
  Frame.h
  SPSCRing.h
  Processor.h Processor.cpp
  Receiver.h Receiver.cpp
  BoostRing.h
  ILockFreeRing.h
MpmcRing.h
)
target_link_libraries(Buffer
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
)

include(GNUInstallDirs)
install(TARGETS Buffer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
