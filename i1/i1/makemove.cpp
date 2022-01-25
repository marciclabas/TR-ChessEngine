#include "Header.h"


/*
for every move made we will bitwise and the castlePerm variable of the board class with the number on this array
	as 15 is 1111 in binary the and will result in no change in castlePerm
	as 13 is 1101 in binary the and will result in the substraction of the 2^1 bit, corresponding to the white queenside castling permission
	as 14 is 1110 in binary the and will result in the substraction of the 2^0 bit, corresponding to the white kingside castling permission
	as 12 is 1100 in binary the and will result in the substraction of the 2^1 bit and the 2^0, corresponding to the white queen and kingside castling permission
	as 7 is 0111 in binary the and will result in the substraction of the 2^3 bit, corresponding to the black queenside castling permission
	as 11 is 1011 in binary the and will result in the substraction of the 2^2 bit, corresponding to the black kingside castling permission
	as 3 is 0011 in binary the and will result in the substraction of the 2^3 bit and the 2^2bit, corresponding to the black queen and kingside castling permission
*/
const int castlePermArray[120] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int sq, C_BOARD* pos) {

	assert(SquareOnBoard(sq));

	int pce = pos->pieces[sq];
	assert(PieceValid(pce));

	int color = pceCol[pce];
	int t_pceNum = -1;

	//hash piece out
	hashpce(pce, sq);

	//empty the square and substract the material value
	pos->pieces[sq] = EMPTY;
	pos->material[color] -= pceVal[pce];

	//substract the number of piece types corresponding
	if (isPceBig[pce]) {
		--pos->bigPcs[color];
		if (isPceMaj[pce])
			--pos->majPcs[color];
		else
			--pos->minPcs[color];
	}
	else {
		clrbit(pos->pawns[color], sq120to64[sq]);
		clrbit(pos->pawns[both], sq120to64[sq]);
	}

	//substract piece from pceList, reduce by 1 the pceNum of the piece type and reallocate the higher pceNum piece to the substracted's piece place
	//loop through the pceList until the piece we are substracting is found
	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		if (pos->pceList[pce][i] == sq) {
			t_pceNum = i;
			break;
		}
	}

	assert(t_pceNum != -1);

	--pos->pceNum[pce];

	//replace the substracted piece by the higher indexed piece of the same very type
	pos->pceList[pce][t_pceNum] = pos->pceList[pce][pos->pceNum[pce]];
}

static void AddPiece(const int sq, C_BOARD* pos, const int pce) {

	assert(PieceValid(pce));
	assert(SquareOnBoard(sq));

	int color = pceCol[pce];

	//hash in piece
	hashpce(pce, sq);

	//set the piece on its square, its square on the piece list and add its material value
	pos->pieces[sq] = pce;
	pos->material[color] += pceVal[pce];
	pos->pceList[pce][pos->pceNum[pce]]=sq;
	++pos->pceNum[pce];

	if (isPceBig[pce]) {
		++pos->bigPcs[color];
		if (isPceMaj[pce])
			++pos->majPcs[color];
		else
			++pos->minPcs[color];
	}
	else {
		setbit(pos->pawns[color], sq120to64[sq]);
		setbit(pos->pawns[both], sq120to64[sq]);
	}
}

static void MovePiece(const int from, const int to, C_BOARD* pos) {

	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	int pce = pos->pieces[from];
	int color = pceCol[pce];

//#ifdef DEBUG
	//int t_pceNum = false;
//#endif // only in debug mode

	//hash out the piece on the from square and hash it in in the to square
	hashpce(pce, from);
	hashpce(pce, to);

	//set the from square to empty and the to square to the piece we are moving
	pos->pieces[from] = EMPTY;
	pos->pieces[to] = pce;

	//set the from quare to empty and the to square to the piece we are moving on the bitboards if the piece is a pawn
	if (isPceP[pce]) {
		clrbit(pos->pawns[color], sq120to64[from]);
		clrbit(pos->pawns[both], sq120to64[from]);
		setbit(pos->pawns[color], sq120to64[to]);
		setbit(pos->pawns[both], sq120to64[to]);
	}

	//change the square of the piece we are moving in the piece list
	for (int i = 0; i < pos->pceNum[pce]; ++i) {
		if (pos->pceList[pce][i] == from) {
			pos->pceList[pce][i] = to;
//#ifdef DEBUG
			//t_pceNum = true;
//#endif // only in debug mode
			break;
		}
	}
	//assert(t_pceNum);
}

