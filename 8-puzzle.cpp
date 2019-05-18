#include <iostream>
#include <algorithm>
#include <vector>

const int N = 9;
const int UP = 0;
const int DOWN = 1;
const int RIGHT = 2;
const int LEFT = 4;

typedef struct node{
    int *field;
    int cost;
}NODE;

//0の位置を引数で与え，動かすことのできる方向をintで返す.
int move(int n){
    int hoge,ret=-1;
    hoge = n % 3;
    switch (hoge){
        case 0: ret = LEFT;
        case 1: ret = LEFT + RIGHT;
        case 2: ret = RIGHT;
    }
    hoge = n / 3;
    switch (hoge){
        case 0: ret += UP;
        case 1: ret += UP + DOWN;
        case 2: ret += DOWN;
    }

    return ret;
}

//正誤数をintで返す
int h(int *field, int *ans){
    int count = 0;
    for (int i=0;i<N;i++){
        if (field[i] == ans[i]) count++;
    }
    return count;
}

//0の位置を返す関数
int zeroPosition(int *field){
    for (int i=0; i<N; i++){
        if (field[i] == 0)  return i;
    }
}

int main(){

    std::vector<NODE> open;

    int field[9] =
    {
        4, 1, 3,
        7, 2, 6,
        5, 0, 8
    } ;
    int ans[9] = 
    {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    //1.初期設定をopenに設定
    //2.ループ開始

    while (!open.empty){
        

    }



}