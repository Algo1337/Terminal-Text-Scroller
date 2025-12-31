#include "src/init.h"

int main(int argc, char *argv[])
{
    
    clear();
    printf(">");
    enable_raw();
    int c;
    fflush(stdin);
    while(c = getc(stdin))
    {
        if(c == -1 || c == 0)
        {
            usleep(30000);
            continue;
        }
        
        if(c == 'q')
        {
            disable_raw();
            clear();
            return 1;
        }

        if(c == 'w')
        {
            printf("\r ");
            printf("\x1b[1A\r>");
        }

        if(c == 's')
        {
            printf("\r ");
            printf("\x1b[1A\x1b[2B\r>");
        }
    }

    disable_raw();
    return 0;
}