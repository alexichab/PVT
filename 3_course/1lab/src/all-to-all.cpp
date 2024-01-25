#include <mpi.h>

#include <iostream>

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int size, rank, sbufsize = 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  char *rbuf = (char *)malloc(sizeof(char) * sbufsize * size);
  char *sbuf = (char *)malloc(sizeof(char) * sbufsize * size);
  for (int i = 0; i < size; ++i) {
    sbuf[i * sbufsize] = 'a' + rank;
    sbuf[i * sbufsize + 1] = '0' + i;
    rbuf[i * sbufsize] = 'X';
    rbuf[i * sbufsize + 1] = 'X';
  }

  MPI_Request *r = (MPI_Request *)malloc(sizeof(MPI_Request) * size * 2);
  for (int i = 0; i < size; ++i) {
    int dst = (rank + i) % size;
    MPI_Isend(&sbuf[dst * sbufsize], sbufsize, MPI_CHAR, dst, 0, MPI_COMM_WORLD,
              &r[i * 2]);
    MPI_Irecv(&rbuf[dst * sbufsize], sbufsize, MPI_CHAR, dst, 0, MPI_COMM_WORLD,
              &r[i * 2 + 1]);
  }
  MPI_Waitall(size * 2, r, MPI_STATUSES_IGNORE);
  int pass = 0;
  for (int i = 0; i < size; ++i) {  //проверка
    char letter = 'a' + i;
    char number = '0' + rank;
    std::string expected = std::string() + letter + number;
    std::string result =
        std::string() + rbuf[i * sbufsize] + rbuf[i * sbufsize + 1];

    if (result != expected) {
      std::cerr << "rbuf[" << i * sbufsize << "] is invalid\n";  //проверка
      pass = 1;
    }
  }
  int passed_global = 0;  // global

  MPI_Reduce(&pass, &passed_global, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    if (passed_global == 0) {
      std::cout << "YDACHNO\n";
    } else
      std::cout << "PROVAL\n";
  }

  free(sbuf);
  free(rbuf);
  free(r);
  MPI_Finalize();
  return 0;
}