bool MakeMove(C_BOARD* pos, int move) {

	assert(CheckBoard(pos));

	int from = fromsq(move);
	int to = tosq(move);
	int side = pos->side;
	
	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));
	assert(SideValid(side));
	assert(PieceValid(pos->pieces[from]));

	//store hash key
	pos->history[pos->hisPly].hashKey = pos->hashKey;

	//if the move is an en passant move we need to remove the captured pawn
	if (move & enpasflag) {
		if (side == white)
			ClearPiece(to - 10, pos);
		else
			ClearPiece(to + 10, pos);
	}
	//if the move is a castle we need to move the rook to the appropiate square
	else if (move & castleflag) {
		switch (to) {
			//white queenside castle
		case C1:
			MovePiece(A1, D1, pos);
			break;
			//white kingside castle
		case G1:
			MovePiece(H1, F1, pos);
			break;
			//black queenside castle
		case C8:
			MovePiece(A8, D8, pos);
			break;
			//black kingside castle
		case G8:
			MovePiece(H8, F8, pos);
			break;
		default:
			assert(false);
			break;
		}
	}

	//hash out the current en passant square and castle permission
	if (pos->enPassant != NO_SQ) 
		hashenpas;
	hashcastle;

	//store move, fifty move counter, en passant square and castle perm
	pos->history[pos->hisPly].move = move;
	pos->history[pos->hisPly].fiftyMoveCount = pos->fiftyMoveCount;
	pos->history[pos->hisPly].enPassant = pos->enPassant;
	pos->history[pos->hisPly].castlePerm = pos->castlePerm;

	//set the castle permission after the move and hash it in
	pos->castlePerm &= castlePermArray[from];
	pos->castlePerm &= castlePermArray[to];
	hashcastle;

	//reset en passant square and increment the fifty move counter
	pos->enPassant = NO_SQ;
	++pos->fiftyMoveCount;

	//if a capture is made clear the captured piece and reset the fifty move counter
	if (capturedpce(move) != EMPTY) {
		assert(PieceValid(capturedpce(move)));
		ClearPiece(to, pos);
		pos->fiftyMoveCount = 0;
	}

	//increment ply (half-moves)
	++pos->hisPly;
	++pos->ply;

	//if we move a pawn reset the fifty move counter and check if its a pawn start move, then set the en passant square and hash it in
	if (isPceP[pos->pieces[from]]) {
		pos->fiftyMoveCount = 0;
		if (move & pawnstflag) {
			if (side == white) {
				pos->enPassant = from + 10;
				assert(ranksBrd[pos->enPassant] == rank_3);
			}
			else {
				pos->enPassant = from - 10;
				assert(ranksBrd[pos->enPassant] == rank_6);
			}
			hashenpas;
		}
	}

	//make the move on the board
	MovePiece(from, to, pos);

	//if we promote a pawn clear the from square and add the promoted piece
	if (promotedpce(move) != EMPTY) {
		assert(PieceValid(promotedpce(move)) and isPceBig[promotedpce(move)]);
		ClearPiece(to, pos);
		AddPiece(to, pos, promotedpce(move));
	}

	//update the king square
	if (isPceK[pos->pieces[to]]) {
		pos->kingSq[side] = to;
	}

	//update the side to move and hash it in
	pos->side = oppside(side);
	hashside;

	assert(CheckBoard(pos));

	//check if the king is attacked and therefore the move we intend to do is illegal
	//pos->side has already been set to opposite side whereas side is still the same who made the move
	if (IsSquareAttacked(pos->kingSq[side],pos->side,pos)) {
		TakeMove(pos);
		return false;
	}
	return true;
}

void TakeMove(C_BOARD* pos){

	assert(CheckBoard(pos));

	//take back ply increments
	--pos->hisPly;
	--pos->ply;

	int move = pos->history[pos->hisPly].move;
	int from = fromsq(move);
	int to = tosq(move);

	assert(SquareOnBoard(from));
	assert(SquareOnBoard(to));

	//if there was a en passant square set hash it out and hash out the castle permission
	if (pos->enPassant != NO_SQ) hashenpas;
	hashcastle;

	//take back castle permission, fifty move counter and en passant square
	pos->castlePerm = pos->history[pos->hisPly].castlePerm;
	pos->fiftyMoveCount = pos->history[pos->hisPly].fiftyMoveCount;
	pos->enPassant = pos->history[pos->hisPly].enPassant;

	//hash in en passant square if any and castle perm
	if (pos->enPassant != NO_SQ) 
		hashenpas;
	hashcastle;

	//take back side to move and hash it in
	pos->side = oppside(pos->side);
	hashside;

	//if an en passant move was made add the captured pawn
	if (move & enpasflag) {
		if (pos->side == white)
			AddPiece(to - 10, pos, bP);
		else
			AddPiece(to + 10, pos, wP);
	}
	else if (move & castleflag) {
			switch (to) {
				//white queenside castle
			case C1:
				MovePiece(D1, A1, pos);
				break;
				//white kingside castle
			case G1:
				MovePiece(F1, H1, pos);
				break;
				//black queenside castle
			case C8:
				MovePiece(D8, A8, pos);
				break;
				//black kingside castle
			case G8:
				MovePiece(F8, H8, pos);
				break;
			default:
				assert(false);
				break;
			}
	}

	//take back the move in the board
	MovePiece(to, from, pos);

	//uptade back king square
	if (isPceK[pos->pieces[from]])
		pos->kingSq[pos->side] = from;

	//if a piece was captured add it back
	if (capturedpce(move) != EMPTY) {
		assert(PieceValid(capturedpce(move)));
		AddPiece(to, pos, capturedpce(move));
	}

	//if a pawn was promoted clear the promoted piece and add back the promoted pawn
	if (promotedpce(move) != EMPTY) {
		assert(PieceValid(promotedpce(move)));
		ClearPiece(from, pos);
		if (pceCol[promotedpce(move)] == white)
			AddPiece(from, pos, wP);
		else
			AddPiece(from, pos, bP);
	}

	assert(CheckBoard(pos));

}
