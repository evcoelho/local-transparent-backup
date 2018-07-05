#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_stat(struct stat buf){
	printf ("\n\t inode: %lld \
		 \n\t size:   %lld \
		 \n\t block size: %lld \
		 \n\t n blocks: %lld \
		 \n\t acesso: %s \
	 modif:  %s \
	 status: %s ",
		(long long) buf.st_ino,
		(long long) buf.st_size,
		(long long) buf.st_blksize,
		(long long) buf.st_blocks,
		ctime(&buf.st_atime),
		ctime(&buf.st_mtime),
		ctime(&buf.st_ctime));
}

#define print_type(t) printf(" %s ", #t);

void print_type_name(unsigned char type) {
	switch(type) {
		case DT_DIR: print_type(DT_DIR); break;
		case DT_REG: print_type(DT_REG); break;
	}
}

int main(int argc, char **argv) {
  struct dirent **namelist;
  int n, i;
  struct stat buf;

  if (argc == 1) {
    n = scandir("./myfolder", &namelist, NULL, alphasort);
  } else {
    n = scandir(argv[1], &namelist, NULL, alphasort);
  }

  if (n < 0)
    perror("scandir()");
  else {
    i = 0;
    while (i < n) {
      printf("%s", namelist[i]->d_name);
      print_type_name(namelist[i]->d_type);
      stat(namelist[i]->d_name, &buf);
      print_stat(buf);
      free(namelist[i]);
      i++;
    }
    free(namelist);
  }
  return 0;
}
