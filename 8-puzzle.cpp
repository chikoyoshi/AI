#include <iostream>
#include <algorithm>
#include <vector>

const int N = 9;
const int UP = 1;
const int DOWN = 10;
const int RIGHT = 100;
const int LEFT = 1000;

typedef struct node{
    int *field;
    int cost;
    int lastmove;
}NODE;

//0の位置を引数で与え，動かすことのできる方向をintで返す.
int canMove(int n){
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
    std::vector<NODE> close;
    int *n;
    int canmove = -1;
    int lastmove = -1;
    NODE temp;

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

    for (int i=0;;i++){
        if (!open.empty)    break;          //オープンリストが空のとき
        n = open.at(1).field;
        if (h(n,ans) == N-1)    break;      //取り出したものが解のとき
        open.erase(open.begin);             //オープンリストからノードを削除

        //nを展開してオープンリストに追加そしてソート
        canmove = canMove(zeroPosition(field));

        //上方向の移動
        if (canmove % 10){
            temp.lastmove = UP;
            temp.field
        }
    }



}