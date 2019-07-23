//c++ program
//design by >^.^<
//@date    2019-07-18 19:11:18
#include <iostream>
// 用来测试对指针的一些想法
// 在我看来指针赋值应该是可以相等的

#include <stdio.h>
int main()
{
  char a = 'a';
  char buffer[1024];
  int *pa = (int*)&a;
  char *ppa = &a;
  printf("%p %p\n", pa, ppa);

  pa =(int*)buffer;
  printf("%p %p\n", buffer, pa);
}
