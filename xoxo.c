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

struct node
{
    //long double value;
    long double weights[9];
    long double bias;
};
// 9I - 9 - 9O
struct node layer1[9];
struct node layerO[9];

long double sigmoid(long double x)
{
    return 1 / (1 + pow(2.71828182845904523536,-x));
}



void printx(long double arr[])
{
    printf(" %d | %d | %d\n", (int)arr[0], (int)arr[1], (int)arr[2]);
    printf("-----------\n");
    printf(" %d | %d | %d\n", (int)arr[3], (int)arr[4], (int)arr[5]);
    printf("-----------\n");
    printf(" %d | %d | %d\n", (int)arr[6], (int)arr[7], (int)arr[8]);
}

int won(long double arr[])   // return 1 if win or draw, 0 if game is left
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

    // draw, no spaces left
    for (int i = 0; i<9; i++) {
        if (arr[i]==0) {
            return 0;
        }
    }
    return 1;

}

void user_play(long double arr[])
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

long double cost(long double a[], long double b[]) {
    long double cost = 0;
    for (int i=0; i<9; i++) {
        cost += pow((a[i] - b[i]) , 2);
    }
    return cost;
}

void f_l(long double input[], struct node layer[], long double output[])
{
    long double sum;
    for (int i=0; i<9; i++)
    {
        sum = 0.0;
        sum += layer[i].bias;

        for (int j=0; j<9; j++)
        {
            sum += input[j]*layer[i].weights[j];
        }

        output[i] = sigmoid(sum);
        //printf("[%d] %Lf | %Lf\n", i, output[i], sum);
    }
}

void nn(long double input[], long double output[]) {
    long double layer1o[9];
    f_l(input, layer1, layer1o);
    f_l(layer1o, layerO, output);
}


// |||||||||||||||||||||||||||||||| BACKPROP ||||||||||||||||||||||||||||||||
void backprop(long double arr[], long double prd[]) { // input, predicted
    printf("\n----------\n");
    printx(arr);
    printx(prd);
    printf("----------\n");
    // --Note generate observed from input arr
    // learning rate = 0.2
    long double rate = -0.01;
    long double del = 0.0001;
    // cost = (observed_i - predicted_i)^2 for i in output_nodes
    // slope = yf - yi / x2 - x1 (0.0001)
    // find  Del. cost / Del. weight
    long double output[9];
    nn(arr, output);
    long double yi = cost( output, prd );
    printf("Ci: %Lf\n", yi);
    // now we go through every weight and change it DL. and find new cost.
    // this way we find slope of cost / weight, and record new weight in array,
    // and will apply all new weights in end
    long double w1[9][9];
    long double w2[9][9];
    long double b1[9];
    long double b2[9];
    // nested for loops to iterate through every weight
    //layer1
    for (int n = 0; n < 9; n++){
        layer1[n].bias+=0.1;
        nn(arr, output);
        long double yf = cost( output, prd );
        long double slope = (yf - yi) / 0.1;
        layer1[n].bias-=0.1;
        b1[n] = slope*rate;

        for (int i = 0; i < 9; i++) {
            layer1[n].weights[i]+=del;
            nn(arr, output);
            long double yf = cost( output, prd );
            //printf("Cf: %Lf\n", yf);
            long double slope = (yf - yi) / del;
            //printf("ow: %Lf", layer1[n].weights[i]);
            layer1[n].weights[i]-=del;
            w1[n][i] = slope*rate;
            //printf("[%d][%d] W:%Lf S:%Lf\n", n, i, layer1[n].weights[i], slope);
        }
    }
    //layer2
    for (int n = 0; n < 9; n++){
        layerO[n].bias+=0.1;
        nn(arr, output);
        long double yf = cost( output, prd );
        long double slope = (yf - yi) / 0.1;
        layerO[n].bias-=0.1;
        b2[n] = slope*rate;
        //printf("[%d] W:%Lf S:%Lf\n", n, layerO[n].bias, slope);
        for (int i = 0; i < 9; i++) {
            layerO[n].weights[i]+=del;
            nn(arr, output);
            long double yf = cost( output, prd );
            //printf("Cf: %Lf\n", yf);
            long double slope = (yf - yi) / del;
            layerO[n].weights[i]-=del;
            //printf("[%d][%d]: %Lf\n", n, i, slope*rate);
            w2[n][i] = slope*rate;
            //printf("[%d][%d] W:%Lf S:%Lf\n", n, i, layerO[n].weights[i], slope);
        }
    }

    // put stored values back into NN
    for (int n=0; n<9; n++) {
        layer1[n].bias += b1[n];
        layerO[n].bias += b2[n];
        for (int i=0; i<9; i++) {
            layer1[n].weights[i] += w1[n][i];
            layerO[n].weights[i] += w2[n][i];
printf("[%d][%d]: %Lf:%Lf | %Lf:%Lf\n",n,i,layer1[n].weights[i],w1[n][i],layerO[n].weights[i],w2[n][i]);
        }
    }


}

// for backprop, we will first record new weights and biases, and record it in arrays
// at the end, changes will be applied by looping


int max_index(long double arr[])
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


void ai_play(long double arr[])
{
    long double layerOo[9];
    long double inv[9] = {0.0};
    for (int i = 0; i<9; i++) {
        inv[i] = -1*arr[i];
    }
    nn(inv, layerOo); // AI thinks that -1 is opp that is why
    //long double temp[] = {0,0,0,1,1,1,0,0,0};
    //backprop(arr,temp);
    // select max from output

    //printf("cost: %Lf\n", cost(layerOo, temp));
    int max = max_index(layerOo);
    printf("max: %d\n", max+1);
    //printf("[%d]: %Lf\n", i+1, layer1o[i]);

    if (arr[max]==0)
    {
        arr[max] = -1;
    }
    else {
        long double inv[9] = {0.0};
        for (int i = 0; i<9; i++) {
            if (arr[i] == 0 ) {
                inv[i] = 1;
            }
            else {
                inv[i] = -1;
            }
        }
        printf("--- INV \n");
        printx(inv);
        backprop(arr, inv);
        ai_play(arr);

    } // teach AI to not place in already used places


}

void play(long double arr[])
{
    // take input from user
    if (won(arr) == 0)
    {
        ai_play(arr);
        printx(arr);
        long double inp[9];
        for (int  i =0; i<9; i++) {
            inp[i] = arr[i];
        }
        if (won(arr) == 0) {
            user_play(arr);}
        else goto wonx;

        backprop(inp, arr); //learn from user
        play(arr);
        return;
    }
    else {
        wonx:
        printf("---GAME OVER---\n");
        printx(arr);
        printf("---NEW GAME---\n");
        long double mash[9] = {0.0};
        play(mash);
        return;
    }

}

long double init_weight()
{
    return ((long double)rand())/((long double)RAND_MAX);
}

void init_wb(struct node layer[])
{
    for (int j = 0; j<9; j++)
    {
        layer[j].bias = (long double)0.0;
        for (int i =0; i<9; i++)
        {
            layer[j].weights[i] = (long double)init_weight();
        }
    }
}

void main(void)
{
    long double mash[9] = {0.0};
    // initalize weights and biases
    init_wb(layer1);
    init_wb(layerO);
    //printx(mash);
    play(mash);

}
