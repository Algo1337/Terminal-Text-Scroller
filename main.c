#include "src/init.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("[ x ] Error Invalid arguments\nUsage: %s <file>\n", argv[0]);
        return 1;
    }

    printf("[ + ] Reading file: %s....\n", argv[1]);

    screen s = {0};

	int rows, cols;
	get_screen_size(&rows, &cols);
    int chk = init_screen(&s, AUTO_FULL_SCREEN, AUTO_FULL_SCREEN);
//	int chk = init_screen(&s, (rows / 2), (cols / 2));
//	printf("%s\n", create_box(cols / 2, rows / 2));
    if(!chk)
        return 1;

    char *data = read_file(argv[1]);
    if(!data)
        return 1;

    start_display(&s, data, 1);

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
            display_up(&s, 1);

        if(c == 's')
            display_down(&s, 1);

        if(c == '`')
            display_up_page(&s, 1);

        if(c == '\t')
            display_down_page(&s, 1);
    }

    disable_raw();
    return 0;
}
