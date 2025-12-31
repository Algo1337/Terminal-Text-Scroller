#include "init.h"

/* -1, -1 */
int init_screen(screen *s, int rows, int cols)
{
    if(rows == 0 || cols == 0)
    {
        printf("[ x ] Error, Invalid size provided....!\n");
        return 0;
    }

    if(rows == AUTO_FULL_SCREEN && cols == AUTO_FULL_SCREEN)
    {
        get_screen_size(&s->length, &s->width);
        return 1;
    }

    s->length = rows;
    s->width = cols;
    return 1;
}

void start_display(screen *s, char *data)
{
    s->content = strdup(data);
    //s->lines = __split(data, "\n", &s->line_count);
    s->lines = split_lines(data, &s->line_count);
    s->nums = count_digits(s->line_count);

    printf("[ + ] Length: %ld | Lines: %d | Line Number Spaces: %d\n", strlen(s->content), s->line_count, s->nums);
    sleep(2);

    for(int i = 0; i < s->length; i++) {
        if(!s->lines[i]) break;
        printf(i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", 
            s->nums, 
            i, 
            s->lines[i]
        );
    }
}

void clear() { printf("\x1b[2J\033[1;1H"); }

void display_up(screen *s)
{
    if(s->top_view_line == 0 || s->top_view_line == 1) {
        clear();
        for(int i = 0; i < s->length; i++) printf( i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, i, s->lines[i]);

        s->top_view_line = 0;
        return;
    }

    s->top_view_line--;

    clear();
    for(int i = 0, c = s->top_view_line; i < s->length; i++, c++) {
        if(!s->lines[c]) break;
        printf(i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, c, s->lines[c]);
    }
}

void display_down(screen *s)
{
    if(s->top_view_line + s->length > s->line_count)
    {
        for(int i = s->line_count - s->length; i < s->length; i++)
            printf(i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, i, s->lines[i]);

        return;
    }

    s->top_view_line++;

    clear();
    for(int i = 0, c = s->top_view_line; i < s->length; i++, c++) {
        if(!s->lines[c]) break;
        printf(i == s->length - 1 ? "[%*d] %s": "[%*d] %s\n", s->nums, c, s->lines[c]);
    }
}

void display_up_page(screen *s)
{
    if(s->top_view_line == 0 || s->top_view_line == 1 || s->top_view_line - s->length <= 0) {
        clear();
        for(int i = 0; i < s->length; i++) printf( i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, i, s->lines[i]);

        s->top_view_line = 0;
        return;
    }

    s->top_view_line--;

    clear();
    for(int i = 0, c = s->top_view_line - s->length; i < s->length; i++, c++) {
        if(!s->lines[c]) break;
        printf(i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, c, s->lines[c]);
    }

    s->top_view_line -= s->length;
}

void display_down_page(screen *s)
{
    if(s->top_view_line > s->line_count)
        return;

    if(s->top_view_line + s->length > s->line_count)
    {
        for(int i = s->line_count - s->length; i < s->length; i++)
            printf(i == s->length - 1 ? "[%*d] %s" : "[%*d] %s\n", s->nums, i, s->lines[i]);

                s->top_view_line = s->line_count;
        return;
    }

    s->top_view_line++;

    clear();
    for(int i = 0, c = s->top_view_line + s->length - 1; i < s->length; i++, c++) {
        if(!s->lines[c]) break;
        printf(i == s->length - 1 ? "[%*d] %s": "[%*d] %s\n", s->nums, c, s->lines[c]);
    }

    s->top_view_line += s->length;
}