#include "Header.h"

std::string PrintSquare(const int sq) {

	std::string sqStr = { char('a' + filesBrd[sq]),char('1' + ranksBrd[sq]) };

	return sqStr;

}

std::string PrintMove(const int move) {

	int fromf = filesBrd[fromsq(move)];
	int fromr = ranksBrd[fromsq(move)];
	int tof = filesBrd[tosq(move)];
	int tor = ranksBrd[tosq(move)];

	std::string moveStr = { char('a' + fromf),char('1' + fromr),char('a' + tof),char('1' + tor) }; //from square and to square

	int promPce = promotedpce(move);

	if (promPce) {
		char promChar = 'q'; //promote queen by default
		if (isPceN[promPce]) promChar = 'n';
		else if (isPceBQ[promPce] and !isPceRQ[promPce]) promChar = 'b';
		else if (isPceRQ[promPce] and !isPceBQ[promPce]) promChar = 'r';
		moveStr += promChar;
	}

	return moveStr;
}

void PrintMoveList(const C_MOVELIST* list, const C_BOARD* pos) {;
	int score = 0;
	int move = 0;

	for (int i = 0; i < list->count; ++i) {

		move = list->moves[i].move;
		score = list->moves[i].score;

		std::cout << "Move" << i + 1 << " -> " << pceChar[pos->pieces[fromsq(move)]] << PrintMove(move) << " (" << score << ')' << std::endl;
	}
	std::cout << "Total Moves Generated: " << list->count << std::endl;
}

int MoveIn(const char* ptr, C_BOARD* pos) {
	//checks the syntax is correct
	if (ptr[0] > 'h' or ptr[0] < 'a') return NO_MOVE;
	if (ptr[2] > 'h' or ptr[2] < 'a') return NO_MOVE;
	if (ptr[1] > '8' or ptr[1] < '1') return NO_MOVE;
	if (ptr[3] > '8' or ptr[3] < '1') return NO_MOVE;

	int from = sq120(ptr[0] - 'a', ptr[1] - '1');
	int to = sq120(ptr[2] - 'a', ptr[3] - '1');
	int move;

	C_MOVELIST list;
	GenerateAllMoves(pos, &list);
	
	//loops through all the possible moves and returns the move if it corresponds with the input move
	for (int i = 0; i < list.count; ++i) {
		move = list.moves[i].move;
		if (fromsq(move) == from and tosq(move) == to) {
			if (promotedpce(move) != EMPTY) {
				if (isPceN[promotedpce(move)] and ptr[4] == 'n') 
					return move;
				if (isPceBQ[promotedpce(move)] and !isPceRQ[promotedpce(move)] and ptr[4] == 'b') 
					return move;
				if (!isPceBQ[promotedpce(move)] and isPceRQ[promotedpce(move)] and ptr[4] == 'r') 
					return move;
				if (isPceBQ[promotedpce(move)] and isPceRQ[promotedpce(move)] and ptr[4] == 'q') 
					return move;
				continue;
			}
			return move;
		}
	}
	//if the input move wasn't found
	return NO_MOVE;
}