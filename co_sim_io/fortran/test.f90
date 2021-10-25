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

! MODULE co_sim_io
!     ! USE, INTRINSIC :: ISO_C_BINDING
!     ! IMPLICIT NONE

!     ! INTERFACE


!     ! END INTERFACE

! END MODULE co_sim_io


program hello
    use co_sim_io
    ! This is a comment line; it is ignored by the compiler
    print *, 'Hello, World!'

    call CoSimIO_Connect("asas", "ddddf")
end program hello