#ifndef CONVERT_UTILS_H
#define CONVERT_UTILS_H

//将十进制无符号整数转换为十六进制字符串（不使用 %x 格式符）

void to_hex(unsigned int num, char *hex_str);

//检测当前系统的字节序（大小端）
int is_little_endian(void);

//将整数转换为二进制字符串（仅用于调试）
void to_binary(unsigned int num, int bits, char *bin_str);

#endif