#include <stdio.h>
#include "matrix.h"

int main(){

    double content[] = {1,2,3,
                        4,5,5,
                        7,8,9};
    struct Matrix m = newMatrix(3,3);
    m.content = content;
    
    printf("m =\n");
    showMatrix(m);

    printf("\n2 * m =\n");
    showMatrix(scaleMatrix(m,2));
    
    printf("\nm + m =\n");
    showMatrix(addMatrix(m,m));

    printf("\nm * m =\n");
    showMatrix(multiplyMatrix(m,m));

    printf("\ndet(m) =\n%0.4f\n",determinant(m));

    printf("\ninv(m) =\n");
    showMatrix(inverse(m));

    killMatrix(m);
    return 0;
}
