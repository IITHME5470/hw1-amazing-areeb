#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Mohammed Areeb Hussain
//ME21BTECH11033
//Introduction to Parallel Scientific Computing, Assignment 1

void matrix_scan(float **A, int n, FILE* my_file2){
	char filename_2[20];
	sprintf(filename_2,"mat_%06d.in",n);
	my_file2=fopen(filename_2,"r");
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(my_file2, "%f, ", &A[i][j]);
        }
    }
}

void eigen_process(int n, int vec_variable, float **A){
	int i,j,k;
	float lambda;
	float err_mar=0.0001;
	FILE *vector_file;
	char vector_file_name[22];
	
	float *vector;
	vector=(float *)malloc(n*sizeof(float));

	// File Processing
	sprintf(vector_file_name,"vec_%06d_%06d.in",n,vec_variable);
	vector_file=fopen(vector_file_name,"r+");

	if (vector_file == NULL) {
    printf("Error: Unable to open file.\n");
	}

	for (k=0;k<n;k++){
		fscanf(vector_file,"%f, ",&vector[k]);
	}

	// Matrix Multiplication
	float *vector_new;
	vector_new=(float *)malloc(n*sizeof(float));
	for (k=0;k<n;k++){
		vector_new[k]=0;
		for(j=0;j<n;j++){
			vector_new[k]=vector_new[k]+A[k][j]*vector[j];
		}
	}

	/*for(k=0;k<n;k++){
		printf("%f ",vector[k]);
	}
	printf("\n");
	for(k=0;k<n;k++){
		printf("%f ",vector_new[k]);
	}
	printf("\n\n");*/

	//BEGIN CHECK FOR LAMBDA

	// Test for non-zero element divide by 0, ie, lambda --> infinity
	for(k=0;k<n;k++){
		if(vector[k]==0 && vector_new[k]!=0){
			printf("%s : Not an eigenvector\n",vector_file_name);
			//printf("Terminated due to 0 division\n");
			free(vector);
			free(vector_new);
			return;
		}
	}

	//Test for lambda --> 0, Assume

	int test_var=0;
	for(k=0;k<n;k++){
		if(vector_new[k]!=0){
			test_var=1;
		}
	}
	if(test_var==0){
		lambda=0;
		printf("%s : Yes : %f\n",vector_file_name,lambda);
		fseek(vector_file, 0, SEEK_END);
		fprintf(vector_file, "%f",lambda);
		//printf("Terminated due to 0 lambda\n");
		free(vector);
		free(vector_new);
		return;
	}
	
	//Now, lambda is neither 0, nor infinite. Check if lambda is there
	i=0;
	while(vector[i]==0){
		i=i+1;
	}
	lambda=vector_new[i]/vector[i];

	for(k=i+1;k<n;k++){
		if(vector[i]==0){
			continue;
		}
		else if(fabsf(lambda-vector_new[k]/vector[k])>err_mar){
			printf("%s : Not an eigenvector\n",vector_file_name);
			//printf("Terminated due to error margin exceeded\n");
			free(vector);
			free(vector_new);
			return;
		}
	}

	printf("%s : Yes : %f\n",vector_file_name,lambda);
	fseek(vector_file, 0, SEEK_END);
	fprintf(vector_file, "%f",lambda);
	free(vector);
	free(vector_new);
	return;
}



int main(){
	int n;
	float**A;

	int i,j;
	int vec_variable;

	FILE *my_file1, *my_file2;

	my_file1=fopen("input.in","r");
	fscanf(my_file1,"%d",&n);
	//printf("In file 1, %d\n",n);

	//creating matrix A, and alotting memory, and assigning values
	A=(float **)malloc(n*sizeof(float *));
	for (i=0;i<n;i++){
		A[i]=(float *)malloc(n*sizeof(int));
	}

	matrix_scan(A,n,my_file2);

	//Printing Matrix
	/*for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			printf("%f ",A[i][j]);
		}
		printf("\n");
	}

	printf("\n");*/

	for (vec_variable=1;vec_variable<5;vec_variable++){
    	eigen_process(n, vec_variable, A);
    }

	//freeing Matrix Memory
	for (i=0;i<n;i++){
		free(A[i]);
	}
	free(A);
}

