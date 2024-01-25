#include <mpi.h>

#include <iostream>

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int rank, p, sbufsize = 1024 * 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  // if (rank == 0) {
  //   std::cout << "all-gather-ring "
  //             << "info: " << p << " processes, message size = " << sbufsize
  //             << " B\n";
  // }

  char *rbuf = (char *)malloc(sizeof(char) * sbufsize * p);
  rbuf[rank * sbufsize] = 'a' + rank;
  // std::cout << rank << ": sbuf = '" << rbuf[rank * sbufsize] << "'\n";

  int sendTo = (rank + 1) % p;
  int receiveFrom = (rank - 1 + p) % p;
  // std::cout << "all-gather-ring " << rank << ": send to " << sendTo
  //           << ", receive from " << receiveFrom << '\n';

  double t = MPI_Wtime();
  for (int i = 0; i < p - 1; i++) {
    int curBufOwner = (rank - i - 1 + p) % p;
    int prevBufOwner = (curBufOwner + 1) % p;
    MPI_Sendrecv(&rbuf[prevBufOwner * sbufsize], sbufsize, MPI_CHAR, sendTo, 0,
                 &rbuf[curBufOwner * sbufsize], sbufsize, MPI_CHAR, receiveFrom,
                 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // std::cout << rank << ": received '" << rbuf[curBufOwner * sbufsize]
    //           << "' to rbuf\n";
  }
  // t = MPI_Wtime() - t;
  int pass = 0;
  // std::cout << rank << ": rbuf = ";
  for (int i = 0; i < sbufsize * p; i += sbufsize) {
    if (rbuf[i] != 'a' + i / sbufsize) {
      // std::cerr << "rbuf[" << i << "] is invalid";
      pass = 1;  //проверка
    }
    // std::cout << rbuf[i];
  }
  int passed_global = 0;  // global
  MPI_Reduce(&pass, &passed_global, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    if (passed_global == 0) {
      std::cout << "YDACHNO\n";
    } else
      std::cout << "PROVAL\n";
  }
  // std::cout << ", took " << t << " seconds" << '\n';

  free(rbuf);
  MPI_Finalize();
  return 0;
}