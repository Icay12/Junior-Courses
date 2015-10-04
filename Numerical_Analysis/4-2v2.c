#include <stdio.h>
#include <math.h>

#define ZERO 1e-13 /* X is considered to be 0 if |X|<ZERO */
#define MAXN 11    /* Max Polynomial Degree + 1 */

double Polynomial_Root(int n, double c[], double a, double b, double EPS);

int main()
{
    int n;
    double c[MAXN], a, b;
    double EPS = 0.00005;
    int i;

    scanf("%d", &n);
    for (i=n; i>=0; i--) 
        scanf("%lf", &c[i]);
    scanf("%lf %lf", &a, &b);
    printf("%.4f\n", Polynomial_Root(n, c, a, b, EPS));

    return 0;
}

/* Your function will be put here */


double Fx(int n, double c[], double x)
{
    double p = 0.0;
    int i;
    for(i = n; i >= 0; --i)
    {
        p = p * x + c[i];
    }
    return p;
}

double dFx(int n, double c[], double x)
{
    double p = 0.0;
    int i;
    for(i = n; i >= 1; --i)
    {
        p = p * x + c[i] * i;
    }
    return p;
}

double d2Fx(int n, double c[], double x)
{
    double p = 0.0;
    int i;
    for(i = n; i >= 2; --i)
    {
        p = p * x + c[i] * i * (i-1);
    }
    return p;
}

double Polynomial_Root(int n, double c[], double a, double b, double EPS)
{
    const int MAX_ITERATION = 1000;
    double minError = 1000;
    double eps = EPS * 1e-7;
    double thisError, x, fx, dfx, d2fx, root;
    int i, j;

    if(a > b)
    {
        i = a;
        a = b;
        b = i;
    }

    for(i = 0; i < 10; ++i)
    {
        x = a + (b - a)*i/10;

        for(j = 0; j < MAX_ITERATION; ++j)
        {
            fx = Fx(n, c, x);
            dfx = dFx(n, c, x);
            d2fx = d2Fx(n, c, x);

            if( fabs(dfx*dfx - fx*d2fx)< eps)
                break;
            x = x - (fx*dfx)/(dfx*dfx - fx*d2fx);
            if(x < a || x > b)
                break;
        }

        thisError = fabs(Fx(n, c, x));
        if(x >= a && x <= b && thisError < minError)
        {
            root = x;
            minError = thisError;
        }
    }
        return root;

}
