cmake_minimum_required(VERSION 3.10)
project(boost_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(BOOST_INCLUDE_DIR
        NAMES boost/beast.hpp
        PATHS /opt/homebrew/include
        /opt/homebrew/opt/boost/include
        /usr/local/include
        /usr/include
        REQUIRED
)

# Экспорт команд для Zed (чтобы работало автодополнение)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(boost_server
  src/main.cpp
  src/CssWork.cpp
        src/Server.cpp
        include/Server.h
        src/ClientSession.cpp
        include/ClientSession.h
        include/GroupData.h
)

target_include_directories(boost_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(
        boost_server PRIVATE
        ${BOOST_INCLUDE_DIR}
        src
)

target_link_libraries(boost_server
        ${Boost_LIBRARIES}
)

# Говорим Beast использовать std::error_code вместо boost::system::error_code
target_compile_definitions(boost_server PRIVATE
        BOOST_BEAST_USE_STD_ERIC=1
        BOOST_ASIO_USE_STD_ERIC=1
)
