#include <stdint.h>
#include <stdio.h>
uint64_t multimod(unsigned long long int a, unsigned long long int b, unsigned long long int m)
{
	int A[70], B[70], SUM[130];
	unsigned long int f;
	f = (unsigned long long int)b;
	int i = 0, j = 0;
	for (; a >= 1 && i < 70; i++)
	{
		A[i] = a & 1;
		a >>= 1;
	}
	if (i >= 1)
		i--;
	for (; f >= 1 && j <= 70; j++)
	{
		B[j] = f & 1;
		f >>= 1;
	}
	j--;
	int p = 0;
	int c = 0;
	for (; p <= j + i + 1; p++)
	{
		int k = 0, n = 0, s = 0;
		for (; k <= i; k++)
		{
			n = p - k;
			if (n < 0)
				break;
			else if (n >= 0 && n <= j)
				s = (A[k] & B[n]) + s;
		}
		SUM[p] = (s + c) & 1;
		c = (s + c) >> 1;
	}
	// printf("%d",c);
	if (c >= 0)
	{
		for (; c >= 1 && p < 150; p++)
		{
			SUM[p] = c & 1;
			c = c >> 1;
		}
	}
	p--;
	unsigned long int yushu = c, he = 0;
	// 	int cishu=0;
	// 	if(p>64){
	// 		for(;p>=64;p--){
	// 		new=-m;
	// 		if(SUM[p]==1){
	// 			cishu=p-64;
	// 			for(int j=0;j<cishu;j++){
	// 				new<<=1;
	// 				while(new>=m)	new=new-m;
	// 			}
	// 	dade=dade+new;
	// 	}
	// 	}
	// }

	for (; p >= 0; p--)
	{
		if (yushu << 1 < yushu)
		{
			yushu <<= 1;
			he = SUM[p] + yushu - m;
		}
		else
		{
			yushu <<= 1;
			he = SUM[p] + yushu;
		}
		while (he >= m)
		{
			he = he - m;
		}
		yushu = he;
	}
	return (uint64_t)yushu;
}

// unsigned long int sum=0,c=0;
// int p=0;
// for(;p<=i;p++){
// 	if(A[p]==1){
// 		if(p+j>=64){
// 			c+=1;
// 			c=c+(b>>(64-p));
// 		}
// 		sum=(b<<p)+sum;
// 		while(sum>=m){
// 		sum=sum-m;}
// 		}
// }
// if(c>0){
// sum=sum+c-1;
// while (sum>=m)
// {
// 	sum=sum-m;
// }
// }
// return (uint64_t) sum;
// return (a * b) % m; // buggy
