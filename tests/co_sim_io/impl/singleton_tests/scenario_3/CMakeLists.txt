include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

add_library(singleton_tests_scenario_3_ext_lib SHARED ext_lib.cpp )
install( TARGETS singleton_tests_scenario_3_ext_lib DESTINATION ${CMAKE_SOURCE_DIR}/bin )

add_executable ( main_scenario_3 main.cpp )
target_link_libraries( main_scenario_3 singleton_tests_scenario_3_ext_lib )
install( TARGETS main_scenario_3 DESTINATION ${CMAKE_SOURCE_DIR}/bin/singleton_tests/scenario_3/ )
set_target_properties( main_scenario_3 PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(NAME singleton_tests_scenario_3 COMMAND sh ../../../../run.sh $<TARGET_FILE:main_scenario_3>  $<TARGET_FILE:main_singleton_partner>)
