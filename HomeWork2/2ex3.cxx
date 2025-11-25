#include <stdio.h>
#define N 5
int main()
{
	int i;
	int j;
	char massiv[N][N];	
	
	 for(i=0;i<N;i++)
	 {
	 	for(j=0;j<N;j++)
	 	{
	 	if(i+j>=N-1)
	 	{
	 		massiv[i][j]=1;
	 	}	
	 	else
	 	{
	 		massiv[i][j]=0;  
	 	}
	 	printf("%d",massiv[i][j]);
	 	}
	 	printf("\n");
	 }
return 0;
}