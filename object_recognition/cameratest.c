#include <stdio.h>
#include <stdlib.h>

int main()
{
	int value = system("raspistill -o hoge.jpg");
	printf("%d\n",value);
	if(value ==17920)
	{
		printf("カメラが接続されてません\n");
	}
	return 0;
}
