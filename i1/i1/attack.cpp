#include "Header.h"

const int nDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
const int rDir[4] = { -1, -10,	1, 10 };
const int bDir[4] = { -9, -11, 11, 9 };
const int kDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

bool IsSquareAttacked(const int sq, const int side, const C_BOARD* pos) { //attacking side

	assert(SquareOnBoard(sq));
	assert(SideValid(side));
	assert(CheckBoard(pos));

	//pawns
	if (side == white) {
		if (pos->pieces[sq - 11] == wP or pos->pieces[sq - 9] == wP)
			return true;
	}

	if (side == black) {
		if (pos->pieces[sq + 11] == bP or pos->pieces[sq + 9] == bP) 
			return true;
	}

	//knights
	int pce;
	for (int i = 0; i < 8;++i) {
		pce = pos->pieces[sq + nDir[i]];
		if (pce != OFFBOARD and isPceN[pce] and pceCol[pce]==side) {
				return true;
		}
	}

	//rooks and queens
	int t_sq,dir; 
	for (int i = 0; i < 4; ++i) {
		dir = rDir[i];
		t_sq = sq + dir;
		pce=pos->pieces[t_sq];
		while (pce != OFFBOARD) {
			if (pce != EMPTY) {
				if (isPceRQ[pce] and pceCol[pce] == side) 
					return true;
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

	//bishops and queens
	for (int i = 0; i < 4; ++i) {
		dir = bDir[i];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while (pce != OFFBOARD) {
			if (pce != EMPTY) {
				if (isPceBQ[pce] and pceCol[pce] == side) 
					return true;
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

	//kings
	for (int i = 0; i < 8; ++i) {
		pce = pos->pieces[sq + kDir[i]];
		if (pce != OFFBOARD and isPceK[pce] and pceCol[pce] == side) {
				return true;
		}
	}

	//otherwise
	return false;
}