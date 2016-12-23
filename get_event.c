#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define MOUSEFILE "/dev/input/event1"
#define MAX_JSON_LENGHT 1400

char lowercase_map[] = {
	0,  0,  '1',  '2',  '3',  '4',  '5', '6',  '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q',  'w',  'e', 'r',  't', 'y', 'u', 'i',
	'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
	'j', 'k', 'l',  ';',  '\'',  '\n', 0,  '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm',  ',',  '.',  '/',  0,  '*',  0,  ' ', 0,  0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  '\r'
};


const char uppercase_map[] = {
	0,  0,  '!',  '@',  '#',  '$',  '%', '^',  '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q',  'W',  'E', 'R',  'T', 'Y', 'U', 'I',
	'O', 'P', '{',  '}',  '\n', 0,   'A', 'S',  'D', 'F', 'G', 'H',
	'J', 'K', 'L',  ':',  '"', '\n', 0,  '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M',  '<',  '>',  '?',  0,  '*',  0,  ' ', 0,  0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  0, 0,  0, 0,
	0,  0,  0,   0,   0,   0,   0,  0,   0,  0,  0,  '\r'
};

// Prove of contept - hard coded values ---
const char BARCODEJSON[] =
"{" "\"id\" : \"first01\"," "\"barcode\" : \"%s\"""}";
// Prove of concept - used mosquitto_pub to send values to mqtt
const char CMDmqtt[] = "mosquitto_pub -h 127.0.0.1 -d -t sensors/barcode -m \'%s\'";


int isbn10()
 {
 return(0);
}


int isbn13()
 {
 return(0);
}


int CodeAnalyser(char bcode[MAX_JSON_LENGHT])
 {
 int a,lenght;
 char tmp[MAX_JSON_LENGHT];
 lenght=strlen(bcode);
 if (lenght==13) 
  {
   for (a=0;a<=2;a++)
    {
    tmp[a]=bcode[a];
   }
   tmp[3]='\0';
   a=atoi(tmp);
   if ((a==978) || (a==979))
    {
    printf("Kod ISBN...");
   }
 }
 printf("Lenght: %d\n",lenght);
return(0);
}



int main(int argc, char *argv[])
{
	int fd;
	char jsonbuffer[MAX_JSON_LENGHT];
	char jsonmessage[MAX_JSON_LENGHT];
	char characters[MAX_JSON_LENGHT];
	short UPPER=0;

	int n=0;
	int a=0;
	struct input_event ie;
	if (argv[1]=="test")
	{
	 exit(1);
	} else {

	if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}
	n=0;

		while(read(fd, &ie, sizeof(struct input_event))) {
			if (ie.value == 1) 
			{
				if ((UPPER==0) && (ie.code!=42)) 
				{
					printf("Mapping: %d ie.code - %c \n", ie.code, lowercase_map[ie.code]);
					characters[n]=lowercase_map[ie.code];
					n++;
				}
				if ((UPPER==1) && (ie.code!=42)) 
				{
					printf("Mapping: %d ie.code - %c \n", ie.code, uppercase_map[ie.code]);
					characters[n]=uppercase_map[ie.code];
					n++;
				}
				if (ie.code==42) {
					UPPER=1;
				} else {
					UPPER=0;
				}
			}
			if ((ie.value == 1) && (ie.code == 28))
			{
				characters[n-1]='\0';
				for (a=0;a<=n;a++)
				{
					printf("%c",characters[a]);
				}
				printf("\n----------\n");
				printf("Content variable characters :%s ",characters);
				sprintf(jsonmessage,BARCODEJSON,characters);
				sprintf(jsonbuffer,CMDmqtt,jsonmessage);
				printf("Content jsonbuffer: %s\n",jsonbuffer);
				system(jsonbuffer);
				n=0;
				characters[n]='\0';

			}
		}
	}
	printf("\n");
	return 0;
}
