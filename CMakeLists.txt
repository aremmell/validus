cmake_minimum_required(
    VERSION 3.0.0...3.27.1
)

# variables
set(PROJECT_NAME validus)
set(EXECUTABLE_NAME validus)
set(STATIC_LIBRARY_NAME validus_static)
set(SHARED_LIBRARY_NAME validus_shared)
set(PROJECT_VERSION_TYPE "-dev")

# policy settings, so CMake doesn't whine.
cmake_policy(SET CMP0025 NEW) # AppleClang
cmake_policy(SET CMP0048 NEW) # project VERSION
cmake_policy(SET CMP0065 NEW) # ENABLE_EXPORTS
cmake_policy(SET CMP0126 NEW) # cache
cmake_policy(SET CMP0056 NEW) # CMAKE_EXE_LINKER_FLAGS
cmake_policy(SET CMP0066 NEW) # CMAKE_<LANG>_FLAGS_<BUILDTYPE>
cmake_policy(SET CMP0102 NEW) # mark_as_advanced

# toolchain-related
set(C_FLAGS_BASE  "-Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${C_FLAGS_BASE} -g3 -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${C_FLAGS_BASE} -O3 -D_FORTIFY_SOURCE=2 -DNDEBUG")

# define the project
project(
    ${PROJECT_NAME}
    VERSION 1.0.3
    LANGUAGES C
    DESCRIPTION "Validus: 192-bit OWHF"
)

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
    c_std_17
)

target_compile_features(
    ${STATIC_LIBRARY_NAME}
    PUBLIC
    c_std_17
)

target_compile_features(
    ${SHARED_LIBRARY_NAME}
    PUBLIC
    c_std_17
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
