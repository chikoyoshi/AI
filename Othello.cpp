/*
オセロゲームのクラス
*/
class Othello
{
    private :
    
    const int SIDE = 10;
    const int BOARDSIZE = SIDE * SIDE;
    const int B = 1;
    const int W = 2;
    const int N = 3;
    const int BLACK_TURN = 0;
    const int WHITE_TURN = 1;
    int board[BOARDSIZE];
    int evalboard[BOARDSIZE];

    public :

    Othello(){
        
    }
}