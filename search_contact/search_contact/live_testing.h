void record_digit()
{
	system("Live_testing\\Recording_Module.exe 3 Live_testing\\input_file.wav Live_testing\\input_file.txt");
	/*int choice;
	do
	{
	system("Live_testing\\Recording_Module.exe 3 Live_testing\\input_file.wav Live_testing\\input_file.txt");
	printf("Playing sound:\n");
	PlaySound(TEXT("Live_testing\\input_file.wav"),NULL,SND_SYNC);
	printf("Press 1:Proceed 2:Re-record digit\n");
	scanf("%d",&choice);
	printf("Recording completed\n");
	}while(choice==2);*/
}
//function to find the short term energy of the given frame
lld short_term_en(int start,lld arr[],int sign)
{
	lld energy = 0;
	if(sign==1)
	{
	for(int i = start; i<=start+320; i++)
		energy += arr[i]*arr[i];
	}
	else
	{
		for(int i = start; i>=start-320; i--)
		energy += arr[i]*arr[i];
	}
	return energy/320;
}

//function to trim the live testing data and extract the useful information from it
void trim_file()
{
	FILE *fptr;
	fptr=fopen("Live_testing\\input_file.txt","r");
	if(fptr==NULL)
	{
		printf("Error opening file\n");
		return;
	}
	int n = 0,l,r;
	lld a;
	lld arr[100000];
	while(fscanf(fptr,"%Lf",&a) != EOF)
		arr[++n] = a;
	fclose(fptr);
	lld previous_energy=0,curr_energy=0,threshold_energy=0;
	threshold_energy=short_term_en(641,arr,1);
	//curr_energy=short_term_energy(1,arr,1);
	for(int i=321;i+320<=n;i+=320)
	{
		//previous_energy=curr_energy;
		curr_energy=short_term_en(i,arr,1);
		if(curr_energy/threshold_energy>=7.0)
		{
			l=i;
			break;
		}
	}
	//curr_energy=short_term_energy(n,arr,-1);
	for(int i=n;i-320>=1;i-=320)
	{
		//previous_energy=curr_energy;
		curr_energy=short_term_en(i,arr,-1);
		if(curr_energy/threshold_energy>=7.0)
		{
			r=i;
			break;
		}
	}
	l-=2*320;
	r+=2*320;
	
	fptr = fopen("Live_testing\\input_file_trimed.txt","w");
	if(fptr==NULL)
	{
		printf("Error opening file\n");
		return;
	}
	//printing the useful extracted data into corresponding text file
	for(int i = l; i<= r; i++)
		fprintf(fptr,"%lf\n",arr[i]);
	fclose(fptr);
}
long double short_term_energy(int l, int r,lld data[])
{
	long double energy = 0;
	for(int i = l; i<=r; i++)
	{
		energy += data[i]*data[i];
	}
	return energy/(r-l+1);
}

//function to trim the live testing data and extract the useful information
void trim_data()
{
	
	FILE *fptr;
	fptr=fopen("Live_testing\\input_file.txt","r");
	if(fptr==NULL)
	{
		printf("Can't open file\n");
		return;
	}
	//FILE *fptr = fopen("live_test_data\\output.txt","r");
	int n = 0;
	long double a;
	lld data[90000];
	while(fscanf(fptr,"%Lf",&a) != EOF)
	{
		data[++n] = a;
	}
	fclose(fptr);
	//fflush(fptr);
	long double threshold = short_term_energy(n-320*3,n-320,data);	//function call to find the threshold energy of the silence part
	int l = 0,r = n,s,f = 0;
	long double max = 0;
	long double frame_energy[1000];
	//loop to find the frame with largest short term energy
	//We move left of this frame to find the starting point of useful data and move right to find the ending point of useful data
	for(int i = 1; i+320<=n; i += 320)
	{
		frame_energy[++f] = short_term_energy(i,i+319,data);
		if(max < frame_energy[f])
		{
			max = frame_energy[f];
			s = f;
		}
	}
	for(int i = s; i>0; i--)
	{
		if(frame_energy[i] < 10*threshold)
		{
			l = i*320 + 1;
			break;
		}
	}
	for(int i = s; i<=f; i++)
	{
		if(frame_energy[i] < 10*threshold)
		{
			r = (i-1)*320 + 1;
			break;
		}
	}
	fptr=fopen("Live_testing\\input_file_trimed.txt","w");
	if(fptr==NULL)
	{
		printf("Error opening file\n");
		return ;
	}//loop to print the extracted data in text file
	for(int i = l; i<= r; i++)
		fprintf(fptr,"%Lf\n",data[i]);
	fclose(fptr);
	//fflush(fptr);
}
int find_digit()
{
	lld probability=0,calc_probability=0;
	int index=0;
	for(int i=1;i<=contact_count;i++)
	{
		read_lambda_values(i);
		calc_probability=forward_procedure();
		//printf("Calc_prob=%e\n",calc_probability);
		if(calc_probability>probability)
		{
			probability=calc_probability;
			index=i;
		}
	}
	return index;
}
int predict_digit()
{
	char filename[40];
	lld arr[40000],frame_arr[framesize+1];
	int index,framecount=0,predicted_digit;
	lld count;
	lld R[p+1],A[p+1],C[p+1];
	load_codebook();
	sprintf(filename,"Live_testing/input_file_trimed.txt");
	FILE *fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("unable to access Live_testing file\n");
		return 0;
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
	T=framecount;
	predicted_digit=find_digit();
	return predicted_digit;
	printf("Predicted digit is: %d\n",predicted_digit);
}
int live_testing()
{
	record_digit();
	//trim_file();
	trim_data();
	return predict_digit();
}