include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

add_library(singleton_tests_scenario_4_ext_lib_1 SHARED ext_lib_1.cpp )
install( TARGETS singleton_tests_scenario_4_ext_lib_1 DESTINATION ${CMAKE_SOURCE_DIR}/bin )

add_library(singleton_tests_scenario_4_ext_lib_2 SHARED ext_lib_2.cpp )
install( TARGETS singleton_tests_scenario_4_ext_lib_2 DESTINATION ${CMAKE_SOURCE_DIR}/bin )

add_executable ( main_scenario_4 main.cpp )
target_link_libraries( main_scenario_4 singleton_tests_scenario_4_ext_lib_1 singleton_tests_scenario_4_ext_lib_2 )
install( TARGETS main_scenario_4 DESTINATION ${CMAKE_SOURCE_DIR}/bin/singleton_tests/scenario_4/ )
set_target_properties( main_scenario_4 PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(NAME singleton_tests_scenario_4 COMMAND sh ../../../../run.sh $<TARGET_FILE:main_scenario_4>  $<TARGET_FILE:main_singleton_partner>)
