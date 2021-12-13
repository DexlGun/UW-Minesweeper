void matrix_multiply (double matrixA[], double matrixB[]);

void matrix_multiply (double matrixA[], double matrixB[]) {
     double a;
     double b;
     double c;
     double d;
     double e;
     double f;
     double g;
     double h;
    for(int n{0}; n < 4; n++) {
        if (n == 0) {
             a = matrixA[0]*matrixB[0];
             b = matrixA[0]*matrixB[1];
        }
        else if(n == 1) {
             c = matrixA[1]*matrixB[2];
             d = matrixA[1]*matrixB[3];
        }
        else if(n == 2) {
             e = matrixA[2]*matrixB[0];
             f = matrixA[2]*matrixB[1];
        }
        else if(n == 3) {
             g = matrixA[3]*matrixB[2];
             h = matrixA[3]*matrixB[3];
        }
    }
    for(int x{0}; x < 4; x++) {
        if(x == 0){
        matrixA[x] = a+c;
        }
        else if(x == 1){
        matrixA[x] = e+g;
        }
        else if(x == 2){
        matrixA[x] = b+d;
        }
        else if(x == 3){
        matrixA[x] = f+h;
        }
    }

}

#ifndef MARMOSET_TESTING
int main();
 #endif
 #ifndef MARMOSET_TESTING
 int main() {
double matrixa[4] = {1,2,3,4};
double matrixb[4] = {1,2,3,4};
matrix_multiply(matrixa,matrixb);
return 0;
}
 #endif
