#include <unistd.h>
#include <stdio.h>

/**
 * solve: Özyinelemeli (Recursive) olarak tüm parantez kombinasyonlarını deneyen fonksiyon.
 * @s: İşlenen dizge (string)
 * @idx: Şu anki karakterin indeksi
 * @l: Silinmesi gereken fazla açılan parantez '(' sayısı
 * @r: Silinmesi gereken fazla kapanan parantez ')' sayısı
 * @b: Mevcut parantez dengesi (balance). b < 0 olursa yapı bozulmuş demektir.
 */
void solve(char *s, int idx, int l, int r, int b)
{
	// Eğer denge negatifse, yani kapanan parantez sayısı açılanı geçtiyse bu dalı iptal et (Pruning)
	if (b < 0)
		return;

	// Dizinin sonuna ulaştığımızda (Base Case)
	if (s[idx] == '\0')
	{
		// Eğer silinmesi gereken tüm adetler kullanılmışsa ve denge tam 0 ise geçerli bir çözümdür
		if (l == 0 && r == 0 && b == 0)
			puts(s); // Sonucu yazdır (puts otomatik olarak \n ekler)
		return;
	}

	char original = s[idx]; // Karakteri yedekle (Backtracking için önemli)

	if (original == '(')
	{
		// Seçenek 1: Bu '(' karakterini SİL (boşluğa çevir)
		// Sadece silmemiz gereken sol parantez hakkımız (l > 0) varsa bunu yapabiliriz
		if (l > 0)
		{
			s[idx] = ' '; // Boşluk yap
			solve(s, idx + 1, l - 1, r, b); // l'yi bir azaltarak devam et
			s[idx] = original; // Geri dönünce karakteri eski haline getir (Backtrack)
		}
		// Seçenek 2: Bu '(' karakterini TUT
		// Karakteri tuttuğumuz için denge (b) 1 artar
		solve(s, idx + 1, l, r, b + 1);
	}
	else if (original == ')')
	{
		// Seçenek 1: Bu ')' karakterini SİL (boşluğa çevir)
		// Sadece silmemiz gereken sağ parantez hakkımız (r > 0) varsa bunu yapabiliriz
		if (r > 0)
		{
			s[idx] = ' '; // Boşluk yap
			solve(s, idx + 1, l, r - 1, b); // r'yi bir azaltarak devam et
			s[idx] = original; // Geri dönünce karakteri eski haline getir (Backtrack)
		}
		// Seçenek 2: Bu ')' karakterini TUT
		// Karakteri tuttuğumuz için denge (b) 1 azalır
		solve(s, idx + 1, l, r, b - 1);
	}
	else
	{
		// Eğer parantez dışında bir karakter varsa (soruda olmayacağı söylense de güvenli kod için)
		// Hiçbir şeyi değiştirmeden bir sonraki karaktere geç
		solve(s, idx + 1, l, r, b);
	}
}

int main(int argc, char **argv)
{
	// Programın tam olarak 1 argüman alması gerekir
	if (argc == 2)
	{
		int l = 0; // Toplamda kaç tane '(' silmemiz gerektiğini tutacak
		int r = 0; // Toplamda kaç tane ')' silmemiz gerektiğini tutacak
		char *s = argv[1];

		// 1. Adım: Minimum silme miktarını (l ve r) hesapla
		for (int i = 0; s[i]; i++)
		{
			if (s[i] == '(')
				l++; // Açılan parantez gördükçe sayacı artır
			else if (s[i] == ')')
			{
				if (l > 0)
					l--; // Eğer elimizde eşleşecek bir '(' varsa dengeyi kur (l'yi azalt)
				else
					r++; // Eşleşecek '(' yoksa bu ')' kesinlikle fazladır, r'yi artır
			}
		}

		// 2. Adım: Özyinelemeli çözümü başlat
		// solve(dizge, başlangıç_indeksi, silinecek_l, silinecek_r, başlangıç_dengesi)
		solve(s, 0, l, r, 0);
		return (0);
	}
	// Argüman yanlışsa hata kodu döndür
	return (1);
}
