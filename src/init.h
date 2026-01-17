#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <clibp_gcc.h>

#define AUTO_FULL_SCREEN -1

typedef struct {
    int     x, y;
} _cor;

typedef struct {
	int top, left, right, bottom;
} _edges;

typedef _cor srl_t;
typedef _cor pos_t;

typedef struct {
    int     width;
    int     length;

    char    *content;
    char    **lines;
    int     line_count;
    int     nums;

    int     top_view_line;
    pos_t   current_pos;
    srl_t   scroller;
} screen;

static struct termios old;

// init.c
int init_screen(screen *s, int row, int col);
void start_display(screen *s, char *data, int output);
void clear();
void display_up(screen *s, int output);
void display_down(screen *s, int output);
void display_up_page(screen *s, int output);
void display_down_page(screen *s, int output);

// utils.c
void get_screen_size(int *rows, int *cols);
char *read_file(char *filename);
void enable_raw(void);
void disable_raw(void);
int count_digits(int num);
char **split_lines(char *buffer, int *idx);
char **__split(char *buffer, char *delim, int *idx);
void free_arr(void **arr);
