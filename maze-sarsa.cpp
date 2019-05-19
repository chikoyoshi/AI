#include <iostream>
#include <random>

const int MAX_X = 11;           //迷路のxサイズ
const int MAX_Y = 8;            //迷路のyサイズ
const int MAX_EPI = 1000;       //エピソード数
const int MAX_STEP = 200;       //ステップ数
const int ACT_NUM = 4;          //行動数

//スタート地点
int sx = 1;
int sy = 1;

// 0:壁, 1:床, 2:ゴール, 3:崖
int s[MAX_Y][MAX_X] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 1, 1, 3, 3, 3, 3, 3, 1, 1, 0},
    {0, 1, 1, 3, 3, 3, 3, 3, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// TD学習用パラメータ
double alpha = 0.2;                 //学習率    
double my_gamma = 0.9;                 //割引率
double epsilon = 0.3;
double Q[MAX_Y][MAX_X][ACT_NUM];    //行動価値

//乱数生成
double my_random(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    return (double)mt() / (double)mt.max();
}

//sarsa
void sarsa(int x, int y, int a, double r, int nx, int ny, int ns){
    Q[y][x][a] = Q[y][x][a] + alpha * (r + my_gamma * Q[ny][nx][ns] - Q[y][x][a]);
}

//行動価値の初期化
void init(){
    for (int i=0; i<MAX_Y; i++){
        for (int j=0; j<MAX_X; j++){
            for (int k=0; k<ACT_NUM; k++){
                Q[i][j][k] = my_random() - 0.5;        //[-0.5,0.5)の範囲で初期化
            }
        }
    }
}

//行動の出力
int action(int x, int y){
    int k;
    int act;

    if (my_random() < epsilon){                        //確率0.3でランダム行動
        return (int)(my_random() * ACT_NUM);
    } else {                                        //greedy
        double max = Q[y][x][0];
        act = 0;
        for (k=1; k<ACT_NUM; k++){
            if (max < Q[y][x][k]){
                max = Q[y][x][k];
                act = k;
            }
        }
        return act;
    }
}

//行動を矢印で表示
void showArrow() {
    int i, j, k;
    int argmaxQ;
    double maxQ;

    for (i=1;i<MAX_Y-1;i++){
        for (j=1;j<MAX_X-1;j++){
            switch (s[i][j]){
                case 1:
                {
                    argmaxQ = 0;
                    maxQ = Q[i][j][0];
                    for (k=1;k<ACT_NUM;k++){
                        if (maxQ < Q[i][j][k]){
                            argmaxQ = k;
                            maxQ = Q[i][j][k];
                        }
                    }
                    switch (argmaxQ) {
                        case 0:
                            std::cout << "↑";
                            break;
                        case 1:
                            std::cout << "↓";
                            break;
                        case 2:
                            std::cout << "←";
                            break;
                        default:
                            std::cout << "→";
                            break;
                    }
                    break;
                }
                case 2:
                    std::cout << "G";
                    break;
                case 3:
                    std::cout << "×";
                    break;
                default:
                    break;
            }
        }
        std::cout << "\n";
    }
}

int main() {
    int epi, step;
    int x, y, a, nx, ny, na;

    init();

    for (epi = 0; epi < MAX_EPI; epi++){
        x = sx;
        y = sy;
        nx = sx;
        ny = sy;
        na = 0;

        a = action(x,y);
        for (step = 0; step < MAX_STEP; step++){
            // 行動aを起こし,報酬rと次状態s'を観測
            switch(a){
                case 0:
                    if (s[y-1][x] != 0) ny--;
                    break;
                case 1:
                    if (s[y+1][x] != 0) ny++;
                    break;
                case 2:
                    if (s[y][x-1] != 0) nx--;
                    break;
                case 3:
                    if (s[y][x+1] != 0) nx++;
                    break;
            }

            na = action(nx, ny);    //次状態における行動aを決定

            //行動価値Qを更新
            if (s[y][x] == 2){
                sarsa(x,y,a,1,nx,ny,na);
                break;
            } else if (s[y][x] == 3) {
                sarsa(x,y,a,-100,nx,ny,na);
                break;
            } else {
                sarsa(x,y,a,-1,nx,ny,na);
            }
            x = nx;
            y = ny;
            a = na;
        }
    }
    showArrow();
    return 0;
}
