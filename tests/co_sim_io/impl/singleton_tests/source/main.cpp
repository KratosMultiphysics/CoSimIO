#include <iostream>
#include "lib1header.hpp"
#include "lib2header.hpp"

#include "singleheader.hpp"

DEFINE_SINGLETON_MAIN( );

int main( )
{
    lib1function1( );
    lib2function1( );

    singleton.increment( );

    lib1function2( );
    lib2function2( );

    lib1function1( );
    lib2function1( );

    singleton.increment( );

    lib1function2( );
    lib2function2( );

    std::cout << "Final count: " << singleton.count( ) << std::endl;

    return singleton.count( ) != 10;
}