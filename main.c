#include "src/init.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("[ x ] Error Invalid arguments\nUsage: %s <file>\n", argv[0]);
        return 1;
    }

    printf("[ + ] Reading file: %s....\n", argv[1]);
    char *data = read_file(argv[1]);
    int rows, cols;
    get_screen_size(&rows, &cols);

    screen s = {0};
    init_screen(&s, rows, cols);
    start_display(&s, data);

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
            display_up(&s);

        if(c == 's')
            display_down(&s);

        if(c == '`')
            display_up_page(&s);

        if(c == '\t')
            display_down_page(&s);
    }

    disable_raw();
    return 0;
}