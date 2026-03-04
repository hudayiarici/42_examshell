#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s2[i] == '\0');
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    char *buffer = NULL;
    char c;
    int len = 0;
    int i = 0;
    int find_len = strlen(argv[1]);
    buffer = malloc(find_len);
    if(!buffer)
        return 1;
    while (read(0, &c, 1) > 0)
    {
        buffer = realloc(buffer, len + 2);
        if (!buffer)
            return 1;
        buffer[len] = c;
        len++;
        buffer[len] = '\0';
    }
    while (i < len)
    {
        if (ft_strcmp(&buffer[i], argv[1]))
        {
            int j = 0;
            while (j < find_len)
            {
                write(1, "*", 1);
                j++;
            }
            i += find_len;
        }
        else
        {
            write(1, &buffer[i], 1);
            i++;
        }
    }
    free(buffer);
    return 0;
}
