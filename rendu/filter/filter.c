#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/**
 * ft_strcmp: s1 dizgesinin s2 dizgesi ile (s2'nin sonuna kadar) eşleşip eşleşmediğini kontrol eder.
 */
int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    // Eğer s2 dizgesinin sonuna kadar eşleşme sağlandıysa (s2[i] == '\0') 1 döndürür.
    return (s2[i] == '\0');
}

int main(int argc, char **argv)
{
    // Program tam olarak 1 argüman almalıdır.
    if (argc != 2 || strlen(argv[1]) == 0)
        return 1;

    char *buffer = NULL;
    char c;
    int len = 0;
    int i = 0;
    int find_len = strlen(argv[1]);

    // Başlangıç için bellek ayır.
    buffer = malloc(find_len);
    if(!buffer)
        return 1;

    // 1. Adım: Standart girdiden (stdin - fd 0) tüm karakterleri oku ve buffer'a kaydet.
    // Not: Problemde rastgele buffer boyutlarıyla test edileceği belirtilmiş.
    // Bu basit çözüm tüm girdiyi önce belleğe alır.
    while (read(0, &c, 1) > 0)
    {
        char *tmp = realloc(buffer, len + 2);
        if (!tmp)
        {
            free(buffer);
            return 1;
        }
        buffer = tmp;
        buffer[len] = c;
        len++;
        buffer[len] = '\0';
    }

    // 2. Adım: Buffer içindeki her bir pozisyonu kontrol et.
    while (i < len)
    {
        // Eğer o anki pozisyonda aranan kelime (argv[1]) başlıyorsa:
        if (ft_strcmp(&buffer[i], argv[1]))
        {
            // Kelimenin uzunluğu kadar '*' yazdır.
            int j = 0;
            while (j < find_len)
            {
                write(1, "*", 1);
                j++;
            }
            // İndeksi kelime uzunluğu kadar kaydır.
            i += find_len;
        }
        else
        {
            // Eşleşme yoksa o anki karakteri aynen yazdır.
            write(1, &buffer[i], 1);
            i++;
        }
    }

    // Belleği serbest bırak.
    free(buffer);
    return 0;
}
