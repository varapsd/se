#include <stdio.h>
#include <stdlib.h>
int main()
{
	
	char t[] = "-123";
	char *p = t;
	printf("%s\n",t );
	printf("%d\n",atoi(t) );
	printf("%s\n",p );
	printf("%d\n",atoi(p) );
}