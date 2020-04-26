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


program test_runner
    implicit none

    CHARACTER(100) :: input_char
    INTEGER :: level_of_coupling

    ! Make sure the right number of command line arguments have been provided
    ! IF(COMMAND_ARGUMENT_COUNT().NE.2)THEN
    !   WRITE(*,*)'ERROR, TWO COMMAND-LINE ARGUMENTS REQUIRED, STOPPING'
    ! !   STOP 1
    ! ENDIF

    ! ! Read the command line arguments
    ! CALL GET_COMMAND_ARGUMENT(1,input_char)
    ! READ(input_char,*)level_of_coupling
    ! CALL GET_COMMAND_ARGUMENT(2,input_char)
    ! ! READ(input_char,*)echo_level

    ! ! Select level of coupling
    ! IF (level_of_coupling == 0) THEN
    !     ! call run_solution_loop()
    ! ELSE IF (level_of_coupling == 1) THEN
    !     ! call run_solution_loop_weak_coupling()
    ! ELSE IF (level_of_coupling == 2) THEN
    !     ! call run_solution_loop_strong_coupling()
    ! ELSE IF (level_of_coupling == 3) THEN
    !     ! call run_solution_co_simulation_orchestrated()
    ! ELSE
    !     WRITE(*,*)'ERROR, WRONG LEVEL OF COUPLING; CAN ONLY BE 0, 1, 2, 3, STOPPING'
    !     ! STOP 1
    ! END IF

end program