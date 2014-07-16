#include <iostream>
#include "math.h"
using namespace std;

double d = 0.4;
double function_g(double x)
{
        double ex = pow(2.718281828,x);
        return ex/(1+ex);
}

double input[4][5]={{0,0,0,1,1},  //{0,0,0,1} {1}
                    {0,0,1,0,1},
                    {0,1,0,0,1},
                    {1,0,0,0,1}};

double edge_1[2][5];
double edge_2[4][3];
double first_layer[5];
double hidden_layer[3]={0,0,1};
double output_layer[4];

double diff()
{
        double ret = 0.0;
        for(int i=0;i<4;++i)
        {
                ret += (output_layer[i]-first_layer[i])*(output_layer[i]-first_layer[i]);
        }
        return ret;
}
void count_second_l( )
{
        for(int i=0;i<2;++i)
        {
                hidden_layer[i] = 0;
                for(int j=0;j<5;++j)
                {
                        hidden_layer[i] += first_layer[j]*edge_1[i][j];
                }
                hidden_layer[i] = function_g(hidden_layer[i]);
        }
        return;
}
void count_last_l()
{
        for(int i=0;i<4;++i)
        {
                output_layer[i] = 0;
                for(int j=0;j<3;++j)
                {
                        output_layer[i] += hidden_layer[j]*edge_2[i][j];
                }
                output_layer[i] = function_g(output_layer[i]);
        }
        return;
}
double total_diff()
{
        double ret = 0.0;
        for( int j=0;j<4;++j)
        {
                first_layer[0]=input[j][0];
                first_layer[1]=input[j][1];
                first_layer[2]=input[j][2];
                first_layer[3]=input[j][3];
                first_layer[4]=input[j][4];
                count_second_l();
                count_last_l();
                ret += diff();
        }
        return ret;
}

int main(void)
{
        for(int i=0;i<2;++i)
                for(int j=0;j<5;++j)
                        edge_1[i][j] = ((i+j)%4+0.1)/10;        //initialize each edges between first_layers and hidden layers

         for(int i=0;i<4;++i)
                for(int j=0;j<3;++j)
                        edge_2[i][j] = ((i+j)%4+0.1)/10;        //initialize each edges between hidden_layers and output_layers

        for(int i=0;i<40000;++i)                                //iterate 40000 times
        {
                double origin_diff=total_diff();
                double direction_1[2][5],direction_2[4][3];
                for(int i=0;i<2;++i)
                        for(int j=0;j<5;++j)
                        {
                                double tmp = edge_1[i][j] ;
                                edge_1[i][j] += d * origin_diff;
                                count_second_l();
                                count_last_l();
                                double diff2 = total_diff();
                                direction_1[i][j] = origin_diff - diff2;
                                edge_1[i][j] = tmp;
                        }
                for(int i=0;i<4;++i)
                        for(int j=0;j<3;++j)
                        {
                                double tmp = edge_2[i][j] ;
                                edge_2[i][j] += d * origin_diff;
                                count_second_l();
                                count_last_l();
                                double diff2 = total_diff();
                                direction_2[i][j] = origin_diff - diff2;
                                edge_2[i][j] = tmp;
                        }
                for(int i=0;i<2;++i)
                        for(int j=0;j<5;++j)
                        {
                                edge_1[i][j] += d * origin_diff * direction_1[i][j];
                        }
                for(int i=0;i<4;++i)
                        for(int j=0;j<3;++j)
                        {
                                edge_2[i][j] += d * origin_diff * direction_2[i][j];
                        }

                double d = 0.0;                
                for( int j=0;j<4;++j)
                {
                        first_layer[0]=input[j][0];
                        first_layer[1]=input[j][1];
                        first_layer[2]=input[j][2];
                        first_layer[3]=input[j][3];
                        first_layer[4]=input[j][4];
                        count_second_l();
                        count_last_l();
                        d += diff();
                        cout<<"####################"<<endl;
                        cout<<"input:\t"<<first_layer[0]<<"\t"<<first_layer[1]<<"\t"<<first_layer[2]<<"\t"<<first_layer[3]<<endl;
                        cout<<"output:\t"<<output_layer[0]<<"\t"<<output_layer[1]<<"\t"<<output_layer[2]<<"\t"<<output_layer[3]<<endl;
                        cout<<"hidden:\t"<<hidden_layer[0]<<"\t"<<hidden_layer[1]<<endl;
                        cout<<"####################"<<endl;
                }
                cout<<"diff_sum:"<<d<<endl;
        }
}
