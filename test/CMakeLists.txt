add_subdirectory("integration")

find_package(Catch2 3 REQUIRED)
add_executable(tests test_ringbuffer.cpp test_gain.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain nuked-sc55-backend nuked-sc55-common)
target_compile_features(tests PRIVATE cxx_std_23)

include(Catch)
catch_discover_tests(tests)
