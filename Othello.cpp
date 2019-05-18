/*******************************************************************

	オセロ プログラム　　　　 
	
	(高専のときのやつ)

*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>//memcpy
#include "Othello.h"

const char* piece[3] = { "  ", " ●", " ○" }; 
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
int ply;
int turn;
unsigned char stonenum[2];
struct Position history[SEARCH_LIMIT_DEPTH];
Move nextmove;

int evalboard[BOARDSIZE] =
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
///////////////////////////AI用に追加　ここまで

//2次元の座標を一次元に変換
int getposition( int x, int y ){ return y * ASIDE + x; };
//一番大事な変数、ここでは一次元で表現
unsigned char board[BOARDSIZE] = //intでも良いが、データ格納のことを考えてcharにしている
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
// 表示関数 display function
void output()
{
	int x,y;
	printf( "   a  b  c  d  e  f  g  h \n" );
	for ( y = 1; y <= SIDE; y++ )
	{
		printf( "%d |", y );
		for ( x = 1; x <= SIDE; x++ )
			printf( "%s|", piece[board[getposition(x,y)]] );//ちょっと簡潔に書くためのヒント
		printf( "\n" );
	}
}

int isLegalMove(Move pos)
{
	int dirx, diry, dir;
	int pos1;
	// 自分の色、相手の色は何か変数に入れておく
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR(OPPONENT(turn));

	if ( board[pos] != 0 )//空きマスでないか
		return FALSE;
	
	// posの周り8方向を調べ相手石が取れるか調べる
	// 8方向ループの書き方1
	for ( dirx = -1; dirx <= 1; dirx++ )
	{
		for ( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			dir = dirx + diry;
			if ( dir == 0 )
				continue;
			pos1 = pos + dir;//posの隣のマス
			//以下は board[pos1] の中身をチェックしながら、
			//pos1を +=dirしていく
			//だめなら continue
			if ( board[pos1] != opponentcolor)//相手の石があるか
				continue;
			do // 相手の石がある間は次を調べる
			{
				pos1 += dir;
			}while ( board[pos1] == opponentcolor );
			// ひっくり返すためには最後に自分の石がないといけない
			if ( board[pos1] != color )
				continue;
			//最後まで来たら成功
			return TRUE;
		}
	}
	return FALSE;	
}

int generateMoves( Move moves[] )	
{
	int num = 0;//生成する合法手の数
	int pos;
	// 左上から順に石を置き、合法手か調べる
	for ( pos = 11; pos <= 88; pos++ )
	{
		if (isLegalMove(pos))
			moves[num++] = pos;//num番目の配列に
	}
	return num;
}

int isTerminalNode( int isPass )
{
	int num;
	Move moves[MOVENUM];
	if ( !isPass )
	{
		num = generateMoves(moves);
		if ( num > 0 )
			return FALSE;

	}
	turn = OPPONENT(turn);
	num = generateMoves(moves);
	turn = OPPONENT(turn);
	if ( num == 0 )
		return TRUE;
	return FALSE;
}

int getTerminalValue()
{
	//石数の差を計算　自分の石ー相手の石
	int diff = stonenum[turn] - stonenum[OPPONENT(turn)];

	if (diff > 0){
		return INFINITY;
	}
	else if(diff < 0){
		return -INFINITY;
	}
	else
		return 0;
}

int getEvaluationValue()
{
	int pos, value, c;
	Move moves[MOVENUM];
	// 合法手数の差を評価関数とする(自由度)
	value = generateMoves(moves);//自分の合法手数を足す
	turn = OPPONENT(turn);
	//相手の合法手数を引く
	value -= generateMoves(moves);
	//手番を戻す
	turn = OPPONENT(turn);

	value *= 30;//自由度１を30点としておく（適当）
	for ( pos = 11; pos <= 88; pos++ )
	{
		c = board[pos];
		if ( c == 0 )
			continue;
		else if ( c == TURNCOLOR(turn) )
			value += evalboard[pos];
		else
			value -= evalboard[pos];
	}
	return value;
}

void makeMove( Move pos, int depth )
{
	int pos1, dir, dirx, diry, count = 0;
	int isLegal = TRUE;
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR( OPPONENT(turn));

	memcpy( history[depth].board, board, sizeof( board ));
	memcpy( history[depth].stonenum, stonenum, sizeof( stonenum ));
	
	if ( pos == PASSMOVE )//パス手の場合
	{
		turn = OPPONENT(turn);
		return;
	}
	board[pos] = color;

	for ( dirx = -1; dirx <= 1; dirx++ )
	{
		for ( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			dir = dirx + diry;
			if ( dir == 0 )
				continue;
			pos1 = pos + dir;//posの隣のマス
			//以下は board[pos1] の中身をチェックしながら、
			//pos1を +=dirしていく
			//だめなら continue
			if ( board[pos1] != opponentcolor)//相手の石があるか
				continue;
			do // 相手の石がある間は次を調べる
			{
				pos1 += dir;
			}while ( board[pos1] == opponentcolor );
			// ひっくり返すためには最後に自分の石がないといけない
			if ( board[pos1] != color )
				continue;

			//最後まで来たら成功!実際にひっくり返す
			pos1 = pos + dir;
			do // 相手の石がある間ひっくり返す　
			{
				board[pos1] = color;
				pos1 += dir;
				count++;
			}while ( board[pos1] == opponentcolor );
		}
	}
	stonenum[turn] += count + 1;
	stonenum[OPPONENT(turn)] -= count;
	turn = OPPONENT(turn);
}

// 手を戻す
void unmakeMove(int depth)
{
	// 配列の中身をコピーするのはmemcpy()を使うと簡単
	// 逆の事をmakeMove()でしないといけない
	// 局面を配列historyから復元
	memcpy( board, history[depth].board, sizeof( board ));
	memcpy( stonenum, history[depth].stonenum, sizeof( stonenum ));
	turn = OPPONENT(turn);
}

// 探索して最も評価の高い手を選ぶ
int search(int depth)
{
	int i;
	int movenum;//手の数
	Move moves[MOVENUM];//手を入れる配列 an array of moves
	int value;
	int bestvalue = -INFINITY -1;//まず最小値を入れる
	if (depth >= MAXDEPTH)
		return getEvaluationValue();

	//手を生成
	movenum = generateMoves( moves );
	if ( movenum == 0 )
	{
		if ( isTerminalNode(TRUE) )// Game Over
			return getTerminalValue();
		else // パス
			moves[movenum++] = PASSMOVE;
	}
	for ( i = 0; i < movenum; i++ )
	{
		makeMove( moves[i], depth );//一手進め
		value = -search(depth + 1);
		output();//for Debug
		printf("i = %d, value = %d, move = %d\n", i, value, moves[i]);//for Debug
		unmakeMove( depth );//一手戻る

		if (value > bestvalue){
			bestvalue = value;
			if (depth == 0)
				nextmove = moves[i];
		}

	}
	return bestvalue;
}

//COMの手を生成する関数
void comPlayer()
{
	int value;
	printf( "Com Thinking...\n");
	value = search(0);
	printf( "value = %d\n", value );
	if ( value == INFINITY )
		printf( "Computer Finds Win!!\n" );
}

///////////////////////////AI用に追加　ここまで

//人間の入力を管理する関数
Move manPlayer()
{
	//入力をさせ、合法手かチェック
	char line[256];
	int x, y, num, move ;
	Move moves[MOVENUM];
	num = generateMoves( moves );
	// 合法手が無い場合
	if(num == 0)
	{
		printf("PASS!\n");
		printf("Press Enter!\n");
		fgets(line,256,stdin);
		return PASSMOVE;
	}
	do
	{
		// input X coordinate
		do
		{
			printf("x(a-h):");
			fgets(line,256,stdin);
		}while(line[0] < 'a' || line[0] > 'h');
		x = line[0] - 'a' + 1;
		// input Y coordinate
		do
		{
			printf("y(1-8):");
			fgets(line,256,stdin);
		}while(line[0] < '1' || line[0] > '8');
		y = line[0] - '0';
		// Check legal move
		move = getposition( x, y );
		if( isLegalMove(move) )
		{
			break;
		}
		printf("Illegal Move!\n");
	}while(1);
	return (Move)move;
}



// ランダムに手を返すプレイヤ
//ランダムに手を返す
Move randplayer()
{
	int num;
	Move moves[MOVENUM];
	num = generateMoves( moves );
	printf("RandPlayer\n");
	if ( num == 0 )
		return PASSMOVE;
	return moves[rand() % num];
}

//グローバル変数などを初期化
void init()
{
	turn = BLACK_TURN;
	ply = 0;
	stonenum[BLACK_TURN] = 2;
	stonenum[WHITE_TURN] = 2;
}
/*
int main()
{
	//まずは変数宣言、初期化など
	int result;
	char line[256];
	int manturn = BLACK_TURN;
	init();
	output();//局面の表示
	while(1)//一回の対局が終わるまでループ
	{
		if ( isTerminalNode(FALSE))//終局かチェック
		{
			//石の数で勝ち負け判定し表示
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];//こんな感じで
			// result により表示を変える
			if ( result == 0 )
				printf ( "GAMEOVER!  DRAW!!\n");
			else 
				printf ( "GAMEOVER! %s WIN!!\n", ( result > 0 ? "BLACK": "WHITE"));
			return 0;
		}
		if ( turn == manturn )
			nextmove = manPlayer();
			//randplayer();
		else
			//nextmove = randPlayer();
			comPlayer();
		makeMove( nextmove, 0 );
		ply++;
		printf( "ply = %d\n", ply );
		output();
		if(nextmove != PASSMOVE)
			printf ( "%s -> %s%d\n\n", ( turn != BLACK_TURN? "BLACK": "WHITE"), abc[X(nextmove)-1], Y(nextmove));
		else
			printf("PASS\n");
	}
	printf( "Press any key and Enter\n");
	fgets(line,256,stdin);
	scanf("");
	return 0;
}*/