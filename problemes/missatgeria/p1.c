#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main() // p1
{
	int fitxer, i;
	char buff[80];

	fitxer=open("fitx.txt",O_CREAT|O_WRONLY,0600);
	for (i = 10; i >= 1; i--) {
		read(0,buff,5);
		lseek(fitxer,(i-1)*5,SEEK_SET);
		write(fitxer,buff,5);
	}
	
	close(fitxer);
	exit(0);
}
