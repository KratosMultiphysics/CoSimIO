import CoSimIO

def cosimio_check_equal(a, b):
    assert a is b


settings = CoSimIO.ConnectionSettings()
settings.SetString("connection_name", "test_connection") # This should be unique for each connection between two solvers
settings.SetString("solver_name", "my_solver") # Not to be confused with the connection name. 
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

return_info = CoSimIO.Connect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), 1)

return_info = CoSimIO.Disconnect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), 0)

