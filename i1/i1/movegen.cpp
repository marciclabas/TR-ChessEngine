#include "Header.h"

//separate slider and non-sliders
const int loopSliderPce[8] = {
 wB, wR, wQ, 0, bB, bR, bQ, 0
};

const int loopNonSliderPce[6] = {
 wN, wK, 0, bN, bK, 0
};

//index to start by color
const int loopSliderIndex[2] = { 0, 4 };
const int loopNonSliderIndex[2] = { 0, 3 };

//consider implementing this variable instead of individual piece directions arrays in "attack.cpp"
const int pceDir[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
}; 

//number of directions every piece has
int numDir[13] = {
 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
};

int MoveExists(C_BOARD* pos, const int move) {

	C_MOVELIST list;
	GenerateAllMoves(pos, &list);

	for (int i = 0; i < list.count; ++i) {

		if (!MakeMove(pos, list.moves[i].move))
			continue;
		TakeMove(pos);
		if (list.moves[i].move == move)
			return true;
	}

	return false;
}

static void AddQuietMove(const C_BOARD* pos, int move, C_MOVELIST *list) {

	assert(SquareOnBoard(fromsq(move)));
	assert(SquareOnBoard(tosq(move)));

	list->moves[list->count].move = move;
	
	if (pos->searchKillers[0][pos->ply] == move)
		list->moves[list->count].score = 900000;
	else if (pos->searchKillers[1][pos->ply] == move)
		list->moves[list->count].score = 800000;
	else
		list->moves[list->count].score = pos->searchHistory[pos->pieces[fromsq(move)]][tosq(move)];

	++list->count;
}

static void AddCaptureMove(const C_BOARD* pos, int move, C_MOVELIST* list) {

	assert(SquareOnBoard(fromsq(move)));
	assert(SquareOnBoard(tosq(move)));
	assert(PieceValid(capturedpce(move)));

	list->moves[list->count].move = move;
	list->moves[list->count].score = mvvLvaScores[capturedpce(move)][pos->pieces[fromsq(move)]]+1000000;
	++list->count;
}

static void AddEnPassantMove(const C_BOARD* pos, int move, C_MOVELIST* list) {

	assert(SquareOnBoard(fromsq(move)));
	assert(SquareOnBoard(tosq(move)));

	list->moves[list->count].move = move;
	list->moves[list->count].score = 105+1000000; //105 is the value of pawn takes pawn
	++list->count;
}

static void AddWhitePawnCapMove(const  C_BOARD* pos, const int from, const int to, const int cap, C_MOVELIST* list) {

	assert(PieceValidOrEmpty(cap));
	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	if (ranksBrd[from] == rank_7) {
		AddCaptureMove(pos, move(from, to, cap, wQ, 0), list);
		AddCaptureMove(pos, move(from, to, cap, wR, 0), list);
		AddCaptureMove(pos, move(from, to, cap, wB, 0), list);
		AddCaptureMove(pos, move(from, to, cap, wN, 0), list);
	}
	else {
		AddCaptureMove(pos, move(from, to, cap, EMPTY, 0), list);
	}
}

static void AddWhitePawnMove(const C_BOARD* pos, const int from, const int to, C_MOVELIST* list) {

	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	if (ranksBrd[from] == rank_7) {
		AddQuietMove(pos, move(from, to, EMPTY, wQ, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, wR, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, wB, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, wN, 0), list);
	}
	else {
		AddQuietMove(pos, move(from, to, EMPTY, EMPTY, 0), list);
	}
}

static void AddBlackPawnCapMove(const  C_BOARD* pos, const int from, const int to, const int cap, C_MOVELIST* list) {

	assert(PieceValidOrEmpty(cap));
	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	if (ranksBrd[from] == rank_2) {
		AddCaptureMove(pos, move(from, to, cap, bQ, 0), list);
		AddCaptureMove(pos, move(from, to, cap, bR, 0), list);
		AddCaptureMove(pos, move(from, to, cap, bB, 0), list);
		AddCaptureMove(pos, move(from, to, cap, bN, 0), list);
	}
	else {
		AddCaptureMove(pos, move(from, to, cap, EMPTY, 0), list);
	}
}

static void AddBlackPawnMove(const C_BOARD* pos, const int from, const int to, C_MOVELIST* list) {

	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	if (ranksBrd[from] == rank_2) {
		AddQuietMove(pos, move(from, to, EMPTY, bQ, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, bR, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, bB, 0), list);
		AddQuietMove(pos, move(from, to, EMPTY, bN, 0), list);
	}
	else {
		AddQuietMove(pos, move(from, to, EMPTY, EMPTY, 0), list);
	}
}

