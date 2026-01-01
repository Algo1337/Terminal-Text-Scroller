#include "src/init.h"

typedef __int8_t *str;
typedef __int8_t **str_arr;
typedef __int8_t *Box;
typedef __int8_t *Shapes;
typedef __int8_t shape_t;
typedef __int32_t counter_t;

typedef struct
{
	pos_t   	size;
    _edges  	position;

    Box     	content;
    int    		length;
} _shape;

/*
	Box main_box = create_box(10, 10);
	shape_t *main_box_t = __GET_SHAPE__(main_box);
*/
#define _SHAPE_STRUCT_OFFSET_ sizeof(_pos) + sizeof(_edges)
#define __GET_SHAPE__(p) ((char *)p) - _SHAPE_STRUCT_OFFSET_
#define __FREE_SHAPE__(p) \
	shape_t n = __GET_SHAPE__(p); \
	memset(n->content, 0, n->length); \
	free(n->length); \
	memset(n, 0, sizeof(_shape)); \
	free(n);

typedef struct
{
        pos_t   size;

        str_arr lines;
        int     length;
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
	_screen s = {
		{0},
		malloc(sizeof(char *) * (height + 1)),
		0
	};

    for(int i = 0; i < height; i++)
		s.lines[s.length++] = create_line(width);

    return s;
}

int place_text(_screen *s, char *data, int col, int row)
{
    if(strstr(data, "\n")) {
    	int line_count = 0;
        char **lines = split_lines(data, &line_count);
        if(line_count == 0)
        	return 0;

        for(int i = row, idx = 0; idx < line_count; idx++, i++)
        {
        	int len = strlen(lines[idx]);
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
	int rows, cols;
	get_screen_size(&rows, &cols);

	Box screen_box = create_box(cols - 2, rows);

    _screen scr = create_screen(cols, rows);
	place_text(&scr, screen_box, 0, 0);

    Box box = create_box(46, 20);
    Box nbox = create_box(15, 20);

    place_text(&scr, box, 2, 3);
    place_text(&scr, nbox, 52, 3);
    place_text(&scr, "TEXT", 55, 5);

	screen s = {0};
	init_screen(&s, cols / 2, rows / 2);

	char *t = read_file("test.c");
	start_display(&s, t, 0);

	place_text(&scr, "[ System Info ]", 18, 1);
	for(int i = 0; i < 8; i++) {
		place_text(&scr, "> ", 5, i + 5);
		place_text(&scr, s.lines[i], 8, i + 5);
	}

    for(int i = 0; i < scr.length; i++)
	    printf(i == rows - 1 ? "%s" : "%s\n", scr.lines[i]);

	pos_t last_pos = {0, 1};
//	place_text(&scr, ">", 0, 1);
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
//			place_text(&scr, ">", last_pos.x - 1, 1);
//			last_pos.x--;
			display_up(&s, 0);
			for(int i = 0, c = s.top_view_line; i < rows; i++, c++)
				place_text(&scr, s.lines[c], 8, i + 5);

			clear();
			for(int i = 0; i < scr.length; i++)
				printf(i == rows - 1 ? "%s" : "%s\n", scr.lines[i]);
        }

        if(c == 's')
        {
//			place_text(&scr, ">", last_pos.x + 1, 1);
//			last_pos.x++;
			display_down(&s, 0);
			for(int i = 0, c = s.top_view_line; i < s.length; i++, c++)
				place_text(&scr, s.lines[c], 8, i + 5);

			clear();
			for(int i = 0; i < scr.length; i++)
				printf(i == rows - 1 ? "%s" : "%s\n", scr.lines[i]);
        }
    }

    disable_raw();
    return 0;
}
