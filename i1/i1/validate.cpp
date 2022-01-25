#include "Header.h"

bool SquareOnBoard(const int sq) {
	if (filesBrd[sq] == OFFBOARD) return false;
	return true;
}

bool SideValid(const int side) {
	if (side == white or side == black) return true;
	return  false;
}

bool FileRankValid(const int fr) {
	if (fr >= 0 and fr <= 7) return true;
	return false;
}

bool PieceValidOrEmpty(const int pce) {
	if (pce >= EMPTY and pce <= bK) return true;
	return false;
}

bool PieceValid(const int pce) {
	if (pce >= wP and pce <= bK) return true;
	return false;
}