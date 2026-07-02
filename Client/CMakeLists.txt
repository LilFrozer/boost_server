cmake_minimum_required(VERSION 3.19)
project(boost_client LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(boost_client
    WIN32 MACOSX_BUNDLE
    src/mainclient.cpp
    src/mainclient.ui
    src/connection.cpp
    src/mainclient.cpp
    src/main.cpp
    src/qcustomplot.cpp
    inc/mainclient.h
    inc/connection.h
    inc/qcustomplot.h
    inc/GroupData.h
)

target_link_libraries(boost_client
    PRIVATE
        Qt::Core
        Qt::Widgets
        # Qt::PrintSupport
        # Qt::Multimedia
)

target_include_directories(boost_client
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${BOOST_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_compile_definitions(boost_client
    PRIVATE
    QT_NO_PRINTER
    BOOST_BEAST_USE_STD_ERIC=1
    BOOST_ASIO_USE_STD_ERIC=1
)

include(GNUInstallDirs)

install(TARGETS boost_client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET boost_client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
