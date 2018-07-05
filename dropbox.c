//Everton da Silva Coelho  101937
//Gabriel Borin Takahashi 101953
//Pedro Manhez Naresi 105615

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#define BLOCO 4096

#define print_type(t) printf(" %s ", #t);

int main(int argc, char **argv) {
	struct dirent **namelist;
	struct dirent **namelist_backup;
	
	struct stat buf;
	struct stat buf_backup;

	int n, b, i, j;

	time_t start, finish;

	char dir_origem[500] =  "myfolder/";
	char dir_dest[500] = "myfolder_backup/";

	if (argc == 1) {
		n = scandir(dir_origem, &namelist, NULL, alphasort);
		b = scandir(dir_dest, &namelist_backup, NULL, alphasort);
	}
	if (n < 0 || b < 0)
		perror("scandir()");
	else {
		while(1){
		    i = 2;
		    while (i < n) {
		    	char nome1[355] = "myfolder/";
		    	j = 2;
		    		while (j < b){
		    			char nome2[355] = "myfolder_backup/";
				    	if (strcmp(namelist[i]->d_name, namelist_backup[j]->d_name)==0){
				    		if(stat(strcat(nome1, namelist[i]->d_name), &buf)==-1){
				    			printf("Erro1\n");
				    		}
				    		else{
				    			printf("%s\n", ctime(&buf.st_mtime));
				    		}
				    		if(stat(strcat(nome2, namelist_backup[j]->d_name), &buf_backup)==-1){
				    			printf("Erro2\n");
				    		}
				    		else{
				    			printf("%s\n", ctime(&buf_backup.st_mtime));
				    		}
				   
				    		start = buf.st_mtime;
				    		finish = buf_backup.st_mtime;

				    		if (difftime(start, finish) > 0){

				    			printf("MODIFICOU\n");

				    			char *origem = dir_origem;
				    			printf("%s\n", origem);
								char *destino = dir_dest;
								printf("%s\n", destino);
								int fd_o, fd_d;
								fd_o = open(nome1, O_RDONLY);
								if (fd_o == -1) {
									perror("open(1)");
									return 0;
								}
								mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
								fd_d = open(nome2, O_CREAT | O_RDWR | O_TRUNC, mode);
								if(fd_d == -1) {
									perror("open(2)");
									close(fd_o);
									return 0;
								}
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
				    		}
				    	}
				    	j++;
				    	strcpy(nome2, "");
				    }
				i++;
				strcpy(nome1, "");
		    }
		    sleep(3);
		}
	}
  return 0;
}
