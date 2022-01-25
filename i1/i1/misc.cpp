#include "Header.h"
#include <Windows.h>
#include <io.h>

int GetTime() {
	return GetTickCount();
}

// code from http://home.arcor.de/dreamlike/chess/
int InputWaiting(){
	static int init = 0, pipe;
	static HANDLE inh;
	DWORD dw;

	if (!init) {
		init = 1;
		inh = GetStdHandle(STD_INPUT_HANDLE);
		pipe = !GetConsoleMode(inh, &dw);
		if (!pipe) {
			SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
			FlushConsoleInputBuffer(inh);
		}
	}
	if (pipe) {
		if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
		return dw;
	}
	else {
		GetNumberOfConsoleInputEvents(inh, &dw);
		return dw <= 1 ? 0 : dw;
	}
}

void ReadInput(C_SEARCHINFO* info) {
	int bytes;
	char input[256] = "", * endc;

	if (InputWaiting()) {
		info->stopped = true;
		do {
			bytes = _read(_fileno(stdin), input, 256);
		} while (bytes < 0);
		endc = strchr(input, '\n');
		if (endc)* endc = 0;

		if (strlen(input) > 0) {
			if (!strncmp(input, "quit", 4)) {
				info->quit = TRUE;
			}
		}
		return;
	}
}

int ThreeFoldRep(const C_BOARD* pos) {

	int r = 0;
	for (int i = 0; i < pos->hisPly; ++i) {
		if (pos->history[i].hashKey == pos->hashKey)
			r++;
	}
	return r;

}

bool DrawByMaterial(const C_BOARD* pos) {

	if (pos->pceNum[wP] or pos->pceNum[bP]) return false;
	if (pos->pceNum[wQ] or pos->pceNum[bQ] or pos->pceNum[wR] or pos->pceNum[bR]) return false;
	if (pos->pceNum[wB] > 1 or pos->pceNum[bP] > 1) return false;
	if (pos->pceNum[wN] > 1 or pos->pceNum[bN] > 1) return false;
	if (pos->pceNum[wN] and pos->pceNum[wB]) return false;
	if (pos->pceNum[bN] and pos->pceNum[bB]) return false;

	return true;
}

bool CheckResult(C_BOARD* pos) {

	if (pos->fiftyMoveCount > 100)
		std::cout << "1/2-1/2 {fifty move ruled (claimed by " << NAME << ")}" << std::endl;
	if (ThreeFoldRep(pos) >= 2)
		std::cout << "1/2-1/2 {3-fold repetition (claimed by " << NAME << ")}" << std::endl;
	if (DrawByMaterial(pos))
		std::cout << "1/2-1/2 {insufficient material (claimed by " << NAME << ")}" << std::endl;

	C_MOVELIST* list = new C_MOVELIST;
	GenerateAllMoves(pos, list);

	int found = 0;

	for (int i = 0; i < list->count; ++i) {
		if (!MakeMove(pos, list->moves[i].move))
			continue;
		found++;
		TakeMove(pos);
		break;
	}

	if (found != 0)
		return false;

	if (IsSquareAttacked(pos->kingSq[pos->side], oppside(pos->side), pos)) {
		if (pos->side == white) {
			std::cout << "0-1 {black mates (claimed by " << NAME << ")}" << std::endl;
			return true;
		}
		std::cout << "1-0 {white mates (claimed by " << NAME << ")}" << std::endl;
		return true;
	}
	std::cout << "1/2-1/2 {stalemate (claimed by " << NAME << ")}" << std::endl;
	return true;

}