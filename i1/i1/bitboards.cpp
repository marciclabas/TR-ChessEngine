#include "Header.h"
#include <math.h>

void SetBitBoard(int sq, U64 *bb){
	U64 setter = 1;
	setter = setter << sq120to64[sq];
	*bb |= setter;
}

void PrintBitBoard(U64 bb){
	U64 tester;
	for (int r = rank_8; r >= rank_1; --r){
		for (int f = file_A; f <= file_H; ++f)
		{
			tester = 1ULL << sq64(f, r);
			if (bb & tester) {
				std::cout << "i ";
			}
			else {
				std::cout << "- ";
			}
		}
		std::cout << std::endl;
	}
}


int CountBits(U64 bb) {
	int r;
	for (r = 0; bb; r++, bb &= bb - 1);
	return r;
}

const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

int PopBits(U64 *bb) {
	U64 b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}


