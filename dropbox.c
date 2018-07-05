#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

int main(int argc, char **argv){
  struct dirent **namelist;
  int n, i;
  struct stat buf;
  while(1){
    //Verifica se os arquivos foram modificados a cada 10 segundos
    if (argc == 1) {
      n = scandir(".", &namelist, NULL, alphasort);
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
    //Se foi modificado faz backup

  }
}

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("uso: %s <arq_origem> <arq_destino>\n", argv[0]);
		return 0;
	}
	char *origem = argv[1];
	char *destino = argv[2];
	int fd_o, fd_d;
	fd_o = open(origem, O_RDONLY);
	if (fd_o == -1) {
		perror("open()");
		return 0;
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd_d = open(destino, O_CREAT | O_RDWR | O_APPEND, mode);
	if(fd_d == -1) {
		perror("open()");
		close(fd_o);
		return 0;
	}
#define BLOCO 4096
	int nr, ns, nw, n;
	unsigned char buffer[BLOCO];
	void *ptr_buff;
	do {
		nr = read(fd_o, buffer, BLOCO);
		if (nr == -1) {
			perror("read()");
			close(fd_o);
			close(fd_d);
			return 0;
		}
		else if (nr > 0) {
			ptr_buff = buffer;
			nw = nr;
			ns = 0;
			do {
				n = write(fd_d, ptr_buff + ns, nw);
				if (n == -1) {
					perror("write()");
                	        	close(fd_o);
	        	                close(fd_d);
		                        return 0;
				}
				ns += n;
				nw -= n;
			} while (nw > 0);
		}
	}while(nr > 0);
	close(fd_o);
	close(fd_d);
	return 0;
}

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


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
    n = scandir(".", &namelist, NULL, alphasort);
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
