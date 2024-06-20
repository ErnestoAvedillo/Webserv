#include <stdio.h>
#include <unistd.h>
int main ()
{
	int i=0;
	char * pepe = "Hola soy tu amigo Juan";
	while (pepe[i])
		write(1, &pepe[i++],1);
	write(1,"\n",1);
	i++;
	pepe[i++]='0';
	return 1;
}
