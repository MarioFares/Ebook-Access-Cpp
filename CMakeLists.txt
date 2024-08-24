cmake_minimum_required(VERSION 3.20)

# Project name & version
project(EbookAccess VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

# Set Language Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================= INCLUSION OF Qt =======================#
set(QT_VERSION_MAJOR 6)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# Windows exclusive Qt path
set(CMAKE_PREFIX_PATH C:/Qt-6.2/6.3.0/msvc2019_64/lib)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Sql REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6PrintSupport REQUIRED)
find_package(Qt6Pdf REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(ICONS_DIR "${CMAKE_SOURCE_DIR}/icons")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

include_directories(${SOURCE_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${ICONS_DIR})
include_directories(${RESOURCES_DIR})


file(GLOB_RECURSE SOURCES
        "${INCLUDE_DIR}/*.h"
        "${SOURCE_DIR}/*.cpp"
        "${ICONS_DIR}/*.*"
        "${RESOURCES_DIR}/*.qrc")

#=================== SETUP EXECUTABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
        $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Set executable icon
set(APP_ICON_RESOURCE_WINDOWS "${RESOURCES_DIR}/icons/icon.ico")

# Add the executable
add_executable(EbookAccess WIN32 ${SOURCES} ${APP_ICON_RESOURCE_WINDOWS})

# Add the target includes for MY_PROJECT
target_include_directories(EbookAccess PRIVATE ${INCLUDE_DIR})
target_include_directories(EbookAccess PRIVATE ${SOURCE_DIR})
target_include_directories(EbookAccess PRIVATE ${ICONS_DIR})
target_include_directories(EbookAccess PRIVATE ${RESOURCES_DIR})
#===================== LINKING LIBRARIES =======================#
target_link_libraries(EbookAccess Qt6::Core Qt6::Widgets Qt6::Sql Qt6::Gui Qt6::PrintSupport Qt6::Pdf)

# Visual studio compiler flag
set(CMAKE_CXX_FLAGS_RELEASE_INIT "/O2")
