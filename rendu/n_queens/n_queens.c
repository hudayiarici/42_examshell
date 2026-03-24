#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * abs_val: Mutlak değer hesaplar (çapraz kontrolü için gerekli).
 */
int abs_val(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

/**
 * is_safe: Bir veziri belirli bir sütun ve satıra koymanın güvenli olup olmadığını kontrol eder.
 * @board: Vezirlerin satır indekslerini tutan dizi. board[sütun] = satır
 * @col: Şu an yerleştirilmek istenen sütun.
 * @row: Şu an yerleştirilmek istenen satır.
 * @n: Tahtanın boyutu.
 */
int is_safe(int *board, int col, int row)
{
	for (int i = 0; i < col; i++)
	{
		// 1. Aynı satırda vezir var mı kontrolü (board[i] == row)
		// 2. Aynı çaprazda vezir var mı kontrolü (satır farkı == sütun farkı mutlak değerce)
		if (board[i] == row || abs_val(board[i] - row) == abs_val(i - col))
			return (0);
	}
	return (1);
}

/**
 * solve: Özyinelemeli (Recursive) olarak vezirleri sütun sütun yerleştirir.
 */
void solve(int *board, int col, int n)
{
	// Tüm sütunlara vezir yerleştirdiysek (Base Case), çözümü yazdır
	if (col == n)
	{
		for (int i = 0; i < n; i++)
		{
			// fprintf(stdout, ...) izin verilen bir fonksiyon
			fprintf(stdout, "%d", board[i]);
			if (i < n - 1)
				fprintf(stdout, " ");
		}
		fprintf(stdout, "\n");
		return;
	}

	// Bu sütundaki her bir satırı (0'dan n-1'e kadar) deniyoruz
	for (int i = 0; i < n; i++)
	{
		if (is_safe(board, col, i))
		{
			board[col] = i;      // Veziri bu satıra yerleştir
			solve(board, col + 1, n); // Bir sonraki sütuna geç
			// Backtrack: board[col] zaten döngünün bir sonraki adımında güncelleneceği için özel bir silme gerekmez.
		}
	}
}

int main(int argc, char **argv)
{
	// Program tam olarak 1 argüman almalıdır
	if (argc == 2)
	{
		int n = atoi(argv[1]);
		
		// Negatif veya 0 değerler için bir şey yazdırmıyoruz
		if (n <= 0)
			return (0);
		
		// Vezirlerin konumlarını tutacak diziyi calloc ile oluşturuyoruz
		int *board = (int *)calloc(n, sizeof(int));
		if (!board)
			return (1);

		// Çözümleri bulmak için fonksiyonu 0. sütundan başlatıyoruz
		solve(board, 0, n);

		free(board); // Ayrılan belleği serbest bırak
	}
	return (0);
}
