#include <stdio.h>

#define MAX_SIZE 10

int EigenV(int n, double a[][MAX_SIZE], double *lambda, double v[], double TOL, int MAXN);

int main()
{
    int n, MAXN, m, i, j, k;
    double a[MAX_SIZE][MAX_SIZE], v[MAX_SIZE];
    double lambda, TOL;

    scanf("%d", &n);
    for (i=0; i<n; i++) 
        for (j=0; j<n; j++) 
            scanf("%lf", &a[i][j]);
    scanf("%lf %d", &TOL, &MAXN);
    scanf("%d", &m);
    for (i=0; i<m; i++) {
        scanf("%lf", &lambda);
        for (j=0; j<n; j++)
            scanf("%lf", &v[j]);
        switch (EigenV(n, a, &lambda, v, TOL, MAXN)) {
            case -1: 
                printf("%12.8f is an eigenvalue.\n", lambda );
                break;
            case 0:
                printf("Maximum number of iterations exceeded.\n");
                break;
            case 1:
                printf("%12.8f\n", lambda );
                for (k=0; k<n; k++)
                    printf("%12.8f ", v[k]);
                printf("\n");
                break;
        }
    }

    return 0;
}


/* Your function will be put here */
#define ZERO 0.0000000001
double Fabs(double a){
    if (a >= 0) {
        return a;
    }
    else {
        return -a;
    }
}

int EigenV(int n, double a[][MAX_SIZE], double *lambda, double v[], double TOL, int MAXN)
{
    int k = 0;
    int p = 0;
    int i, j, r;
    double max = v[0];
    double q = *lambda;
    double mu, ERR, temp;
    double b[MAX_SIZE][MAX_SIZE], l[MAX_SIZE][MAX_SIZE], u[MAX_SIZE][MAX_SIZE];
    double y[MAX_SIZE];

    //compute inf norm of v
    for(i = 0; i < n; ++i)
    {
        if(Fabs(v[i]) > Fabs(max))
        {
            max = v[i];
            p = i;
        }
    }
    for(i = 0;i < n; ++i)
    {
        v[i] /= max;
    }

    while(k < MAXN)
    {
        //copy A matrix
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; j++) {
                b[i][j] = a[i][j];
            }
        }
        //l for compute (A-qI)^-1
        //b is A-qI
        for (i = 0; i < n; ++i) {
            l[i][i] = 1.0;
            b[i][i] -= *lambda;
        }
        
        for (i = 0; i < n; ++i) {
            for (j = i; j < n; ++j) {
                temp = b[i][j];
                for (r = 0; r < i; ++r) 
                {
                    temp -= l[i][r] * u[r][j];
                }
                u[i][j] = temp;
            }
            if (Fabs(u[i][i]) < ZERO) {
                return -1;
            }
            for (j = i + 1; j < n; ++j) {
                temp = b[j][i];
                for ( r = 0; r < i; ++r)
                {
                    temp -= l[j][r] * u[r][i];
                }
                l[j][i] = temp / u[i][i];
            }

        }
        for (i = 0; i < n; ++i) {
            temp = 0.0;
            for (j = 0; j < i; ++j)
            {
                temp += l[i][j] * y[j];
            }
            y[i] = v[i] - temp;
        }
        for (i = n - 1; i >= 0; --i) {
            temp = 0.0;
            for (j = i + 1; j < n; ++j)
            {
                temp += u[i][j] * y[j];
            }
            y[i] = (y[i] - temp) / u[i][i];
        }

////////////////////////////////////////////////////
        mu = y[p];

        //compute inf norm of y
        max = y[0];
        p = 0;
        for(i = 1; i < n; ++i)
        {
            if(Fabs(y[i]) > Fabs(max))
            {
                max = y[i];
                p = i;
            }
        }

        //compute error
        ERR = 0;
        for(i = 0; i < n; ++i)
        {
            double t = Fabs(v[i] - y[i]/max);
            if( t > ERR)
            {
                ERR = t;
            }
            v[i] = y[i]/max;
        }

        if(ERR < TOL)
        {
            *lambda = 1.0 / mu + *lambda;
            return 1;
        }   

        ++k;
    }
    return 0;
}
