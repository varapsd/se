#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>


void initiliasedata(int *memory_slots,int m)
{
	int i;
	#pragma omp parallel for num_threads(4) shared(memory_slots) private(i)
	for(i=0;i<m;i++)
	{
		memory_slots[i] = i;
		//printf("by thread %d\n",omp_get_thread_num() );
	}
}

int  read(int *memory_slots ,int pos)
{
	// memory_slot[pos] read
	return  memory_slots[pos];
}

int check(char *opparend)
{
	if(opparend[0]=='+')
		return 1;
	else if(opparend[0]=='-')
		return 2;
	else
		return 0;
}

void write(int *memory_slots,int pos,char *opparend)
{
	int opcheck = check(opparend);
	int i;
	switch(opcheck)
	{
		case 0 : 
				memory_slots[pos] = atoi(opparend);
				break;
		case 1 :
				memory_slots[pos]+=atoi(opparend);
				break;
		case 2 : 
				memory_slots[pos]+=atoi(opparend);
				break;
		default : printf("invalid\n");
	}

}

int main()
{
	FILE *ptr;
	int m= 10;
	ptr = fopen("dataset1.txt","r");
	char trans_operation[100][6];
	int trans_memory[100];
	int memory_slots[m];
	char trans_opparend[100][10];
	int i=0;
	/*
	trans_operation[i] = read
	trans_memory[i] = 1
	trans_opparend[i] = None

	memroy_slots[10] = []
	*/
	printf("--- Reading data set ---\n");
	while(!feof(ptr))
	{
		fscanf(ptr,"%s",trans_operation[i]);
		fscanf(ptr,"%d",&trans_memory[i]);
		fscanf(ptr,"%s",trans_opparend[i]);
		i++;
	}

	int n = i; // number of transactions
	/*
	for (i = 0; i < n; ++i)
	{
		printf("\t%s %d %s\n",trans_operation[i],trans_memory[i],trans_opparend[i] );
	}
	*/

	printf("Total %d transacitons\n",n );

	initiliasedata(memory_slots,m);
	/*
	all the thransaction in the arrays
	with their index values
	*/
	#pragma omp parallel for num_threads(6) schedule(static,6)
	for(i=0;i<n;i++)
	{
		if(!strcmp(trans_operation[i],"read"))
		{
			printf("thread[%d] ,Reading memory_slot %d ::: value is %d\n",omp_get_thread_num(),trans_memory[i],read(memory_slots,trans_memory[i]));
		}
		else
		{
			write(memory_slots,trans_memory[i],trans_opparend[i]);
			printf("thread[%d], writing on %d position :: value is %d in %d \n",omp_get_thread_num(),trans_memory[i],memory_slots[trans_memory[i]], i );
		}
	}

}