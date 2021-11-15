#include <stdio.h>
#include <malloc.h>
#include <assert.h>


typedef struct { int verse, column; float* ptr; } matrix;

matrix mat_create(int verse, int column)
{
	float* ptr = calloc(verse * column, sizeof(float));
	assert(ptr);
	return (matrix) { verse, column, ptr };
}

//for writing out elements of matrix
void mat_read(matrix A)
{
	for (int i = 0; i < A.column * A.verse; i++, (i % A.column == 0 && i>1? printf("\n") : 1))
	{
		printf("%2.2f \t", *(A.ptr + i)); 
	}
}

//cutting out verses and columns that won't be used for calculating det_partial
matrix mat_cut(matrix A, int v_cut, int c_cut)
{
	float* temp = calloc((A.column - 1) * (A.verse - 1), sizeof(float));
	//pointer to the first element of new matrix
	float* ptr = temp; 
	for (int verse = 0; verse < A.verse; verse++)
	{
		(verse == v_cut ? verse++ : 1);
		for (int column = 0; column < A.column; column++)
		{
			(column == c_cut ? column++ : 1);
			if (column >= A.column) break; //needed, otherwise gets wrong elements when c_cut is last column
			*(ptr) = *(A.ptr + verse * A.column + column);
			ptr++;
		}
	}
	return (matrix) { A.column - 1, A.verse - 1, temp};
}


//counts detriment for (0,i) element of inital matrix
float mat_det_partial(matrix A)
{
	assert(A.column == A.verse);
	float sum = 0;
	for (int i = 0, sign = 1; i < A.column; i++, sign=sign*(-1))
	{
		if (A.column == 2)
		{
			return *(A.ptr) * *(A.ptr + 3) - *(A.ptr + 1) * *(A.ptr + 2);
		}
		else
		{
			sum += *(A.ptr + i) * sign * mat_det_partial(mat_cut(A, 0, i)); 
		}
	}
	return sum; 
}

//sums all partial detriments 
void mat_det(matrix A)
{
	float sum = 0;
	for (int i = 0, sign = 1; i < A.column; i++, sign *= -1)
	{
		sum += mat_det_partial(mat_cut(A, 0, i)) * sign * *(A.ptr + i);
	}
	printf("%f", sum);
}

void main()
{
	float t[] = { 3,0,4,1,5,2,7,4,8,1,6,3,7,0,8,9 }; //example matrix 4x4
	matrix A = { 4,4,t};
	/*float t[] = { 3,0,4,1,5,2,7,4,8 };
	matrix A = { 3,3,t };*/
	mat_det(A);

}