#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

char *ft_strchr(char *s, int c)
{
    if (!s)
        return (NULL);
    int i = 0;
    while (s[i] && s[i] != (char)c)
        i++;
    if (s[i] == (char)c)
        return s + i;
    return NULL;
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
    if (!src || !dest)
        return (NULL);
    size_t  i = 0;
    while (i < n)
    {
        ((char *)dest)[i] = ((char *)src)[i];
        i++;
    }
    return dest;
}

size_t ft_strlen(char *s)
{
    if (!s)
        return 0;
    size_t ret = 0;
    while (*s)
    {
        s++;
        ret++;
    }
    return (ret);
}
int str_append_mem(char **s1, char *s2, size_t size2)
{
    size_t size1 = ft_strlen(*s1);
    char *tmp = malloc(size2 + size1 +1);
    if (!tmp)
        return 0;
    ft_memcpy(tmp, *s1, size1);
    ft_memcpy(tmp + size1, s2, size2);
    tmp[size1 + size2] = 0;
    free(*s1);
    *s1 = tmp;
    return 1;
}
int str_append_str(char **s1, char *s2)
{
    return str_append_mem(s1, s2, ft_strlen(s2));
}
void *ft_memmove(void *dest, const void *src, size_t n)
{
    if (dest == src)
        return dest;
    if (dest > src)
    {
        size_t i = n;
        while (i-- > 0)
            ((char *)dest)[i] = ((char *)src)[i];
    }
    else
    {
        size_t i = 0;
        while (i < n)
        {
            ((char *)dest)[i] = ((char *)src)[i];
            i++;
        }
    }
    return dest;
}

char *get_next_line(int fd)
{
    static char b[BUFFER_SIZE + 1] = "";
    char *ret = malloc(1);
    if (!ret)
        return (NULL);
    ret[0] = 0;
    char *tmp = ft_strchr(b, '\n');
    while (!tmp)
    {
        if (!str_append_str(&ret, b))
        {
            free(ret);
            return NULL;
        }
        int read_ret = read(fd, b, BUFFER_SIZE);
        if (read_ret == -1)
        {
            free(ret);
            return NULL;
        }
        if(read_ret == 0)
        {
            b[0] = 0;
            if(*ret)
                return (ret);
            free(ret);
            return NULL;
        }
        b[read_ret] = 0;
        tmp = ft_strchr(b, '\n');
    }
    if (!str_append_mem(&ret, b, tmp - b  + 1))
    {
        free(ret);
        return (NULL);
    }
    ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);
    return ret;
}