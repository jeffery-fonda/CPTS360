typedef unsigned long u32;

int rpu(u32 x, u32 base);
int prints(char *s); //strings
int printu(u32 x); //unsigned ints
int printd(int x); //signed ints
int printo(u32 x); //OCTALs
int printx(u32 x); //HEX
int my_printf(char *fmt, ...); //my printf function

int my_printf(char *fmt, ...)
{
	char *cp = fmt; //from book
  int *ip = &fmt + 1; //from book

  while(*cp) //cp is formatted string
  {
  	if (*cp != '%')
    {
    	if (putchar(*cp) == '\n') //for each '\n'
			{
				putchar('\r');  //print a '\r'
			}
    }
    else
    {
    	cp++; //advance through cp

    	switch (*cp) //*cp = char after %
    	{
				case 'c': putchar(*ip);		ip++;	break; //chars
				case 's': prints((char*)(*ip));	ip++;	break; //strings
				case 'd': printd(*ip);		ip++;	break; //ints
				case 'o': printo(*ip);		ip++;	break; //octals
				case 'x': printx(*ip);		ip++;	break; //HEX
				case 'u': printu(*ip);		ip++;	break; //unsigned ints
				case '%': putchar('%');	break; // %% -> %
				default: // unknown specifier
					putchar('%');
        	putchar(*cp);
        	break;
				}
      }
    cp++;
	}
}

int rpu(u32 x, u32 base) //copied from book and modified
{
	char c;
	static char *ctable = "0123456789ABCDEF";

  if (x)
  {
  	c = ctable[x % base];
    rpu(x / base, base);
    putchar(c);
  }
}

int prints(char* s) //for printing string
{
	char *cp = s; //simply modify cp into a string

  while(*cp)
	{
    putchar(*cp++); //print contents of string
	}
}

int printu(u32 x) //modified from book, for printing unsigned long ints
{
  if (x==0) //the easy case
	{
    putchar('0');
  }
	else
	{
    rpu(x,10);
	}
}

int printd(int x) //for printing signed ints
{
  if (x==0)
	{
  	putchar('0');
	}
  else if (x < 0)
  {
  	putchar('-'); //number is negative so print a minus sign first
    rpu(-x,10);
  }
  else
	{
  	rpu(x,10);
	}
}

int printo(u32 x) //printing OCTAL
{
  putchar('0');

  if (x==0)
	{
  	putchar('0');
	}
  else
	{
    rpu(x,8);
	}
}

int printx(u32 x) //printing unsigned HEX
{
  putchar('0'); //print the first two parts of the hex
  putchar('x');

  if (x==0)
	{
  	putchar('0');
	}
  else
	{
    rpu(x,16);
	}
}
