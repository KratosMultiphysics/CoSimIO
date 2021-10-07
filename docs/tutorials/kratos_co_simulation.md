# Tutorial for performing coupled simulations with the Kratos CoSimulationApplication

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

This tutorial gives an introduction for performing coupled simulations by using the CoSimulationApplication of Kratos. It is strongly recommended to read the [README of the CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/README.md) first.

The _CoSimIO_ can be used in two different ways for conducting coupled simulations.

1. The first approach is the "classical" way of performing coupled simulations. This is usually done when using dedicated coupling tools. Here the code that is to be coupled implements the same coupling sequence as the coupling tool. The advantage is that the sequence of the coupling can be seen, at the cost of lack of flexibility.

    An example of this approach (for explicit/weak coupling) is shown in the following:
    ```py
    # solver initializes ...

    # solver connects to the CoSimulationApplication through the CoSimIO
    CoSimIO.Connect(...)

    CoSimIO.ExportMesh(...) # send meshes to the CoSimulationApplication

    # start solution loop
    while time < end_time:
        CoSimIO.ImportData(...) # get interface data from the CoSimulationApplication

        # solve the time step

        CoSimIO.ExportData(...) # send new data to the CoSimulationApplication

    # solver disconnects
    CoSimIO.Disconnect(...)
    ```

    The tutorial for this approach can be found [here](co_sim_classical_approach.md).

    A full example for Kratos using the classical approach can be found [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication#how-to-couple-a-new-solver--software-tool).

2. The second approach is called "remote controlled CoSimulation". Here the control of when to execute which part of the coupling sequence is given to the CoSimulationApplication. This has the big advantage that a change in the coupling sequence (e.g. explicit to implicit coupling) does not require to change the source code of the including code. This is achieved by registering functions in the _CoSimIO_ that perform the individual tasks in the solving procedure such as solving or data exchange. The disadvantage of this approach is that the including code must be flexible enough to implement those functions.

    An example of this approach is shown in the following:
    ```py
    # solver initializes ...
    solver = Solver(...)

    # solver connects to the CoSimulationApplication through the CoSimIO
    CoSimIO.Connect(...)

    # defining functions that get registered in CoSimIO
    # each of them has a CoSimIO.Info as input and output, which might be used
    def fct_solve_solution_step(info):
        solver.Solve()
        return CoSimIO.Info() # empty for now

    def fct_export_data(info):
        data_identifier = info.GetString("identifier")
        # using data_identifier to access database
        # how the database is accessed depends on the solver
        if data_identifier = "temperature":
            data = solver.GetTemperature()
        elif data_identifier = "heat_flux":
            data = solver.GetHeatFlux()
        else:
            raise Exception('unknown data requested')

        export_info = CoSimIO.Info()
        export_info.SetString("connection_name", connection_name) # connection_name comes from calling "CoSimIO.Connect"
        export_info.SetString("identifier", data_identifier)

        # now exporting the data
        CoSimIO.ExportData(export_info, data)

        return CoSimIO.Info() # empty for now

    def fct_import_mesh(info):
        mesh_identifier = info.GetString("identifier")

        import_info = CoSimIO.Info()
        import_info.SetString("connection_name", connection_name) # connection_name comes from calling "CoSimIO.Connect"
        import_info.SetString("identifier", mesh_identifier)

        model_part_import = CoSimIO.ModelPart("for_import")

        # importing the mesh
        CoSimIO.ImportMesh(import_info, model_part_import)

        # now use the mesh, custom to the solver
        # ...

        return CoSimIO.Info() # empty for now

    # solver registers functions in the CoSimIO
    CoSimIO.Register(fct_solve_solution_step)
    CoSimIO.Register(fct_export_data)
    CoSimIO.Register(fct_import_mesh)
    # register more functions if necessary, depending on the solver

    # the Run method performs the coupled simulation
    # it returns only at the end of the simulation
    CoSimIO.Run()

    # solver disconnects
    CoSimIO.Disconnect(...)
    ```

    The tutorial for this approach can be found [here](co_sim_remote_controlled.md).

    A full example for Kratos using the remote control approach can be found [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication#remote-controlled-cosimulation).
