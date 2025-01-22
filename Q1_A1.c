#include <stdio.h>
#include <stdlib.h>

//Mohammed Areeb Hussain
//ME21BTECH11033
//Introduction to Parallel Scientific Computing, Assignment 1

void print_to_file(int n, double**A, int format_flag){
	FILE *file_out;
	char str[21];
	int i,j;

	if(format_flag==0){
		sprintf(str,"array_%06d_asc.out",n);
		file_out=fopen(str,"w");
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				fprintf(file_out,"%.15f ",A[i][j]);
			}
			fprintf(file_out,"\n");
		}
		fclose(file_out);
	}

	else if(format_flag==1){
		sprintf(str,"array_%06d_bin.out",n);
		file_out=fopen(str,"wb");
		fwrite(A,sizeof(double),n*n,file_out);
		fclose(file_out);
	}

	else{
		printf("Print Function Error in format_flag\n");
	}
}

int main(){
	int n;
	FILE *file_in;

	file_in=fopen("input.in","r");
	fscanf(file_in,"%d",&n);
	//printf("In file 1, %d\n",n); to check if input is received appropriately

	double **A;
	int i,j;

	//creating matrix A, and alotting memory, and assigning values
	A=(double **)malloc(n*sizeof(double *));
	for (i=0;i<n;i++){
		A[i]=(double *)malloc(n*sizeof(double));
	}
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			A[i][j]=i+j;
		}
	}

	//print function
	print_to_file(n,A,0);

	//freeing Matrix Memory
	for (i=0;i<n;i++){
		free(A[i]);
	}
	fclose(file_in);
	free(A);
}

