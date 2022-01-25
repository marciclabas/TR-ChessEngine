#include "Header.h"

U64 fileMask[8];
U64 rankMask[8];

U64 whitePassedMask[64];
U64 blackPassedMask[64];
U64 isolatedMask[64];
U64 whiteWeakMask[64];
U64 blackWeakMask[64];
U64 whiteDoubledMask[64];
U64 blackDoubledMask[64];

U64 whiteClosedMask[4] = { 0ULL,0ULL,0ULL,0ULL }; //in the central 4 squares, 2 pawns of the same colour i diagonal and 1 of the opposite stopping them
U64 blackClosedMask[4] = { 0ULL,0ULL,0ULL,0ULL }; //in the central 4 squares, 2 pawns of the same colour i diagonal and 1 of the opposite stopping them
U64 whiteSopenMask[2] = { 0ULL,0ULL }; //in the central 4 squares, 2 pawns opposated pawns of different colour
U64 blackSopenMask[2] = { 0ULL,0ULL }; //in the central 4 squares, 2 pawns opposated pawns of different colour
U64 centralMask = 0ULL;
U64 whiteSemiCentralMask = 0ULL;
U64 blackSemiCentralMask = 0ULL;

U64 whiteQCastleMask = 0ULL;
U64 whiteKCastleMask = 0ULL;
U64 blackQCastleMask = 0ULL;
U64 blackKCastleMask = 0ULL;

