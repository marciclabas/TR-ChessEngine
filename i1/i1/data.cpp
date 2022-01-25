#include "Header.h"
using namespace std;

//chars for printing
char pceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rankChar[] = "12345678";
char fileChar[] = "abcdefgh";

//piece characteristics booleans
bool isPceBig[13] = { false ,false ,true,true,true,true,true,false ,true,true,true,true,true };
bool isPceMaj[13] = { false ,false ,false ,false ,true,true,true,false ,false ,false ,true,true,true };
bool isPceMin[13] = { false ,false ,true,true,false ,false ,false ,false ,true,true,false ,false ,false  };

//color of each piece
int pceCol[13] = { both,white,white,white,white,white,white,black,black,black,black,black,black };

//piece types moving direction wise
bool isPceP[13] = { false, true, false, false, false, false, false, true, false, false, false, false, false };
bool isPceN[13] = { false , false , true, false , false , false , false , false , true, false , false , false , false  }; //is it a knight
bool isPceK[13] = { false , false , false , false , false , false , true, false , false , false , false , false , true }; //is it a king
bool isPceRQ[13] = { false , false , false , false , true, true, false , false , false , false , true, true, false  }; //is it a rook or queen
bool isPceBQ[13] = { false , false , false , true, false , true, false , false , false , true, false , true, false  }; //is it a bishop or queen
bool isPceSlider[13] = { false , false , false , true, true, true, false , false , false , true, true, true, false  };

