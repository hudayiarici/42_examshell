#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *board;         // board[col] = col sütunundaki vezirin satır konumu
int board_size;     // tahta boyutu (n)

// Mevcut çözümü yazdır
void print_solution(void)
{
    int i;
    
    for (i = 0; i < board_size; i++)
    {
        // i sütunundaki vezirin satır konumunu yazdır
        fprintf(stdout, "%d", board[i]);
        
        // Sayılar arasına boşluk ekle, sonuncu hariç
        if (i < board_size - 1)
            fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
}

// Orijinal abs() fonksiyonu yasak olduğu için kendi abs() fonksiyonumuz
int ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return n;
}

// Veziri (satır, sütun) konumuna yerleştirmenin güvenli olup olmadığını kontrol et
int is_safe(int row, int col)
{
	// Daha önce yerleştirilmiş tüm vezirleri kontrol et (0'dan sütun-1'e kadar)
	// i: sütun yineleyici (iterator)
	for (int i = 0; i < col; i++)
	{
		if (board[i] == row) // vezirlerin aynı satırda olup olmadığını kontrol et
			return 0;
		
		// Çapraz saldırıları kontrol et
        // Eğer satır farkı sütun farkına eşitse, aynı çapraz üzerindedirler
		if (ft_abs(board[i] - row) == ft_abs(i - col))
			return 0;
	}
	return 1;
}

void solve(int col)
{
	// temel durum kontrolü: eğer tüm sütunlara vezir yerleştirdiysek
	if (col == board_size)
	{
		print_solution();
		return ;
	}
	// Veziri mevcut sütunun her satırına yerleştirmeyi dene
	for (int row = 0; row < board_size; row++)
	{
		// Veziri mevcut sütunun her satırına yerleştirmeyi dene
		if (is_safe(row, col))
		{
			// Veziri bu konuma yerleştir
			board[col] = row;
			// Bir sonraki sütun için özyinelemeli olarak çöz
			solve(col + 1);
			// Geri İzleme (veziri kaldır) - bir sonraki yinelemede veya fonksiyon döndüğünde 
            // board[col] üzerine yazdığımız için örtük (implicit) olarak gerçekleşir
		}
	}
}

int main(int ac, char **av)
{
	// hatalı argüman sayısını yönet
	if (ac != 2)
	{
		write(1, "\n", 1);
		return 0;
	}
	int n = atoi(av[1]);
	// geçersiz girdileri yönet: negatif sayılar, çözülemeyen boyutlar
	if (n <= 3)
	{
		write(1, "\n", 1);
		return 0;
	}
	board_size = n; // global değişkeni ayarla
	board = malloc(sizeof(int) * board_size);
	if (!board)
		return 1;
	// Çözmeye 0. sütundan başla
	solve(0);
	free(board);
	return 0;
}