void InitEvalMasks() {

	//initialize file and rank masks
	for (int i = 0; i < 8; ++i) {
		fileMask[i] = 0ULL;
		rankMask[i] = 0ULL;
	}

	//set file and rank masks
	for (int rank = rank_1; rank <= rank_8; ++rank) {
		for (int file = file_A; file <= file_H; ++file) {
			fileMask[file] |= (1ULL << sq64(file, rank));
			rankMask[rank] |= (1ULL << sq64(file, rank));
			SetBitBoard(sq120(file, rank), &rankMask[rank]);
		}
	}
	
	//initialize passed and isolated masks
	for (int i = 0; i < 64; ++i) {
		isolatedMask[i] = 0ULL;
		whiteWeakMask[i] = 0ULL;
		blackWeakMask[i] = 0ULL;
		whitePassedMask[i] = 0ULL;
		blackPassedMask[i] = 0ULL;
		whiteDoubledMask[i] = 0ULL;
		blackDoubledMask[i] = 0ULL;
	}

	int tsq;

	//central mask
	centralMask |= (1ULL << sq120to64[E4]);
	centralMask |= (1ULL << sq120to64[D4]);
	centralMask |= (1ULL << sq120to64[E5]);
	centralMask |= (1ULL << sq120to64[D5]);

	//semi central masks
	whiteSemiCentralMask |= (1ULL << sq120to64[C4]);
	whiteSemiCentralMask |= (1ULL << sq120to64[C3]);
	whiteSemiCentralMask |= (1ULL << sq120to64[D3]);
	whiteSemiCentralMask |= (1ULL << sq120to64[E3]);
	whiteSemiCentralMask |= (1ULL << sq120to64[F3]);
	whiteSemiCentralMask |= (1ULL << sq120to64[F4]);

	blackSemiCentralMask |= (1ULL << sq120to64[C5]);
	blackSemiCentralMask |= (1ULL << sq120to64[C6]);
	blackSemiCentralMask |= (1ULL << sq120to64[D6]);
	blackSemiCentralMask |= (1ULL << sq120to64[E6]);
	blackSemiCentralMask |= (1ULL << sq120to64[F6]);
	blackSemiCentralMask |= (1ULL << sq120to64[F5]);

	//closed masks
	//[0]
	whiteClosedMask[0] |= (1ULL << sq120to64[D4]);
	whiteClosedMask[0] |= (1ULL << sq120to64[E5]);
	blackClosedMask[0] |= (1ULL << sq120to64[D5]);
	//[1]
	whiteClosedMask[1] |= (1ULL << sq120to64[E4]);
	whiteClosedMask[1] |= (1ULL << sq120to64[D5]);
	blackClosedMask[1] |= (1ULL << sq120to64[E5]);
	//[2]
	blackClosedMask[2] |= (1ULL << sq120to64[E4]);
	blackClosedMask[2] |= (1ULL << sq120to64[D5]);
	whiteClosedMask[2] |= (1ULL << sq120to64[D4]);
	//[3]
	blackClosedMask[3] |= (1ULL << sq120to64[E5]);
	blackClosedMask[3] |= (1ULL << sq120to64[D4]);
	whiteClosedMask[3] |= (1ULL << sq120to64[E4]);

	//semi closed masks
	//[0]
	whiteSopenMask[0] |= (1ULL << sq120to64[E4]);
	blackSopenMask[0] |= (1ULL << sq120to64[E5]);
	//[1]
	whiteSopenMask[1] |= (1ULL << sq120to64[D4]);
	blackSopenMask[1] |= (1ULL << sq120to64[D5]);

	//castle masks
	//white queenside
	whiteQCastleMask |= (1ULL << sq120to64[A2]);
	whiteQCastleMask |= (1ULL << sq120to64[A3]);
	whiteQCastleMask |= (1ULL << sq120to64[B2]);
	whiteQCastleMask |= (1ULL << sq120to64[B3]);
	whiteQCastleMask |= (1ULL << sq120to64[C2]);

	//white kingside
	whiteKCastleMask |= (1ULL << sq120to64[H2]);
	whiteKCastleMask |= (1ULL << sq120to64[H3]);
	whiteKCastleMask |= (1ULL << sq120to64[G2]);
	whiteKCastleMask |= (1ULL << sq120to64[G3]);
	whiteKCastleMask |= (1ULL << sq120to64[F2]);

	//black queenside
	blackQCastleMask |= (1ULL << sq120to64[A7]);
	blackQCastleMask |= (1ULL << sq120to64[A6]);
	blackQCastleMask |= (1ULL << sq120to64[B7]);
	blackQCastleMask |= (1ULL << sq120to64[B6]);
	blackQCastleMask |= (1ULL << sq120to64[C7]);

	//white kingside
	blackKCastleMask |= (1ULL << sq120to64[H7]);
	blackKCastleMask |= (1ULL << sq120to64[H6]);
	blackKCastleMask |= (1ULL << sq120to64[G7]);
	blackKCastleMask |= (1ULL << sq120to64[G6]);
	blackKCastleMask |= (1ULL << sq120to64[F7]);

	//square indexed eval masks
	for (int sq = 0; sq < 64; ++sq) {
		//whiteDoubledMask
		if (sq > 7 and sq < 48) {
			tsq = sq + 8;
			while (tsq < 48) {
				whiteDoubledMask[sq] |= (1ULL << (tsq));
				tsq += 8;
			}
		}

		///blackDoubledMask
		if (sq > 15 and sq < 56) {
			tsq = sq - 8;
			while (tsq > 15) {
				blackDoubledMask[sq] |= (1ULL << (tsq));
				tsq -= 8;
			}
		}
		
		//whitePassedMask init pawns in front
		tsq = sq + 8;
		while (tsq < 64){
			whitePassedMask[sq] |= (1ULL << tsq);
			tsq += 8;
		}

		//blackPassedMask init pawns in front
		tsq = sq - 8;
		while (tsq > 0) {
			blackPassedMask[sq] |= (1ULL << tsq);
			tsq -= 8;
		}

		//files other than a and h (in files a and h we need to check only one file on the side)
		if (filesBrd[sq64to120[sq]] > file_A) {

			//isolated mask
			isolatedMask[sq] |= fileMask[filesBrd[sq64to120[sq]] - 1];

			//passed masks
			tsq = sq + 7;
			while (tsq < 64) {
				whitePassedMask[sq] |= (1ULL << tsq);
				tsq += 8;
			}

			tsq = sq - 9;
			while (tsq >= 0) {
				blackPassedMask[sq] |= (1ULL << tsq);
				tsq -= 8;
			}

			//weak masks, only for rank < 6 for white and rank < 3
			if (ranksBrd[sq64to120[sq]] < rank_6) {
				tsq = sq - 1;
				while (tsq > 7) {
					whiteWeakMask[sq] |= (1ULL << tsq);
					tsq -= 8;
				}
			}
			else {
				whiteWeakMask[sq] = 0xFFFFFFFFFFFFFFFF;
			}

			if (ranksBrd[sq64to120[sq]] > rank_3) {
				tsq = sq - 1;
				while (tsq < 56) {
					blackWeakMask[sq] |= (1ULL << tsq);
					tsq += 8;
				}
			}
			else {
				blackWeakMask[sq] = 0xFFFFFFFFFFFFFFFF;
			}
			
		}

		if (filesBrd[sq64to120[sq]] < file_H) {

			//isolated mask
			isolatedMask[sq] |= fileMask[filesBrd[sq64to120[sq]] + 1];

			//passed masks
			tsq = sq + 9;
			while (tsq < 64) {
				whitePassedMask[sq] |= (1ULL << tsq);
				tsq += 8;
			}

			tsq = sq - 7;
			while (tsq >= 0) {
				blackPassedMask[sq] |= (1ULL << tsq);
				tsq -= 8;
			}

			//weak masks, only for rank < 6 for white and rank < 3
			if (ranksBrd[sq64to120[sq]] < rank_6) {
				tsq = sq + 1;
				while (tsq > 7) {
					whiteWeakMask[sq] |= (1ULL << tsq);
					tsq -= 8;
				}
			}
			else {
				whiteWeakMask[sq] = 0xFFFFFFFFFFFFFFFF;
			}

			if (ranksBrd[sq64to120[sq]] > rank_3) {
				tsq = sq + 1;
				while (tsq < 56) {
					blackWeakMask[sq] |= (1ULL << tsq);
					tsq += 8;
				}
			}
			else {
				blackWeakMask[sq] = 0xFFFFFFFFFFFFFFFF;
			}
		}
	} 
	/*for (int i = 0; i < 2; ++i) {
		std::cout << "sq " << i << std::endl;
		PrintBitBoard(whiteSopenMask[i]);
		std::cout << std::endl;
		PrintBitBoard(blackSopenMask[i]);
		std::cout << std::endl;
	}*/
}





