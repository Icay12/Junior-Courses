#include <stdio.h>
#include <math.h>

double f0( double x, double l, double t )
{
    return sqrt(1.0+l*l*t*t*cos(t*x)*cos(t*x));
}

double Integral(double a, double b, double (*f)(double x, double y, double z), 
double eps, double l, double t);

int main()
{
    double a=0.0, b, eps=0.005, l, t;

    scanf("%lf %lf %lf", &l, &b, &t);
    printf("%.2f\n", Integral(a, b, f0, eps, l, t));

    return 0;
}

/* Your function will be put here */
#define PI 3.141592653589
#define MAXI 20
double Romberg(double a, double b, double (*f)(double x, double y, double z), 
double eps, double l, double t)
{
	double R[3][MAXI+1];
	int i, j, k;

	//Step 1
	double h = b - a;
	R[1][1] = h / 2.0 * (f(a, l, t) + f(b, l, t));
	//Step 2

	//Step 3 loop
	for (i = 2; i <= MAXI; ++i)
	{
		//Step 4
		double sum = 0.0;
		for (k = 1; k <= (1 << (i - 2)); ++k)
		{
			sum += f((a + ((double)k - 0.5) * h), l, t);
		}
		R[2][1] = 0.5 * (R[1][1] + h * sum);

		//Step 5
		for (j = 2; j <= i; ++j)
		{
			R[2][j] = R[2][j - 1] + (R[2][j - 1] - R[1][j - 1]) / (double)(1 << (2 * j - 2) - 1);
		}
		//Step 6

		//Step 7
		h = h / 2.0;
		//Step 8
		for(j = 1; j <=i; ++j)
		{
			R[1][j] = R[2][j];
		}
	}
	return R[1][MAXI];
}

double Integral(double a, double b, double (*f)(double x, double y, double z), 
double eps, double l, double t)
{
	//count cycles
	double T = 2.0 * PI / t;
	int count_T = (int)(b / T);
	double remain = b - (double)count_T * T;

	double length_T = Romberg(0.0, T, f0, eps, l, t);
	double length_r = Romberg(0.0, remain, f0, eps, l, t);

	double result = (length_T * count_T + length_r) / 100.0;
	return result;
}

