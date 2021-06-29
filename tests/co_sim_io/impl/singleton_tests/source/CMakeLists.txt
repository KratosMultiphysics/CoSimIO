
cmake_minimum_required( VERSION 3.12 )

project( test LANGUAGES CXX )

set( WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( library1 lib1/lib1source1.cpp lib1/lib1source2.cpp lib1/lib1header.hpp )
add_library( library2 lib2/lib2source1.cpp lib2/lib2source2.cpp lib2/lib2header.hpp )

target_include_directories( library1 PUBLIC . lib1 )
target_include_directories( library2 PUBLIC . lib2 )

add_executable( exe main.cpp )

target_link_libraries( exe library1 library2 )

add_test(singleton_philipp exe)
