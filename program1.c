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
	
	int memory_slots[m];
	int rdcount[10],wtcount[10],thcount[10];
	
	/*
	trans_operation[i] = read
	trans_memory[i] = 1
	trans_opparend[i] = None

	memroy_slots[10] = []
	*/
	for(int dset = 0;dset<10;dset++)
	{	
		char trans_operation[1000][6];
		int trans_memory[1000];
		char trans_opparend[1000][10];
		int i=0;
		char filename[20];
		sprintf(filename,"./datasets/dataset%d",dset);
		strcat(filename,".txt");
		ptr = fopen(filename,"r");
		printf("--- Reading data set ---\n");
		int rds,wts;
		rds = 0;
		wts = 0;
		printf("%s\n",filename);
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
	
		//printf("Total %d transacitons\n",n );
	
		initiliasedata(memory_slots,m);
		/*
		all the thransaction in the arrays
		with their index values
		*/
		double min = 99999;
		for(int nthreads = 2;nthreads<16;nthreads+=2)
		{
			double t1 = omp_get_wtime();
			
			
			#pragma omp parallel for num_threads(nthreads) schedule(static,5) ordered
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
					//printf("%d\n",op );
					if(op==3);
						//printf("read %d\n",read(memory_slots,trans_memory[i]));
					else if(op==0)
						memory_slots[trans_memory[i]] = atoi(trans_opparend[i]);
					else
						write(memory_slots,trans_memory[i],trans_opparend[i]);
				}
			}
			double t2 = omp_get_wtime();
			printf("\t%d - %lf\n",nthreads,t2-t1 );
			if(min>t2-t1)
			{
				min = t2-t1;
				thcount[dset] = nthreads;
			}
		}
		rdcount[dset] = rds;
		wtcount[dset] = wts;
		printf("%d %d %lf\n",rds,wts,min );
	}
	FILE *ptr1;
	ptr1 = fopen("traindata.txt","w");
	fprintf(ptr1, "threads,read,write\n");
	for(int i=0;i<10;i++)
	{
		printf("%d,%d,%d\n",thcount[i],rdcount[i],wtcount[i]);
		fprintf(ptr1,"%d,%d,%d\n",thcount[i],rdcount[i],wtcount[i]);
	}
	fclose(ptr);
	fclose(ptr1);

}