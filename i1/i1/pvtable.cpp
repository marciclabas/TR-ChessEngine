#include "Header.h"

//prints line in pvArray and returns the number of halfmoves
int GetPvLine(const int depth, C_BOARD* pos) {

	assert(depth < MAX_DEPTH);

	int move = ReadPvTable(pos);
	int count = 0;

	while (move != NO_MOVE and count < MAX_DEPTH) {

		assert(count < MAX_DEPTH);

		if (MoveExists(pos, move)) {
			MakeMove(pos, move);
			pos->pvArray[count] = move;
			++count;
		}
		else
			break;
		move = ReadPvTable(pos);
	}

	while (pos->ply > 0)
		TakeMove(pos);
	return count;
}

//resets all entries
void ClearPvTable(C_PVTABLE* table) {

	for (C_PVENTRY* ptr = table->ptrTable; ptr < table->ptrTable + table->numEntries; ++ptr) {
		ptr->hashKey = 0;
		ptr->move = 0;
	}


}

//stores a move in the corresponding entry class
void StorePvMove(const C_BOARD* pos, const int move) {

	int i = pos->hashKey % pos->pvTable->numEntries;
	assert(i >= 0 and i <= pos->pvTable->numEntries - 1);

	pos->pvTable->ptrTable[i].move = move;
	pos->pvTable->ptrTable[i].hashKey = pos->hashKey;
 }

int ReadPvTable(const C_BOARD* pos) {

	int i = pos->hashKey % pos->pvTable->numEntries;
	assert(i >= 0 and i <= pos->pvTable->numEntries - 1);

	if (pos->pvTable->ptrTable[i].hashKey == pos->hashKey)
		return pos->pvTable->ptrTable[i].move;
	return NO_MOVE;
}