#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUF 10

int main(int argc, char* argv[])
{
	int fd, read_size, write_size, opt;
	char buf[MAX_BUF];
	int count = 0;
	int add_cof = 0;

	if((argc != 2) && (argc != 3))
	{
		printf("ERROR!\n");
		printf("1. USAGE: %s <file_name>\n", argv[0]);
		printf("2. USAGE: %s -c <file_name>\n", argv[0]);
		exit(-1);
	}
	
	if(argc == 3)
	{
		while((opt = getopt(argc, argv, "c")) != -1)
		{
			switch(opt)
			{
				case 'c' :
					add_cof = 1;
					break;
				default :
					printf("ERROR!\n");
					printf("USAGE: %s -c file_name\n", argv[0]);
					exit(-1);
			}
		}
	}

	fd = open(argv[optind], O_RDONLY);

	if(fd < 0)
	{
		printf("ERROR!\n");
		printf("Fail to open file: %s / errno : %d \n", argv[optind], errno);
		exit(-1);
	}
		
	while((read_size = read(fd, buf, MAX_BUF)) > 0)	
	{
		write_size = write(STDOUT_FILENO, buf, read_size);
		count++;
	}
	
	if(add_cof)
	{
		printf("MAX_BUF가 %d일 때, %s파일을 화면에 출력하기 위해 읽고 쓰기를 반복한 횟수 : %d\n", MAX_BUF, argv[optind], count);
	}
	
	close(fd);
	return 0;
}

