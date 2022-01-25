#pragma once
#include <iostream>
//#include <conio.h> //to use keyPress
#include <bitset>
#include "assert.h"
#include <iomanip> //to use setw()
#include <string> //to cout string
//#include <vector>
#define NDEBUG

typedef unsigned long long U64;

//defs
#define NAME "i1"
#define BRD_SQ_NUM 120
#define MAX_NUMBER_MOVES 2048
#define MAX_POS_MOVES 256
#define NO_MOVE 0
#define MAX_DEPTH 64
#define INPUT_BUFFER 2400
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN_IN "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
#define TEST "k7/8/8/8/8/8/8/K4Q2 w - - 0 1"

//ext variables
extern int sq64to120[64];
extern int sq120to64[BRD_SQ_NUM];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 pceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];
extern char pceChar[];
extern char sideChar[];
extern char rankChar[];
extern char fileChar[];
extern bool isPceBig[13];
extern bool isPceMaj[13];
extern bool isPceMin[13];
extern bool isPceP[13];
extern bool isPceN[13];
extern bool isPceK[13];
extern bool isPceRQ[13];
extern bool isPceBQ[13];
extern int pceVal[13];
extern int pceCol[13];
extern int filesBrd[BRD_SQ_NUM];
extern int ranksBrd[BRD_SQ_NUM];
extern int mvvLvaScores[13][13]; //[victim][attacker]

//eval masks
extern U64 fileMask[8];
extern U64 rankMask[8];

extern U64 whitePassedMask[64];
extern U64 blackPassedMask[64];
extern U64 isolatedMask[64];
extern U64 whiteWeakMask[64];
extern U64 blackWeakMask[64];
extern U64 whiteDoubledMask[64];
extern U64 blackDoubledMask[64];

extern U64 whiteClosedMask[4];
extern U64 blackClosedMask[4];
extern U64 whiteSopenMask[2];
extern U64 blackSopenMask[2];
extern U64 centralMask;
extern U64 whiteSemiCentralMask;
extern U64 blackSemiCentralMask;

extern U64 whiteQCastleMask;
extern U64 whiteKCastleMask;
extern U64 blackQCastleMask;
extern U64 blackKCastleMask;

//enums
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum { file_A, file_B, file_C, file_D, file_E, file_F, file_G, file_H };
enum { rank_1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8 };
enum { white, black, both };
enum { uci_mode, xboard_mode, console_mode };

enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { WKC = 1, WQC = 2, BKC = 4, BQC = 8 }; //castling permission per side and colour in a 4-bit number (2^3,2^2,2^1,2^0 respectively)

//macros
//file and rank to square
#define sq120(f,r) (10*(r+2)+f+1)		
#define sq64(f,r) (8*r+f)

//bitboards
#define clrbit(bb,sq) (bb &=ClearMask[sq])
#define setbit(bb,sq) (bb |=SetMask[sq])

//unsigned long long random generator
#define rndU64 ((U64)rand())|((U64)rand()<<15)|((U64)rand()<<30)|((U64)rand()<<45)|(((U64)rand()&0xf)<<60)

//move variable extractors
#define fromsq(m) ((m)&(0x7F))
#define tosq(m) (((m)>>7)&(0x7F))
#define capturedpce(m) (((m)>>14)&(0xF))
#define promotedpce(m) (((m)>>20)&(0xF))
#define enpasflag 0x40000
#define pawnstflag 0x80000
#define castleflag 0x1000000
#define captureflag 0x7C000
#define promotionflag 0xF00000

//hash in or out
#define hashpce(pce,sq) (pos->hashKey ^= (pceKeys[(pce)][(sq)]))
#define hashcastle (pos->hashKey ^= (castleKeys[(pos->castlePerm)]))
#define hashside (pos->hashKey ^= (sideKey))
#define hashenpas (pos->hashKey ^= (pceKeys[EMPTY][(pos->enPassant)]))

//create move int
#define move(from,to,captured,promoted,flag) ((from) | ((to) << 7) | ((captured) << 14) | ((promoted) << 20) | (flag))

//is square offboard
#define sqoffboard(sq) (filesBrd[sq]==OFFBOARD)

//return opposite side
//as white=0 and black=1 are the inverse of each other we can xor them to return the opposite color in this manner: 0^1=1, 1^1=0
#define oppside(side) ((side)^1)

//classes
class C_MOVE {
public:
	int move; //representation per bits: 1 bit(castle bool),4 bits(promoted piece[if any]), 1 bit(pawn start move), 1 bit(en passant capture bool),4 bits(captured piece[if any]),6 bits(ending square),6 bits(initial square), 25 in total
	int score;
};

