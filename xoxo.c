#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//  0 0 0  1 2 3
//  0 0 0  4 5 6
//  0 0 0  7 8 9

// player is 1; whereas AI is -1

// random move
void rand_play(int arr[])
{
    int ran = rand() % 9;
    if (arr[ran - 1 ] == 0)
    {
        arr[ran - 1] = -1;
    }
    else rand_play(arr);
}

typedef struct node
{
    double value;
    double weights[9];
    double bias;
};
// 9I - 9 - 9O
struct node layer1[9];
struct node layerO[9];

double sigmoid(double x)
{
    return 1 / (1 + pow(2.71828182845904523536,-x));
}



void printx(double arr[])
{
    printf(" %d | %d | %d\n", (int)arr[0], (int)arr[1], (int)arr[2]);
    printf("-----------\n");
    printf(" %d | %d | %d\n", (int)arr[3], (int)arr[4], (int)arr[5]);
    printf("-----------\n");
    printf(" %d | %d | %d\n", (int)arr[6], (int)arr[7], (int)arr[8]);
}

int won(double arr[])   // return 1 if win or draw, 0 if game is left
{
    // 0 1 2
    // 3 4 5
    // 6 7 8

    // Straight
    // Vertical
    for (int i=0; i<3; i++){
        if (arr[i]==arr[i+3] && arr[i+3]==arr[i+6] && (arr[i]==1 || arr[i]==-1)) {
            return 1;
        }}

    // Horizontal
    for (int i=0;i<=6;i+=3) {
        if (arr[i]==arr[i+1] && arr[i+1]==arr[i+2] && (arr[i]==1 || arr[i]==-1)) {
            return 1;
        }}

    //Diagonal
    // right to left
    if (arr[0]==arr[4] && arr[4]==arr[8] && (arr[4]==1 || arr[4]==-1)) {
            return 1;
        }
    // left to right
    if (arr[2]==arr[4] && arr[4]==arr[6] && (arr[4]==1 || arr[4]==-1)) {
            return 1;
        }
    return 0;

}

void user_play(double arr[])
{
    int num;
    printf("enter num: ");
    scanf("%d", &num);
    if (arr[num - 1] == 0)
        arr[num-1] = 1;
    else
    {
        printf("select vacant spot\n");
        user_play(arr);
    }
}

double cost(double a[], double b[]) {
    double cost = 0;
    for (int i=0; i<9; i++) {
        cost += pow((a[i] - b[i]) , 2);
    }
    return cost;
}

void f_l(double input[], struct node layer[], double output[])
{
    double sum;
    for (int i=0; i<9; i++)
    {
        sum = 0.0;
        sum += layer[i].bias;

        for (int j=0; j<9; j++)
        {
            sum += input[j]*layer[i].weights[j];
        }

        output[i] = sigmoid(sum);
        printf("[%d] %f | %f\n", i, output[i], sum);
    }
}

void nn(double input[], struct node lay1[], struct node lay2[], double output[]) {
    double layer1o[9];
    // feed forward 1st layer
    f_l(input, layer1, layer1o);
    // 2nd layer
    f_l(layer1o, layerO, output);
}

void nn(double input[], double output[]) {
    double layer1o[9];
    f_l(input, layer1, layer1o);
    f_l(layer1o, layerO, output);
}

void backprop(double arr[], double prd[]) { // input, predicted
    // --Note generate observed from input arr

    // cost = (observed_i - predicted_i)^2 for i in output_nodes
    // slope = yf - yi / x2 - x1 (0.0001)
    // find  Del. cost / Del. weight
    double output[]
    nn(arr, output)
    double yi = cost( output, prd )

    // now we go through every weight and change it DL. and find new cost.
    // this way we find slope of cost / weight, and record new weight in array,
    // and will apply all new weights in end


}

// for backprop, we will first record new weights and biases, and record it in arrays
// at the end, changes will be applied by looping

//


int max_index(double arr[])
{
    int max = 0;
    for (int i=0; i<9; i++)
    {
        if (arr[i]>arr[max])
        {
            max = i;
        }
    }
    return max;
}


void ai_play(double arr[])
{
    double layerOo[9];
    nn(arr, layer1, layerO, layerOo);
    // select max from output
    double temp[] = {0,0,0,1,1,1,0,0,0};
    //printf("cost: %f\n", cost(layerOo, temp));
    int max = max_index(layerOo);
    printf("max: %d\n", max);
    //printf("[%d]: %f\n", i+1, layer1o[i]);

    if (arr[max]==0)
    {
        arr[max] = -1;
    }

}

void play(double arr[])
{
    // take input from user
    if (won(arr) == 0)
    {
        ai_play(arr);
        printx(arr);
        user_play(arr);
        play(arr);
        return;
    }
    else {
        printf("---GAME OVER---\n");
        printx(arr);
        return;
    }

}

double init_weight()
{
    return ((double)rand())/((double)RAND_MAX);
}

void init_wb(struct node layer[])
{
    for (int j = 0; j<9; j++)
    {
        layer[j].bias = (double)0.0;
        for (int i =0; i<9; i++)
        {
            layer[j].weights[i] = (double)init_weight();
        }
    }
}

int main(void)
{
    double mash[9] = {0.0};
    // initalize weights and biases
    init_wb(layer1);
    init_wb(layerO);
    //printx(mash);
    play(mash);

    return 0;
}
