#include <stdio.h>

#define MAX_N 10

void Cubic_Spline(int n, double x[], double f[], int Type, double s0, double sn, double a[], double b[], double c[], double d[]);

double S( double t, double Fmax, int n, double x[], double a[], double b[], double c[], double d[] );

int main()
{
    int n, Type, m, i;
    double x[MAX_N], f[MAX_N], a[MAX_N], b[MAX_N], c[MAX_N], d[MAX_N];
    double s0, sn, Fmax, t0, tm, h, t;

    scanf("%d", &n);
    for (i=0; i<=n; i++) 
        scanf("%lf", &x[i]);
    for (i=0; i<=n; i++) 
        scanf("%lf", &f[i]);
    scanf("%d %lf %lf %lf", &Type, &s0, &sn, &Fmax);

    Cubic_Spline(n, x, f, Type, s0, sn, a, b, c, d);
    for (i=1; i<=n; i++)
        printf("%12.8e %12.8e %12.8e %12.8e \n", a[i], b[i], c[i], d[i]);

    scanf("%lf %lf %d", &t0, &tm, &m);
    h = (tm-t0)/(double)m;
    for (i=0; i<=m; i++) {
        t = t0+h*(double)i;
        printf("f(%12.8e) = %12.8e\n", t, S(t, Fmax, n, x, a, b, c, d));
    }

    return 0;
}

/* Your functions will be put here */

void Cubic_Spline(int n, double x[], double f[], int Type, double s0, double sn,
                     double a[], double b[], double c[], double d[])
{
    int i, j, k;
    double FPO = s0, FPN = sn;
    double h[MAX_N], alpha[MAX_N], l[MAX_N], miu[MAX_N], z[MAX_N];

    //Input
    for(i = 0; i <= n; ++i)
    {
        a[i] = f[i];
    }

    //clamp
    if(Type == 1)
    {
        //Step 1
        for (i = 0; i <= n - 1; ++i)
        {
            h[i] = x[i + 1] - x[i];
        }
        //Step 2
        alpha[0] = 3.0 * (a[1] - a[0]) / h[0] - 3.0 * FPO;
        alpha[n] = 3.0 * FPN - 3.0 * (a[n] - a[n - 1]) / h[n - 1];
        //Step 3
        for (i = 1; i <= n - 1; ++i)
        {
            alpha[i] = 3.0 * (a[i + 1] - a[i]) / h[i] - 3.0 * (a[i] - a[i - 1]) / h[i - 1];
        }
        //Step 4
        l[0] = 2.0 * h[0];
        miu[0] = 0.5;
        z[0] = alpha[0] / l[0];
        //Step 5
        for (i = 1; i <= n - 1; ++i)
        {
            l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * miu[i - 1];
            miu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }
        //Step 6
        l[n] = h[n - 1] * (2.0 - miu[n - 1]);
        z[n] = (alpha[n] - h[n - 1] * z[n - 1]) / l[n];
        c[n] = z[n];
        //Step 7
        for (j = n - 1; j >= 0; --j)
        {
            c[j] = z[j] - miu[j] * c[j + 1];
            b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
            d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
        }
    }
    //natural spline
    else if(Type == 2)
    {
        //Step 1
        for (i = 0; i <= n - 1; ++i)
        {
            h[i] = x[i + 1] - x[i];
        }
        //Step 2
        for (i = 1; i <= n - 1; ++i)
        {
            alpha[i] = 3.0 / h[i] * (a[i + 1] - a[i]) - 3.0 / h[i - 1] * (a[i] - a[i - 1]);
        }
        //Step 3
        l[0] = 1;
        miu[0] = 0;
        z[0] = 0;
        //Step 4
        for (i = 1; i <= n - 1; ++i)
        {
            l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * miu[i - 1];
            miu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }
        //Step 5
        l[n] = 1;
        z[n] = 0;
        c[n] = 0;
        //Step 6
        for (j = n - 1; j >= 0; --j) {
            c[j] = z[j] - miu[j] * c[j + 1];
            b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
            d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
        }
    }
    //Output
    //Notice that in main function, a,b,c,d begin at [1]
    for (i = n; i >= 0; --i)
    {
        a[i + 1] = a[i];
        b[i + 1] = b[i];
        c[i + 1] = c[i];
        d[i + 1] = d[i];
    }
}

double S( double t, double Fmax, int n, double x[], double a[], double b[], double c[], double d[])
{
    int i;
    //for a,b,c,d start at [1]
    double *aa = a + 1;
    double *bb = b + 1;
    double *cc = c + 1;
    double *dd = d + 1;
    if (t < x[0] || t > x[n])
    {
        return Fmax;
    }
    for (i = 0; i < n; ++i)
    {
        if (t == x[i]) 
        {
            return aa[i];
        }
        if (t > x[i] && t < x[i + 1])
        {
            return aa[i] + bb[i] * (t - x[i]) + cc[i] * (t - x[i]) * (t - x[i]) + dd[i] * (t - x[i]) * (t - x[i]) * (t - x[i]);
        }
    }
    return aa[n];
}
