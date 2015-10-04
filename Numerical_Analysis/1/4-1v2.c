#include <stdio.h>

void Series_Sum( double sum[] );

int main()
{
    int i;
    double x, sum[3001];
    
    Series_Sum( sum );

    x = 0.0;
    for (i=0; i<3001; i++)
        printf("%6.2f %16.12f\n", x + (double)i * 0.10, sum[i]);

    return 0;
}
//in this version the denominator is k^4
void Series_Sum( double sum[] )
{
    int i;
    double k;
    double x = 0.00;
    double dsum = 0.0;
    //calculate tail
    for(i=14000; i>10000; i--) 
    {
        dsum+= 1/((double)(i)*(double)(i)*(double)(i+1)*(double)(i+2)*(double)(i+3)*(double)(i+4));
	}
    //calculate when x = 0.1,0.2 ... 0.8,0.9
    for(i = 0; i < 10; ++i)
    {
        sum[i] = dsum;
        for(k = 10000; k >= 1; --k)
        {
            sum[i]+=1.0/(k*(k+1)*(k+2)*(k+3)*(k+x)*(k+4));
        }

        sum[i]=((2.0-x)*( (3.0-x)*  ((4.0-x)*sum[i]+1.0/96)+1.0/18)+0.25)*(1.0-x)+1.0;
        x += 0.10;
    }
    //calulater others
	for( i = 10; i < 3001; i++){
        int afterDot = i % 10;
        int beforeDot = i / 10;
		int j;
        double diff = 0.0;
        double decimal = afterDot*0.1;
        // not an integer
        if(decimal != 0){
            for(j= 1; j <= beforeDot; ++j){
                diff += 1.0 /((double)(j) + decimal);
            }
        
            sum[i] = (sum[afterDot]*decimal + diff)/x;
        }

        //integer
        else{
            for(j = 1; j <= beforeDot ; ++j){
                diff += 1/(double)(j);
            }
            sum[i] = diff/beforeDot;
        }
        x += 0.10;
    }
}
