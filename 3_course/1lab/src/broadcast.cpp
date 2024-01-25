#include <mpi.h>

#include <iostream>

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int rank, size, bufsize = 1024, root = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char *buf = (char *)malloc(sizeof(char) * bufsize);
  if (rank == root) {
    std::cout << "broadcast " << size << " processes, root = " << root
              << ", bufsize = " << bufsize << " B\n";
  }

  if (rank == root) {
    buf[0] = 'x';  // "buf" is sbuf for root
    for (int i = 0; i < size; i++) {
      if (i == root) continue;
      MPI_Send(buf, bufsize, MPI_CHAR, i, 0, MPI_COMM_WORLD);
      std::cout << "broadcast " << rank << ": sent buf \"" << buf[0] << "\" to "
                << i << '\n';
    }
  } else {
    // "buf" is rbuf for non-root
    MPI_Recv(buf, bufsize, MPI_CHAR, root, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    if (buf[0] == 'x') {
      std::cout << "broadcast " << rank << ": received buf \"" << buf[0]
                << "\" from " << root << '\n';
      std::cout << "PASSED\n";
    } else
      std::cerr << "broadcast " << rank << ": invalid buf\n";  //проверка
  }

  free(buf);
  MPI_Finalize();
  return 0;
}