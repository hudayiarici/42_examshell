#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * find_subsets: Özyinelemeli (Recursive) olarak tüm alt kümeleri bulan fonksiyon.
 */
void find_subsets(int target, int *nums, int count, int index, int current_sum, int *subset, int subset_len)
{
	if (index == count)
	{
		if (current_sum == target)
		{
			for (int i = 0; i < subset_len; i++)
			{
				fprintf(stdout, "%d", subset[i]);
				if (i < subset_len - 1)
					fprintf(stdout, " ");
			}
			fprintf(stdout, "\n");
		}
		return;
	}

	// Seçenek 1: Bu elemanı alt kümeye DAHİL ETME (Exclude)
	find_subsets(target, nums, count, index + 1, current_sum, subset, subset_len);

	// Seçenek 2: Bu elemanı alt kümeye DAHİL ET (Include)
	subset[subset_len] = nums[index];
	find_subsets(target, nums, count, index + 1, current_sum + nums[index], subset, subset_len + 1);
}

int main(int argc, char **argv)
{
	// Test beklentisi: Hedef sayıdan sonra en az bir küme elemanı olmalı (argc >= 3)
	// Eğer sadece ./powerset veya ./powerset n verilirse hata (1) dönmeli.
	if (argc < 3)
	{
		return (1);
	}

	int target = atoi(argv[1]);
	int count = argc - 2;
	
	// Girdi elemanlarını tutacak diziyi oluştur
	int *nums = (int *)malloc(sizeof(int) * count);
	if (!nums)
		return (1);

	for (int i = 0; i < count; i++)
		nums[i] = atoi(argv[i + 2]);

	// Alt kümeyi tutacak geçici dizi
	int *subset = (int *)malloc(sizeof(int) * count);
	if (!subset)
	{
		free(nums);
		return (1);
	}

	// Çözümü başlat
	find_subsets(target, nums, count, 0, 0, subset, 0);

	// Belleği serbest bırak
	free(nums);
	free(subset);
	
	return (0);
}
