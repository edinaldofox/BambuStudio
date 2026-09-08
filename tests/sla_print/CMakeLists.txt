get_filename_component(_TEST_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${_TEST_NAME}_tests ${_TEST_NAME}_tests_main.cpp
    sla_print_tests.cpp
    resin_profile_tests.cpp
    sla_test_utils.hpp sla_test_utils.cpp
    sla_supptgen_tests.cpp
    sla_raycast_tests.cpp)
target_link_libraries(${_TEST_NAME}_tests
    test_common
    libslic3r
    libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
)
target_compile_definitions(${_TEST_NAME}_tests PRIVATE RESIN_PROFILE_DIR=R"\(${PROJECT_SOURCE_DIR}/resources/profiles\)")
set_property(TARGET ${_TEST_NAME}_tests PROPERTY FOLDER "tests")

# catch_discover_tests(${_TEST_NAME}_tests TEST_PREFIX "${_TEST_NAME}: ")
add_test(${_TEST_NAME}_tests ${_TEST_NAME}_tests ${CATCH_EXTRA_ARGS})
