#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char *ctable = "0123456789ABCDEF";
int base = 10;

int rpu(int x)
{
    char c;
    if(x)
    {
        c = ctable[x % base];
        rpu(x / base);
        putchar(c);
    }
}

int printu(unsigned int x)
{
    x == 0 ? putchar('0') : rpu(x);
}

int printd(int x)
{
    if(x == 0)
    {
        putchar('0');
    }
    else if(x < 0)
    {
        putchar('-');
        rpu(-x);
    }else{
        rpu(x);
    }
}

int HEX(unsigned int x)
{
    char c;
    if(x)
    {
        c = ctable[x % 16];
        HEX(x / 16);
        putchar(c);
    }
    return 0;
}
int printx(unsigned int x)
{
    putchar('0');
    putchar('x');
    HEX(x);
}

int prints(char *s)
{
    puts(s);
}

int printtab(){
    int i;
    for(i = 0; i < 8; i++)
    {
        putchar(' ');
    }
}

int printc(char c)
{
    putchar(c);
}
int myprintf(char *fmt,  ...)
{
    int i, len;
    va_list ap;
    va_start(ap, fmt);
    i = 0;
    while( fmt[i] != '\0')
    {
        if(fmt[i] != '%')
        {
            putchar(fmt[i]);
        }
        else
        {
            switch (fmt[++i]) 
            {
                case 'd':
                    printd(va_arg(ap, int));
                    break;
                case 'u':
                    printu(va_arg(ap, unsigned int));
                    break;
                case 'x':
                    printx(va_arg(ap, unsigned int));
                    break;
                case 'c':
                    printc(va_arg(ap, int));
                    break;
                case 's':
                    prints(va_arg(ap, char *));
                    break;
                case 't':
                    printtab();
                    break;
                default:
                    putchar(fmt[i]);
                    break;
            }
        }
        i++;
    }
    va_end(ap);
}

int main()
{
    unsigned int b = 1234565;
    int a = -100000;
    char *s = "it works !!!";
    char c = 'c';
    myprintf("int : %d, unsigned int : %u ", a, b);
    myprintf("HEX : %x, char : %c, string : %s", b, c, s);
    //printc(c);
}

