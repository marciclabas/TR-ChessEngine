#include "Header.h"

void PrintOptions() {
	std::cout << "feature ping=1 setboard=1 colors=0 usermove=1" << std::endl;
	std::cout << "feature done=1" << std::endl;
}

void XBoardLoop(C_BOARD* pos, C_SEARCHINFO* info) {

	info->gamemode = xboard_mode;
	info->postthinking = true;
	PrintOptions();

	std::string line, fen = START_FEN;
	int depth = -1, movestogo[2] = { 30,30 }, movetime = -1;
	int time = -1, inc = 0;
	int engineSide = black;
	int timeLeft = 0; 
	int sec = 0;
	int mps = 0;
	int move = NO_MOVE;
	int score;

	FenIn(&fen[0], pos);

	while (true) {

		if (pos->side == engineSide and !CheckResult(pos)) {
			info->starttime = GetTime();
			info->depth = depth;

			if (time != -1) {
				info->timeset = true;
				time = (time / movestogo[pos->side]) - 50;
				info->stoptime = info->starttime + time + inc;
			}

			if (depth == -1 or depth > MAX_DEPTH)
				info->depth = MAX_DEPTH;

			SearchPosition(pos, info);

			if (mps != 0) {
				movestogo[oppside(pos->side)]--;
				if (movestogo[oppside(pos->side)] > 1)
					movestogo[oppside(pos->side)] = mps;
			}
		}

		std::getline(std::cin, line);

		if (line[0] == '\n')
			continue;

		std::cout << "command seen: " << line << std::endl;

		if (!strncmp(&line[0], "quit", 4))
			break;

		if (!strncmp(&line[0], "force", 5)) {
			engineSide = both;
			continue;
		}


		if (!strncmp(&line[0], "protover", 8)) {
			PrintOptions();
			continue;
		}

		if(!strncmp(&line[0],"sd",2)){
			sscanf_s(&line[0], "sd %d", &depth);
			continue;
		}

		if (!strncmp(&line[0], "st", 2)) {
			sscanf_s(&line[0], "st %d", &movetime);
			continue;
		}

		if (!strncmp(&line[0], "time", 4)) {
			sscanf_s(&line[0], "time %d", &time);
			time *= 10;
		}

		if (!strncmp(&line[0], "level", 5)) {
			sec = 0;
			movetime = -1;

			if (sscanf_s(&line[0], "level %d %d %d", &mps, &timeLeft, &inc) != 3) 
				sscanf_s(&line[0], "level %d %d:%d %d", &mps, &timeLeft, &sec, &inc);
			
			timeLeft = (timeLeft * 60000) + (sec * 1000);
			movestogo[0] = movestogo[1] = 30;

			if (mps != 0)
				movestogo[0] = movestogo[1] = mps;

			time = -1;
			continue;
		}

		if (!strncmp(&line[0], "ping", 4)) {
			std::cout << "pong" << std::endl << line[5];
			continue;
		}

		if (!strncmp(&line[0], "new", 3)) {
			engineSide = black;
			fen = START_FEN;
			FenIn(&fen[0], pos);
			continue;
		}

		if (!strncmp(&line[0], "undo", 4)) {
			TakeMove(pos);
			continue;
		}

		if (!strncmp(&line[0], "setboard", 8)) {
			engineSide = both;
			FenIn(&line[9], pos);
			continue;
		}

		if (!strncmp(&line[0], "go", 2)) {
			engineSide = pos->side;
			continue;
		}

		if (!strncmp(&line[0], "usermove", 8)) {
			move = MoveIn(&line[9], pos);
			if (move == NO_MOVE)
				continue;
			MakeMove(pos, move);
			pos->ply = 0;
		}

	}
}