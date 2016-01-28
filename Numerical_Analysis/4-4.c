#include <stdio.h>
#include <math.h>

#define MAX_SIZE 10
#define bound pow((double)2, 127)
#define ZERO 1e-9 /* X is considered to be 0 if |X|<ZERO */

int Jacobi( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN );

int Gauss_Seidel( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN );

int main()
{
    int n, MAXN, i, j, k;
    double a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE], x[MAX_SIZE];
    double TOL;

    scanf("%d", &n);
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
            scanf("%lf", &a[i][j]);
        scanf("%lf", &b[i]);
    }
    scanf("%lf %d", &TOL, &MAXN);

    printf("Result of Jacobi method:\n");
    for ( i=0; i<n; i++ )
        x[i] = 0.0;
    k = Jacobi( n, a, b, x, TOL, MAXN );
    switch ( k ) {
        case -2:
            printf("No convergence.\n");
            break;
        case -1: 
            printf("Matrix has a zero column.  No unique solution exists.\n");
            break;
        case 0:
            printf("Maximum number of iterations exceeded.\n");
            break;
        default:
            printf("no_iteration = %d\n", k);
            for ( j=0; j<n; j++ )
                printf("%.8f\n", x[j]);
            break;
    }
    printf("Result of Gauss-Seidel method:\n");
    for ( i=0; i<n; i++ )
        x[i] = 0.0;
    k = Gauss_Seidel( n, a, b, x, TOL, MAXN );
    switch ( k ) {
        case -2:
            printf("No convergence.\n");
            break;
        case -1: 
            printf("Matrix has a zero column.  No unique solution exists.\n");
            break;
        case 0:
            printf("Maximum number of iterations exceeded.\n");
            break;
        default:
            printf("no_iteration = %d\n", k);
            for ( j=0; j<n; j++ )
                printf("%.8f\n", x[j]);
            break;
    }

    return 0;
}

/* Your function will be put here */


int Jacobi( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN )
{
    int k = 1;
    double XO[MAX_SIZE];
    // Check if the matrix has a zero column
    int checkZeroColumn = 0;
    int i, j;
    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            if(fabs(a[i][j]) > ZERO)
            {
                ++checkZeroColumn;
                break;
            }
        }
    }
    if(checkZeroColumn != n)
    {
        return -1;
    }

    //Exchange rows
    for(i = 0; i < n; ++i)
    {
        //start from a[i][i]
        int maxRow = i; 
        for(j = i+1; j < n; ++j)
        {
            if(fabs(a[j][i]) > fabs(a[maxRow][i]))
            {
                maxRow = j;
            }
        }
        // If that entry is non-zero, swap it to the diagonal
        if(fabs(a[maxRow][i]) > ZERO)
        {
            if(maxRow != i)
            {
                double tmpValue;
                for(j = 0; j < n; ++j)
                {
                    tmpValue = a[maxRow][j];
                    a[maxRow][j] = a[i][j];
                    a[i][j] = tmpValue;
                }
                tmpValue = b[maxRow];
                b[maxRow] = b[i];
                b[i] = tmpValue;
            }
        }   
        // Upward
        else
        {
            maxRow = i - 1;
            for(j = i-1; j >= 0; --j)
            {
                if(fabs(a[j][i]) > fabs(a[maxRow][i]))
                {
                    maxRow = j;
                }
            }
            if(fabs(a[maxRow][i]) > ZERO)
            {
                for(j = 0; j < n; ++j)
                {
                    a[i][j] += a[maxRow][j];
                }
                b[i] += b[maxRow];
            }
            else
            {
                return -1;
            }
        }
    }

    //Jacobi
    for(i = 0; i < n; ++i)
    {
        XO[i] = x[i];
    }
    while(k <= MAXN)
    {
        double distance = 0;
        for(i = 0; i < n; ++i)
        {
            double tmpValue = 0;
            for(j = 0; j < n; ++j)
            {
                if(j != i)
                {
                    tmpValue += a[i][j] * XO[j];
                }
            }
            x[i] = (-tmpValue + b[i]) / a[i][i];
            //if there is no convergence
            if(fabs(x[i]) > bound)
            {
                return -2;
            }
            //calculate distance
            if(fabs(x[i] - XO[i]) > distance)
            {
                distance = fabs(x[i] - XO[i]);
            }
        }
        if(distance < TOL)
        {
            return k;
        }
        ++k;
        for(i = 0; i < n; ++i)
        {
            XO[i] = x[i];
        }
    }
    return 0;
}

int Gauss_Seidel( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN )
{
    int k = 1;
    double XO[MAX_SIZE];
    // Check if the matrix has a zero column
    int checkZeroColumn = 0;
    int i, j;
    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            if(fabs(a[i][j]) > ZERO)
            {
                ++checkZeroColumn;
                break;
            }
        }
    }
    if(checkZeroColumn != n)
    {
        return -1;
    }

    //Exchange rows
    for(i = 0; i < n; ++i)
    {
        //start from a[i][i]
        int maxRow = i; 
        for(j = i+1; j < n; ++j)
        {
            if(fabs(a[j][i]) > fabs(a[maxRow][i]))
            {
                maxRow = j;
            }
        }
        // If that entry is non-zero, swap it to the diagonal
        if(fabs(a[maxRow][i]) > ZERO)
        {
            if(maxRow != i)
            {
                double tmpValue;
                for(j = 0; j < n; ++j)
                {
                    tmpValue = a[maxRow][j];
                    a[maxRow][j] = a[i][j];
                    a[i][j] = tmpValue;
                }
                tmpValue = b[maxRow];
                b[maxRow] = b[i];
                b[i] = tmpValue;
            }
        }   
        // Upward
        else
        {
            maxRow = i - 1;
            for(j = i-1; j >= 0; --j)
            {
                if(fabs(a[j][i]) > fabs(a[maxRow][i]))
                {
                    maxRow = j;
                }
            }
            if(fabs(a[maxRow][i]) > ZERO)
            {
                if(maxRow != i)
                {
                    for(j = 0; j < n; ++j)
                    {
                        a[i][j] += a[maxRow][j];
                    }
                    b[i] += b[maxRow];
                }
            }
            else
            {
                return -1;
            }
        }
    }
    //Gauss
    for(i = 0; i < n; ++i)
    {
        XO[i] = x[i];
    }
    while(k <= MAXN)
    {
        double distance = 0;
        for(i = 0; i < n; ++i)
        {
            double tmpValue = 0;
            for(j = 0; j < n; ++j)
            {
                if(j < i)
                {
                    tmpValue += a[i][j] * x[j];
                }
                else if(j > i)
                {
                    tmpValue += a[i][j] * XO[j];
                }
            }
            x[i] = (-tmpValue + b[i]) / a[i][i];
            //if there is no convergence
            if(fabs(x[i]) > bound)
            {
                return -2;
            }
            //calculate distance
            if(fabs(x[i] - XO[i]) > distance)
            {
                distance = fabs(x[i] - XO[i]);
            }
        }
        if(distance < TOL)
        {
            return k;
        }
        ++k;
        for(i = 0; i < n; ++i)
        {
            XO[i] = x[i];
        }
    }
    return 0;
}





