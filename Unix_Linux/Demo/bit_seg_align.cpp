#include <stdio.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) {
	// cmd0
	unsigned int subsys :5;
	unsigned int type :3;
	// cmd1
//	unsigned int id :8;
	unsigned char id;
}Cmd;

int main()
{
	int i;
	Cmd cmd;
	
	cmd.type = 0x1;
	cmd.subsys = 0x1;
	cmd.id = 0x2;

	printf( "%u\n", sizeof(cmd) );
	
	//!! struct cannot be converted to simple type even by force.
//	printf( "%x\n", (unsigned int)(cmd) );
	
	//!! we can see its value through pointer.
	//printf( "%x\n", *((unsigned short*)(&cmd)) );

	//!! this is recommended
	for( i = 0; i < sizeof(Cmd)/sizeof(char); ++i )
		printf( "%x\n", *((unsigned char*)&cmd + i) );
	
	return 0;
}