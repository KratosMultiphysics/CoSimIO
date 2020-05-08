// CoSimulation includes
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a   \
                  << " is not equal to " << b << std::endl; \
        return 1;                                                \
    }

int main(){
    CoSimIO::ConnectionSettings settings;
    settings.Set("connection_name", "test_connection"); // This should be unique for each connection between two solvers
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto return_info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(return_info.Get<bool>("connection_status"), true);

    return_info = CoSimIO::Disconnect(settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(return_info.Get<bool>("is_connected"), false);
    
    return 0;
}