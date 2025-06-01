cmake_minimum_required(VERSION 3.22)
project(Integracja)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system json beast asio)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(Integracja
    main.cpp
    includes.h
    helperFunctions.h
    helperFunctions.cpp
    requestHandlingAndRouting.h
    requestHandlingAndRouting.cpp
    server.h
    server.cpp
    ComparisonData.cpp
    ComparisonData.h
    indexAssigners.h
)

target_link_libraries(Integracja
    PRIVATE
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    unofficial::mysql-connector-cpp::connector
)