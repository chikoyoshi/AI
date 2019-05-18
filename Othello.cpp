/*******************************************************************

	�I�Z�� �v���O�����@�@�@�@ 

*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>//memcpy
#include <time.h>
#include "Othello.h"

const char* piece[3] = { "  ", " ��", " ��" }; 
const char* abc[8] = { "a","b","c","d","e","f","g","h"};
int ply;
int turn;
unsigned char stonenum[2];
struct Position history[SEARCH_LIMIT_DEPTH];
Move nextmove;

int evalboard[BOARDSIZE] =
{
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  99, -20,  -5, -10, -10,  -5, -20,  99,   0,
   0, -20, -25,  -5,  -3,  -3,  -5, -25, -20,   0,
   0,  -5,  -5,  -1,  -1,  -1,  -1,  -5,  -5,   0,
   0, -10,  -3,  -1,  -1,  -1,  -1,  -3, -10,   0,
   0, -10,  -3,  -1,  -1,  -1,  -1,  -3, -10,   0,
   0,  -5,  -5,  -1,  -1,  -1,  -1,  -5,  -5,   0,
   0, -20, -25,  -6,  -3,  -3,  -5, -25, -20,   0,
   0,  99, -20,  -5, -10, -10,  -5, -20,  99,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
///////////////////////////AI�p�ɒǉ��@�����܂�

//2�����̍��W���ꎟ���ɕϊ�
int getposition( int x, int y ){ return y * ASIDE + x; };
//��ԑ厖�ȕϐ��A�����ł͈ꎟ���ŕ\��
unsigned char board[BOARDSIZE] = //int�ł��ǂ����A�f�[�^�i�[�̂��Ƃ��l����char�ɂ��Ă���
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
// �\���֐� display function
void output()
{
	int x,y;
	printf( "   a  b  c  d  e  f  g  h \n" );
	for ( y = 1; y <= SIDE; y++ )
	{
		printf( "%d |", y );
		for ( x = 1; x <= SIDE; x++ )
			printf( "%s|", piece[board[getposition(x,y)]] );//������ƊȌ��ɏ������߂̃q���g
		printf( "\n" );
	}
}

int isLegalMove(Move pos)
{
	int dirx, diry, dir;
	int pos1;
	// �����̐F�A����̐F�͉����ϐ��ɓ���Ă���
	int color = TURNCOLOR( turn );
	int opponentcolor = TURNCOLOR(OPPONENT(turn));

	if ( board[pos] != 0 )//�󂫃}�X�łȂ���
		return FALSE;
	
	// pos�̎���8�����𒲂ב���΂����邩���ׂ�
	// 8�������[�v�̏�����1
	for ( dirx = -1; dirx <= 1; dirx++ )
	{
		for ( diry = -ASIDE; diry <= ASIDE; diry += ASIDE )
		{
			dir = dirx + diry;
			if ( dir == 0 )
				continue;
			pos1 = pos + dir;//pos�ׂ̗̃}�X
			//�ȉ��� board[pos1] �̒��g���`�F�b�N���Ȃ���A
			//pos1�� +=dir���Ă���
			//���߂Ȃ� continue
			if ( board[pos1] != opponentcolor)//����̐΂����邩
				continue;
			do // ����̐΂�����Ԃ͎��𒲂ׂ�
			{
				pos1 += dir;
			}while ( board[pos1] == opponentcolor );
			// �Ђ�����Ԃ����߂ɂ͍Ō�Ɏ����̐΂��Ȃ��Ƃ����Ȃ�
			if ( board[pos1] != color )
				continue;
			//�Ō�܂ŗ����琬��
			return TRUE;
		}
	}
	return FALSE;	
}

int generateMoves( Move moves[] )	
{
	int num = 0;//�������鍇�@��̐�
	int pos;
	// ���ォ�珇�ɐ΂�u���A���@�肩���ׂ�
	for ( pos = 11; pos <= 88; pos++ )
	{
		if (isLegalMove(pos))
			moves[num++] = pos;//num�Ԗڂ̔z���
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
	//�ΐ��̍����v�Z�@�����̐΁[����̐�
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
	// ���@�萔�̍���]���֐��Ƃ���(���R�x)
	value = generateMoves(moves);//�����̍��@�萔�𑫂�
	turn = OPPONENT(turn);
	//����̍��@�萔������
	value -= generateMoves(moves);
	//��Ԃ�߂�
	turn = OPPONENT(turn);

	value *= 30;//���R�x�P��30�_�Ƃ��Ă����i�K���j
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
	
	if ( pos == PASSMOVE )//�p�X��̏ꍇ
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
			pos1 = pos + dir;//pos�ׂ̗̃}�X
			//�ȉ��� board[pos1] �̒��g���`�F�b�N���Ȃ���A
			//pos1�� +=dir���Ă���
			//���߂Ȃ� continue
			if ( board[pos1] != opponentcolor)//����̐΂����邩
				continue;
			do // ����̐΂�����Ԃ͎��𒲂ׂ�
			{
				pos1 += dir;
			}while ( board[pos1] == opponentcolor );
			// �Ђ�����Ԃ����߂ɂ͍Ō�Ɏ����̐΂��Ȃ��Ƃ����Ȃ�
			if ( board[pos1] != color )
				continue;

			//�Ō�܂ŗ����琬��!���ۂɂЂ�����Ԃ�
			pos1 = pos + dir;
			do // ����̐΂�����ԂЂ�����Ԃ��@
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

// ���߂�
void unmakeMove(int depth)
{
	// �z��̒��g���R�s�[����̂�memcpy()���g���ƊȒP
	// �t�̎���makeMove()�ł��Ȃ��Ƃ����Ȃ�
	// �ǖʂ�z��history���畜��
	memcpy( board, history[depth].board, sizeof( board ));
	memcpy( stonenum, history[depth].stonenum, sizeof( stonenum ));
	turn = OPPONENT(turn);
}

/*
// �T�����čł��]���̍������I��(alpha-beta)(ave : 0.537sec)
int search(int depth,int alpha,int beta)
{
	int i;
	int temp;
	int movenum;//��̐�
	Move moves[MOVENUM];//�������z�� an array of moves
	int value;
	int bestvalue = -INFINITY -1;//�܂��ŏ��l������
	if (depth >= MAXDEPTH)
		return getEvaluationValue();
	//��𐶐�
	movenum = generateMoves( moves );
	if ( movenum == 0 )
	{
		if ( isTerminalNode(TRUE) )// Game Over
			return getTerminalValue();
		else // �p�X
			moves[movenum++] = PASSMOVE;
	}
	for ( i = 0; i < movenum; i++ )
	{
		makeMove( moves[i], depth );//���i��
		if ((depth % 2) == 0){
			temp = -search(depth + 1,alpha,beta);						
			unmakeMove(depth);
			if (alpha < temp){
				alpha = temp;
			}
			if (alpha >= beta) {
				//printf("depth=%d,alpha=%d,beta=%d\n",depth,alpha,beta);
				//printf("beta cut\n");
				return beta;
			}
			if (i == (movenum - 1)) {
				if (depth == 0){
					nextmove = moves[i];
				}
				return alpha;
			}
		}
		else{
			temp = -search(depth + 1,alpha,beta);		
			unmakeMove(depth);
			if (beta > temp){
				beta = temp;
			}
			if (beta <= alpha) {
				//printf("depth=%d,alpha=%d,beta=%d\n",depth,alpha,beta);
				//printf("alpha cut\n");
				return alpha;
			}
			if (i == (movenum - 1)) {
//				unmakeMove(depth);
				if (depth == 0){
					nextmove = moves[i];
				}
				return beta;
			}
		}
	}
	//return bestvalue;
}
*/

