#include "get_next_line.h"

/**
 * ft_strchr: Bir dizge içinde belirli bir karakteri (c) arar.
 * Bulursa o karakterin adresini, bulamazsa NULL döndürür.
 */
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

/**
 * ft_memcpy: Kaynak (src) bellek bölgesinden hedef (dest) bölgesine n byte kopyalar.
 */
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

/**
 * ft_strlen: Bir dizgenin uzunluğunu (karakter sayısını) hesaplar.
 */
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

/**
 * str_append_mem: Mevcut bir dizgenin (s1) sonuna belirli bir boyuttaki (size2)
 * başka bir veriyi (s2) ekler. Belleği yeniden ayırır.
 */
int str_append_mem(char **s1, char *s2, size_t size2)
{
    size_t size1 = ft_strlen(*s1);
    char *tmp = malloc(size2 + size1 + 1);
    if (!tmp)
        return 0;
    ft_memcpy(tmp, *s1, size1);
    ft_memcpy(tmp + size1, s2, size2);
    tmp[size1 + size2] = 0;
    free(*s1);
    *s1 = tmp;
    return 1;
}

/**
 * str_append_str: Bir dizgenin sonuna başka bir dizgeyi ekler.
 */
int str_append_str(char **s1, char *s2)
{
    return str_append_mem(s1, s2, ft_strlen(s2));
}

/**
 * ft_memmove: Bellek bölgeleri üst üste biniyorsa (overlap) bile güvenli kopyalama yapar.
 */
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

/**
 * get_next_line: Dosya tanımlayıcıdan bir sonraki satırı okur.
 */
char *get_next_line(int fd)
{
    // Statik tampon (b): Fonksiyon çağrıları arasında veriyi korur
    static char b[BUFFER_SIZE + 1] = "";
    char *ret = malloc(1); // Boş bir başlangıç dizgesi oluştur
    if (!ret)
        return (NULL);
    ret[0] = 0;

    // Mevcut tamponda satır sonu (\n) olup olmadığını kontrol et
    char *tmp = ft_strchr(b, '\n');

    // Satır sonu bulana kadar okumaya devam et
    while (!tmp)
    {
        // Mevcut tamponu sonuca ekle
        if (!str_append_str(&ret, b))
        {
            free(ret);
            return NULL;
        }

        // Yeni veri oku
        int read_ret = read(fd, b, BUFFER_SIZE);
        if (read_ret == -1) // Okuma hatası
        {
            free(ret);
            return NULL;
        }
        if(read_ret == 0) // Dosya sonu (EOF)
        {
            b[0] = 0;
            if(*ret) // Eğer elimizde okunan bir veri varsa döndür
                return (ret);
            free(ret);
            return NULL;
        }
        b[read_ret] = 0; // Okunan veriyi sonlandır
        tmp = ft_strchr(b, '\n');
    }

    // Satır sonu bulundu, o noktaya kadar olan kısmı sonuca ekle
    if (!str_append_mem(&ret, b, tmp - b  + 1))
    {
        free(ret);
        return (NULL);
    }

    // Tamponun geri kalanını (bir sonraki satır için) başa taşı
    ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);

    return ret;
}
