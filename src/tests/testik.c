#include <stdio.h>

int main(){
int c = 0;



FILE * src= fopen("dedent_eof.src", "r");

c = getc(src);

while (c != EOF) {
	if (c == '\n')
		printf("\"EOL\"");
printf("\"%c\"\n", c);
	
	//putchar(c);
c = getc(src);
if (c == EOF)
	printf("\"EOF\"");
}





}
