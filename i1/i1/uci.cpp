#include "Header.h"
#include <string>

void GoIn(char* line, C_SEARCHINFO* info, C_BOARD* pos) {

	int depth = -1, movestogo = 30, movetime = -1;
	int time = -1, inc = 0;
	char* ptr = NULL;
	info->timeset = false;

	if ((ptr = strstr(line, "infinite"))) {
		;
	}

	if ((ptr = strstr(line, "binc")) and pos->side == black) {
		inc = atoi(ptr + 5);
	}

	if ((ptr = strstr(line, "winc")) and pos->side == white) {
		inc = atoi(ptr + 5);
	}

	if ((ptr = strstr(line, "wtime")) and pos->side == white) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line, "btime")) and pos->side == black) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line, "movestogo"))) {
		movestogo = atoi(ptr + 10);
	}

	if ((ptr = strstr(line, "movetime"))) {
		movetime = atoi(ptr + 9);
	}

	if ((ptr = strstr(line, "depth"))) {
		depth = atoi(ptr + 6);
	}

	if (movetime != -1) {
		time = movetime;
		movestogo = 1;
	}

	info->starttime = GetTime();
	info->depth = depth;

	if (time != -1) {
		info->timeset = true;
		time = (time / movestogo) - 50;
		info->stoptime = info->starttime + time + inc;
	}

	if (depth == -1) {
		info->depth = MAX_DEPTH;
	}

	std::cout << "time: " << std::dec << time << " start: " << std::dec << info->starttime << " stop: " << std::dec << info->stoptime << " depth: " << std::dec << info->depth << " timeset: " << std::dec << info->timeset << std::endl;
	SearchPosition(pos, info);

}

void PositionIn(char* lineIn, C_BOARD* pos) {

	std::string fen;
	lineIn += 9;
	char* ptrChar = lineIn;

	if (!strncmp(lineIn, "startpos",8)){
		fen = START_FEN;
		FenIn(&fen[0], pos);
	}
	else {
		ptrChar = strstr(lineIn, "fen");
			ptrChar += 4;
			FenIn(ptrChar, pos);
	}

	ptrChar = strstr(lineIn, "moves");
	
	if (ptrChar != NULL) {
		int move;
		ptrChar += 6;
		while (*ptrChar) {
			move = MoveIn(ptrChar, pos);
			if (move == NO_MOVE)
				break;
			MakeMove(pos, move);
			pos->ply = 0;
			while (*ptrChar and *ptrChar != ' ')
				++ptrChar;
			++ptrChar;
		}
	}
	
	PrintBoard(pos);

}

void UciLoop(C_BOARD* pos,C_SEARCHINFO* info) {

	std::string line;
	std::cout << "id name " << NAME << std::endl;
	std::cout << "id author M4RC1" << std::endl;
	std::cout << "uciok" << std::endl;

	while (true) {

		std::getline(std::cin, line);

		if (line[0] == '\n')
			continue;

		if (!strncmp(&line[0], "isready", 7)) {
			std::cout << "readyok" << std::endl;
			continue;
		}
		else if (!strncmp(&line[0], "position", 7))
			PositionIn(&line[0], pos);
		else if (!strncmp(&line[0], "ucinewgame", 10)) {
			line = "position startpos";
			PositionIn(&line[0], pos);
		}
		else if (!strncmp(&line[0], "go", 2))
			GoIn(&line[0], info, pos);
		else if (!strncmp(&line[0], "quit", 4)) {
			info->quit = true;
			break;
		}
		else if (!strncmp(&line[0], "uci", 3)) {
			std::cout << "id name " << NAME << std::endl;
			std::cout << "id author M4RC1" << std::endl;
			std::cout << "uciok" << std::endl;
		}

		if (info->quit)
			break;

		
	}
}