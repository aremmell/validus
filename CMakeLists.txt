cmake_minimum_required(
    VERSION 3.0.0...3.27.1
)

# variables
set(PROJECT_NAME validus)
set(EXECUTABLE_NAME validus)
set(STATIC_LIBRARY_NAME validus_static)
set(SHARED_LIBRARY_NAME validus_shared)
set(PROJECT_VERSION_TYPE "")

# create compile commands for static analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# select the C/C++ standards we can use based on the CMake version
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.21")
    set(CMAKE_C_STANDARD 17)
else()
    set(CMAKE_C_STANDARD 11)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12")
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# policy settings, so CMake doesn't whine.
cmake_policy(SET CMP0025 NEW) # AppleClang
cmake_policy(SET CMP0048 NEW) # project VERSION
cmake_policy(SET CMP0065 NEW) # ENABLE_EXPORTS
cmake_policy(SET CMP0056 NEW) # CMAKE_EXE_LINKER_FLAGS
cmake_policy(SET CMP0066 NEW) # CMAKE_<LANG>_FLAGS_<BUILDTYPE>

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0077 NEW) # option
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.17")
    cmake_policy(SET CMP0102 NEW) # mark_as_advanced
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.21")
    cmake_policy(SET CMP0126 NEW) # cache
endif()

# define the project
project(
    ${PROJECT_NAME}
    VERSION 1.0.4
    LANGUAGES C
    DESCRIPTION "Validus: 192-bit OWHF"
)

# toolchain-related
add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

if (MSVC)
    add_compile_options(
        /W4 /MP /GS
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/Ox>
    )
else()
    # -Wformat-nonliteral is also an option
    add_compile_options(
        -Wall -Wextra -Wpedantic -Werror -Wdouble-promotion -Wconversion -Wno-sign-conversion -Wno-string-conversion -Wshift-overflow
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Release>:-O3>
    )

    if (CMAKE_COMPILER_IS_GNUCC)
        add_compile_options(
            -Wformat-truncation
        )
    endif()

    add_compile_definitions(
        $<$<CONFIG:Release>:_FORTIFY_SOURCE=2>
    )
endif()

# get the git commit hash for use in the version header
execute_process(
    COMMAND git rev-parse --short --verify HEAD
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ECHO_OUTPUT_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
)

configure_file(
    version.h.in
    include/version.h
    NEWLINE_STYLE LF
)

add_executable(
    ${EXECUTABLE_NAME}
    validuscli.c
)

add_library(
    ${STATIC_LIBRARY_NAME}
    STATIC
    validus.c
    validusutil.c
)

add_library(
    ${SHARED_LIBRARY_NAME}
    SHARED
    validus.c
    validusutil.c
)

if(WIN32)
    add_library(libcmt.lib STATIC IMPORTED)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    set_target_properties(${STATIC_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    set_target_properties(${SHARED_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif()

target_link_libraries(
    ${EXECUTABLE_NAME}
    ${STATIC_LIBRARY_NAME}
)

target_include_directories(
    ${EXECUTABLE_NAME}
    PUBLIC
    .
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_features(
    ${EXECUTABLE_NAME}
    PUBLIC
    ${C_STANDARD}
)

target_compile_features(
    ${STATIC_LIBRARY_NAME}
    PUBLIC
    ${C_STANDARD}
)

target_compile_features(
    ${SHARED_LIBRARY_NAME}
    PUBLIC
    ${C_STANDARD}
)

install(
    TARGETS ${EXECUTABLE_NAME}
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    CONFIGURATIONS Release
)

install(
    TARGETS ${STATIC_LIBRARY_NAME} ${SHARED_LIBRARY_NAME}
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    CONFIGURATIONS Release
)

install(
    FILES validus.h validusutil.h
    DESTINATION include
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    CONFIGURATIONS Release
)