// �T�����čł��]���̍������I��(MIN-MAX) (ave : 10.792)
int search(int depth,int k,int l)//(alpha-beta �ɑ����邽�߂ɕϐ��ǉ�)
{
	int i;
	int movenum;//��̐�
	Move moves[MOVENUM];//�������z�� an array of moves
	int value;
	int bestvalue = -INFINITY -1;//�܂��ŏ��l������
	if (depth >= MAXDEPTH)
		return getEvaluationValue();

	//��𐶐�
	movenum = generateMoves( moves );
	if ( movenum == 0 )
	{
		if ( isTerminalNode(TRUE) )// Game Over
			return getTerminalValue();
		else // �p�X
			moves[movenum++] = PASSMOVE;
	}
	for ( i = 0; i < movenum; i++ )
	{
		makeMove( moves[i], depth );//���i��
		value = -search(depth + 1,0,0);
		//output();//for Debug
		//printf("i = %d, value = %d, move = %d\n", i, value, moves[i]);//for Debug
		unmakeMove( depth );//���߂�

		if (value > bestvalue){
			bestvalue = value;
			if (depth == 0)
				nextmove = moves[i];
		}

	}
	return bestvalue;
}


//COM�̎�𐶐�����֐�
void comPlayer()
{
	int value;
	printf( "Com Thinking...\n");
	value = search(0,-2147483647,2147483647);
	printf( "value = %d\n", value );
	if ( value == INFINITY )
		printf( "Computer Finds Win!!\n" );
}

///////////////////////////AI�p�ɒǉ��@�����܂�

//�l�Ԃ̓��͂��Ǘ�����֐�
Move manPlayer()
{
	//���͂������A���@�肩�`�F�b�N
	char line[256];
	int x, y, num, move ;
	Move moves[MOVENUM];
	num = generateMoves( moves );
	// ���@�肪�����ꍇ
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



// �����_���Ɏ��Ԃ��v���C��
//�����_���Ɏ��Ԃ�
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

//�O���[�o���ϐ��Ȃǂ�������
void init()
{
	turn = BLACK_TURN;
	ply = 0;
	stonenum[BLACK_TURN] = 2;
	stonenum[WHITE_TURN] = 2;
}

int main()
{
	//�܂��͕ϐ��錾�A�������Ȃ�
	int result;
	char line[256];
	int manturn = BLACK_TURN;
	clock_t start,end;
	init();
	output();//�ǖʂ̕\��
	printf("Press Enter Key\n");
	fgetchar();
	srand(time(NULL));
	start = clock();
	while(1)//���̑΋ǂ��I���܂Ń��[�v
	{
		if ( isTerminalNode(FALSE))//�I�ǂ��`�F�b�N
		{
			//�΂̐��ŏ����������肵�\��
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];//����Ȋ�����
			// result �ɂ��\����ς���
			if ( result == 0 )
				printf ( "GAMEOVER!  DRAW!!\n");
			else {
				printf ( "GAMEOVER! %s WIN!!\n", ( result > 0 ? "BLACK": "WHITE"));
				printf ( "RANDOM:%d,AI:%d\n",stonenum[BLACK_TURN],stonenum[WHITE_TURN]);
			}
			break;
		}
		if ( turn == manturn )
			//nextmove = manPlayer();
			nextmove = randplayer();
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
	end = clock();
	printf ("%f sec \n",(double)(end-start)/CLOCKS_PER_SEC);
	printf( "Press any key and Enter\n");
	fgets(line,256,stdin);
	scanf("");
	return 0;
}