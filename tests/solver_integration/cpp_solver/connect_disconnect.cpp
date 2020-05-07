// CoSimulation includes
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a   \
                  << " is not equal to " << b << std::endl; \
        return 1;                                                \
    }

int main(){
    // const std::string connection_name = "my_solver"; // this is different for every solver
    // CoSimIO::ConnectionSettings settings;
    // settings.Set("echo_level", 1);
    // settings.Set("solver_version", "1.25");

    // auto info = CoSimIO::Connect(connection_name, settings);
    // COSIMIO_CHECK_EQUAL(info.Get<bool>("is_connected"), true);

    // auto info = CoSimIO::Disconnect(connection_name); // disconnect afterwards
    // COSIMIO_CHECK_EQUAL(info.Get<bool>("is_connected"), false);

    
    return 0;
}