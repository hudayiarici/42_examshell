#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <float.h> // FLT_MAX için bu kütüphaneyi ekleyin
// -lm bayrağı ile derlemeyi unutmayın!

/* Gezgin Satıcı Problemini çözmek için bu yaklaşım,
küçük boyutlarda (N <= 11) oldukça iyi çalışan bir kaba kuvvet (brute-force)
permütasyon üretme stratejisinden yararlanır.
Burada kullanılan özyinelemeli geri izleme fonksiyonu (generate_perms()),
başka bir Rank 03 sınav sorusu olan "permutations" (yine depomda mevcut) 
çözümümde kullandığımla benzer bir yapıdadır. Bunları yan yana incelemenizi öneririm.

Önce şehir indislerini (0'dan N-1'e) içeren bir dizi ilklendirirsiniz.
Ardından, bu şehir indislerinin mümkün olan her sıralamasını (permütasyonunu) 
sistematik olarak üretmek için özyinelemeli bir geri izleme fonksiyonu 
(Heap algoritması varyantı) kullanırsınız.
Her tam permütasyon için, ardışık şehirler arasındaki mesafeleri toplayarak 
ve son şehirden ilk şehre olan mesafeyi ekleyerek toplam tur uzunluğunu hesaplarsınız;
daha kısa bir yol bulunursa best_distance değişkenini güncellersiniz. */

// iki nokta arasındaki mesafeyi hesapla
float    distance(float a[2], float b[2])
{
    return sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}


/* FONKSİYONLARINIZ BURADA BAŞLAR */


// Verilen bir yolun (şehir indislerinin permütasyonu) toplam mesafesini hesaplar.
// array: Şehir koordinatlarının ana dizisi.
// perm: Mevcut yol için şehir indislerinin sırasını temsil eden bir dizi.
// size: Toplam şehir sayısı.
// Toplam mesafeyi float olarak döndürür.
float calc_total_distance(float (*array)[2], int *perm, int size)
{
	int city_index_current;
	int city_index_next;
	int i;
	float actual_distance = 0.0f;
	// Permütasyondaki ardışık şehirler arasındaki mesafeleri topla.
	for (i = 0; i < size - 1; i++)
	{
		city_index_current = perm[i];
		city_index_next = perm[i + 1];
		actual_distance += distance(array[city_index_current], array[city_index_next]);
	}
	// Döngüyü kapatmak için son şehirden ilk şehre olan mesafeyi ekle.
	city_index_current = perm[i]; // i şu an döngüden dolayı (size - 1) değerindedir
	city_index_next = perm[0]; // Permütasyondaki ilk şehir
	actual_distance += distance(array[city_index_current], array[city_index_next]);
	return (actual_distance);
}	

// Ana fonksiyon: permütasyon üretmek için standart geri izleme (backtracking) yöntemini kullanır
// 'mutable_array'in tüm permütasyonlarını üretir ve yol uzunluklarını hesaplar.
// array: Şehir koordinatlarının ana dizisi.
// mutable_array: Elemanları permüte edilen dizi (içeriği özyineleme sırasında değişir).
// size: Dizideki/permütasyondaki eleman sayısı.
// mutable_index_current: Mevcut permütasyon üretme adımı için başlangıç indisi (mevcut derinlik).
// best_distance: Şimdiye kadar bulunan minimum mesafeyi saklayan bir float değişkenine işaretçi.
void generate_perms(float (*array)[2], int *mutable_array, int size,
					int mutable_index_current, float *best_distance)
{
	// Temel durum: Eğer mutable_index_current size'a ulaşırsa, 
	// tam bir permütasyon oluşmuş demektir.
	if (mutable_index_current == size)
	{
		float actual_distance = calc_total_distance(array, mutable_array, size);
		if (actual_distance < *best_distance)
			*best_distance = actual_distance;
		return ;
	}
	// Özyinelemeli adım: current_index için eleman seçmek üzere 
	// mutable_index_current'tan size-1'e kadar dön.
	for (int i = mutable_index_current; i < size; i++)
	{
		// Mevcut seviye için yeni bir düzenleme oluşturmak üzere elemanları takas et.
		int temp = mutable_array[mutable_index_current];
		mutable_array[mutable_index_current] = mutable_array[i];
		mutable_array[i] = temp;
		// Dizinin geri kalanı için permütasyon üretmek üzere özyineleme yap (sonraki seviye)
		generate_perms(array, mutable_array, size, mutable_index_current + 1,
						best_distance);
		// Geri İzleme: Diziyi özyinelemeli çağrıdan önceki durumuna döndürmek için tekrar takas et,
        // böylece mevcut seviye için diğer permütasyonların doğru şekilde üretilmesini sağla.
		temp = mutable_array[mutable_index_current];
		mutable_array[mutable_index_current] = mutable_array[i];
		mutable_array[i] = temp;
	}
}

