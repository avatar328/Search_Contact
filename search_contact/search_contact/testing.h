int test_digit(int digit)
{
	lld probability=0,calc_probability=0;
	int index=-1;
	for(int i=1;i<=10;i++)
	{
		read_lambda_values(i);
		calc_probability=forward_procedure();
		if(calc_probability>probability)
		{
			probability=calc_probability;
			index=i;
		}
	}
	if(index==digit)
		return 1;
	return 0;
}
void testing()
{
	char filename[40];
	lld arr[40000],frame_arr[framesize+1];
	int index,framecount;
	lld count;
	lld R[p+1],A[p+1],C[p+1];
	load_codebook();
	int accuracy_count=0,overall_accuracy_count=0;
	printf("\n--------------------------------------TESTING---------------------------------------\n");
	for(int i=1;i<=10;i++)
	{
		accuracy_count=0;
		for(int j=16;j<=20;j++)
		{
			sprintf(filename,"testing_sample/%d_%d.txt",i,j);
			FILE *fp=fopen(filename,"r");
			if(fp==NULL)
			{
				printf("unable to access testing sample\n");
				return;
			}
			int k=0;
			while(fscanf(fp,"%Lf",&arr[k])!=EOF)
				k++;
			fclose(fp);
			dc_shift(arr);
			normalisation(arr);
			framecount=0;
			for(int x=0;x+framesize<k;x+=80)
			{
				index=0;
				for(int y=x;y<x+framesize;y++)
				{
					frame_arr[index]=arr[y];
					index++;
				}
				hammingWindow(frame_arr);
				calculateRi(frame_arr,R);
				calculateAi(R,A);
				calculateCi(R,A,C);
				raisedsinewindow(C);
				framecount++;
				O[framecount]=tokhura_distance_index(C);
			}
			/*printf("testing_sample/224101059_E_%d_%d.txt\n",i,j);
			for(int i=1;i<=framecount;i++)
				printf("%d ",O[i]);
			printf("\n");*/
			T=framecount;
			accuracy_count+=test_digit(i);
		}
		printf("Accuracy of %d is: %d\n",i,accuracy_count*20);
		overall_accuracy_count+=accuracy_count;
	}
	printf("Overall accuracy of model is: %d\n",2*overall_accuracy_count);
}