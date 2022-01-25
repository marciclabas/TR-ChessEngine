#include "Header.h"
#define INFINITE 30000
#define MATE 29000

bool IsRepetition(const C_BOARD* pos) {

	//as after a capture or a pawn move all the positions before can't be repeated anymore, to check for repetitions we can start at the moment where the fifty move counter was reseted
	for (int i = pos->hisPly-pos->fiftyMoveCount; i < pos->hisPly-1; ++i) {

		assert(i>=0 and i<MAX_NUMBER_MOVES);

		if (pos->hashKey == pos->history[i].hashKey)
			return true;
	}
	return false;
}

static void CheckUp(C_SEARCHINFO* info) {

	if (info->timeset == true and GetTime() > info->stoptime)
		info->stopped = true;
	ReadInput(info);

}

static void ClearForSearch(C_BOARD* pos,C_SEARCHINFO* info) {

	for (int i = 0; i < 13; ++i) {
		for (int j = 0; j < BRD_SQ_NUM; ++j)
			pos->searchHistory[i][j] = 0;
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < MAX_DEPTH; ++j)
			pos->searchKillers[i][j] = 0;
	}

	ClearPvTable(pos->pvTable);
	pos->ply = 0;
	info->stopped = 0;
	info->nodes = 0;
	info->fh = 0;
	info->fhf = 0;
}

static void PickMove(int moveNum, C_MOVELIST* list) {

	C_MOVE tempMove;
	int bestScore = 0;
	int bestNum = moveNum;

	for (int i = moveNum; i < list->count; ++i) {
		if (list->moves[i].score > bestScore) {
			bestScore = list->moves[i].score;
			bestNum = i;
		}
	}

	//exchange actual moveNum place by the best score found throughout the move list
	tempMove = list->moves[moveNum];
	list->moves[moveNum] = list->moves[bestNum];
	list->moves[bestNum] = tempMove;

}

static int Quiescence(int alpha, int beta, C_BOARD* pos, C_SEARCHINFO* info) {
	
	assert(CheckBoard(pos));

	++info->nodes;

	if (IsRepetition(pos) or pos->fiftyMoveCount >= 100)
		return 0;

	if (pos->ply > MAX_DEPTH - 1)
		return EvaluatePosition(pos);

	int score = EvaluatePosition(pos);

	if (score >= beta) 
		return beta;

	if (score > alpha)
		alpha = score;

	C_MOVELIST list[1];
	GenerateAllCaptures(pos, list);

	int legal = 0;
	int oldAlpha = alpha;
	int bestMove = NO_MOVE;
	score = -INFINITE;
	int pvMove = ReadPvTable(pos);

	for (int moveNum = 0; moveNum < list->count; ++moveNum) {

		PickMove(moveNum, list);

		if (!MakeMove(pos, list->moves[moveNum].move))
			continue;

		++legal;
		score = -Quiescence(-beta,-alpha, pos, info);
		TakeMove(pos);

		if (info->stopped == true)
			return 0;

		if (score > alpha) {
			if (score >= beta) {
				if (legal == 1)
					++info->fhf;
				++info->fh;
				return beta;
			}
			alpha = score;
			bestMove = list->moves[moveNum].move;

		}
	}

	if (alpha != oldAlpha)
		StorePvMove(pos, bestMove);

	return alpha;
}

static int AlphaBeta(int alpha,int beta,int currentDepth,C_BOARD* pos, C_SEARCHINFO* info, bool doNull) {
	
	assert(CheckBoard(pos));

	if (currentDepth == 0) {
		return Quiescence(alpha, beta, pos, info);
	}

	if ((info->nodes & 2047) == 0)
		CheckUp(info);

	++info->nodes;

	if (IsRepetition(pos) or pos->fiftyMoveCount >= 100 and pos->ply)
		return 0;
	
	if (pos->ply > MAX_DEPTH - 1)
		return EvaluatePosition(pos);

	bool inCheck = IsSquareAttacked(pos->kingSq[pos->side], oppside(pos->side), pos);

	if (inCheck)
		currentDepth++;

	C_MOVELIST list[1];
	GenerateAllMoves(pos, list);

	int legal = 0;
	int oldAlpha = alpha;
	int bestMove = NO_MOVE;
	int score = -INFINITE;
	int pvMove = ReadPvTable(pos);

	//if there is a principal variation stored from a previous search we'll search it first
	if (pvMove != NO_MOVE) {
		for (int moveNum = 0; moveNum < list->count; ++moveNum) {
			if (list->moves[moveNum].move == pvMove)
				list->moves[moveNum].score = 2000000;
			break;
		}
	}

	for (int moveNum = 0; moveNum < list->count; ++moveNum) {

		PickMove(moveNum, list);

		if (!MakeMove(pos, list->moves[moveNum].move)) {
			continue;
		}

		++legal;
		score = -AlphaBeta(-beta, -alpha, currentDepth - 1, pos, info, true);
		TakeMove(pos);

		if (info->stopped == true)
			return 0;

		if (score > alpha) {
			if (score >= beta) {
				if (legal == 1)
					++info->fhf;
				++info->fh;

				//set a killer to index 0 if the move is not a capture
				if (!(list->moves[moveNum].move & captureflag)) {
					pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
					pos->searchKillers[0][pos->ply] = list->moves[moveNum].move;
				}

				return beta;
			}
			alpha = score;
			bestMove = list->moves[moveNum].move;

			if (!(list->moves[moveNum].move & captureflag))
				pos->searchHistory[pos->pieces[fromsq(bestMove)]][tosq(bestMove)] += currentDepth;
		}
	}

	//if there is no legal move return mate or stalemate
	if (legal == 0) {
		if (inCheck)
			return (-MATE + pos->ply);
		return 0;
	}

	if (alpha != oldAlpha)
		StorePvMove(pos, bestMove);

	return alpha;

}

void SearchPosition(C_BOARD* pos,C_SEARCHINFO* info) {
	int bestMove = NO_MOVE;
	int bestScore = -INFINITE;
	int pvMoves = 0;

	ClearForSearch(pos, info);

	//iterative deepening (increasing depth from 1)
	for (int currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {

		bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, true);

		if (info->stopped == true)
			break;

		pvMoves = GetPvLine(currentDepth, pos);
		bestMove = pos->pvArray[0];
		
		if (info->gamemode == uci_mode) {
			printf("info score cp %d depth %d nodes %ld time %d ",
				bestScore, currentDepth, info->nodes, GetTime() - info->starttime);
		}
		else if (info->gamemode == xboard_mode && info->postthinking == true) {
			printf("%d %d %d %ld ",
				currentDepth, bestScore, (GetTime() - info->starttime) / 10, info->nodes);
		}
		else if (info->postthinking == true) {
			printf("score:%d depth:%d nodes:%ld time:%d(ms) ",
				bestScore, currentDepth, info->nodes, GetTime() - info->starttime);
		}

		if (info->gamemode == uci_mode || info->postthinking == true) {
			pvMoves = GetPvLine(currentDepth, pos);
			printf("pv");
			for (int i = 0; i < pvMoves; ++i)
				printf(" %s", &PrintMove(pos->pvArray[i])[0]);
			std::cout << std::endl;
		}
	}

	if (info->gamemode == uci_mode) {
		printf("bestmove %s\n", &PrintMove(bestMove)[0]);
	}
	else if (info->gamemode == xboard_mode) {
		printf("move %s\n", &PrintMove(bestMove)[0]);
		MakeMove(pos, bestMove);
	}
	
}