#include "Header.h"

int main() {

	AllInit();
	PrintBitBoard(ClearMask[E4]);
	PrintBitBoard(SetMask[E4]);

	C_BOARD* pos = new C_BOARD;
	C_SEARCHINFO* info = new C_SEARCHINFO;

	std::cout << "Welcome to " << NAME << ", type " << '"' << "console" << '"' << " for console mode" << std::endl;
	std::string line;

	while (true) {
		std::getline(std::cin, line);

		if (line[0] == '\n')
			continue;

		if (!strncmp(&line[0], "uci", 3)) {
			UciLoop(pos, info);
			if (info->quit == true)
				break;
			continue;
		}

		if (!strncmp(&line[0], "xboard", 6)) {
			XBoardLoop(pos, info);
			if (info->quit == true)
				break;
			continue;
		}

		if (!strncmp(&line[0], "console", 7)) {
			ConsoleLoop(pos, info);
			if (info->quit == true)
				break;
			continue;
		}

		if (!strncmp(&line[0], "quit", 4))
			break;
	}

	delete pos;
	delete info;

}