//2.写一程序，实现从一个 DWORD 表示的秒数（从 2000 年 1 月 1 日 0 时 0 分 0 秒为起点计的秒数）
//转化为形如 YYYY 年 MM 月 DD 日 HH 时 mm 分 SS 秒表示的时间（即把秒数转化为年月日时分秒）

#include <stdio.h>

typedef struct Storage_date
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int time;
    unsigned int minute;
    unsigned int second;
};

int year_conversion(unsigned int a,unsigned int first_year){
    //平年=31536000，闰年=31622400
    struct Storage_date end={
        end.year,
    };
    while (1)
    {
        while (is_leap_year(first_year))
        {
            if (a > 31622400)
            {
                a -= 31622400;
                first_year++;
                break;
            }else
                end.year = first_year;
                return a;
        }
        
        while (!is_leap_year(first_year))
        {
            if (a > 31536000)
            {
                a -= 31536000;
                first_year++;
                break;
            }else
                end.year = first_year;
                return a;
        }
    }
    
}

int month_conversion(unsigned int a,unsigned int end_year){
    if(a < 86400){
        return 1;
    }
    int days_in_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int is_day = a / 86400;
    int now_day;
    if (is_leap_year(end_year));
    else{
        days_in_month[1] = 28;
    }
    struct Storage_date end={
        end.month = 1,
    };
    for (end.month = 1; end.month < 13; end.minute++)
    {
        now_day = is_day - days_in_month[end.month];
    }
    
}

int is_leap_year(int year){
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }else
    return 0;
}

int main(int argc, char const *argv[])
{
    unsigned int total;
    struct Storage_date first={
        first.year = 2000,
        first.month = 01,
        first.day = 01,
        first.time =0,
        first.minute = 0,
        first.second =0
    };

    total = year_conversion(total, first.year);
    int month = month_conversion(total,first.year);

    return 0;
}
