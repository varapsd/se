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
	ptr = fopen("./datasets/dataset1.txt","r");
	char trans_operation[1000][6];
	int trans_memory[1000];
	int memory_slots[m];
	char trans_opparend[1000][10];
	int i=0;
	/*
	trans_operation[i] = read
	trans_memory[i] = 1
	trans_opparend[i] = None

	memroy_slots[10] = []
	*/
	printf("--- Reading data set ---\n");
	int rds=0,wts=0;
	while(!feof(ptr))
	{
		fscanf(ptr,"%s",trans_operation[i]);
		fscanf(ptr,"%d",&trans_memory[i]);
		fscanf(ptr,"%s",trans_opparend[i]);
		if(!strcmp(trans_operation[i],"read"))
				rds++;
			else
				wts++;
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
	FILE *eq;
	char cnst[20];
	char rdcnst[5];
	char wtcnst[5];
	eq = fopen("eq.txt","r");
		fscanf(eq,"%s",cnst);
		//fscanf(eq,"%s",rdcnst);
		//fscanf(eq,"%s",wtcnst);
	printf("%s\n",cnst );
	int wrd = 0;
	char words[3][5];
	int k=0;
	double result[3];
	for(int i=0;cnst[i]!='\0'&&i<20;i++){
		//printf("%s\n",cnst[i] );
		if(cnst[i]==',')
		{
			//printf("%s\n",words[wrd] );
			result[wrd] = strtod(words[wrd],NULL);
			wrd++;
			k=0;
		}
		else
		{
			words[wrd][k] = cnst[i];
			k++;
		}
	}
	result[wrd] = strtod(words[wrd],NULL);
	for(int i=0;i<3;i++){
		printf("%f\n",result[i] );
	}
	int nthreads = result[0]+result[1]*rds + result[2]*wts;
	printf("%d\n",nthreads );
	double t1,t2;
	t1 = omp_get_wtime();
	#pragma omp parallel for num_threads(nthreads) schedule(static,6) ordered
	for(i=0;i<n;i++)
	{
		int op;
		//printf("%s\n",trans_operation[i] );
		if(!strcmp(trans_operation[i],"read"))
			op = 3;
		else
			op = check(trans_opparend[i]);

		#pragma omp ordered
		{
			if(op==3)
				printf("read %d\n",read(memory_slots,trans_memory[i]));
			else if(op==0)
				memory_slots[trans_memory[i]] = atoi(trans_opparend[i]);
			else
				write(memory_slots,trans_memory[i],trans_opparend[i]);
		}
	}
	t2 = omp_get_wtime();
	printf("time taken is %lf\n",t2-t1 );
	fclose(ptr);
	fclose(eq);
}