cmake_minimum_required(VERSION 3.16)

project(TestStepsTool VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard and static flag for a static build
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

# Add CMAKE_PREFIX_PATH if Qt6 static installation is not in the default path
# Update "/path/to/Qt6/installation" with the actual path to your Qt6 static libraries
#set(CMAKE_PREFIX_PATH "/path/to/Qt6/installation")

# Find Qt6 Widgets and Network modules
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# List of source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Define executable
qt_add_executable(TestStepsTool
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Link Qt6 libraries (Widgets and Network in this case)
target_link_libraries(TestStepsTool PRIVATE Qt6::Widgets Qt6::Network)

# macOS-specific bundle settings
if(APPLE)
    set_target_properties(TestStepsTool PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TestStepsTool
    )
endif()

# Windows-specific settings
if(WIN32)
    set_target_properties(TestStepsTool PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Install settings
include(GNUInstallDirs)
install(TARGETS TestStepsTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable (Qt 6 only)
qt_finalize_executable(TestStepsTool)

