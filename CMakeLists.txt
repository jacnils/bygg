# bygg CMakeLists.txt
cmake_minimum_required(VERSION 3.1...3.29)

project(bygg VERSION 0.1.0
    DESCRIPTION "Component-based HTML/CSS builder for C++17."
    HOMEPAGE_URL "https://github.com/jcbnilsson/bygg"
    LANGUAGES CXX
)

add_compile_definitions(BYGG_VERSION="${PROJECT_VERSION}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (APPLE)
    set(INSTALL_RPATH_USE_LINK_PATH ON)
endif()

if (NOT DEFINED BYGG_USE_LIBXML2)
    set(BYGG_USE_LIBXML2 ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PUBLIC_HEADERS
        include/bygg/CSS/CSS.hpp
        include/bygg/CSS/element.hpp
        include/bygg/CSS/formatting_enum.hpp
        include/bygg/CSS/property.hpp
        include/bygg/CSS/properties.hpp
        include/bygg/CSS/stylesheet.hpp
        include/bygg/CSS/color.hpp
        include/bygg/CSS/color_type_enum.hpp
        include/bygg/CSS/color_struct.hpp
        include/bygg/HTML/document.hpp
        include/bygg/HTML/element.hpp
        include/bygg/HTML/formatting_enum.hpp
        include/bygg/HTML/HTML.hpp
        include/bygg/HTML/properties.hpp
        include/bygg/HTML/property.hpp
        include/bygg/HTML/section.hpp
        include/bygg/HTML/tag.hpp
        include/bygg/HTML/type_enum.hpp
        include/bygg/bygg.hpp
        include/bygg/except.hpp
        include/bygg/types.hpp
        include/bygg/version.hpp
)

if (BYGG_USE_LIBXML2)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS}
            include/bygg/HTML/parser.hpp
    )

    add_compile_definitions(BYGG_USE_LIBXML2)
endif()

set(SOURCE_FILES
        ${PUBLIC_HEADERS}
        include/bygg/CSS/impl/color_conversions.hpp
        src/CSS/element.cpp
        src/CSS/property.cpp
        src/CSS/properties.cpp
        src/CSS/stylesheet.cpp
        src/CSS/color.cpp
        src/HTML/document.cpp
        src/HTML/element.cpp
        src/HTML/properties.cpp
        src/HTML/property.cpp
        src/HTML/section.cpp
        src/HTML/tag.cpp
        src/CSS/impl/color_conversions.cpp)

if (BYGG_USE_LIBXML2)
    set(SOURCE_FILES ${SOURCE_FILES}
            include/bygg/HTML/impl/libxml2.hpp
            src/HTML/parser.cpp
            src/HTML/impl/libxml2.cpp
    )
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
include_directories(include)

if (BYGG_USE_LIBXML2)
    find_package(LibXml2 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE LibXml2::LibXml2)
endif()

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${PUBLIC_HEADERS}
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        FILE_SET HEADERS
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE bygg::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(FILES "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

if (NOT WIN32 AND NOT APPLE)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(Catch2 3 REQUIRED)

    add_executable(${PROJECT_NAME}_test
        tests/test.cpp
    )

    target_include_directories(${PROJECT_NAME}_test PRIVATE
        "${PROJECT_SOURCE_DIR}"
    )

    target_link_libraries(${PROJECT_NAME}_test PRIVATE
        Catch2::Catch2WithMain
    )

    if (BYGG_USE_LIBXML2)
        target_link_libraries(${PROJECT_NAME}_test PRIVATE LibXml2::LibXml2)
    endif()

    add_custom_command(
         TARGET ${PROJECT_NAME}_test
         COMMENT "Run tests"
         POST_BUILD
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
         COMMAND ${PROJECT_NAME}_test
    )
endif()
