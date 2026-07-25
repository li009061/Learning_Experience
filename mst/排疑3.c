// // 1.	分析下面 C 语言程序的错误
// // a)
// char * str1="123123";
// char str2[256];
// char * str3;
// strcpy(str2,str1);
// strcpy(str3,str2);
// str3没有初始化，没有指向合法内存。str3访问非法地址，导致段错误



// // b)
// void func1(void)
// {
//     int i;
//     for(i=0;i<100;i++)
//     {
//         printf("%d",i);
//         int *pj=malloc(sizeof(int));
//         *pj=i%10;
//         printf("%d",*pj);
//     }
// }
// 没有释放*pj的内存，内存泄漏，指针pj会变成野指针



// // c)
// void func1(int i)
// {
//     if(i>100) 
//     printf("%d",i)；
//     i--;
//     func1(i);
// }
//没有结束标志，会无限调用函数。递归缺少终止条件，导致栈溢出