const int victimScore[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
int mvvLvaScores[13][13]; //[victim][attacker]

//mvvlva stands for most valuable victim / least valuable attacker
//we order the capture score so queen takes pawn has the lowest score and pawn takes queen has the highest one
void InitMvvLva() {
	for (int att = wP; att <= bK; ++att) {
		for (int vict = wP; vict <= bK; ++vict) {
			mvvLvaScores[vict][att] = victimScore[vict] + 6 - (victimScore[att] / 100);
		}
	}
}

int filesBrd[BRD_SQ_NUM];
int ranksBrd[BRD_SQ_NUM];

void InitFilesRanksBoard() {

	for (int i = 0; i < BRD_SQ_NUM; ++i) {
		filesBrd[i] = OFFBOARD;
		ranksBrd[i] = OFFBOARD;
	}
	
	for (int rank = rank_1; rank <= rank_8; ++rank) {
		for (int file = file_A; file <= file_H; ++file) {
			filesBrd[sq120(file, rank)] = file;
			ranksBrd[sq120(file, rank)] = rank;
		}
	}
}

U64 SetMask[64];
U64 ClearMask[64];

void InitBitMasks() {
	for (int i = 0; i < 64; ++i) {
		SetMask[i] = 0ULL;
	}

	for (int i = 0; i < 64; ++i) {
		SetMask[i] |= (1ULL << i);
		ClearMask[i] |= ~SetMask[i];
	}
}

int sq64to120[64];
int sq120to64[BRD_SQ_NUM];

void InitSq120To64() {
	for (int i = 0; i < BRD_SQ_NUM; ++i) {
		sq120to64[i] = 64;
	}
	//64 to 120 and 120 to 64
	int count = 0;
	for (int r = file_A; r <= file_H; ++r) {
		for (int f = rank_1; f <= rank_8; ++f) {
			sq64to120[count] = sq120(f,r);
			sq120to64[sq120(f,r)] = count;
			++count;
		}
	}
}

U64 pceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

void InitHashKeys() {
	for (int i = 0; i < 13; ++i) {
		for (int j = 0; j < 120; ++j) {
			pceKeys[i][j] = rndU64;
		}
	}
	sideKey = rndU64;
	for (int i = 0; i < 16; ++i) {
		castleKeys[i] = rndU64;
	}
}

void AllInit() {
	InitMvvLva();
	InitSq120To64();
	InitBitMasks();
	InitHashKeys();
	InitFilesRanksBoard();
	InitEvalMasks();
}