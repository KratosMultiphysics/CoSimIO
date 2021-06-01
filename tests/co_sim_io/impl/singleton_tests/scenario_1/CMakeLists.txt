include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

add_library(singleton_tests_scenario_1_ext_lib SHARED ext_lib.cpp )
install( TARGETS singleton_tests_scenario_1_ext_lib DESTINATION ${CMAKE_SOURCE_DIR}/bin )

add_executable ( main main.cpp )
target_link_libraries( main singleton_tests_scenario_1_ext_lib )
install( TARGETS main DESTINATION ${CMAKE_SOURCE_DIR}/bin/singleton_tests/scenario_1/ )
set_target_properties( main PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(NAME singleton_tests_scenario_1 COMMAND sh ../../../../run.sh $<TARGET_FILE:main>  $<TARGET_FILE:main_singleton_partner>)
