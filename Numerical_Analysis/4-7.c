#include <stdio.h>
#include <math.h>

#define MAX_m 200
#define MAX_n 5

double f1(double x)
{
    return sin(x);
}

double f2(double x)
{
    return exp(x);
}

int OPA( double (*f)(double t), int m, double x[], double w[], double c[], double *eps );

void print_results( int n, double c[], double eps)
{				
    int i;

    printf("%d\n", n);
    for (i=0; i<=n; i++)
        printf("%12.4e ", c[i]);
    printf("\n");
    printf("error = %9.2e\n", eps);
    printf("\n");
}

int main()
{
    int m, i, n;
    double x[MAX_m], w[MAX_m], c[MAX_n+1], eps;

    m = 90;
    for (i=0; i<m; i++) {
        x[i] = 3.1415926535897932 * (double)(i+1) / 180.0;
        w[i] = 1.0;
    }
    eps = 0.001;
    n = OPA(f1, m, x, w, c, &eps);
    print_results(n, c, eps);

    m = 200;
    for (i=0; i<m; i++) {
        x[i] = 0.01*(double)i;
        w[i] = 1.0;
    }
    eps = 0.001;
    n = OPA(f2, m, x, w, c, &eps);
    print_results(n, c, eps);

    return 0;
}

/* Your function will be put here */



int OPA( double (*f)(double t), int m, double x[], double w[], double c[], double *eps )
{
    int i, j, k;
    double err = 0.0;
    double B, C;
    double fai0[MAX_n+1], fai1[MAX_n+1], fai2[MAX_n+1];
    double y[MAX_m];
    double a[MAX_n+1];

    double sum_all = 0, sum_w = 0, sum_x = 0;

    //Step 1
    for(i = 0; i <= MAX_n; ++i)
    {
        fai0[i] = 0;
        fai1[i] = 0;
        fai2[i] = 0;
    }

    fai0[0] = 1;
    for(i = 0; i < m; ++i)
    {
        y[i] = f(x[i]);
        sum_all += w[i] * y[i];
        sum_w += w[i];
    }
    a[0] = sum_all / sum_w;

    for(i = 0; i <= MAX_n; ++i)
    {
        c[i] = a[0] * fai0[i];
    }

    for(i = 0; i < m; ++i)
    {
        err += w[i] * y[i] * y[i];
    }
    err -= a[0] * sum_all;

    //Step 2
    for(i = 0; i < m; ++i)
    {
        sum_x += w[i] * x[i];
    }
    fai1[0] = -1.0 * sum_x / sum_w;
    fai1[1] = 1;

    sum_all = sum_w = 0.0;
    for(i = 0; i < m; ++i)
    {
        sum_all += w[i] * (fai1[0] + x[i]) * y[i];
        sum_w += w[i] * (fai1[0] + x[i]) * (fai1[0] + x[i]);
    }
    a[1] = sum_all / sum_w;

    for(i = 0; i <= MAX_n; ++i)
    {
        c[i] += a[1] * fai1[i];
    }
    err -= a[1] * sum_all;

    //Step 3
    k = 1;

    //Step 4;
    while( (k < MAX_n) && (fabs(err) >= (*eps)) )
    {
        double t1, t2, t3, t4;

        //Step 5
        ++k;
        //Step 6
        t1 = t2 = t3 = t4 = 0.0;

        for(i = 0; i < m; ++i)
        {
            double t5 = 0.0, t6 = 0.0;
            for(j = 0; j <= MAX_n; ++j)
            {
                t5 += fai1[j] * pow(x[i],j);
                t6 += fai0[j] * pow(x[i],j);
            }
            t1 += x[i] * t5 * t5;
            t2 += t5 * t5;
            t3 += x[i] * t5 * t6;
            t4 += t6 * t6;
        }
        B = t1 / t2;
        C = t3 / t4;

        fai2[0] = -1.0 * B * fai1[0] - C * fai0[0];
        for (i = 1; i <= MAX_n; ++i)
        {
            fai2[i] = fai1[i-1] - B * fai1[i] - C * fai0[i];
        }

        t1 = t2 = t3 = t4 = 0.0;
        for (i = 0; i < m; ++i)
        {
            double t5=0;
            for (j = 0; j <=MAX_n; ++j)
            {
                t5 += fai2[j] * pow(x[i],j);
            }
            t1 += t5 * y[i];
            t2 += t5 * t5;
        }
        a[k] = t1 / t2;

        for (i = 0; i <= MAX_n; ++i)
        {
            c[i] += a[k] * fai2[i];
        }

        err -= a[k] * t1;

        //Step 7
        for (i = 0; i <= MAX_n; ++i)
        {
            fai0[i] = fai1[i];
            fai1[i] = fai2[i];
        }
    }
    //Step 8
    *eps = err;
    return k;
}
