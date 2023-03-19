#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//  0 0 0  1 2 3
//  0 0 0  4 5 6
//  0 0 0  7 8 9

// player is 1; whereas AI is -1

struct node {
  double weights[9];
  int bias;
};

double init_weight() { return ((double)rand())/((double)RAND_MAX); }
double sigmoid(double x) { return 1 / (1 + pow(2.71828182845904523536,-x)); }
struct node layer1[9];
struct node output[9];

void printx(int arr[]) {
  printf(" %d | %d | %d\n", arr[0], arr[1], arr[2]);
  printf("-----------\n");
  printf(" %d | %d | %d\n", arr[3], arr[4], arr[5]);
  printf("-----------\n");
  printf(" %d | %d | %d\n", arr[6], arr[7], arr[8]);
}

int won() { // return 1 if win or draw, 0 if game is left
  return 0;
  // if any line is made

  // diagnol
 // if
}

void user_play(int arr[]) {
  int num;
  printx(arr);
  printf("enter num: ");
  scanf("%d", &num);
  if (arr[num - 1] == 0)
    arr[num-1] = 1;
  else {
    printf("select vacant spot\n");
    user_play(arr);}
}



int max_index(double arr[]) {
  int max = 0;
  for (int i=0; i<9; i++) {
    if (arr[i]>arr[max]) {
        max = i;
    }
  }
  return max;
}
// forward layer 1
int f_l1(int input[], int output[]) {
    double sum = 0;
    for (int i=0; i<9; i++) {
        sum = 0;
        sum += layer1[i].bias;
        for (int j=0; j<9; j++) {
            sum += (double)input[j]*layer1[i].weights[j];
        }
    output[i] = sigmoid(sum);
    }
}

void ai_play(int arr[]) {
  // iterate through nodes in 1 st layer
  double sum = 0;
  double layer1o[9];
  f_l1(arr, layer1o);
  // select max from output

  int max = max_index(layer1o);
  //printf("[%d]: %f\n", i+1, layer1o[i]);

  if (arr[max]==0) {
    arr[max] = -1;
  }

}


void rand_play(int arr[]) {
  int ran = rand() % 9;
  if (arr[ran - 1 ] == 0) {
    arr[ran - 1] = -1;
  }
  else ai_play(arr);
}

void play(int arr[]) {
  // take input from user
  if (won(arr) == 0) {
    ai_play(arr);
    //rand_play(arr);
    user_play(arr);
    play(arr);
  }
  else return;
}


int main(void) {
  int mash[9] = {0};
  // initalize weights and biasesi
  for (int j = 0; j<9; j++) {
    layer1[j].bias = 0;
    for (int i =0; i<9; i++){
      layer1[j].weights[i] = init_weight();
      //printf("%f", layer1[0].weights[i]);
  }}
  //printx(mash);
  play(mash);

  return 0;
}
