#include <unistd.h>

int i;
char buff[80];

main()
{
	i=read(0,buff, 80);
	while(i>0)
	{
		write(1,buff, i);
		i=read(0,buff,80);
	}
}
