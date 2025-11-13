#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUF 10

int main(int argc, char* argv[])
{
	int fd1, fd2, read_size, write_size, new_offset;
	char buf[MAX_BUF];

	if(argc != 3)
	{
		printf("ERROR!\n");
		printf("Usage: %s <source> <destination>\n", argv[0]);
		exit(-1);
	}

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 < 0)
	{	
		printf("ERROR!\n");
		printf("Fail to open file: %s / error : %d\n", argv[1], errno);
		exit(-1);
	}
	
	fd2 = open(argv[2], O_RDWR | O_CREAT | O_EXCL, 0644);
	if(fd2 < 0)
	{
		printf("ERROR!\n");
		printf("Fail to open file: %s / errno : %d\n", argv[2], errno);
		exit(-1);
	}

	while((read_size = read(fd1, buf, MAX_BUF)) > 0)
	{
		write_size = write(fd2, buf, read_size);
	}

	close(fd1);

	new_offset = lseek(fd2, 0, SEEK_SET);
	while((read_size = read(fd2, buf, MAX_BUF)) > 0)
	{
		write_size = write(STDOUT_FILENO, buf, read_size);
	}

	close(fd2);
	return 0;

}
