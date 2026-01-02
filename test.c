#include "src/init.h"

#include <stddef.h>

typedef __int8_t *str;
typedef __int8_t **str_arr;
typedef __int8_t *Box;
typedef __int64_t *Shapes;
typedef __int64_t counter_t;

typedef struct
{
	pos_t   	size;
    _edges  	position;

    Box     	content;
    int    		length;
} _shape;

typedef _shape *shape_t;

/*
	Box main_box = create_box(10, 10);
	shape_t *main_box_t = __GET_SHAPE__(main_box);
*/
#define _SHAPE_STRUCT_OFFSET_ sizeof(_shape) - (sizeof(int) * 6)
#define __GET_SHAPE__(p) \
    ((shape_t)((char *)(p) - offsetof(_shape, size)))

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

        free_arr((void *)lines);
        return 1;
	}

    int len = strlen(data);
    for(int i = 0, c = col; i < len; i++, c++) {
    	s->lines[row][c] = data[i];
    }

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

    /* Initialize main screen */
    _screen scr = create_screen(cols, rows);

    /* Initialize screen layout */
	Box screen_box = create_box(cols - 2, rows);
	place_text(&scr, screen_box, 0, 0);

    /* Create and place to boxes */
    int box_text_start = 4;
    Box box = create_box((cols / 2) - 5, (rows / 2) - 3);
    
    place_text(&scr, box, 2, 2);
    place_text(&scr, box, (cols / 2) + 1, 2);
    place_text(&scr, box, 2, (rows / 2) + 2);
    place_text(&scr, box, (cols / 2) + 1, (rows / 2) + 2);


	char *t = read_file("t.txt");
    int line_count;
    char **lines = split_lines(t, &line_count);

    place_text(&scr, "Multi-Screening", (cols / 2) - (strlen("Multi-Screening") / 2), 1);
	place_text(&scr, "[ System Info ]", 18, 4);
    
    for(int i = 0, row = 6; i < line_count; i++, row++)
        place_text(&scr, lines[i], 4, row);

    for(int i = 0; i < scr.length; i++)
	    printf(i == rows - 1 ? "%s" : "%s\n", scr.lines[i]);

    enable_raw();
    int c;
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
            
        }

        if(c == 's')
        {

        }
    }

    disable_raw();
    return 0;
}
