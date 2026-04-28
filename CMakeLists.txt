cmake_minimum_required(VERSION 3.28.3)
project(HttpLibTesting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_executable(HttpLibTesting main.cpp
        include/header/model/weather/weather.hpp
        include/header/model/weather/forecast.hpp
        include/header/model/weather/error.hpp
        include/header/constants/constants.hpp
        src/header/open_weather/open_weather.cpp
        include/header/open_weather/open_weather.hpp
        src/header/model/weather/weather.cpp
        src/header/model/weather/forecast.cpp
        include/lib/logger/manager/manager.hpp
        src/lib/logger/manager/manager.cpp
        include/lib/http/base_client/base_client.hpp
        src/lib/http/base_client/base_client.cpp
        include/lib/http/error/api_error.hpp
        src/lib/http/error/api_error.cpp
        include/lib/http/types/types.hpp
        include/lib/http/constants/constants.hpp
        include/lib/random/random.hpp
        src/lib/random/random.cpp
        include/header/date/dob.hpp
        src/header/date/dob.cpp
)

set(CPM_DOWNLOAD_VERSION 0.42.1)

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake" ${CPM_DOWNLOAD_LOCATION}
    )
endif ()

message("${CPMstatus}")

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME fmt
        VERSION 12.1.0
        URL "https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS "FMT_TEST OFF" "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME nlohmann_json
        VERSION 3.12.0
        URL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz"
        URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS "JSON_BuildTests OFF"
)

CPMAddPackage(
        NAME spdlog
        VERSION 1.17.0
        URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS
        "SPDLOG_FMT_EXTERNAL ON"   # Tells spdlog to find and use your 'fmt' package
        "SPDLOG_BUILD_TESTS OFF"   # Disables internal spdlog tests
        "SPDLOG_BUILD_EXAMPLE OFF" # Skips spdlog's example folder
        "SPDLOG_BUILD_BENCH OFF" 
        "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME httplib
        VERSION 0.38.0
        URL "https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.38.0.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS
        "HTTPLIB_USE_NON_BLOCKING_GETADDRINFO OFF"
        "HTTPLIB_TEST OFF"
        "HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF"
        "HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF"
        "HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF"
)

CPMAddPackage(
    NAME date
    VERSION 3.0.4
    URL "https://github.com/HowardHinnant/date/archive/refs/tags/v3.0.4.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    OPTIONS
    "ENABLE_DATE_TESTING OFF"
    "BUILD_TZ_LIB ON"
    "USE_SYSTEM_TZ_DB ON" 
)

find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
target_compile_definitions(HttpLibTesting PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_definitions(-DWINVER=0x0A00)
    if (MINGW)
        target_compile_options(HttpLibTesting PRIVATE -Wa,-mbig-obj)
    endif ()
endif ()

target_include_directories(HttpLibTesting PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(HttpLibTesting PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(HttpLibTesting PRIVATE fmt::fmt nlohmann_json::nlohmann_json spdlog::spdlog httplib::httplib)
target_link_libraries(HttpLibTesting PRIVATE date::date date::date-tz)

if (WIN32)
    # Required for Windows Networking, Timers, and OpenSSL Certificate handling
    target_link_libraries(HttpLibTesting PRIVATE ws2_32 winmm crypt32)

    #     MSVC specific: OpenSSL applink is often required for correct memory management
    if (MSVC)
        target_link_libraries(HttpLibTesting PRIVATE OpenSSL::applink)
    endif ()
elseif (APPLE)
    #     Required for macOS system certificate access
    target_link_libraries(HttpLibTesting PRIVATE "-framework CoreFoundation" "-framework Security")
    target_compile_definitions(HttpLibTesting PRIVATE CPPHTTPLIB_USE_CERTS_FROM_MACOSX_KEYCHAIN)
endif ()

# 7. Standard Threading
find_package(Threads REQUIRED)
target_link_libraries(HttpLibTesting PRIVATE Threads::Threads)

if (GCC OR CLANG)
    target_precompile_headers(HttpLibTesting PRIVATE
            <httplib.h>
            <nlohmann/json.hpp>
            <fmt/format.h>
            <fmt/std.h>
            <fmt/base.h>
            <fmt/color.h>
            <fmt/ranges.h>
            <iostream>
            <vector>
            <array>
            <string>
            <deque>
            <memory>
            <new>
            <stack>
            <queue>
            <utility>
    )
endif ()