#include "Header.h"

int pceVal[13] = { 0,100,325,325,550,1000,50000,100,325,325,550,1000,50000 };

const int blackNTable[64] = {
-1	,	0	,	0	,	0	,	0	,	0	,	0	,	-1	,
0	,	0	,	0	,	1	,	1	,	0	,	0	,	0	,
0	,	1	,	1	,	2	,	2	,	1	,	1	,	0	,
0	,	1	,	1	,	2	,	2	,	3	,	1	,	0	,
0	,	1	,	1	,	2	,	2	,	1	,	1	,	0	,
0	,	1	,	1	,	3	,	3	,	1	,	1	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
-1	,	0	,	0	,	0	,	0	,	0	,	0	,	-1
};

const int whiteNTable[64] = {
-1	,	0	,	0	,	0	,	0	,	0	,	0	,	-1	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	1	,	3	,	3	,	1	,	0	,	0	,
0	,	1	,	1	,	2	,	2	,	1	,	1	,	0	,
0	,	1	,	1	,	2	,	2	,	3	,	1	,	0	,
0	,	0	,	1	,	2	,	2	,	1	,	0	,	0	,
0	,	0	,	0	,	1	,	1	,	0	,	0	,	0	,
-1	,	0	,	0	,	0	,	0	,	0	,	0	,	-1
};

const int blackNqTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
2	,	1	,	2	,	0	,	0	,	0	,	0	,	0	,
1	,	2	,	3	,	2	,	0	,	0	,	0	,	0	,
2	,	2	,	1	,	0	,	0	,	0	,	0	,	0	,
1	,	1	,	3	,	2	,	0	,	0	,	0	,	0
};

const int blackNkTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	2	,	1	,	2	,
0	,	0	,	0	,	0	,	2	,	3	,	2	,	1	,
0	,	0	,	0	,	0	,	0	,	1	,	2	,	2	,
0	,	0	,	0	,	0	,	2	,	3	,	1	,	1
};

const int whiteNqTable[64] = {
1	,	1	,	3	,	2	,	0	,	0	,	0	,	0	,
2	,	2	,	1	,	0	,	0	,	0	,	0	,	0	,
1	,	2	,	3	,	2	,	0	,	0	,	0	,	0	,
2	,	1	,	2	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int whiteNkTable[64] = {
0	,	0	,	0	,	0	,	2	,	3	,	1	,	1	,
0	,	0	,	0	,	0	,	0	,	1	,	2	,	2	,
0	,	0	,	0	,	0	,	2	,	3	,	2	,	1	,
0	,	0	,	0	,	0	,	0	,	2	,	1	,	2	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int blackBqTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
1	,	2	,	2	,	0	,	0	,	0	,	0	,	0	,
1	,	4	,	2	,	0	,	0	,	0	,	0	,	0	,
1	,	1	,	3	,	0	,	0	,	0	,	0	,	0
};

const int blackBkTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	2	,	2	,	1	,
0	,	0	,	0	,	0	,	0	,	2	,	4	,	1	,
0	,	0	,	0	,	0	,	0	,	3	,	1	,	1
};

