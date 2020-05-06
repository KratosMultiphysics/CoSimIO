// CoSimulation includes
#include "co_sim_io.hpp"

// move this macro to a checks.hpp in tests folder?
#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a   \
                  << " is not equal to " << b << std::endl; \
        return 1;                                                \
    }
    
int main(){
    auto info = CoSimIO::Hello();

    std::cout << info << std::endl;

    int major_version = info.Get<int>("major_version");
    int minor_version = info.Get<int>("minor_version");
    std::string patch_version = info.Get<std::string>("patch_version");

    COSIMIO_CHECK_EQUAL(major_version, 0);
    COSIMIO_CHECK_EQUAL(minor_version, 0);


    return 0;
}