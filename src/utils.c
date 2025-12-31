#include "init.h"

void get_screen_size(int *rows, int *cols)
{   
    struct winsize w;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        printf("[ x ] Error, Unable to get screen size....!\n");
        return;
    }

    *rows = w.ws_row;
    *cols = w.ws_col;
}

char *read_file(char *filename)
{
    if(!filename)
        return NULL;

    FILE *stream = fopen(filename, "r");
    if(!stream)
        return NULL;

    fseek(stream, 0, SEEK_END);
    long sz = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    char *data = malloc(sz + 1);
    data[sz] = '\0';
    int bytes = fread(data, 1, sz, stream);

    fclose(stream);
    if(bytes > 1)
        return data;

    free(data);
    return NULL;
}

void enable_raw(void)
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &old);
    t = old;
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void disable_raw(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

int count_digits(int num) {
        int n = num;
    if(n == 0) return 1;
    if(n < 0) n = -n;

    int count = 0;
    while(n > 0) {
        n /= 10;
        count++;
    }

    return count;
}

/* Grab all lines including empty lines */
char **split_lines(char *buffer, int *idx)
{
    if(!buffer)
        return NULL;

    int len = strlen(buffer);
    char **arr = (char **)malloc(sizeof(char *));
    *idx = 0;
    int _len = 0;

    char LINE[1024 * 2];
    for(int i = 0; i < len; i++)
    {
        if(buffer[i] == '\0')
            break;

        if(buffer[i] == '\n')
        {
            int n = strlen(LINE);
            if(n == 0)
            {
                LINE[0] = ' ';
                LINE[1] = '\0';
            }

            arr[(*idx)++] = strdup(LINE);
            arr = realloc(arr, sizeof(char *) * ((*idx) + 1));
            if(!arr) printf("ERR\n");
            arr[*idx] = NULL;
            LINE[0] = '\0';
            _len = 0;
            continue;
        }

        LINE[_len++] = buffer[i];
        LINE[_len] = '\0';
    }

    if(*idx > 0)
        return arr;

    free_arr((void *)arr);
    return NULL;
}

char **__split(char *buffer, char *delim, int *idx)
{
    if(!buffer || !delim)
        return NULL;

    char **arr = (char **)malloc(sizeof(char *) * 1);
    if(!arr)
    {
        return NULL;
    }
    *idx = 0;

    char *token, *copy = strdup(buffer), *rest = copy;
    while((token = strtok_r(rest, delim, &rest)))
    {
        int len = strlen(token);

        if(!(arr[*idx] = malloc(len + 1)))
            printf("Malloc err\n");
        memcpy(arr[*idx], token, len + 1);

        (*idx)++;

        if(!(arr = realloc(arr, sizeof(char *) * (*idx + 1)))) {
            free(copy);
            free_arr((void *)arr);
            return NULL;
        }

    }

    free(copy);
    arr[*idx] = NULL;
    return arr;
}

void free_arr(void **arr)
{
    for(int i = 0; arr[i] != NULL; i++)
        free(arr[i]);

    free(arr);
}