#include <stdlib.h>
#include "throwerror.h"
#include "inttuple.h"

struct Matrix{
    int rows;
    int columns;
    double* content;
};

struct IntTuple size(struct Matrix m){
    struct IntTuple result = newIntTuple(m.rows, m.columns);
    return result;
}

int inMatrixRange(struct Matrix m, int row, int column){
    if (row<m.rows && column<m.columns){
        return 1;
    }
    return 0;
}

double* matrixGet(struct Matrix m, int row, int column){
    if (!inMatrixRange(m,row,column)){
        printf("ERROR - ATTEMPTING TO GET MATRIX POSITION OUTSIDE OF RANGE\n"); 
        throwError();
    }
    return &m.content[row*m.columns+column];
}

void matrixSet(struct Matrix m, int row, int column, double value){
    if (!inMatrixRange(m,row,column)){
        printf("ERROR - ATTEMPTING TO SET MATRIX POSITION OUTSIDE OF RANGE\n"); 
        throwError();
    }
    
    *matrixGet(m,row,column) = value;
    return;
}

struct Matrix newMatrix(int rows, int columns){
    struct Matrix m;
    m.rows = rows;
    m.columns = columns;
    m.content = (double*) malloc(sizeof(double)*rows*columns);
    return m;
}

struct Matrix newSameSizedMatrix(struct Matrix m){
    return newMatrix(m.rows,m.columns);
}

struct Matrix copyMatrix(struct Matrix m){
    struct Matrix mCopied = newSameSizedMatrix(m);
    for (int i = 0; i < m.rows; i++){
        for (int j = 0; j < m.columns; j++){
            matrixSet(mCopied,i,j,*matrixGet(m,i,j));
        }
    }

    return mCopied;
}

struct Matrix zeroMatrix(int rows, int columns){
    struct Matrix m = newMatrix(rows,columns);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++)
            matrixSet(m,i,j,0);
    }
    return m;
}

struct Matrix identityMatrix(int size){
    struct Matrix m = newMatrix(size,size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (i == j)
                matrixSet(m,i,j,1);
            else
                matrixSet(m,i,j,0);
        }
    }
    return m;
}

struct Matrix transpose(struct Matrix m){
    struct Matrix t = newSameSizedMatrix(m);
    for (int i = 0; i < m.rows; i++){
        for (int j = 0; j < m.columns; j++)
            matrixSet(t,j,i,*matrixGet(m,i,j));
    }
    return t;
}

void killMatrix(struct Matrix m){
    free(m.content);
    return;
}

int compareMatrixSize(struct Matrix a, struct Matrix b){
    if (compareIntTuple(size(a),size(b))){
        return 1;
    }
    return 0;
}

struct Matrix addMatrix(struct Matrix a, struct Matrix b){
    if (!compareMatrixSize(a,b)){
        printf("ERROR - ATTEMPTING TO ADD MATRICES OF DIFFERENT SIZE\n"); 
        throwError();
    }    

    struct Matrix aplusb = copyMatrix(a);
    for (int i = 0; i < a.rows; i++){
        for (int j = 0; j < a.columns; j++){
            *matrixGet(aplusb,i,j) += *matrixGet(b,i,j);
        }   
    }
    return aplusb;
}

struct Matrix scaleMatrix(struct Matrix m, double scalar){
    struct Matrix mScaled = copyMatrix(m);
    for (int i = 0; i < mScaled.rows; i++){
        for (int j = 0; j < mScaled.columns; j++){
            *matrixGet(mScaled,i,j) *= scalar;
        }
    }
    return mScaled;
}

struct Matrix multiplyMatrix(struct Matrix a, struct Matrix b){
    if (a.columns != b.rows){
        printf("ERROR - ATTEMPTING TO MULTIPLY NON-COMPATIBLE MATRICES\n");
        throwError();
    }
    struct Matrix atimesb = zeroMatrix(a.rows, b.columns);
    for (int a_i = 0; a_i < a.rows; a_i++){
        for (int b_j = 0; b_j < b.columns; b_j++){
            for (int b_i = 0; b_i < b.rows; b_i++){
                *matrixGet(atimesb,a_i,b_j) += *matrixGet(a,a_i,b_i) * *matrixGet(b,b_i,b_j);
            }
        }
    }
    return atimesb;
}

int isSquareMatrix(struct Matrix m){
    if (m.rows == m.columns){
        return 1;
    }
    return 0;
}

struct Matrix trimMatrix(struct Matrix m, int row, int column){
    struct Matrix mTrimmed = newMatrix(m.rows-1, m.columns-1);
    int currentRow = 0, currentColumn = 0;
    for (int i = 0, currentRow = 0; i < m.rows; i++){
        if (i == row){ continue; }
        for (int j = 0, currentColumn = 0; j < m.columns; j++){
            if (j == column){ continue; }
            matrixSet(mTrimmed,currentRow,currentColumn,*matrixGet(m,i,j));
            currentColumn++;
        }
        currentRow++;
    }
    return mTrimmed;
}

int signedEven(int number){
    if (number & 1){
        return -1;
    }
    return 1;
    
}

double determinant(struct Matrix m){
    if (!isSquareMatrix(m)){
        printf("ERROR - ATTEMPTING TO GET THE DETERMINANT OF A NON-SQUARE MATRIX\n");
    }

    if (compareIntTuple(size(m), newIntTuple(2,2))){
        return (*matrixGet(m,0,0) * *matrixGet(m,1,1) - *matrixGet(m,0,1) * *matrixGet(m,1,0));
    }
    
    double result = 0;
    for (int j = 0; j < m.columns; j++){
        struct Matrix mTrimmed = trimMatrix(m,0,j);
        result += signedEven(j) * *matrixGet(m,0,j) * determinant(mTrimmed);
        killMatrix(mTrimmed);
    }

    return result;
}

struct Matrix adjoint(struct Matrix m){
    if (!isSquareMatrix(m)){
        printf("ERROR - ATTEMPTING TO GET THE ADJOINT OF A NON-SQUARE MATRIX\n");
    }
    
    struct Matrix w = newSameSizedMatrix(m);
    for (int i = 0; i < m.rows; i++){
        for (int j = 0; j < m.columns; j++){
            matrixSet(w,i,j,signedEven(i+j) * determinant(trimMatrix(m,i,j)));
        }
    }
    w = transpose(w);
    return w;
}

struct Matrix inverse(struct Matrix m){
    if (!isSquareMatrix(m)){
        printf("ERROR - ATTEMPTING TO GET THE INVERSE OF A NON-SQUARE MATRIX\n");
        throwError();
    }

    return (scaleMatrix(adjoint(m),(double)1/determinant(m)));
}

void showMatrix(struct Matrix m){
    for (int i = 0; i < m.rows; i++){
        printf("( ");
        for (int j = 0; j < m.columns; j++){
            if (j+1 == m.columns){
                printf("%0.4f", *matrixGet(m,i,j));
            }else{    
                printf("%0.4f ", *matrixGet(m,i,j));
            }
        }
        printf(" )");
        printf("\n");
    }
    return;
}

