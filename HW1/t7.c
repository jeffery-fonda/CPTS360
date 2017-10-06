#include <stdio.h>

int *FP; // a global pointer

int main(int argc, char *argv[], char *env[])
{ 
	int a,b,c;

	printf("enter main: &argc=%x &argv=%x\n", &argc, &argv);

	printf("&a=%x &b=%x &c=%x\n", &a, &b, &c);

	a=1; b=2; c=3;
	A(a,b);
	printf("exit main\n");
}

int A(int x, int y)
{ 
	int d,e,f;
	printf("enter A\n");
	d=4; e=5; f=6;
	B(d,e);
	printf("exit A\n");
}

int B(int x, int y)
{ 
	int u,v,w;
	int *p;

	printf("enter B\n");
	u=7; v=8; w=9;

	p = (int *)&p;

	asm("movl %ebp, FP"); // set FP=CPU’s %ebp register

	printf("FP=%8x p=%8x\n", FP, p);

	while(FP)
	{
		printf("%8x->", FP);
		FP = (int *)*FP;
	}

	printf("NULL\n");

	getchar();	
	
	// Write C code to DO (1)-(3) AS SPECIFIED BELOW

	for (int i=0; i<100; i++)
	{
		printf("%8x %8x\n", p, *p);
		p++;
	}

	printf("exit B\n");
}
