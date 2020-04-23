!     ______     _____ _           ________
!    / ____/___ / ___/(_)___ ___  /  _/ __ |
!   / /   / __ \\__ \/ / __ `__ \ / // / / /
!  / /___/ /_/ /__/ / / / / / / // // /_/ /
!  \____/\____/____/_/_/ /_/ /_/___/\____/
!  Kratos CoSimulationApplication
!
!  License:         BSD License, see license.txt
!
!  Main authors:    Philipp Bucher (https://github.com/philbucher)
!


MODULE solver_globals
    USE iso_c_binding
    IMPLICIT NONE

    INTEGER :: echo_level
    REAL :: delta_time = 0.1
    REAL :: end_time = 0.5

    INTEGER :: size_real_array = 0
    INTEGER :: num_elements = 0

    REAL(8), DIMENSION(:), pointer :: f_ptr_data_real
    INTEGER, DIMENSION(:), pointer :: f_ptr_data_elem_conn
    INTEGER, DIMENSION(:), pointer :: f_ptr_data_elem_types

    ! assigning null otherwise memory is corrupted and valgrind complains
    type (c_ptr) :: c_ptr_data_real = c_null_ptr
    type (c_ptr) :: c_ptr_data_elem_conn = c_null_ptr
    type (c_ptr) :: c_ptr_data_elem_types = c_null_ptr

END MODULE solver_globals

program simple_solver_fortran
    use co_sim_io
    use solver_globals
    implicit none

    CHARACTER(100) :: input_char
    INTEGER :: level_of_coupling

    ! Make sure the right number of command line arguments have been provided
    IF(COMMAND_ARGUMENT_COUNT().NE.2)THEN
      WRITE(*,*)'ERROR, TWO COMMAND-LINE ARGUMENTS REQUIRED, STOPPING'
      STOP
    ENDIF

    ! Read the command line arguments
    CALL GET_COMMAND_ARGUMENT(1,input_char)
    READ(input_char,*)level_of_coupling
    CALL GET_COMMAND_ARGUMENT(2,input_char)
    READ(input_char,*)echo_level

    ! Select level of coupling
    IF (level_of_coupling == 0) THEN
        call run_solution_loop()
    ELSE IF (level_of_coupling == 1) THEN
        call run_solution_loop_weak_coupling()
    ELSE IF (level_of_coupling == 2) THEN
        call run_solution_loop_strong_coupling()
    ELSE IF (level_of_coupling == 3) THEN
        call run_solution_co_simulation_orchestrated()
    ELSE
        WRITE(*,*)'ERROR, WRONG LEVEL OF COUPLING; CAN ONLY BE 0, 1, 2, 3, STOPPING'
        STOP
    END IF

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    CONTAINS

    subroutine solver_print(string_to_print, req_echo_level)
        character(*), INTENT(IN) :: string_to_print
        integer, INTENT(IN) :: req_echo_level
        IF (echo_level >= req_echo_level) THEN
            write(*,*) "Solver [F]: ", string_to_print
        End IF
    end subroutine solver_print

    !!! solving-functions !!!
    SUBROUTINE advance_in_time (time)
        !! TODO time is no longer INOUT!
        REAL, intent(inout):: time
        character(len=1024) :: print_string
        time = time + delta_time
        write(print_string,*) "AdvanceInTime; new time: ", time
        call solver_print(adjustl(trim(print_string)), 2)
    END SUBROUTINE advance_in_time

    SUBROUTINE initialize_solution_step ()
        call solver_print("InitializeSolutionStep", 2)
    END SUBROUTINE initialize_solution_step

    SUBROUTINE solve_solution_step ()
        call solver_print("SolveSolutionStep", 2)
    END SUBROUTINE solve_solution_step

    SUBROUTINE finalize_solution_step ()
        call solver_print("FinalizeSolutionStep", 2)
    END SUBROUTINE finalize_solution_step

    !!! data-exchange functions !!!
    SUBROUTINE import_data_from_co_sim (connection_name, identifier)
        character(*), INTENT(IN) :: connection_name
        character(*), INTENT(IN) :: identifier
        call solver_print("    Before Importing Data from CoSim", 3)

        call CoSimIO_ImportData(connection_name//c_null_char, identifier//c_null_char,&
            size_real_array, c_ptr_data_real)

        call c_f_pointer(c_ptr_data_real, f_ptr_data_real, [size_real_array])
        call solver_print("    After Importing Data from CoSim", 3)
    END SUBROUTINE import_data_from_co_sim

    SUBROUTINE export_data_to_co_sim (connection_name, identifier)
        character(*), INTENT(IN) :: connection_name
        character(*), INTENT(IN) :: identifier
        call solver_print("    Before Exporting Data to CoSim", 3)

        ! TODO actually send some values ...

        call CoSimIO_ExportData(connection_name//c_null_char, identifier//c_null_char,&
            size_real_array, c_ptr_data_real)

            call solver_print("    After Exporting Data to CoSim", 3)
    END SUBROUTINE export_data_to_co_sim

    SUBROUTINE import_mesh_from_co_sim (connection_name, identifier)
        character(*), INTENT(IN) :: connection_name
        character(*), INTENT(IN) :: identifier
        call solver_print("    Before Importing Mesh from CoSim", 3)

        call CoSimIO_ImportMesh(connection_name//c_null_char, identifier//c_null_char, &
            size_real_array, num_elements, c_ptr_data_real, c_ptr_data_elem_conn,&
            c_ptr_data_elem_types)

        call c_f_pointer(c_ptr_data_real, f_ptr_data_real, [size_real_array])
        call c_f_pointer(c_ptr_data_elem_conn, f_ptr_data_elem_conn, [num_elements])
        call c_f_pointer(c_ptr_data_elem_types, f_ptr_data_elem_types, [num_elements])

        call solver_print("    After Importing Mesh from CoSim", 3)
    END SUBROUTINE import_mesh_from_co_sim

    SUBROUTINE export_mesh_to_co_sim (connection_name, identifier)
        character(*), INTENT(IN) :: connection_name
        character(*), INTENT(IN) :: identifier
        INTEGER :: i

        !    -- Mesh --
        ! 0      2      3
        ! x------x------x
        !  \     |     /|\
        !   \  1 |  2 / | \
        !    \   |   /  |  \
        !     \  |  /   |   \
        !      \ | /  3 |  4 \
        !       \|/     |     \
        !        x------x-----x
        !        1      4     5
        !

        real :: nodal_coords(18) = (/ &
        0.0, 2.5, 1.0, & ! 0
        2.0, 0.0, 1.5, & ! 1
        2.0, 2.5, 1.5, & ! 2
        4.0, 2.5, 1.7, & ! 3
        4.0, 0.0, 1.7, & ! 4
        6.0, 0.0, 1.8  & ! 5
        /)

        size_real_array = 18

        call allocate_c_memory_real(size_real_array, c_ptr_data_real, f_ptr_data_real)

        do i = 1, size_real_array
            f_ptr_data_real(i) = nodal_coords(i)
        end do

        ! TODO add missing parts of Mesh

        call solver_print("    Before Exporting Mesh to CoSim", 3)

        call CoSimIO_ExportMesh(connection_name//c_null_char, identifier//c_null_char, &
            size_real_array, num_elements, c_ptr_data_real, c_ptr_data_elem_conn,&
            c_ptr_data_elem_types)

        call solver_print("    After Exporting Mesh to CoSim", 3)
    END SUBROUTINE export_mesh_to_co_sim

    ! helper function to allocate memory in C - Integer version
    SUBROUTINE allocate_c_memory_int(size, c_pointer, f_pointer)
        type (c_ptr), INTENT(IN) :: c_pointer
        INTEGER, DIMENSION(:), pointer, INTENT(INOUT) :: f_pointer
        INTEGER, INTENT(IN) :: size
        call solver_print("    Before allocating memory in C", 3)
        call AllocateCMemoryInt(size, c_pointer)
        call solver_print("    After allocating memory in C", 3)
        call c_f_pointer(c_pointer, f_pointer, [size]) ! necessary according to valgrind
    END SUBROUTINE allocate_c_memory_int

    ! helper function to allocate memory in C - Real version
    SUBROUTINE allocate_c_memory_real(size, c_pointer, f_pointer)
        type (c_ptr), INTENT(IN) :: c_pointer
        REAL(8), DIMENSION(:), pointer, INTENT(INOUT) :: f_pointer
        INTEGER, INTENT(IN) :: size
        call solver_print("    Before allocating memory in C", 3)
        call AllocateCMemoryReal(size, c_pointer)
        call solver_print("    After allocating memory in C", 3)
        call c_f_pointer(c_pointer, f_pointer, [size]) ! necessary according to valgrind
    END SUBROUTINE allocate_c_memory_real

    SUBROUTINE free_c_memory ()
        ! deallocating them all at once bcs does not matter if "free" is called on NULL
        call solver_print("    Before freeing memory in C", 3)
        call FreeCMemory(c_ptr_data_real)
        call FreeCMemory(c_ptr_data_elem_conn)
        call FreeCMemory(c_ptr_data_elem_types)
        call solver_print("    After freeing memory in C", 3)
    END SUBROUTINE free_c_memory


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!! Functions for executing the different versions of CoSimulation
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SUBROUTINE run_solution_loop ()
        real :: current_time = 0.0

        call solver_print("Doing STANDALONE simulation", 1)

        do while (current_time < end_time)
            write(*,*)
            call advance_in_time(current_time)
            call initialize_solution_step()
            call solve_solution_step()
            call finalize_solution_step()
        end do

        call solver_print("Exiting", 1)
    END SUBROUTINE run_solution_loop


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SUBROUTINE run_solution_loop_weak_coupling ()
        real :: current_time = 0.0

        call solver_print("Doing COUPLED simulation (weakly coupled)", 1)

        call CoSimIO_Connect("external_simple_solver"//c_null_char, "unspecified"//c_null_char)

        ! call import_mesh_from_co_sim("external_simple_solver", "interface_1")
        call export_mesh_to_co_sim("external_simple_solver", "interface_mesh_tri")

        do while (current_time < end_time)
            write(*,*)
            call advance_in_time(current_time)
            call initialize_solution_step()
            call import_data_from_co_sim("external_simple_solver", "field_pressure")
            call solve_solution_step()
            call export_data_to_co_sim("external_simple_solver", "field_velocity")
            call finalize_solution_step()
        end do

        call CoSimIO_Disconnect("external_simple_solver"//c_null_char)

        call free_c_memory()

        call solver_print("Exiting", 1)
    END SUBROUTINE run_solution_loop_weak_coupling


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SUBROUTINE run_solution_loop_strong_coupling ()
        real :: current_time = 0.0

        call solver_print("Doing COUPLED simulation (strongly coupled)", 1)

        call CoSimIO_Connect("external_simple_solver"//c_null_char, "unspecified"//c_null_char)

        ! call import_mesh_from_co_sim("external_simple_solver", "interface_1")
        call export_mesh_to_co_sim("external_simple_solver", "interface_mesh_tri")

        do while (current_time < end_time)
            write(*,*)
            call advance_in_time(current_time)
            call initialize_solution_step()
            do ! convergence loop, execute until convergence is achieved
                call import_data_from_co_sim("external_simple_solver", "field_pressure")
                call solve_solution_step()
                call export_data_to_co_sim("external_simple_solver", "field_velocity")
                ! TODO fix this!
                ! call CoSimIO_IsConverged("external_simple_solver"//c_null_char)
                ! if(convergence_signal.NE.0) exit
            end do

            call finalize_solution_step()

        end do

        call CoSimIO_Disconnect("external_simple_solver"//c_null_char)

        call free_c_memory()

        call solver_print("Exiting", 1)
    END SUBROUTINE run_solution_loop_strong_coupling


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SUBROUTINE run_solution_co_simulation_orchestrated ()
        call solver_print("Doing COUPLED simulation (orchestrated by CoSimulation)", 1)

        call CoSimIO_Connect("external_simple_solver"//c_null_char, "unspecified"//c_null_char)

        call CoSimIO_RegisterSolvingFunction("external_simple_solver"//c_null_char,&
            "AdvanceInTime"//c_null_char, C_FUNLOC(advance_in_time))
            call CoSimIO_RegisterSolvingFunction("external_simple_solver"//c_null_char,&
                "InitializeSolutionStep"//c_null_char, C_FUNLOC(initialize_solution_step))
        call CoSimIO_RegisterSolvingFunction("external_simple_solver"//c_null_char,&
            "SolveSolutionStep"//c_null_char, C_FUNLOC(solve_solution_step))

        call CoSimIO_RegisterDataExchangeFunction("external_simple_solver"//c_null_char,&
            "ImportData"//c_null_char, C_FUNLOC(import_data_from_co_sim))
        call CoSimIO_RegisterDataExchangeFunction("external_simple_solver"//c_null_char,&
            "ExportData"//c_null_char, C_FUNLOC(export_data_to_co_sim))

        call CoSimIO_RegisterDataExchangeFunction("external_simple_solver"//c_null_char,&
            "ImportMesh"//c_null_char, C_FUNLOC(import_mesh_from_co_sim))
        call CoSimIO_RegisterDataExchangeFunction("external_simple_solver"//c_null_char,&
            "ExportMesh"//c_null_char, C_FUNLOC(export_mesh_to_co_sim))

        call CoSimIO_Run("external_simple_solver"//c_null_char)

        call CoSimIO_Disconnect("external_simple_solver"//c_null_char)

        call free_c_memory()

        call solver_print("Exiting", 1)
    END SUBROUTINE run_solution_co_simulation_orchestrated

end program