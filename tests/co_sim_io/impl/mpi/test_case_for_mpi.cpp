#include "doctest/extensions/doctest_mpi.h"

int my_function_to_test(MPI_Comm comm) {
  int rank;
  MPI_Comm_rank(comm,&rank);
  if (rank == 0) {
    return 10;
  }
  return 11;
}


MPI_TEST_CASE("test over two processes",3) { // Parallel test on 2 processes
return;
  int x = my_function_to_test(test_comm);

  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
}

MPI_TEST_CASE("my_test",2) {
  CHECK( test_nb_procs == 2 );
//   MPI_CHECK( i, test_rank==i ); // for any i<N
}