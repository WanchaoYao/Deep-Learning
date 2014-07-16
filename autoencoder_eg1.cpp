// Reference: http://blog.sina.com.cn/s/blog_593af2a70101endk.html

#include <iostream>
#include <cmath>
#include <random>
#include <cstring>
#include <unistd.h>
using namespace std;

double sigmoid(int x)
{
    return 1.0 / (1.0 + exp(-x));
}

double loss(double *input_layer, double *output_layer, int size_output)
{
    double loss = 0.0;
    for (int i = 0; i < size_output; i++)
    {
        loss += (input_layer[i] - output_layer[i]) * (input_layer[i] - output_layer[i]);
    }
    return loss;
}

void cal_hidden(double *input_layer, int size_input, double *hidden_layer, int size_hidden, double **w_input_hidden)
{
    for (int i = 0; i < size_hidden - 1; i++)
    {
        hidden_layer[i] = 0.0;
        for (int j = 0; j < size_input; j++)
        {
            hidden_layer[i] += w_input_hidden[j][i] * input_layer[j];
        }
        hidden_layer[i] = sigmoid(hidden_layer[i]);
    }
}   

void cal_output(double *hidden_layer, int size_hidden, double *output_layer, int size_output, double **w_hidden_output)
{
    for (int i = 0; i < size_output; i++)
    {
        output_layer[i] = 0.0;
        for (int j = 0; j < size_hidden; j++)
        {
            output_layer[i] += w_hidden_output[j][i] * hidden_layer[j];
        }
        output_layer[i] = sigmoid(output_layer[i]);
    }
}

void cal_delta_output(double *input_layer, double *output_layer, double *delta_output, int size_output)
{
    for (int i = 0; i < size_output; i++)
    {
        delta_output[i] = -(input_layer[i] - output_layer[i]) * output_layer[i] * (1 - output_layer[i]);
    }
}

void cal_delta_hidden(double **w_hidden_output, double *delta_hidden, double *delta_output, double *hidden_layer, int size_output, int size_hidden)
{
    for (int i = 0; i < size_hidden; i++)
    {
        delta_hidden[i] = 0.0;
        for (int j = 0; j < size_output; j++)
        {
            delta_hidden[i] += w_hidden_output[i][j] * delta_output[j];
        }
        delta_hidden[i] *= hidden_layer[i] * (1 - hidden_layer[i]);
    }
}

void initilize_weight(double **w, int size_input, int size_output)
{

    default_random_engine generator;
    normal_distribution<double> distribution(0.0, 0.01);

    for (int i = 0; i < size_input; i ++)
        for (int j = 0; j < size_output; j++)
            w[i][j] = distribution(generator);
}

int main(void)
{
    const int size_input = 5, size_hidden = 3, size_output = 4;
    const int size_training_set = 4;

    double training_set[size_training_set][size_input] = {{0.0, 0.0, 0.0, 1.0, 1.0}, // {0.0, 0.0, 0.0, 1.0} {1.0}
                                                          {0.0, 0.0, 1.0, 0.0, 1.0},
                                                          {0.0, 1.0, 0.0, 0.0, 1.0},
                                                          {1.0, 0.0, 0.0, 0.0, 1.0}};

    double input_layer[size_input], output_layer[size_output], hidden_layer[size_hidden] = {0.0, 0.0, 1.0}; // {#, #} {1.0}
    double **w_input_hidden = new double*[size_input], **w_hidden_output = new double*[size_hidden];
    double **w_input_hidden_update = new double*[size_input], **w_hidden_output_update = new double*[size_hidden];
    double delta_output[size_output], delta_hidden[size_hidden];
    
    const double alpha = 0.4;

    for (int i = 0; i < size_input; i++) 
    {
        w_input_hidden[i] = new double[size_hidden];
        w_input_hidden_update[i] = new double[size_hidden];
    }

    for (int i = 0; i < size_hidden; i++)
    {
        w_hidden_output[i] = new double[size_output];
        w_hidden_output_update[i] = new double[size_output];
    }

    initilize_weight(w_input_hidden, size_input, size_hidden);
    initilize_weight(w_hidden_output, size_hidden, size_output);

    for (int count = 0; count < 40000; count++) 
    {
        cout << "***** Iteration " << count + 1 << " : *****" << endl;

        for (int i = 0; i < size_hidden; i++)
            for (int j = 0; j < size_output; j++)
               w_hidden_output_update[i][j] = w_hidden_output[i][j];

        for (int i = 0; i < size_input; i++)
            for (int j = 0; j < size_hidden; j++)
                w_input_hidden_update[i][j] = w_input_hidden[i][j];

        double total_loss = 0.0;
        for (int m = 0; m < size_training_set; m++)
        {
            memcpy(input_layer, training_set[m], size_input * sizeof(double));
            cal_hidden(input_layer, size_input, hidden_layer, size_hidden, w_input_hidden);
            cal_output(hidden_layer, size_hidden, output_layer, size_output, w_hidden_output);
            cal_delta_output(input_layer, output_layer, delta_output, size_output);
            cal_delta_hidden(w_hidden_output, delta_hidden, delta_output, hidden_layer, size_output, size_hidden);
            
            for (int i = 0; i < size_hidden; i++)
                for (int j = 0; j < size_output; j++)
                   w_hidden_output_update[i][j] -= alpha * 1.0 / size_training_set * hidden_layer[i] * delta_output[j];

            for (int i = 0; i < size_input; i++)
                for (int j = 0; j < size_hidden; j++)
                    w_input_hidden_update[i][j] -= alpha * 1.0 / size_training_set * input_layer[i] * delta_hidden[j];
        
            total_loss += loss(input_layer, output_layer, size_output);

            cout << "Input: ";
            for (int i = 0; i < size_input - 1; i++)
                cout << input_layer[i] << " ";
            cout << endl;

            cout << "Output: ";
            for (int i = 0; i < size_output; i++)
                cout << output_layer[i] << " ";
            cout << endl;
            
            cout << "Hidden: ";
            for (int i = 0; i < size_hidden - 1; i++)
                cout << hidden_layer[i] << " ";
            cout << endl;

            cout << "loss: " << loss(input_layer, output_layer, size_input) << endl;
        }

        cout << "w_ij^(1): ";
        for (int i = 0; i < size_input; i++)
            for (int j = 0; j < size_hidden; j++)	
            {
                w_input_hidden[i][j] = w_input_hidden_update[i][j];
                cout << w_input_hidden[i][j] << " ";
            }
        cout << endl;
    
        cout << "w_ij^(2): ";
        for (int i = 0; i < size_hidden; i++)
            for (int j = 0; j < size_output; j++)
            {
                w_hidden_output[i][j] = w_hidden_output_update[i][j];
                cout << w_hidden_output[i][j] << " ";
            }
        cout << endl;

        cout << "Total loss: " << total_loss << endl << endl;
    }
   
    return 0;
}