// Gezgin Satıcı Problemini çözmek için ana fonksiyon.
// (iskelet fonksiyon sağlanmıştır, boşlukları doldurmanız gerekir.)
// array: Şehir koordinatlarını içeren bir diziye işaretçi ([x, y] çiftleri).
// size: Dizideki şehir sayısı.
// Tüm şehirleri ziyaret eden mümkün olan en kısa kapalı yolun uzunluğunu döndürür.
float tsp(float (*array)[2], ssize_t size)
{
    float best_distance;
    
	
	// ... KODUNUZ BURADA BAŞLAR 


	best_distance = FLT_MAX; // en iyi mesafeyi çok büyük bir sayıya ilklendir 
	
	 // 0 veya 1 şehir için uç durumları yönet: yol uzunluğu 0'dır.
    if (size <= 1) {
        return 0.0f;
    }
	// 0'dan size - 1'e kadar uzanan bir tam sayı dizisi oluştur.
    // Bu dizi şehirlerin indislerini tutacak ve permüte edilecektir.
	int *mutable_array = malloc(sizeof(int) * size);
	if (!mutable_array)
		return FLT_MAX; // hata belirtmek için çok büyük bir sayı döndür
	
	for (int i = 0; i < size; i++)
		mutable_array[i] = i;

	// Permütasyonları üret ve en iyi mesafeyi bul.
    // Optimizasyon: Permütasyon üretmeye 1. indisten başlayın (mutable_index_current = 1)
    // ve mutable_array[0]'daki şehri sabit tutun.
    // Bu, kapalı bir döngü için permütasyon sayısını N!'den (N-1)!'e düşürür.
	int mutable_index_start = 1;
	generate_perms(array, mutable_array, size, mutable_index_start, &best_distance);
	free(mutable_array);


	// ... KODUNUZ BİTER 


    return (best_distance);
}


/* FONKSİYONLARINIZ BURADA BİTER */


// Bir dosyadaki satır (şehir) sayısını belirleyen fonksiyon.
// file: FILE akışına bir işaretçi.
// Satır sayısını döndürür veya hata durumunda -1 döndürür.
ssize_t    file_size(FILE *file)
{
    char    *buffer = NULL;
    size_t    n = 0;
    ssize_t ret;

    errno = 0;
	// Satırları saymak için tek tek oku.
    for (ret = 0; getline(&buffer, &n, file) != -1; ret++);

    free(buffer); // getline tarafından ayrılan belleği serbest bırak
    if (errno || fseek(file, 0, SEEK_SET))
        return -1;
    return ret;
}

// Şehir koordinatlarını dosyadan bir diziye okuyan fonksiyon.
// array: Koordinatların saklanacağı float[2] dizisine işaretçi.
// 		Her "satırın" tam olarak 2 sütunu olduğu 2D bir dizidir 
// 		(örn. float coordinates[N][2]). 
// 		Her float[2] tek bir şehrin [x, y] koordinatlarını temsil eder.
// file: Okunacak FILE akışına işaretçi.
// Başarı durumunda 0, hata durumunda -1 döndürür.
int        retrieve_file(float (*array)[2], FILE *file)
{
    int tmp;
	// Dosya boyunca döngü kur, her satırda iki float oku.
    for (size_t i = 0; (tmp = fscanf(file, "%f, %f\n", array[i] + 0, array[i] + 1)) != EOF; i++)
        if (tmp != 2) // Eğer fscanf tam olarak iki float okumadıysa, bu bir hatadır.
        {
            errno = EINVAL;
            return -1;
        }
	// Diğer dosya okuma hatalarını kontrol et (örn. G/Ç hatası).
    if (ferror(file))
        return -1;
    return 0;
}

int        main(int ac, char **av)
{
    char *filename = "stdin"; // Varsayılan girdi standart girdidir.
    FILE *file = stdin; // Varsayılan dosya işaretçisi stdin'dir.
	// Komut satırı argümanı olarak bir dosya adı verilmişse, o dosyayı aç.
    if (ac > 1)
    {
        filename = av[1];
        file = fopen(filename, "r");
    }
	// Dosyanın başarıyla açılıp açılmadığını kontrol et.
    if (!file)
    {
        fprintf(stderr, "Error opening %s: %m\n", filename);
        return 1;
    }
	// Dosyadaki şehir (satır) sayısını belirle.
    ssize_t size = file_size(file);
    if (size == -1)
    {
        fprintf(stderr, "Error reading %s: %m\n", filename);
        fclose(file);
        return 1;
    }
	// Şehir koordinatlarını saklamak için diziye bellek ayır.
    float (*array)[2] = calloc(size, sizeof (float [2]));
    if (!array)
    {
        fprintf(stderr, "Error: %m\n");
        fclose(file);
        return 1;
    }
	// Şehir koordinatlarını dosyadan diziye al.
    if (retrieve_file(array, file) == -1)
    {
        fprintf(stderr, "Error reading %s: %m\n", av[1]);
        fclose(file);
        free(array);
        return 1;
    }
	// Eğer dosya açılmışsa (yani stdin değilse) dosyayı kapat.
    if (ac > 1)
        fclose(file);

    // En kısa yol uzunluğunu hesapla ve iki ondalık basamakla biçimlendirilmiş olarak yazdır.
    printf("%.2f\n", tsp(array,	 size));
    free(array);
    return (0);
}
