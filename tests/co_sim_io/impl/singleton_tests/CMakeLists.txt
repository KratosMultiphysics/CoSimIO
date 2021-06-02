include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

add_executable ( main_singleton_partner main_partner.cpp )
install( TARGETS main_singleton_partner DESTINATION ${CMAKE_SOURCE_DIR}/bin/singleton_tests )
set_target_properties( main_singleton_partner PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory(scenario_1)
add_subdirectory(scenario_2)
add_subdirectory(scenario_3)
add_subdirectory(scenario_4)