const int whiteBqTable[64] = {
1	,	1	,	3	,	0	,	0	,	0	,	0	,	0	,
1	,	4	,	2	,	0	,	0	,	0	,	0	,	0	,
1	,	2	,	2	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int whiteBkTable[64] = {
0	,	0	,	0	,	0	,	0	,	3	,	1	,	1	,
0	,	0	,	0	,	0	,	0	,	2	,	4	,	1	,
0	,	0	,	0	,	0	,	0	,	2	,	2	,	1	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	2	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int squareColTable[64] = {
1	,	0	,	1	,	0	,	1	,	0	,	1	,	0	,
0	,	1	,	0	,	1	,	0	,	1	,	0	,	1	,
1	,	0	,	1	,	0	,	1	,	0	,	1	,	0	,
0	,	1	,	0	,	1	,	0	,	1	,	0	,	1	,
1	,	0	,	1	,	0	,	1	,	0	,	1	,	0	,
0	,	1	,	0	,	1	,	0	,	1	,	0	,	1	,
1	,	0	,	1	,	0	,	1	,	0	,	1	,	0	,
0	,	1	,	0	,	1	,	0	,	1	,	0	,	1
};



//values will be added when evaluating white and substracted when evaluating black
class C_EVALUATION_VALUES {
public:

	C_EVALUATION_VALUES() {
		//pawns
		kPisolated[0] = -5;
		kPisolated[1] = -8;
		kPweak = -3;
		kPdoubled = -5;
		kPpassed[0] = 5;
		kPpassed[1] = 1;
		kPpassed[2] = 5;
		kPcentral = 3;
		kPscentral = 1;

		//knights
		kNweaksq = 5;
		kNclosedpos = 5;
		kNdefending = 1;

		//bishops
		kBpair[0] = 15;
		kBpair[1] = 10;
		kBpair[2] = 5;
		kBopenpos = 5;
		kBdefending = 1;

		//rooks
		kRopenfile = 10;
		kRsopenfile = 8;
		kRseventh = 20;
		kRopenpos = 5;

		//queens 
		kQopenfile = 5;
		kQsopenfile = 3;

		//kings
		kKpawns = 5;
		kKnotcastled = -5;
	}

	//pawns
	int kPisolated[2]; //[0]-> isolated pawn on closed file; [1]-> isolated pawn on semiopen file
	int kPweak; //pawn that doesn't have any protectors of its kind behind it and it's in a semiopen file
	int kPdoubled; //doubled pawns
	int kPpassed[3]; //[0]-> all passed pawns; [1]->to be added when a pawn advances a square from its initial rank; [2]->to be added if the pawn is not isolated
	int kPcentral; //pawn on 4 central squares
	int kPscentral; //pawn on 6 bottom surrounding central squares

	//knights
	int kNweaksq;
	int kNclosedpos;
	int kNdefending;

	//bishops
	int kBpair[3];
	int kBopenpos;
	int kBdefending;

	//rooks
	int kRopenfile;
	int kRsopenfile;
	int kRseventh;
	int kRopenpos;

	//queens
	int kQopenfile;
	int kQsopenfile;

	//kings
	int kKpawns;
	int kKnotcastled;

} val;

int score;
int pce, sq, bishopCol;
bool closedPos, sopenPos, openPos;
bool whiteQs, whiteKs, blackQs, blackKs;


int EvaluatePosition(C_BOARD* pos) {

	score = 0;

	//pre
	//type of position
	closedPos = false;
	sopenPos = false;
	openPos = false;

	for (int i = 0; i < 4; i++) {
		if ((pos->pawns[white] & whiteClosedMask[i])== whiteClosedMask[i] and (pos->pawns[black] & blackClosedMask[i])== blackClosedMask[i]) {
			closedPos = true;
			//std::cout << "closed pos" << std::endl;
			break;
		}
	}

	if (closedPos == false) {
		for (int i = 0; i < 2; i++) {
			if ((pos->pawns[white] & whiteSopenMask[i])== whiteSopenMask[i] and (pos->pawns[black] & blackSopenMask[i])== blackSopenMask[i]) {
				sopenPos = true;
				//std::cout << "sopen pos" << std::endl;
				break;
			}
		}
	}

	if (closedPos == false and sopenPos == false) {
		openPos = true;
		//std::cout << "open pos" << std::endl;
	}

	//king pos
	whiteQs = false;
	whiteKs = false;
	blackQs = false;
	blackKs = false;

	//white
	sq = pos->kingSq[white];
	if (sq == A1 or sq == A2 or sq == B1 or sq == B2 or sq == C1) {
		whiteQs = true;
	}
	else if (sq == H1 or sq == H2 or sq == G1 or sq == G2 or sq == F1) {
		whiteKs = true;
	}

	//black
	sq = pos->kingSq[black];
		if (sq == A8 or sq == A7 or sq == B8 or sq == B7 or sq == C8) {
			blackQs = true;
		}
		else if (sq == H8 or sq == H7 or sq == G8 or sq == G7 or sq == F8) {
			blackKs = true;
		}

	//pure material
	score = pos->material[white] - pos->material[black];

	//white
	//pawns##################################################################################################################
	pce = wP;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//isolated pawns
		if ((pos->pawns[white] & isolatedMask[sq120to64[sq]]) == 0) {
			//in semiopen file
			if (pos->pawns[black] & fileMask[filesBrd[sq]] or pos->pawns[white] & whiteDoubledMask[sq120to64[sq]]) {
				score += val.kPisolated[0];
				//std::cout << "isolated white pawn in closed file on " << sq << " [" << val.kPisolated[0] << ']' << std::endl;
			}
			else {
				score += val.kPisolated[1];
				//std::cout << "isolated white pawn in semiopen file on " << sq << " [" << val.kPisolated[1] << ']' << std::endl;
			}
		}
		//weak pawns
		else if ((pos->pawns[white] & whiteWeakMask[sq120to64[sq]]) == 0 and (pos->pawns[black] & fileMask[filesBrd[sq]]) == 0 and (pos->pawns[white] & whiteDoubledMask[sq120to64[sq]])==0) {
			score += val.kPweak;
			//std::cout << "weak white pawn on " << sq << " [" << val.kPweak << ']' << std::endl;
		}

		//doubled pawns
		if (pos->pawns[white] & whiteDoubledMask[sq120to64[sq]]) {
			score += val.kPdoubled;
			//std::cout << "doubled white pawn on " << sq << " [" << val.kPdoubled << ']' << std::endl;
		}

		//passed pawns
		if ((pos->pawns[black] & whitePassedMask[sq120to64[sq]]) == 0) {
			score += (val.kPpassed[0] + val.kPpassed[1] * (ranksBrd[sq] - 1));
			//std::cout << "passed white pawn on " << sq << " [" << (val.kPpassed[0] + val.kPpassed[1] * (ranksBrd[sq] - 1)) << ']' << std::endl;
			if (pos->pawns[white] & isolatedMask[sq120to64[sq]]) {
				score += val.kPpassed[2];
				//std::cout << "passed and defensed white pawn on " << sq << " [" << val.kPpassed[2] << ']' << std::endl;
			}

		}

		//central pawns
		if (sq == E4 or sq == D4 or sq == E5 or sq == D5) {
			score += val.kPcentral;
			//std::cout << "central white pawn on " << sq << " [" << val.kPcentral << ']' << std::endl;
		}

		//semicentral pawns
		if (sq == C4 or sq == C3 or sq == D3 or sq == E3 or sq == F3 or sq == F4) {
			score += val.kPscentral;
			//std::cout << "scentral white pawn on " << sq << " [" << val.kPscentral << ']' << std::endl;
		}
	}

	//knights################################################################################################################
	pce = wN;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//weak square
		if (ranksBrd[sq] < 6) {
			if ((pos->pawns[black] & blackWeakMask[sq120to64[sq] + 8]) == 0) {
				score += (val.kNweaksq * whiteNTable[sq120to64[sq]]);
				//std::cout << "white knight on weak square " << sq << " [" << val.kNweaksq* whiteNTable[sq120to64[sq]] << ']' << std::endl;
			}
		}
		else {
			score += (val.kNweaksq * whiteNTable[sq120to64[sq]]);
			//std::cout << "white knight on weak square " << sq << " [" << val.kNweaksq* whiteNTable[sq120to64[sq]] << ']' << std::endl;
		}

		//closed position
		if (closedPos) {
			score += val.kNclosedpos;
			//std::cout << "white knight in a closed position" << std::endl;
		}

		//knight table
		score += whiteNTable[sq120to64[sq]];
		//std::cout << "white knight in " << sq << " [" << whiteNTable[sq120to64[sq]] << ']' << std::endl;

		//king safety
		if (whiteQs) {
			score += val.kNdefending*whiteNqTable[sq120to64[sq]];
			//if (whiteNqTable[sq120to64[sq]])
				//std::cout << "defending white knight on sq " << sq << " [" << val.kNdefending* whiteNqTable[sq120to64[sq]] << ']' << std::endl;
		}

		if (whiteKs) {
			score += whiteNkTable[sq120to64[sq]];
			//if (whiteNkTable[sq120to64[sq]])
				//std::cout << "defending white knight on sq " << sq << " [" << val.kNdefending* whiteNkTable[sq120to64[sq]] << ']' << std::endl;
		}
	}

	//bishops################################################################################################################
	pce = wB;
	bishopCol = -1;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//bishop pair
		if (squareColTable[sq120to64[sq]] != bishopCol and bishopCol != -1) {
			if (openPos) {
				score += val.kBpair[0];
				//std::cout << "white bishop pair on open position [" << val.kBpair[0] << ']' << std::endl;
			}
			else if (sopenPos) {
				score += val.kBpair[1];
				//std::cout << "white bishop pair on sopen position [" << val.kBpair[1] << ']' << std::endl;
			}
			else {
				score += val.kBpair[0];
				//std::cout << "white bishop pair on closed position [" << val.kBpair[2] << ']' << std::endl;
			}
		}
		else {
			bishopCol = squareColTable[sq120to64[sq]];
		}

		//open pos
		if (openPos) {
			score += val.kBopenpos;
			//std::cout << "white bishop on open position on sq " << sq << " [" << val.kBopenpos << ']' << std::endl;
		}

		//king safety
		if (whiteQs) {
			score += val.kBdefending*whiteBqTable[sq120to64[sq]];
			//if (whiteBqTable[sq120to64[sq]])
				//std::cout << "defending white bishop on sq " << sq << " [" << val.kBdefending* whiteBqTable[sq120to64[sq]] << ']' << std::endl;
		}

		if (whiteKs) {
			score += whiteBkTable[sq120to64[sq]];
			//if (whiteBkTable[sq120to64[sq]])
				//std::cout << "defending white bishop on sq " << sq << " [" << val.kBdefending* whiteBqTable[sq120to64[sq]] << ']' << std::endl;
		}
	}

	//rooks##################################################################################################################
	pce = wR;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//open and sopen file
		if ((pos->pawns[white] & fileMask[filesBrd[sq]]) == 0) {
			//open
			if ((pos->pawns[black] & fileMask[filesBrd[sq]]) == 0) {
				score += val.kRopenfile;
				//std::cout << "white rook on open file on sq " << sq << " [" << val.kRopenfile << ']' << std::endl;
			}
			else {
				score += val.kRsopenfile;
				//std::cout << "white rook on sopen file on sq " << sq << " [" << val.kRsopenfile << ']' << std::endl;
			}
		}

		//seventh rank
		if (ranksBrd[sq] == 6) {
			score += val.kRseventh;
			//std::cout << "white rook on seventh rank on sq " << sq << " [" << val.kRseventh << ']' << std::endl;
		}

		if (openPos) {
			score += val.kRopenpos;
			//std::cout << "white rook on open position on sq " << sq << " [" << val.kRopenpos << ']' << std::endl;
		}
	}

	//queens#################################################################################################################
	pce = wQ;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//open and sopen file
		if ((pos->pawns[white] & fileMask[filesBrd[sq]]) == 0) {
			//open
			if ((pos->pawns[black] & fileMask[filesBrd[sq]]) == 0) {
				score += val.kQopenfile;
				//std::cout << "white queen on open file on sq " << sq << " [" << val.kQopenfile << ']' << std::endl;
			}
			else {
				score += val.kRsopenfile;
				//std::cout << "white queen on sopen file on sq " << sq << " [" << val.kQsopenfile << ']' << std::endl;
			}
		}
	}

	//king###################################################################################################################
	sq = pos->kingSq[white];

	//king situation
	//king on the queenside
	if (whiteQs) {
		score += val.kKpawns * (CountBits(pos->pawns[white] & whiteQCastleMask));
		//std::cout << "white king on queenside defended by "<< CountBits(pos->pawns[white] & whiteQCastleMask)<<" pawns [" << val.kKpawns* (CountBits(pos->pawns[white] & whiteQCastleMask)) << ']' << std::endl;
	}
	//king on the kingside
	else if (whiteKs) {
		score += val.kKpawns * (CountBits(pos->pawns[white] & whiteKCastleMask));
		//std::cout << "white king on kingside defended by " << CountBits(pos->pawns[white] & whiteKCastleMask) << " pawns [" << val.kKpawns* (CountBits(pos->pawns[white] & whiteKCastleMask)) << ']' << std::endl;
	}
	else {
		score += val.kKnotcastled;
		//std::cout << "white king not castled/in the middle of the board [" << val.kKnotcastled << ']' << std::endl;
	}


	//black
	//pawns##################################################################################################################
	pce = bP;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//isolated pawns
		if ((pos->pawns[black] & isolatedMask[sq120to64[sq]]) == 0 or pos->pawns[black] & blackDoubledMask[sq120to64[sq]]) {
			//in semiopen file
			if (pos->pawns[white] & fileMask[filesBrd[sq]]) {
				score -= val.kPisolated[0];
				//std::cout << "isolated black pawn in closed file on " << sq << " [-" << val.kPisolated[0] << ']' << std::endl;
			}
			else {
				score -= val.kPisolated[1];
				//std::cout << "isolated black pawn in semiopen file on " << sq << " [-" << val.kPisolated[1] << ']' << std::endl;
			}
		}
		//weak pawns
		else if ((pos->pawns[black] & blackWeakMask[sq120to64[sq]]) == 0 and (pos->pawns[white] & fileMask[filesBrd[sq]]) == 0 and (pos->pawns[black] & blackDoubledMask[sq120to64[sq]])==0) {
			score -= val.kPweak;
			//std::cout << "weak black pawn on " << sq << " [-" << val.kPweak << ']' << std::endl;
		}

		//doubled pawns
		if (pos->pawns[black] & blackDoubledMask[sq120to64[sq]]) {
			score -= val.kPdoubled;
			//std::cout << "doubled black pawn on " << sq << " [-" << val.kPdoubled << ']' << std::endl;
		}

		//passed pawns
		if ((pos->pawns[white] & blackPassedMask[sq120to64[sq]]) == 0) {
			score -= (val.kPpassed[0] + val.kPpassed[1] * (6 - ranksBrd[sq]));
			//std::cout << "passed black pawn on " << sq << " [-" << -(val.kPpassed[0] + val.kPpassed[1] * (6 - ranksBrd[sq])) << ']' << std::endl;
			if (pos->pawns[black] & isolatedMask[sq120to64[sq]]) {
				score -= val.kPpassed[2];
				//std::cout << "passed and defensed black pawn on " << sq << " [-" << val.kPpassed[2] << ']' << std::endl;
			}

		}

		//central pawns
		if (sq == E4 or sq == D4 or sq == E5 or sq == D5) {
			score -= val.kPcentral;
			//std::cout << "central black pawn on " << sq << " [-" << val.kPcentral << ']' << std::endl;
		}

		//semicentral pawns
		if (sq == C5 or sq == C6 or sq == D6 or sq == E6 or sq == F6 or sq == F5) {
			score -= val.kPscentral;
			//std::cout << "scentral black pawn on " << sq << " [-" << val.kPscentral << ']' << std::endl;
		}
	}

	//knights################################################################################################################
	pce = bN;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//weak square
		if (ranksBrd[sq] > 1) {
			if ((pos->pawns[white] & whiteWeakMask[sq120to64[sq] - 8]) == 0) {
				score -= (val.kNweaksq * blackNTable[sq120to64[sq]]);
				//std::cout << "black knight on weak square " << sq << " [-" << val.kNweaksq* whiteNTable[sq120to64[sq]] << ']' << std::endl;
			}
		}
		else {
			score -= (val.kNweaksq * blackNTable[sq120to64[sq]]);
			//std::cout << "black knight on weak square " << sq << " [-" << val.kNweaksq* whiteNTable[sq120to64[sq]] << ']' << std::endl;
		}

		//closed position
		if (closedPos) {
			score -= val.kNclosedpos;
			//std::cout << "black knight in a closed position" << std::endl;
		}

		//knight table
		score -= blackNTable[sq120to64[sq]];
		//std::cout << "black knight in " << sq << " [-" << blackNTable[sq120to64[sq]] << ']' << std::endl;

		//king safety
		if (blackQs) {
			score -= val.kNdefending*blackNqTable[sq120to64[sq]];
			//if (blackNqTable[sq120to64[sq]])
				//std::cout << "defending black knight on sq " << sq << " [-" << val.kNdefending* blackNqTable[sq120to64[sq]] << ']' << std::endl;
		}

		if (whiteKs) {
			score -= blackNkTable[sq120to64[sq]];
			//if (blackNkTable[sq120to64[sq]])
				//std::cout << "defending black knight on sq " << sq << " [-" << val.kNdefending* blackNkTable[sq120to64[sq]] << ']' << std::endl;
		}
	}

	//bishops################################################################################################################
	pce = bB;
	bishopCol = -1;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//bishop pair
		if (squareColTable[sq120to64[sq]] != bishopCol and bishopCol!=-1) {
			if (openPos) {
				score -= val.kBpair[0];
				//std::cout << "black bishop pair on open position [-" << val.kBpair[0] << ']' << std::endl;
			}
			else if (sopenPos) {
				score -= val.kBpair[1];
				//std::cout << "black bishop pair on sopen position [-" << val.kBpair[1] << ']' << std::endl;
			}
			else {
				score -= val.kBpair[0];
				//std::cout << "black bishop pair on closed position [-" << val.kBpair[2] << ']' << std::endl;
			}
		}
		else {
			bishopCol = squareColTable[sq120to64[sq]];
		}

		//open pos
		if (openPos) {
			score -= val.kBopenpos;
			//std::cout << "black bishop on open position on sq " << sq << " [-" << val.kBopenpos << ']' << std::endl;
		}

		//king safety
		if (blackQs) {
			score -= val.kBdefending*blackBqTable[sq120to64[sq]];
			//if (blackBqTable[sq120to64[sq]])
				//std::cout << "defending black bishop on sq " << sq << " [-" << val.kBdefending* blackBqTable[sq120to64[sq]] << ']' << std::endl;
		}

		if (blackKs) {
			score -= blackBkTable[sq120to64[sq]];
			//if (blackBqTable[sq120to64[sq]])
				//std::cout << "defending black bishop on sq " << sq << " [-" << val.kBdefending* blackBqTable[sq120to64[sq]] << ']' << std::endl;
		}
	}

	//rooks##################################################################################################################
	pce = bR;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//open and sopen file
		if ((pos->pawns[black] & fileMask[filesBrd[sq]]) == 0) {
			//open
			if ((pos->pawns[white] & fileMask[filesBrd[sq]]) == 0) {
				score -= val.kRopenfile;
				//std::cout << "black rook on open file on sq " << sq << " [-" << val.kRopenfile << ']' << std::endl;
			}
			else {
				score -= val.kRsopenfile;
				//std::cout << "black rook on sopen file on sq " << sq << " [-" << val.kRsopenfile << ']' << std::endl;
			}
		}

		//seventh rank
		if (ranksBrd[sq] == 1) {
			score -= val.kRseventh;
			//std::cout << "black rook on second rank on sq " << sq << " [-" << val.kRseventh << ']' << std::endl;
		}

		if (openPos) {
			score -= val.kRopenpos;
			//std::cout << "black rook on open position on sq " << sq << " [-" << val.kRopenpos << ']' << std::endl;
		}
	}

	//queens#################################################################################################################
	pce = bQ;

	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		sq = pos->pceList[pce][i];
		assert(SquareOnBoard(sq));

		//open and sopen file
		if ((pos->pawns[black] & fileMask[filesBrd[sq]]) == 0) {
			//open
			if ((pos->pawns[white] & fileMask[filesBrd[sq]]) == 0) {
				score -= val.kQopenfile;
				//std::cout << "black queen on open file on sq " << sq << " [-" << val.kQopenfile << ']' << std::endl;
			}
			else {
				score -= val.kRsopenfile;
				//std::cout << "black queen on sopen file on sq " << sq << " [-" << val.kQsopenfile << ']' << std::endl;
			}
		}
	}

	//king###################################################################################################################
	sq = pos->kingSq[white];

	//king situation
	//king on the queenside
	if (blackQs) {
		score -= val.kKpawns * (CountBits(pos->pawns[black] & blackQCastleMask));
		//std::cout << "black king on queenside defended by " << CountBits(pos->pawns[black] & blackQCastleMask) << " pawns [-" << val.kKpawns * (CountBits(pos->pawns[black] & blackQCastleMask)) << ']' << std::endl;
	}
	//king on the kingside
	else if (blackKs) {
		score -= val.kKpawns * (CountBits(pos->pawns[black] & blackKCastleMask));
		//std::cout << "black king on kingside defended by " << CountBits(pos->pawns[black] & blackKCastleMask) << " pawns [-" << val.kKpawns * (CountBits(pos->pawns[black] & blackKCastleMask)) << ']' << std::endl;
	}
	else {
		score -= val.kKnotcastled;
		//std::cout << "black king not castled/in the middle of the board [-" << val.kKnotcastled << ']' << std::endl;
	}



	if (pos->side == white)
		return score;
	return -score;
}




























/*temporal
const int PawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int KnightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int BishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int RookTable[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

//return the equivalent index for black pieces
const int Mirror64[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};*/
