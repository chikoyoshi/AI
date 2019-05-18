#ifndef _OTHELLO_H_
#define _OTHELLO_H_


const int SIDE = 8;                     // 一辺の長さ 
const int ASIDE = (SIDE + 2);           // 局面用配列の一辺 緩衝地帯分２を足す Side for array, add 2
const int BOARDSIZE = (ASIDE * ASIDE);  // ボードの大きさ　Size of board
const int UP = (-ASIDE);
const int DOWN = ASIDE;
const int RIGHT = 1;
const int LEFT = (-1);
const int B = 1;                        // BLACK 1
const int W = 2;                        // WHITE 2
const int N = 3;                        // 番兵 Sentinel (out of board) 3
const int BLACK_TURN = 0; 
const int WHITE_TURN = 1;

#define X(pos) (pos % ASIDE)           //座標からX座標またはY座標を計算するマクロ
#define Y(pos) (pos / ASIDE)
#define TURNCOLOR( turn ) (turn + 1)
#define OPPONENT(turn) !turn

const int TRUE = 1;
const int FALSE = 0;

const int MOVENUM = 32;
const int PASSMOVE = 99;            //パス手には99を入れる 
const int MAXDEPTH = 5;             //探索を行う最大深さ
const int INFINITY = 1000;          //十分大きい数を無限大として扱う
const int SEARCH_LIMIT_DEPTH = 128; //探索深さの上限

extern const char* piece[3];    //= { "  ", "●", "○" };       //表示に使う文字定数
extern const char* abc[8];      //{ "a","b","c","d","e","f","g","h"};
extern int turn;                    //ターン保存用 
extern int ply;                     //手数

typedef char Move;                  //手だとわかりやすくするため型を宣言（しなくてもよい）
extern Move nextmove;               //次の手 
extern unsigned char stonenum[2];   //石の数を記憶する変数

struct Position //過去の局面などを記憶するための構造体
{
	unsigned char board[BOARDSIZE];
	unsigned char stonenum[2];
};
extern struct Position history[SEARCH_LIMIT_DEPTH];     //履歴保存用
extern int evalboard[BOARDSIZE] ;       //フィールドの評価値
extern unsigned char board[BOARDSIZE];  //フィールドの変数

int getPosition(int, int);              //2次元の座標を一次元に変換
void output();                      //表示関数 display function
int isLegalMove(Move);              //合法手かどうか判定する関数 
int generateMoves(Move *);          //生成した手の数を返している 
int isTerminalNode(int);            //ゲーム終了局面ならTRUEを返す
int getTerminalValue();             //ゲーム終了局面の評価値を返す
int getEvaluationValue();           //評価関数の計算
void makeMove(Move, int);           //実際に手を進める
void unmakeMove(int);               //手を戻す
int search(int);                    //探索して最も評価の高い手を選ぶ
void comPlayer();                   //COMの手を生成する関数
Move manPlayer();                   //人間の入力を管理する関数
Move randplayer();                  //ランダムに手を返す
void init();                        //グローバル変数などを初期化

#endif //_OTHELLO_H_