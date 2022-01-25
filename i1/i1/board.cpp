#include "Header.h"

bool CheckBoard(const C_BOARD* pos) {
	int t_pceNum[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; //t stands for temporary, t variables will be used to check the original variables are correct
	int t_bigPcs[2] = { 0,0 }; //t stands for temporary, t variables will be used to check the original variables are correct
	int t_majPcs[2] = { 0,0 }; //t stands for temporary, t variables will be used to check the original variables are correct
	int t_minPcs[2] = { 0,0 }; //t stands for temporary, t variables will be used to check the original variables are correct
	int t_material[2] = { 0,0 }; //t stands for temporary, t variables will be used to check the original variables are correct

	U64 t_pawns[3]; //t stands for temporary, t variables will be used to check the original variables are correct

	t_pawns[white] = pos->pawns[white];
	t_pawns[black] = pos->pawns[black];
	t_pawns[both] = pos->pawns[both];
	 
	int sq;//120-based
	//checks pceList agrees with pieces
	for (int t_piece = wP; t_piece <= bK; ++t_piece) {
		for (int i = 0; i < pos->pceNum[t_piece]; ++i) {
			sq = pos->pceList[t_piece][i];
			if (!(pos->pieces[sq] == t_piece))
				sq = sq;
			assert(pos->pieces[sq] == t_piece);
		}
	}

	//sets types of pieces and material counts
	int t_piece;
	for (sq = 0; sq < 64; ++sq) {
		t_piece = pos->pieces[sq64to120[sq]]; //64 to 120-based
		int color = pceCol[t_piece];
		++t_pceNum[t_piece];
		if (isPceBig[t_piece]) ++t_bigPcs[pceCol[t_piece]];
		if (isPceMaj[t_piece]) ++t_majPcs[pceCol[t_piece]];
		if (isPceMin[t_piece]) ++t_minPcs[pceCol[t_piece]];

		t_material[color] += pceVal[t_piece];
	}

	//checks piece count
	for (int t_piece = wP; t_piece <= bK; ++t_piece) {
		assert(t_pceNum[t_piece] == pos->pceNum[t_piece]);
	}

	//checks bitboards count
	int pcount = CountBits(t_pawns[white]);
	if (pcount != pos->pceNum[wP]) {
		PrintBitBoard(pos->pawns[white]);
	}
	assert(pcount == pos->pceNum[wP]);
	pcount = CountBits(t_pawns[black]);
	assert(pcount == pos->pceNum[bP]);
	pcount = CountBits(t_pawns[both]);
	assert(pcount == (pos->pceNum[bP] + pos->pceNum[wP]));

	//check bitboards squares
	while (t_pawns[white]) {
		sq = PopBits(&t_pawns[white]); //64-based
		assert(pos->pieces[sq64to120[sq]] == wP);
	}

	while (t_pawns[black]) {
		sq = PopBits(&t_pawns[black]); //64-based
		assert(pos->pieces[sq64to120[sq]] == bP);
	}

	while (t_pawns[both]) {
		sq = PopBits(&t_pawns[both]); //64-based
		assert(pos->pieces[sq64to120[sq]] == wP or pos->pieces[sq64to120[sq]] == bP);
	}

	//checks material and types of pieces
	assert(t_material[white] == pos->material[white] and t_material[black] == pos->material[black]);
	assert(t_minPcs[white] == pos->minPcs[white] and t_minPcs[black] == pos->minPcs[black]);
	assert(t_majPcs[white] == pos->majPcs[white] and t_majPcs[black] == pos->majPcs[black]);
	assert(t_bigPcs[white] == pos->bigPcs[white] and t_bigPcs[black] == pos->bigPcs[black]);

	//checks there is a side to move
	assert(pos->side == white || pos->side == black);

	//checks hash key
	if ((GenerateHashKey(pos) != pos->hashKey))
		PrintBoard(pos);
	assert(GenerateHashKey(pos) == pos->hashKey);

	//checks that there is no en passant square or that it is in the corresponding rank for the side to move
	assert(pos->enPassant == NO_SQ || (ranksBrd[pos->enPassant] == rank_6 and pos->side == white)
		|| (ranksBrd[pos->enPassant] == rank_3 and pos->side == black));

	//checks king squares
	assert(pos->pieces[pos->kingSq[white]] == wK); 
	assert(pos->pieces[pos->kingSq[black]] == bK);

	return true;
}

void UpdateMaterial(C_BOARD* pos) {
	int piece = EMPTY;
	int color = both;
	for (int sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if (piece != OFFBOARD and piece != EMPTY) {

			color = pceCol[piece];

			if (isPceBig[piece]) ++pos->bigPcs[color];
			if (isPceMaj[piece]) ++pos->majPcs[color];
			if (isPceMin[piece]) ++pos->minPcs[color];

			pos->material[color] += pceVal[piece];

			pos->pceList[piece][pos->pceNum[piece]]=sq;
			pos->pceNum[piece]++;

			if (piece == wK) pos->kingSq[white] = sq;
			if (piece == bK) pos->kingSq[black] = sq;

			if (piece == wP) {
				setbit(pos->pawns[white], sq120to64[sq]);
				setbit(pos->pawns[both], sq120to64[sq]);
			}
			if (piece == bP) {
				setbit(pos->pawns[black], sq120to64[sq]);
				setbit(pos->pawns[both], sq120to64[sq]);
			}
		}
	}
}

int FenIn(char* fen, C_BOARD* pos) {
	assert(fen != 0);
	assert(pos != 0);

	int piece = 0;
	int count = 0;
	int sq = 0; //120-based
	int rank = rank_8;
	int file = file_A;

	ResetBoard(pos);

	//board
	while (rank >= rank_1 and *fen) {
		count = 1;
		switch (*fen) {
			//black pieces
			case 'p': 
				piece = bP;
				break;
			case 'n':
				piece = bN;
				break;
			case 'b':
				piece = bB;
				break;
			case 'r':
				piece = bR;
				break;
			case 'q':
				piece = bQ;
				break;
			case 'k':
				piece = bK;
				break;

			//white pieces
			case 'P':
				piece = wP;
				break;
			case 'N':
				piece = wN;
				break;
			case 'B':
				piece = wB;
				break;
			case 'R':
				piece = wR;
				break;
			case 'Q':
				piece = wQ;
				break;
			case 'K':
				piece = wK;
				break;

			//empty squares
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				piece = EMPTY;
				count = *fen - '0'; //ascii
				break;

			//end of line
			case '/':
			case ' ':
				--rank;
				file = file_A;
				fen++;
				continue;
			default:
				std::cout << "Fen error" << std::endl;
				return -1;
		}
			for (int i = 0; i < count; ++i) {
			if (piece != EMPTY) {
				pos->pieces[sq120(file, rank)] = piece;
			}
			++file;
		}
		++fen;
	}

	assert(*fen == 'w' or *fen == 'b');

	//side to move
	if (*fen == 'w') {
		pos->side = white;
	}
	else
	{
		pos->side = black;
	}
	fen += 2;

	//castling permission
	for (int i = 0; i < 4; ++i) {
		if (*fen == ' ') break;
		switch (*fen) {
			case 'K':
				pos->castlePerm |= WKC;
				break;
			case 'Q':
				pos->castlePerm |= WQC;
				break;
			case 'k':
				pos->castlePerm |= BKC;
				break;
			case 'q':
				pos->castlePerm |= BQC;
				break;
			default:
				break;
		}
		++fen;
	}

	++fen;

	assert(pos->castlePerm >= 0 and pos->castlePerm <= 15);

	//enPassant square
	if (*fen != '-') {
		file = fen[0] - 'a'; //ascii
		rank = fen[1] - '1';

		assert(file >= file_A and file <= file_H);
		assert(rank >= rank_1 and rank <= rank_8);

		pos->enPassant = sq120(file, rank);
	}

	pos->hashKey=GenerateHashKey(pos);

	UpdateMaterial(pos);

	return 1;
}

void ResetBoard(C_BOARD *pos) {
	for (int i = 0; i < BRD_SQ_NUM; ++i) {
		pos->pieces[i] = OFFBOARD;
	}
	for (int i = 0; i<64; ++i) {
		pos->pieces[sq64to120[i]] = EMPTY;
	}
	for (int i = 0; i < 2; ++i) {
		pos->bigPcs[i] = 0;
		pos->majPcs[i] = 0;
		pos->minPcs[i] = 0;
	}

	for (int i = white; i <=both; ++i) {
		pos->pawns[i] = 0ULL;
	}

	for (int i = 0; i < 13; ++i) {
		pos->pceNum[i] = 0;
	}

	pos->kingSq[white] = NO_SQ;
	pos->kingSq[black] = NO_SQ;
	pos->side = both;
	pos->enPassant = NO_SQ;
	pos->fiftyMoveCount = 0;
	pos->ply = 0;
	pos->hisPly = 0;
	pos->castlePerm = 0;
	pos->hashKey = 0ULL;
	pos->material[white] = 0;
	pos->material[black] = 0;

	for (int i = 0; i < 13; ++i) {
		for (int j = 0; j > BRD_SQ_NUM; ++j)
			pos->searchHistory[i][j] = 0;
	}
	
	for (int i = 0; i < MAX_DEPTH; ++i) {
		pos->searchKillers[0][i] = 0;
		pos->searchKillers[1][i] = 0;
	}

}

void PrintBoard(const C_BOARD *pos){
	std::cout << "Gameboard" << std::endl << std::endl;

	for (int rank = rank_8; rank >= rank_1; --rank) {
		for (int file = file_A; file <= file_H; ++file) {
			std::cout << std::setw(2) <<pceChar[pos->pieces[sq120(file, rank)]];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	//castling permission

	if (WKC & pos->castlePerm) std::cout << "K";
	if (WQC & pos->castlePerm) std:: cout << "Q";
	if (BKC & pos->castlePerm) std::cout << "k";
	if (BQC & pos->castlePerm) std::cout << "q";
	std::cout << std::endl;
	
	std::cout << pos->enPassant << std::endl;
	std::cout << pos->hashKey << std::endl;
}