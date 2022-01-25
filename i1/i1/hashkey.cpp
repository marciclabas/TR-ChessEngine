#include "Header.h"
using namespace std;

U64 GenerateHashKey(const C_BOARD *pos){
	U64 hashKey = 0;
	int piece = EMPTY;
	for (int sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if (piece != NO_SQ and piece !=EMPTY and piece!=OFFBOARD) {
			assert(piece >= wP and piece <= bK);
			hashKey ^= pceKeys[piece][sq];
		}
	}

	if (pos->side == white) {
		hashKey ^= sideKey;
	}

	if (pos->enPassant != NO_SQ) {
		assert(pos->enPassant >= 0 and pos->enPassant < BRD_SQ_NUM);
		hashKey ^= pceKeys[EMPTY][pos->enPassant]; //index EMPTY is being used to store enPassant squares as it is not gonna be used by any piece
	}

	assert(pos->castlePerm >= 0 and pos->castlePerm <= 15);

	hashKey ^= castleKeys[pos->castlePerm];

	return hashKey;
}
