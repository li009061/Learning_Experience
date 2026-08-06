#include <math.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

// 数据规模: 100万
#define SCALE  1000*1000

// 查找次数统计
static int count;

void swap(unsigned *a, unsigned *b)
{
	unsigned tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(unsigned data[], int len)
{
	if(len <= 1)
		return 0;

	int i = 0;
	int j = len-1;
	while(i < j)
	{
		while(i<j && data[i]<=data[j]) j--;
		swap(&data[i], &data[j]);

		while(i<j && data[i]<=data[j]) i++;
		swap(&data[i], &data[j]);
	}

	return i;
}

void quick_sort(unsigned data[], int len)
{
	if(len <= 1)
		return;

	int pivot = partition(data, len);

	quick_sort(data, pivot);
	quick_sort(data+pivot+1, len-pivot-1);
}


void store(unsigned *data, const char *file)
{
    FILE *fp = fopen(file, "w");
    for(int i=0; i<SCALE; i++)
        fprintf(fp, "%06d.%u\n", i, data[i]); // 数据规模：100万（6位数）

    fclose(fp);
}

int main(int argc, char const *argv[])
{
    // 1. 产生SCALE个随机整数序列
    unsigned *data = calloc(SCALE, sizeof(unsigned));

    srand(time(NULL));
    for(int i=0; i<SCALE; i++)
        data[i] = rand()%((int)pow(10, rand()%8+5));

    // 2. 排序
    store(data, "chaos.txt");   // 排序前，无序序列存档chaos.txt
    quick_sort(data, SCALE);
    store(data, "order.txt");   // 排序后，有序序列存档order.txt

    // 3. 进行二分查找
    unsigned n;
    printf("请输入你要查找的正整数:\n");
    while(1)
    {
        scanf("%u", &n);

        int low, high, mid;
        low = 0, high = SCALE-1;

        bool found = false;
        while(low <= high)
        {
            count++;
            mid = (low+high)/2;

            if(n == data[mid])
            {
                printf("你要找的数据在第%d行", mid);
                found = true;
                break;
            }

            if(n < data[mid])
                high = mid - 1;
            else
                low = mid + 1;
        } 

        if(!found)
            printf("找不到你要的数据");

        printf("【找了%d次】\n", count);
        count=0;
    }

    return 0;
}