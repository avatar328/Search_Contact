

//function to trim the live testing data and extract the useful information
void live_trim_data(int i)
{
	char file_location[400];
	sprintf(file_location,"training_sample\\%d_%d.txt",contact_count,i);
	FILE *fptr;
	fptr=fopen(file_location,"r");
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
	fptr = fopen(file_location,"w");
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

void live_trim_file(int i)
{
	char file_location[400];
	sprintf(file_location,"training_sample\\%d_%d.txt",contact_count,i);
	FILE *fptr;
	fptr=fopen(file_location,"r");
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
	
	fptr = fopen(file_location,"w");
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

void live_record_digit(int i)
{
	char file_location[400];
	sprintf(file_location,"Live_testing\\Recording_Module.exe 3 Live_testing\\input_file.wav training_sample\\%d_%d.txt",contact_count,i);
	system(file_location);
	//live_trim_file(i);
	live_trim_data(i);
}
