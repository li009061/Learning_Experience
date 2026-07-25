#include <stdio.h>

// 定义日期时间结构体
typedef struct Storage_date {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} Storage_date;

// 判断闰年
int is_leap_year(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    else
        return 0;
}

/**
 * 从总秒数中减去年份，返回该年剩余的秒数，并通过指针返回年份
 * @param total_seconds  传入总秒数（会被修改为剩余秒数）
 * @param start_year     起始年份（如2000）
 * @param out_year       输出参数，返回计算出的年份
 * @return               该年剩余的秒数（即从该年1月1日0时开始的秒数）
 */
unsigned int year_conversion(unsigned int total_seconds, unsigned int start_year, unsigned int *out_year) {
    unsigned int year = start_year;
    while (1) {
        // 获取当年的秒数（平年31536000，闰年31622400）
        unsigned int seconds_in_year = is_leap_year(year) ? 31622400 : 31536000;
        if (total_seconds >= seconds_in_year) {
            total_seconds -= seconds_in_year;
            year++;
        } else {
            *out_year = year;
            return total_seconds;   // 返回该年剩余的秒数
        }
    }
}

/**
 * 从一年内的秒数计算出月份和日，并返回月份
 * @param seconds_of_year  一年内的秒数（0 ~ 31535999 或 31622399）
 * @param year             年份（用于判断闰年）
 * @param out_day          输出参数，返回日期（1~31）
 * @return                 月份（1~12）
 */
unsigned int month_conversion(unsigned int seconds_of_year, unsigned int year, unsigned int *out_day) {
    // 平年每月天数
    int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(year)) {
        days_in_month[1] = 29;   // 闰年2月改为29天
    }

    unsigned int day_of_year = seconds_of_year / 86400;   // 从1月1日0时起的天数（0起始）
    unsigned int month = 1;
    for (; month <= 12; month++) {
        if (day_of_year < days_in_month[month - 1]) {
            break;   // 当前月就是目标月
        }
        day_of_year -= days_in_month[month - 1];
    }
    *out_day = day_of_year + 1;   // 转为1起始的日
    return month;
}

int main() {
    unsigned int total_seconds;
    printf("请输入从2000-01-01 00:00:00开始的秒数：");
    scanf("%u", &total_seconds);

    // 1. 提取年份和该年剩余秒数
    unsigned int year;
    unsigned int seconds_of_year = year_conversion(total_seconds, 2000, &year);

    // 2. 提取月份和日
    unsigned int day;
    unsigned int month = month_conversion(seconds_of_year, year, &day);

    // 3. 提取时、分、秒
    unsigned int seconds_of_day = seconds_of_year % 86400;  // 当天内的秒数
    unsigned int hour = seconds_of_day / 3600;
    unsigned int minute = (seconds_of_day % 3600) / 60;
    unsigned int second = seconds_of_day % 60;

    // 4. 输出结果
    printf("%04u年%02u月%02u日 %02u时%02u分%02u秒\n",
           year, month, day, hour, minute, second);

    return 0;
}