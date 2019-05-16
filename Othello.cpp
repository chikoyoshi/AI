/*
オセロゲームのクラス
*/


//--------------------------------ゲームに関する定数
const int SIDE = 10;
const int BOARDSIZE = SIDE * SIDE;
const int B = 1;
const int W = 2;
const int N = 3;
const int BLACK_TURN = 0;
const int WHITE_TURN = 1;
const char* piece[3] = { "  ", "●", "○" };
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
const unsigned char boardOrg[BOARDSIZE] = 
        {
            N, N, N, N, N, N, N, N, N, N, 
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, 0, 0, 0, B, W, 0, 0, 0, N,
            N, 0, 0, 0, W, B, 0, 0, 0, N,
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, 0, 0, 0, 0, 0, 0, 0, 0, N,
            N, N, N, N, N, N, N, N, N, N   
        };

const int evalboardOrg[BOARDSIZE] = 
        {
            0,   0,   0,   0,  0,  0,  0,   0,     0,   0,
            0,  45, -11,   4, -1, -1,  4, -11,    45,   0,
            0, -11, -16,  -1, -3, -3,  2, -16,   -11,   0,
            0,   4,  -1,   2, -1, -1,  2,  -1,     4,   0,
            0,  -1,  -3,  -1,  0,  0, -1,  -3,    -1,   0,
            0,  -1,  -3,  -1,  0,  0, -1,  -3,    -1,   0,
            0,   4,  -1,   2, -1, -1,  2,  -1,     4,   0,
            0, -11, -16,  -1, -3, -3, -1, -16,   -11,   0,
            0,  45, -11,   4, -1, -1,  4, -11,    45,   0,
            0,   0,   0,   0,  0,  0,  0,  0,      0,   0

        };        

//--------------------------------

class Othello
{
    private :

    //---------------------------------ボード
    int board[BOARDSIZE];
    unsigned char evalboard[BOARDSIZE];
    //---------------------------------

    public :

    Othello(){
        memcpy(board, boardOrg, sizeof(board));
        memcpy(evalboard, evalboardOrg, sizeof(evalboard));
    }

    int getPosition(int x,int y){
        return y * SIDE + x;
    }

    void display() {
        int x,y;
        std::cout << "   a  b  c  d  e  f  g  h " << std::endl;
        for (y=1;y<=(SIDE-2);y++) {
            std::cout << y << " |" ;
            for (x=1;x<=(SIDE-2);x++)
                std::cout << piece[board[getPosition(x,y)]] ;
            std::cout << "\n";
        }
    }
};