class C_MOVELIST {
public:
	C_MOVE moves[MAX_POS_MOVES];
	int count;
};

class C_PVENTRY {
public:
	C_PVENTRY() {
		hashKey = 0ULL;
		move = NO_MOVE;
	}

	U64 hashKey;
	int move;
};

class C_PVTABLE {
public:
	C_PVTABLE() {
		numEntries = (0x100000 * 2 / sizeof(C_PVENTRY)) - 2;
		ptrTable = new C_PVENTRY[numEntries];
	}
	int numEntries;
	C_PVENTRY *ptrTable;
};

class  C_UNDO {
public: //consider making it private?
	int move;
	int castlePerm;
	int enPassant;
	int fiftyMoveCount;
	U64 hashKey;
};

class C_BOARD {
public:
	int pieces[BRD_SQ_NUM]; //stores all pieces
	U64 pawns[3];
	int kingSq[2];
	int side; //to move
	int enPassant;
	int fiftyMoveCount;
	int ply; //half moves
	int hisPly;
	int castlePerm;
	U64 hashKey; //position id

	int bigPcs[2]; //other than pawns
	int minPcs[2]; //knights and bishops
	int majPcs[2]; //rooks and queens
	int material[2];

	C_UNDO history[MAX_NUMBER_MOVES];

	int pceList[13][10] = { NO_SQ };
	int pceNum[13]; 

	C_PVTABLE* pvTable= new C_PVTABLE;
	int pvArray[MAX_DEPTH];

	int searchHistory[13][BRD_SQ_NUM]; //[piece moved][to square]
	int searchKillers[2][MAX_DEPTH]; //killers are quiet moves who lead to a beta cut off. We store the 2 last killers for each ply, the first one indexed being the most recent one
	
};

class C_SEARCHINFO {
public:
	C_SEARCHINFO() {
		starttime=0;
		stoptime=0;
		depth=0;
		depthset=false;
		timeset=false;
		movestogo=0;
		infinite=false;
		nodes=0;
		quit=false;
		stopped=false;

		fh=0;
		fhf=0;

		gamemode = 0;
		postthinking = true;
	}

	int starttime;
	int stoptime;
	int depth;
	bool depthset;
	bool timeset;
	int movestogo;
	bool infinite;
	long nodes;
	bool quit;
	bool stopped;

	float fh;
	float fhf;

	int gamemode;
	bool postthinking;

};

//functions
void SetBitBoard(int sq, U64 *bb);
void PrintBitBoard(U64 bb);
int CountBits(U64 bb);
int PopBits(U64 *bb);
void AllInit();
U64 GenerateHashKey(const C_BOARD* pos);
void ResetBoard(C_BOARD* pos);
int FenIn(char* fen, C_BOARD *pos);
void PrintBoard(const C_BOARD *pos);
bool IsSquareAttacked(const int sq, const int side, const C_BOARD* pos);
std::string PrintSquare(const int sq);
std::string PrintMove(const int move);
bool SquareOnBoard(const int sq);
bool SideValid(const int side);
bool FileRankValid(const int fr);
bool PieceValidOrEmpty(const int pce);
bool PieceValid(const int pce);
bool CheckBoard(const C_BOARD* pos);
void PrintMoveList(const C_MOVELIST* list, const C_BOARD* pos);
void GenerateAllMoves(const C_BOARD* pos, C_MOVELIST* list);
void GenerateAllCaptures(const C_BOARD* pos, C_MOVELIST* list);
bool MakeMove(C_BOARD* pos, int move);
void TakeMove(C_BOARD* pos);
void PerftTest(int depth, C_BOARD* pos);
int MoveIn(const char* ptr, C_BOARD* pos);
bool IsRepetition(const C_BOARD* pos);
int GetTime();
void ClearPvTable(C_PVTABLE* table);
void StorePvMove(const C_BOARD* pos, const int move);
int ReadPvTable(const C_BOARD* pos);
int MoveExists(C_BOARD* pos, const int move);
int GetPvLine(const int depth, C_BOARD* pos);
int EvaluatePosition(C_BOARD* pos);
void SearchPosition(C_BOARD* pos, C_SEARCHINFO* info);
void UciLoop(C_BOARD* pos, C_SEARCHINFO* info);
void XBoardLoop(C_BOARD* pos, C_SEARCHINFO* info);
void ConsoleLoop(C_BOARD* pos, C_SEARCHINFO* info);
bool CheckResult(C_BOARD* pos);
void ReadInput(C_SEARCHINFO* info);