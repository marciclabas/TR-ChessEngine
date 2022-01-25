#include "Header.h"

long leafNodes;

void Perft(int depth, C_BOARD* pos) {

	assert(CheckBoard(pos));

	if (depth == 0) {
		++leafNodes;
		return;
	}

	C_MOVELIST list;
	//std::cout << "depth: " << 3 - depth << std::endl;
	GenerateAllMoves(pos, &list);

	for (int i = 0; i < list.count; ++i) {

		//check for illegal moves and avoid generating them
		if (!MakeMove(pos, list.moves[i].move))
			continue;
		Perft(depth - 1, pos);
		TakeMove(pos);
	}
	

	return;
}

void PerftTest(int depth, C_BOARD* pos) {

	assert(CheckBoard(pos));

	U64 time0 = GetTime();

	PrintBoard(pos);
	std::cout << "Starting test, depth: " << depth << std::endl;
	leafNodes = 0;

	C_MOVELIST list;
	GenerateAllMoves(pos, &list);

	int move;

	for (int i = 0; i < list.count; ++i) {
		move = list.moves[i].move;
		if (!MakeMove(pos, move)) continue;
		long cumnodes = leafNodes;
		Perft(depth - 1, pos);
		TakeMove(pos);
		long oldnodes = leafNodes - cumnodes;
		std::cout << "move: " << i + 1 << ' ' << PrintMove(move) << ' ' << oldnodes << std::endl;
	}
	
	std::cout << "Total nodes: " << leafNodes << " found in " << (U64)GetTime() - time0 << " milliseconds" << std::endl;

	return;
}
