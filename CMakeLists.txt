#
# Project Definition
#
cmake_minimum_required(VERSION 3.8.2)
project(LedgerJRI)

#
# Find dependencies
#
find_package(UtilTime 0.1 REQUIRED)
find_package(ThreadComms 0.1 REQUIRED)
find_package(FixedJSON 0.1 REQUIRED)
find_package(Ledger 3 REQUIRED)
find_package(DevToolsLog REQUIRED)
find_package(OSDevTools REQUIRED)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem date_time)

#
# Exported Library (libLedgerJRI)
#
add_library(LedgerJRI STATIC
        include/Ledger.h
        include/LedgerBalanceQuery.h
        include/LedgerCommands.h
        include/LedgerCommodity.h
        include/LedgerJSON.h
        include/LedgerQuery.h
        include/LedgerQueryInterface.h
        include/LedgerSession.h
        include/LedgerSessionData.h
        include/LedgerValue.h
        src/Ledger.cpp
        src/LedgerAccount.cpp
        src/LedgerAmount.cpp
        src/LedgerBalanceQuery.cpp
        src/LedgerCommands.cpp
        src/LedgerCommodity.cpp
        src/LedgerJSON.cpp
        src/LedgerQuery.cpp
        src/LedgerSession.cpp
        src/LedgerValue.cpp
)


target_link_libraries(LedgerJRI
PUBLIC
    ThreadComms::ThreadComms
    Ledger::libLedger
    FixedJSON::FixedJSON
)
target_include_directories(LedgerJRI PUBLIC
    $<BUILD_INTERFACE:${LedgerJRI_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_property(TARGET LedgerJRI PROPERTY PUBLIC_HEADER
    ${LedgerJRI_SOURCE_DIR}/include/JournalEmitter.h
)
target_compile_features(LedgerJRI PRIVATE cxx_std_17)

#
# Test Configuration
#
find_package(GTest REQUIRED)


add_executable(balance test/balance.cpp test/TestUtils.h test/TestUtils.cpp test/BalancesJSON.h test/BalanceJSON.cpp)
target_link_libraries(balance LedgerJRI FixedJSON::FixedJSON UtilTime::Time GTest::GTest GTest::Main)
target_compile_features(balance PRIVATE cxx_std_17)

add_executable(session test/session.cpp test/TestUtils.h test/TestUtils.cpp)
target_link_libraries(session LedgerJRI FixedJSON::FixedJSON UtilTime::Time GTest::GTest GTest::Main)
target_compile_features(session PRIVATE cxx_std_17)

add_executable(allTests   test/TestUtils.h test/TestUtils.cpp test/balance.cpp test/session.cpp test/BalancesJSON.h test/BalanceJSON.cpp include/LedgerAmount.h include/LedgerAccount.h test/session.cpp)
target_link_libraries(allTests LedgerJRI FixedJSON::FixedJSON UtilTime::Time GTest::GTest GTest::Main)
target_compile_features(allTests PRIVATE cxx_std_17)


#
# NOTE: Valgrind must be configured *before* testing is imported
#
set(MEMORYCHECK_COMMAND_OPTIONS "--gen-suppressions=all --track-origins=yes --freelist-vol=2000000000 --error-exitcode=1 --track-fds=yes --num-callers=50 --fullpath-after= --trace-children=yes --leak-check=full" )
find_program(MEMORYCHECK_COMMAND valgrind )
include (CTest)

enable_testing()
add_test(session session)
add_test(balance balance)

#
# Installation instructions
#
install(TARGETS LedgerJRI EXPORT LedgerJRITargets
    ARCHIVE  DESTINATION lib
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install (EXPORT LedgerJRITargets
    FILE         LedgerJRITargets.cmake
    NAMESPACE    LedgerJRI::
    DESTINATION  lib/cmake/LedgerJRI
)

#
# Define our package version
#
include(CMakePackageConfigHelpers)
write_basic_package_version_file("LedgerJRIConfigVersion.cmake"
    VERSION       0.1
    COMPATIBILITY SameMajorVersion
)
install (
    FILES       ${LedgerJRI_BINARY_DIR}/LedgerJRIConfigVersion.cmake
    DESTINATION lib/cmake/LedgerJRI
)

#
# Finally export the Config which CMake will use
# when looking for matching libraries to link against.
#
install (
    FILES       LedgerJRIConfig.cmake
    DESTINATION lib/cmake/LedgerJRI
)

#
# Configure Coverage Build
#
SET(CMAKE_CXX_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE)
