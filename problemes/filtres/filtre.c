#include <unistd.h>

int i;
char buff[80];

main(int argc, char *argv[])
{
	if (argc == 2) {
		printf(argv[1]);
		i=read(0,buff, 80);
		while(i>0)
		{
			write(1,buff, i);
			i=read(0,buff,80);
		}
	} else {
		printf("error amb els parametres");
	}
}
