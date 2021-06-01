include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

add_library(singleton_tests_scenario_2_ext_lib SHARED ext_lib.cpp )
install( TARGETS singleton_tests_scenario_2_ext_lib DESTINATION ${CMAKE_SOURCE_DIR}/bin )

add_executable ( main_scenario_2 main.cpp )
target_link_libraries( main_scenario_2 singleton_tests_scenario_2_ext_lib )
install( TARGETS main_scenario_2 DESTINATION ${CMAKE_SOURCE_DIR}/bin/singleton_tests/scenario_1/ )
set_target_properties( main_scenario_2 PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(NAME singleton_tests_scenario_2 COMMAND sh ../../../../run.sh $<TARGET_FILE:main_scenario_2>  $<TARGET_FILE:main_singleton_partner>)