void GenerateAllMoves(const C_BOARD* pos, C_MOVELIST* list) {
	
	assert(CheckBoard(pos));

	list->count = 0;

	int sq = 0; int t_sq = 0; int dir;

	//white
	if (pos->side == white) {
		//wP
		for (int i = 0; i < pos->pceNum[wP]; ++i) {
			sq = pos->pceList[wP][i];
			assert(SquareOnBoard(sq));

			//normal pawn movement
			if (pos->pieces[sq + 10] == EMPTY) {
				AddWhitePawnMove(pos, sq, sq + 10, list);
				//start pawn movement
				if (ranksBrd[sq] == rank_2 and pos->pieces[sq + 20] == EMPTY)
					AddQuietMove(pos, move(sq, sq + 20, EMPTY, EMPTY, pawnstflag), list);
			}

			//capture pawn movements
			if (!sqoffboard(sq + 9) and pceCol[pos->pieces[sq + 9]] == black)
				AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
			if (!sqoffboard(sq + 11) and pceCol[pos->pieces[sq + 11]] == black)
				AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);

			//en passant capture pawn movements
			if (pos->enPassant != NO_SQ) {
				if (pos->enPassant == sq + 9)
					AddEnPassantMove(pos, move(sq, sq + 9, EMPTY, EMPTY, enpasflag), list);
				if (pos->enPassant == sq + 11)
					AddEnPassantMove(pos, move(sq, sq + 11, EMPTY, EMPTY, enpasflag), list);
			}
		}

		//castle
		//kingside
		if ((pos->castlePerm & WKC) and (pos->pieces[F1] == EMPTY and pos->pieces[G1] == EMPTY)) {
			if (!IsSquareAttacked(E1, black, pos) and !IsSquareAttacked(F1, black, pos)) {
				AddQuietMove(pos, move(E1, G1, EMPTY, EMPTY, castleflag), list);
			}
		}
		//queenside
		if ((pos->castlePerm & WQC) and (pos->pieces[D1] == EMPTY and pos->pieces[C1] == EMPTY and pos->pieces[B1] == EMPTY)) {
			if (!IsSquareAttacked(E1, black, pos) and !IsSquareAttacked(D1,black,pos)) {
				AddQuietMove(pos, move(E1, C1, EMPTY, EMPTY, castleflag), list);
			}
		}
	}

	//black
	if (pos->side == black) {
		//bP
		for (int i = 0; i < pos->pceNum[bP]; ++i) {
			sq = pos->pceList[bP][i];
			assert(SquareOnBoard(sq));

			//normal pawn movement
			if (pos->pieces[sq - 10] == EMPTY) {
				AddBlackPawnMove(pos, sq, sq - 10, list);
				//start pawn movement
				if (ranksBrd[sq] == rank_7 and pos->pieces[sq - 20] == EMPTY)
					AddQuietMove(pos, move(sq, sq - 20, EMPTY, EMPTY, pawnstflag), list);
			}

			//capture pawn movements
			if (!sqoffboard(sq - 9) and pceCol[pos->pieces[sq - 9]] == white)
				AddBlackPawnCapMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
			if (!sqoffboard(sq - 11) and pceCol[pos->pieces[sq - 11]] == white)
				AddBlackPawnCapMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);

			//en passant capture pawn movements
			if (pos->enPassant != NO_SQ) {
				if (pos->enPassant == sq - 9)
					AddEnPassantMove(pos, move(sq, sq - 9, EMPTY, EMPTY, enpasflag), list);
				if (pos->enPassant == sq - 11)
					AddEnPassantMove(pos, move(sq, sq - 11, EMPTY, EMPTY, enpasflag), list);
			}
		}

		//castle
		//kingside
		if ((pos->castlePerm & BKC) and (pos->pieces[F8] == EMPTY and pos->pieces[G8] == EMPTY)) {
			if (!IsSquareAttacked(E8, white, pos) and !IsSquareAttacked(F8, white, pos)) {
				AddQuietMove(pos, move(E8, G8, EMPTY, EMPTY, castleflag), list);
			}
		}
		//queenside
		if ((pos->castlePerm & BQC) and (pos->pieces[D8] == EMPTY and pos->pieces[C8] == EMPTY and pos->pieces[B8] == EMPTY)) {
			if (!IsSquareAttacked(E8, white, pos) and !IsSquareAttacked(D8, white, pos)) {
				AddQuietMove(pos, move(E8, C8, EMPTY, EMPTY, castleflag), list);
			}
		}
	}

	//non-sliders
	int iPce = loopNonSliderIndex[pos->side];
	int pce = loopNonSliderPce[iPce];
	++iPce;

	while (pce != 0) {
		assert(PieceValid(pce));

		for (int pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pceList[pce][pceNum];
			assert(SquareOnBoard(sq));

			for (int i = 0; i < numDir[pce]; ++i) {
				t_sq = sq + pceDir[pce][i];

				if (sqoffboard(t_sq)) 
					continue;

				
				if (pos->pieces[t_sq] != EMPTY) {
					if (pceCol[pos->pieces[t_sq]] == oppside(pos->side)) AddCaptureMove(pos, move(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
					continue;
				}
				else {
					AddQuietMove(pos, move(sq, t_sq, EMPTY, EMPTY, 0), list);
				}

			}
		}
		pce = loopNonSliderPce[iPce];
		++iPce;
	}

	iPce = loopSliderIndex[pos->side];
	pce = loopSliderPce[iPce];
	++iPce;
	//sliders
	while (pce != 0) {
		assert(PieceValid(pce));

		for (int pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pceList[pce][pceNum];
			
			assert(SquareOnBoard(sq));

			for (int i = 0; i < numDir[pce]; ++i) {
				dir = pceDir[pce][i];
				t_sq = sq + dir;
				
				while (!sqoffboard(t_sq)) {

					if (pos->pieces[t_sq] != EMPTY) {
						if (pceCol[pos->pieces[t_sq]] == oppside(pos->side))
							AddCaptureMove(pos, move(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
						break;
					}
					AddQuietMove(pos, move(sq, t_sq, EMPTY, EMPTY, 0), list);
					t_sq += dir;
				}

			}
		}
		pce = loopSliderPce[iPce];
		++iPce;
	
	}
}

void GenerateAllCaptures(const C_BOARD* pos, C_MOVELIST* list) {

	assert(CheckBoard(pos));

	list->count = 0;

	int sq = 0; int t_sq = 0; int dir;

	//white
	if (pos->side == white) {
		//wP
		for (int i = 0; i < pos->pceNum[wP]; ++i) {
			sq = pos->pceList[wP][i];
			assert(SquareOnBoard(sq));

			//capture pawn movements
			if (!sqoffboard(sq + 9) and pceCol[pos->pieces[sq + 9]] == black)
				AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
			if (!sqoffboard(sq + 11) and pceCol[pos->pieces[sq + 11]] == black)
				AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);

			//en passant capture pawn movements
			if (pos->enPassant != NO_SQ) {
				if (pos->enPassant == sq + 9)
					AddEnPassantMove(pos, move(sq, sq + 9, EMPTY, EMPTY, enpasflag), list);
				if (pos->enPassant == sq + 11)
					AddEnPassantMove(pos, move(sq, sq + 11, EMPTY, EMPTY, enpasflag), list);
			}
		}
	}

	//black
	if (pos->side == black) {
		//bP
		for (int i = 0; i < pos->pceNum[bP]; ++i) {
			sq = pos->pceList[bP][i];
			assert(SquareOnBoard(sq));

			//capture pawn movements
			if (!sqoffboard(sq - 9) and pceCol[pos->pieces[sq - 9]] == white)
				AddBlackPawnCapMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
			if (!sqoffboard(sq - 11) and pceCol[pos->pieces[sq - 11]] == white)
				AddBlackPawnCapMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);

			//en passant capture pawn movements
			if (pos->enPassant != NO_SQ) {
				if (pos->enPassant == sq - 9)
					AddEnPassantMove(pos, move(sq, sq - 9, EMPTY, EMPTY, enpasflag), list);
				if (pos->enPassant == sq - 11)
					AddEnPassantMove(pos, move(sq, sq - 11, EMPTY, EMPTY, enpasflag), list);
			}
		}
	}

	//non-sliders
	int iPce = loopNonSliderIndex[pos->side];
	int pce = loopNonSliderPce[iPce];
	++iPce;

	while (pce != 0) {
		assert(PieceValid(pce));

		for (int pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pceList[pce][pceNum];
			assert(SquareOnBoard(sq));

			for (int i = 0; i < numDir[pce]; ++i) {
				t_sq = sq + pceDir[pce][i];

				if (sqoffboard(t_sq))
					continue;


				if (pos->pieces[t_sq] != EMPTY) {
					if (pceCol[pos->pieces[t_sq]] == oppside(pos->side)) 
						AddCaptureMove(pos, move(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
					continue;
				}
			}
		}
		pce = loopNonSliderPce[iPce];
		++iPce;
	}

	iPce = loopSliderIndex[pos->side];
	pce = loopSliderPce[iPce];
	++iPce;
	//sliders
	while (pce != 0) {
		assert(PieceValid(pce));

		for (int pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pceList[pce][pceNum];

			assert(SquareOnBoard(sq));

			for (int i = 0; i < numDir[pce]; ++i) {
				dir = pceDir[pce][i];
				t_sq = sq + dir;

				while (!sqoffboard(t_sq)) {

					if (pos->pieces[t_sq] != EMPTY) {
						if (pceCol[pos->pieces[t_sq]] == oppside(pos->side))
							AddCaptureMove(pos, move(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
						break;
					}
					t_sq += dir;
				}

			}
		}
		pce = loopSliderPce[iPce];
		++iPce;

	}
}
