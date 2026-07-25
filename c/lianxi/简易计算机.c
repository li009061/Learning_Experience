#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a , b;
    char s;
    scanf("%d %c %d", &a , &s , &b);
    if (s == '+')
    {
       printf("%d\n", a+b);
    }if (s == '-')
    {
       printf("%d\n", a-b);
    }if (s == '*')
    {
       printf("%d\n", a*b);
    }if (s == '/')
    {
       printf("%d\n", a/b);
    }
    return 0;
}
    
    
