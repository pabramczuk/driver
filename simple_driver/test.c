#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/simple_drv"

int main()
{
	int i,fd;
	char ch, write_b[100],read_b[100];

	fd = open(DEVICE, O_RDWR);
	if(fd==-1)
	{
		printf("Eroor open)");
		return -1;
	}

	printf("read from device :");
	scanf("%c",&ch);

	switch(ch)
	{
		case 'w':
			printf("enter data:");
			scanf(" %[^\n]",write_b);
			write(fd,write_b,sizeof(write_b));
			break;

		case 'r':
			read(fd,read_b,sizeof(read_b));
			printf("read: %s\n",read_b);
			break;		

	}
	close (fd);
	return 0;

}
