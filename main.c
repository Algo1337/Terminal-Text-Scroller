#include "src/init.h"

typedef __int8_t *str;
typedef __int8_t **str_arr;
typedef __int8_t *Box;

typedef struct
{
	pos_t	size;
	_edges 	position;

	Box		content;
	int 	length;
} _shape;

typedef struct
{
	pos_t	size;

	str_arr lines;
	int 	length;
} _screen;

str create_line(int len)
{
    str data = (str)malloc(len);
	int i;
    for(i = 0; i < len; i++)
        data[i] = ' ';

	data[i] = '\0';
    return data;
}

_screen create_screen(int width, int height)
{
	_screen s = { {0}, malloc(8 * height) };

	for(int i = 0; i < height; i++)
		s.lines[s.length++] = create_line(width);

	return s;
}

int place_text(_screen *s, char *data, int row, int col)
{
	if(strstr(data, "\n")) {
		int line_count, arg_c;
		char **lines = split_lines(data, &line_count);
		if(line_count == 0)
			return 0;

		for(int i = row, idx = 0; i < line_count; idx++, i++)
		{
			int len = strlen(lines[0]);
			for(int c = 0, n = col; c < len; c++, n++)
				s->lines[i][n] = lines[idx][c];
		}

		return 1;
	}

	int len = strlen(data);
	for(int i = col, c = 0; c < len; i++)
		s->lines[row][i] = data[c++];

	return 1;
}

Box create_box(int width, int height)
{
    _shape *box = malloc(sizeof(_shape));
    if(!box)
        printf("ERR\n");

    box->size = (pos_t){ width + 3, height };
    if(!(box->content = malloc(height * (width + 3) + 1)))
        printf("ERR\n");

    box->length = 0;
    for(int i = 0; i < height; i++)
    {
        box->content[box->length++] = '|';
        for(int c = 0; c < width; c++)
        {
            box->content[box->length++] = (i == 0 || i == height - 1 ? '-' : ' ');
        }

        box->content[box->length++] = '|';
        box->content[box->length++] = '\n';
    }

    box->content[box->length] = '\0';

    return box->content;
}

int main(int argc, char *argv[])
{
	_screen scr = create_screen(50, 12);
	Box box = create_box(10, 10);
	Box nbox = create_box(10, 10);
	place_text(&scr, box, 0, 0);
	place_text(&scr, nbox, 0, 15);
	place_text(&scr, "TEXT", 3, 4);
	for(int i = 0; i < scr.length; i++)
		printf("'%s'\n", scr.lines[i]);
	return 1;
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
