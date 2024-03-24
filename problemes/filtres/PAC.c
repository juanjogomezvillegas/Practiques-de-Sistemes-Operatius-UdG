#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

main() {
	int i;
	char buff[80];

	i=read(0,buff,80);
	close(1);
	while (i > 0) {
		write(2,buff,i);
		i=read(0,buff,80);
	}
	close(2);
}

