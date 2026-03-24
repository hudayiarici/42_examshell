#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * ft_strlen: Dizgenin uzunluğunu hesaplar.
 */
int ft_strlen(char *s)
{
	int i = 0;
	if (!s) return 0;
	while (s[i])
		i++;
	return (i);
}

/**
 * sort_str: Dizgeyi alfabetik sıraya koyar (Bubble Sort).
 * Permütasyonların alfabetik sırada çıkması için başlangıç dizgesinin sıralı olması gerekir.
 */
void sort_str(char *s, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (s[i] > s[j])
			{
				char tmp = s[i];
				s[i] = s[j];
				s[j] = tmp;
			}
		}
	}
}

/**
 * solve: Özyinelemeli olarak tüm permütasyonları oluşturur.
 * @s: Sıralanmış ana dizge.
 * @res: O an oluşturulan permütasyon.
 * @used: Karakterlerin kullanılıp kullanılmadığını tutan dizi.
 * @index: Şu an doldurulan pozisyon.
 * @len: Toplam karakter sayısı.
 */
void solve(char *s, char *res, int *used, int index, int len)
{
	// Eğer tüm pozisyonlar dolduysa (Base Case)
	if (index == len)
	{
		res[len] = '\0';
		//puts izin verilen bir fonksiyon
		puts(res);
		return;
	}

	// Her bir pozisyon için tüm karakterleri sırayla dene
	for (int i = 0; i < len; i++)
	{
		// Eğer karakter daha önce bu dalda kullanılmadıysa
		if (!used[i])
		{
			used[i] = 1;         // Kullanıldı olarak işaretle
			res[index] = s[i];   // Karakteri sonuca ekle
			solve(s, res, used, index + 1, len); // Bir sonraki pozisyona geç
			used[i] = 0;         // Backtrack: İşareti kaldır
		}
	}
}

int main(int argc, char **argv)
{
	// Program tam olarak 1 argüman almalıdır
	if (argc == 2)
	{
		char *s = argv[1];
		int len = ft_strlen(s);

		if (len == 0)
		{
			write(1, "\n", 1);
			return (0);
		}

		// 1. Önce girdi dizgesini alfabetik olarak sırala (alfabetik çıktı için şart)
		sort_str(s, len);

		// 2. Bellek ayır: calloc ile used dizisini 0 ile başlatırız
		char *res = (char *)malloc(len + 1);
		int *used = (int *)calloc(len, sizeof(int));

		if (res && used)
			solve(s, res, used, 0, len);

		// 3. Belleği serbest bırak
		if (res) free(res);
		if (used) free(used);
		return(0);
	}
	return (1);
}
