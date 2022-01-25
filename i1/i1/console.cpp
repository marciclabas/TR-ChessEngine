#include "Header.h"

void ConsoleLoop(C_BOARD* pos, C_SEARCHINFO* info) {

	system("cls");
	std::cout << NAME << "> " << NAME << " is running in console mode with normal presets" << std::endl;
	std::cout << NAME << "> " << "Type help for info on commands to change configuration" << std::endl << std::endl;

	info->gamemode = console_mode;
	info->postthinking = true;

	int depth = MAX_DEPTH;
	int movetime = 3000;
	int engineSide = black;
	int move = NO_MOVE;

	std::string line, fen=START_FEN;

	FenIn(&fen[0], pos);

	while (true) {

		if (pos->side == engineSide and !CheckResult(pos)) {
			info->starttime = GetTime();
			info->depth = depth;

			if (movetime != 0) {
				info->timeset = true;
				info->stoptime = info->starttime + movetime;
			}

			SearchPosition(pos, info);
		}

		std::cout << std::endl << '/';

		std::getline(std::cin, line);

		if (line[0] == '\n')
			continue;

		if (!strncmp(&line[0], "help", 4)) {
			std::cout << "Commands:" << std::endl;
			std::cout << "quit - quit game" << std::endl;
			std::cout << "force - computer will not think" << std::endl;
			std::cout << "analyze - show thinking only" << std::endl;
			std::cout << "play - do not show thinking and do moves" << std::endl;
			std::cout << "new - start new game" << std::endl;
			std::cout << "go - set computer thinking" << std::endl;
			std::cout << "depth x - set depth to x" << std::endl;
			std::cout << "time x - set thinking time to x seconds (depth still applies if set)" << std::endl;
			std::cout << "view - show current depth and movetime settings" << std::endl;
			std::cout << "** note ** - to reset time and depth, set to 0" << std::endl;
			std::cout << "enter moves using b7b8q notation" << std::endl << std::endl << std::endl;
			continue;
		}

		if (!strncmp(&line[0], "quit", 4)) {
			info->quit = true;
			break;
		}

		if (!strncmp(&line[0], "analyze", 7)) {
			system("cls");
			PrintBoard(pos);
			info->postthinking = true;
			continue;
		}

		if (!strncmp(&line[0], "play", 4)) {
			system("cls");
			PrintBoard(pos);
			info->postthinking = false;
			continue;
		}

		if (!strncmp(&line[0], "force", 5)) {
			system("cls");
			PrintBoard(pos);
			engineSide = both;
			continue;
		}

		if (!strncmp(&line[0], "view", 4)) {
			if (depth == MAX_DEPTH)
				std::cout << NAME << "> " << "depth not set" << std::endl;
			else
				std::cout << NAME << "> " << "depth " << depth << std::endl;

			if (movetime != 0)
				std::cout << NAME << "> " << "movetime " << movetime / 1000;
			else
				std::cout << NAME << "> " << "movetime not set" << std::endl;

			continue;
		}

		if (!strncmp(&line[0], "depth", 5)) {
			system("cls");
			PrintBoard(pos);
			sscanf_s(&line[0], "depth %d", &depth);
			if (depth == 0)
				depth = MAX_DEPTH;
			continue;
		}

		if (!strncmp(&line[0], "time", 4)) {
			system("cls");
			PrintBoard(pos);
			sscanf_s(&line[0], "time %d", &movetime);
			movetime *= 1000;
			continue;
		}

		if (!strncmp(&line[0], "new", 3)) {
			engineSide = black;
			fen = START_FEN;
			FenIn(&fen[0], pos);
			system("cls");
			PrintBoard(pos);
			continue;
		}

		if (!strncmp(&line[0], "fen", 3)) {
			engineSide = black;
			FenIn(&line[4], pos);
			system("cls");
			PrintBoard(pos);
			continue;
		}

		if (!strncmp(&line[0], "go", 2)) {
			system("cls");
			PrintBoard(pos);
			engineSide = pos->side;
			continue;
		}	

		if (!strncmp(&line[0], "eval", 4)) {
			std::cout << std::endl;
			std::cout << "static eval: " << EvaluatePosition(pos) << std::endl;
			continue;
		}

		move = MoveIn(&line[0], pos);
		if (move == NO_MOVE) {
			std::cout << "Command unknown: " << line;
			continue;
		}

		MakeMove(pos, move);
		pos->ply = 0;

		system("cls");
		PrintBoard(pos);

	} 
}