#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

/**
 * City: Şehir koordinatlarını tutan yapı.
 */
typedef struct {
	float x, y;
} City;

City cities[12]; // En fazla 11 şehir geleceği belirtilmiş
int n = 0;
float dist_matrix[12][12];
float min_path = FLT_MAX; // Başlangıçta minimum yolu sonsuz (en büyük değer) kabul et

/**
 * get_dist: İki şehir arasındaki Öklid mesafesini hesaplar.
 */
float get_dist(City a, City b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/**
 * solve: Özyinelemeli (Recursive) olarak tüm şehirleri ziyaret eden yolları dener.
 * @curr: O an bulunulan şehrin indeksi
 * @mask: Hangi şehirlerin ziyaret edildiğini tutan bit maskesi (1 << n - 1 tüm şehirler)
 * @curr_dist: Şu ana kadar kat edilen toplam mesafe
 * @count: Toplam kaç şehir ziyaret edildi
 */
void solve(int curr, int mask, float curr_dist, int count) {
	// Eğer tüm şehirler ziyaret edildiyse
	if (count == n) {
		// Son şehirden başlangıç (0.) şehre geri dön (Closed Curve)
		float total = curr_dist + dist_matrix[curr][0];
		if (total < min_path)
			min_path = total;
		return;
	}

	// Budama: Eğer şu anki yol zaten bulduğumuz en kısadan uzunsa bu daldan devam etme
	if (curr_dist >= min_path)
		return;

	// Diğer şehirlere gitmeyi dene
	for (int i = 0; i < n; i++) {
		// Eğer i. şehir henüz ziyaret edilmediyse (mask içindeki i. bit 0 ise)
		if (!(mask & (1 << i))) {
			solve(i, mask | (1 << i), curr_dist + dist_matrix[curr][i], count + 1);
		}
	}
}

int main() {
	// 1. Girdi okuma: %f, %f formatında koordinatları al
	while (fscanf(stdin, "%f, %f", &cities[n].x, &cities[n].y) == 2) {
		n++;
	}

	// Eğer hiç şehir yoksa bir şey yazma veya 0.00 yaz
	if (n == 0)
		return (0);
	
	// Sadece bir şehir varsa yol uzunluğu 0'dır
	if (n == 1) {
		fprintf(stdout, "0.00\n");
		return (0);
	}

	// 2. Mesafe matrisini önceden hesapla (her seferinde sqrtf çağırmamak için)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dist_matrix[i][j] = get_dist(cities[i], cities[j]);
		}
	}

	// 3. Başlangıç (0.) şehirden çözümü başlat
	// solve(başlangıç_şehri, maske, mesafe, ziyaret_sayısı)
	solve(0, (1 << 0), 0.0, 1);

	// 4. Sonucu %.2f formatında yazdır
	fprintf(stdout, "%.2f\n", min_path);

	return (0);
}
