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

program hello
    use co_sim_io
    call CoSimIO_Hello() ! TODO print the returned info (see "Hello" tests for other languages)
end program hello
