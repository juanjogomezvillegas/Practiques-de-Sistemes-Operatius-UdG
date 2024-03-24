#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main() // p2
{
	int fitxer, i;
	char buff[80];

	fitxer=open("fitx.txt",O_RDONLY);
	for (i = 10; i >= 1; i--) {
		lseek(fitxer,(i-1)*5,SEEK_SET);
		read(fitxer,buff,5);
		write(1,buff,5);
	}
	
	close(fitxer);
	exit(0);
}
