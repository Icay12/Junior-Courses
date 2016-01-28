#include <stdio.h>

#define Max_size 10000 /* max number of dishes */

void Price( int n, double p[] );

int main()
{
    int n, i;
    double p[Max_size];

    scanf("%d", &n);
    for (i=0; i<n; i++) 
        scanf("%lf", &p[i]);
    Price(n, p);
    for (i=0; i<n; i++)
        printf("%.2f ", p[i]);
    printf("\n");

    return 0;
}

/* Your function will be put here */

//when n == 3

//难道是a没算对？
void Price( int n, double p[] )
{
    double a[Max_size], b[Max_size], c[Max_size];

    int i;
	double temp;

    //initialize
    for(i = 0; i < n; ++i)
    {
        a[i] = 0.5;
        b[i] = 2;
        c[i] = 0.5;
    }

    //1th  set all b's to 1s
    c[0] /= b[0];
    a[0] /= b[0];
    p[0] /= p[0];

    for(i = 1; i < n-2; ++i)
    {
        temp = b[i] - a[i]*c[i-1];
        c[i] /= temp;
        a[i] = (0 - a[i]*a[i-1])/temp;
        p[i] = (p[i] - p[i-1]*a[i])/temp;
    }
    temp = b[i] - a[i]*c[i-1];
    p[i] = (p[i] - p[i-1]*a[i])/temp;
    a[i] = (c[i] - a[i-1]*a[i])/temp;

    //2th set all c's to 0s
    for(i = n-3; i >= 0; --i)
    {
        a[i] -= a[i+1]*c[i];
        p[i] -= p[i+1]*c[i];
    }

    //set a[n-1] to 0
    i = n - 1;
    b[i] -= a[i-1]*a[i];
    p[i] -= p[i-1]*a[i];

    //solve
    p[n-1] = (p[0]-p[n-1]/c[n-1]) / (a[0]-b[n-1]/c[n-1]);

    for(i = 0; i < n-1; ++i)
    {
        p[i] = p[i] - a[i]*p[n-1];
    }

}


/*
void Price(int n, double p[]) {
	double a[Max_size], b[Max_size], c[Max_size];
	int i;
	double temp;	
	for(i = 0; i < n; i++) {
		a[i] = 0.50;
		b[i] = 2.0;
		c[i] = 0.50;
	}
	a[0] /= b[0];
	c[0] /= b[0];
	p[0] /= b[0];
	for(i = 1; i < n-1; i++) {
		temp = b[i] - a[i]*c[i-1];
		c[i] /= temp;
		p[i] = (p[i] - a[i]*p[i-1])/temp;
		a[i] = -a[i]*a[i-1]/temp;
	}
	a[n-2] = -a[n-2] - c[n-2];
	for(i = n-3; i >= 0; i--) {
		a[i] = -a[i] - c[i]*a[i+1];
		p[i] -= c[i]*p[i+1];
	}
	p[n-1] -= (c[n-1]*p[0] + a[n-1]*p[n-2]);
	p[n-1] /= (c[n-1]*a[0] + a[n-1]*a[n-2] + b[n-1]);
	for(i = n-2; i >= 0; i--)
		p[i] += a[i]*p[n-1];
}

*/

