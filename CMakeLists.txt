cmake_minimum_required(VERSION 3.0.0)

# variables
set(PROJECT_NAME validus)

# policy settings, so CMake doesn't whine.
cmake_policy(SET CMP0048 NEW) # project VERSION
cmake_policy(SET CMP0065 NEW) # ENABLE_EXPORTS
cmake_policy(SET CMP0126 NEW) # cache
cmake_policy(SET CMP0056 NEW) # CMAKE_EXE_LINKER_FLAGS
cmake_policy(SET CMP0066 NEW) # CMAKE_<LANG>_FLAGS_<BUILDTYPE>
cmake_policy(SET CMP0102 NEW) # mark_as_advanced

# toolchain-related
set(C_FLAGS_DEBUG -g -O0 -DDEBUG)
set(C_FLAGS_RELEASE -O3 -D_FORTIFY_SOURCE=2 -DNDEBUG)

# define the project
project(
    ${PROJECT_NAME}
    VERSION 1.0.1
    LANGUAGES C
    DESCRIPTION "Validus: 192-bit OWHF"
)

execute_process(
    COMMAND git rev-parse --short --verify HEAD
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ECHO_OUTPUT_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
)

add_executable(
    ${PROJECT_NAME}
    validus.c
    validuslibrary.c
    main.c
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    .
    .
)

target_compile_features(
    ${PROJECT_NAME}
    PUBLIC
    c_std_11
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
    TYPE BIN
    CONFIGURATIONS Release
)
