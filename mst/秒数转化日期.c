#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct date
{
    int year, month, day;
    int hour, minute, second;
};

int leap[]   = {31,29,31,30,31,30,31,31,30,31,30,31};
int common[] = {31,28,31,30,31,30,31,31,30,31,30,31};

bool is_leap(int year)
{
    return ((year%4==0 && year%100!=0) || (year%400==0));
}

int main(int argc, char const *argv[])
{
    int32_t sec;
    scanf("%d", &sec); // 100000000秒

    // 500天
    int days = sec/(3600*24);

    struct date result;
    bzero(&result, sizeof(result));

    // 计算年份
    int cur_year = 2000;
    while(1)
    {
        int d = is_leap(cur_year) ? 366 : 365;

        if(days > d)
        {
            cur_year++;
            days -= d;
        }
        else
            break;
    }
    result.year = cur_year;

    // 129天
    int cur_mon = 1;
    while(1)
    {
        int m = is_leap(cur_year) ? leap[cur_mon] : common[cur_mon];
        if(days > m)
        {
            cur_mon++;
            days -= m;
        }
        else
            break;
    }
    result.month = cur_mon;

    // 16天
    result.day = days+1;

    // 计算剩下的不足一天的秒数 --> 时分秒
    int remind_sec = sec % (3600*24);

    // 4000秒
    result.hour = remind_sec / 3600;
    result.minute = (remind_sec-result.hour*3600) / 60;
    result.second = remind_sec % 60;

    printf("经过%d秒之后是：%d年%d月%d日，", sec, result.year,
                                                  result.month,
                                                result.day);
    printf("%d时%d分%d秒\n", result.hour,
                             result.minute,
                             result.second);

    return 0;
}
