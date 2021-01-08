#include <iostream>
using namespace std;

#define KEYLENGTH 128
#define WORDNUMBER 8
#define WORDNUMBER4 32


static int size = 128;
static int kw = 4;
static int roundNumber = 10;
// static int affineTransformationRight[WORDNUMBER] = {1,1,0,0,0,1,1,0};
// static int affineTransformationLeft[WORDNUMBER] = {1,0,0,0,1,1,1,1};
static int mixColumnsComplex[4][2] = {{1,0},{1,1},{0,1},{0,1}};
static int invMixColumnsComplex[4][4] = {{1,1,1,0},{1,0,1,1},{1,1,0,1},{1,0,0,1}};
static int mixColumnsConstantComplex[WORDNUMBER] = {0,0,0,1,1,0,1,1};

static int RconMod[10][8] = {{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,0},{0,0,0,0,0,1,0,0},{0,0,0,0,1,0,0,0},{0,0,0,1,0,0,0,0},{0,0,1,0,0,0,0,0},{0,1,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{0,0,0,1,1,0,1,1},{0,0,1,1,0,1,1,0}};

static int encSbox[16][16][8] = {{{0,1,1,0,0,0,1,1},{0,1,1,1,1,1,0,0},{0,1,1,1,0,1,1,1},{0,1,1,1,1,0,1,1},{1,1,1,1,0,0,1,0},{0,1,1,0,1,0,1,1},{0,1,1,0,1,1,1,1},{1,1,0,0,0,1,0,1},{0,0,1,1,0,0,0,0},{0,0,0,0,0,0,0,1},{0,1,1,0,0,1,1,1},{0,0,1,0,1,0,1,1},{1,1,1,1,1,1,1,0},{1,1,0,1,0,1,1,1},{1,0,1,0,1,0,1,1},{0,1,1,1,0,1,1,0}},
								 {{1,1,0,0,1,0,1,0},{1,0,0,0,0,0,1,0},{1,1,0,0,1,0,0,1},{0,1,1,1,1,1,0,1},{1,1,1,1,1,0,1,0},{0,1,0,1,1,0,0,1},{0,1,0,0,0,1,1,1},{1,1,1,1,0,0,0,0},{1,0,1,0,1,1,0,1},{1,1,0,1,0,1,0,0},{1,0,1,0,0,0,1,0},{1,0,1,0,1,1,1,1},{1,0,0,1,1,1,0,0},{1,0,1,0,0,1,0,0},{0,1,1,1,0,0,1,0},{1,1,0,0,0,0,0,0}},
								 {{1,0,1,1,0,1,1,1},{1,1,1,1,1,1,0,1},{1,0,0,1,0,0,1,1},{0,0,1,0,0,1,1,0},{0,0,1,1,0,1,1,0},{0,0,1,1,1,1,1,1},{1,1,1,1,0,1,1,1},{1,1,0,0,1,1,0,0},{0,0,1,1,0,1,0,0},{1,0,1,0,0,1,0,1},{1,1,1,0,0,1,0,1},{1,1,1,1,0,0,0,1},{0,1,1,1,0,0,0,1},{1,1,0,1,1,0,0,0},{0,0,1,1,0,0,0,1},{0,0,0,1,0,1,0,1}},
								 {{0,0,0,0,0,1,0,0},{1,1,0,0,0,1,1,1},{0,0,1,0,0,0,1,1},{1,1,0,0,0,0,1,1},{0,0,0,1,1,0,0,0},{1,0,0,1,0,1,1,0},{0,0,0,0,0,1,0,1},{1,0,0,1,1,0,1,0},{0,0,0,0,0,1,1,1},{0,0,0,1,0,0,1,0},{1,0,0,0,0,0,0,0},{1,1,1,0,0,0,1,0},{1,1,1,0,1,0,1,1},{0,0,1,0,0,1,1,1},{1,0,1,1,0,0,1,0},{0,1,1,1,0,1,0,1}},
								 {{0,0,0,0,1,0,0,1},{1,0,0,0,0,0,1,1},{0,0,1,0,1,1,0,0},{0,0,0,1,1,0,1,0},{0,0,0,1,1,0,1,1},{0,1,1,0,1,1,1,0},{0,1,0,1,1,0,1,0},{1,0,1,0,0,0,0,0},{0,1,0,1,0,0,1,0},{0,0,1,1,1,0,1,1},{1,1,0,1,0,1,1,0},{1,0,1,1,0,0,1,1},{0,0,1,0,1,0,0,1},{1,1,1,0,0,0,1,1},{0,0,1,0,1,1,1,1},{1,0,0,0,0,1,0,0}},
								 {{0,1,0,1,0,0,1,1},{1,1,0,1,0,0,0,1},{0,0,0,0,0,0,0,0},{1,1,1,0,1,1,0,1},{0,0,1,0,0,0,0,0},{1,1,1,1,1,1,0,0},{1,0,1,1,0,0,0,1},{0,1,0,1,1,0,1,1},{0,1,1,0,1,0,1,0},{1,1,0,0,1,0,1,1},{1,0,1,1,1,1,1,0},{0,0,1,1,1,0,0,1},{0,1,0,0,1,0,1,0},{0,1,0,0,1,1,0,0},{0,1,0,1,1,0,0,0},{1,1,0,0,1,1,1,1}},
								 {{1,1,0,1,0,0,0,0},{1,1,1,0,1,1,1,1},{1,0,1,0,1,0,1,0},{1,1,1,1,1,0,1,1},{0,1,0,0,0,0,1,1},{0,1,0,0,1,1,0,1},{0,0,1,1,0,0,1,1},{1,0,0,0,0,1,0,1},{0,1,0,0,0,1,0,1},{1,1,1,1,1,0,0,1},{0,0,0,0,0,0,1,0},{0,1,1,1,1,1,1,1},{0,1,0,1,0,0,0,0},{0,0,1,1,1,1,0,0},{1,0,0,1,1,1,1,1},{1,0,1,0,1,0,0,0}},
								 {{0,1,0,1,0,0,0,1},{1,0,1,0,0,0,1,1},{0,1,0,0,0,0,0,0},{1,0,0,0,1,1,1,1},{1,0,0,1,0,0,1,0},{1,0,0,1,1,1,0,1},{0,0,1,1,1,0,0,0},{1,1,1,1,0,1,0,1},{1,0,1,1,1,1,0,0},{1,0,1,1,0,1,1,0},{1,1,0,1,1,0,1,0},{0,0,1,0,0,0,0,1},{0,0,0,1,0,0,0,0},{1,1,1,1,1,1,1,1},{1,1,1,1,0,0,1,1},{1,1,0,1,0,0,1,0}},
								 {{1,1,0,0,1,1,0,1},{0,0,0,0,1,1,0,0},{0,0,0,1,0,0,1,1},{1,1,1,0,1,1,0,0},{0,1,0,1,1,1,1,1},{1,0,0,1,0,1,1,1},{0,1,0,0,0,1,0,0},{0,0,0,1,0,1,1,1},{1,1,0,0,0,1,0,0},{1,0,1,0,0,1,1,1},{0,1,1,1,1,1,1,0},{0,0,1,1,1,1,0,1},{0,1,1,0,0,1,0,0},{0,1,0,1,1,1,0,1},{0,0,0,1,1,0,0,1},{0,1,1,1,0,0,1,1}},
								 {{0,1,1,0,0,0,0,0},{1,0,0,0,0,0,0,1},{0,1,0,0,1,1,1,1},{1,1,0,1,1,1,0,0},{0,0,1,0,0,0,1,0},{0,0,1,0,1,0,1,0},{1,0,0,1,0,0,0,0},{1,0,0,0,1,0,0,0},{0,1,0,0,0,1,1,0},{1,1,1,0,1,1,1,0},{1,0,1,1,1,0,0,0},{0,0,0,1,0,1,0,0},{1,1,0,1,1,1,1,0},{0,1,0,1,1,1,1,0},{0,0,0,0,1,0,1,1},{1,1,0,1,1,0,1,1}},
								 {{1,1,1,0,0,0,0,0},{0,0,1,1,0,0,1,0},{0,0,1,1,1,0,1,0},{0,0,0,0,1,0,1,0},{0,1,0,0,1,0,0,1},{0,0,0,0,0,1,1,0},{0,0,1,0,0,1,0,0},{0,1,0,1,1,1,0,0},{1,1,0,0,0,0,1,0},{1,1,0,1,0,0,1,1},{1,0,1,0,1,1,0,0},{0,1,1,0,0,0,1,0},{1,0,0,1,0,0,0,1},{1,0,0,1,0,1,0,1},{1,1,1,0,0,1,0,0},{0,1,1,1,1,0,0,1}},
								 {{1,1,1,0,0,1,1,1},{1,1,0,0,1,0,0,0},{0,0,1,1,0,1,1,1},{0,1,1,0,1,1,0,1},{1,0,0,0,1,1,0,1},{1,1,0,1,0,1,0,1},{0,1,0,0,1,1,1,0},{1,0,1,0,1,0,0,1},{0,1,1,0,1,1,0,0},{0,1,0,1,0,1,1,0},{1,1,1,1,0,1,0,0},{1,1,1,0,1,0,1,0},{0,1,1,0,0,1,0,1},{0,1,1,1,1,0,1,0},{1,0,1,0,1,1,1,0},{0,0,0,0,1,0,0,0}},
								 {{1,0,1,1,1,0,1,0},{0,1,1,1,1,0,0,0},{0,0,1,0,0,1,0,1},{0,0,1,0,1,1,1,0},{0,0,0,1,1,1,0,0},{1,0,1,0,0,1,1,0},{1,0,1,1,0,1,0,0},{1,1,0,0,0,1,1,0},{1,1,1,0,1,0,0,0},{1,1,0,1,1,1,0,1},{0,1,1,1,0,1,0,0},{0,0,0,1,1,1,1,1},{0,1,0,0,1,0,1,1},{1,0,1,1,1,1,0,1},{1,0,0,0,1,0,1,1},{1,0,0,0,1,0,1,0}},
								 {{0,1,1,1,0,0,0,0},{0,0,1,1,1,1,1,0},{1,0,1,1,0,1,0,1},{0,1,1,0,0,1,1,0},{0,1,0,0,1,0,0,0},{0,0,0,0,0,0,1,1},{1,1,1,1,0,1,1,0},{0,0,0,0,1,1,1,0},{0,1,1,0,0,0,0,1},{0,0,1,1,0,1,0,1},{0,1,0,1,0,1,1,1},{1,0,1,1,1,0,0,1},{1,0,0,0,0,1,1,0},{1,1,0,0,0,0,0,1},{0,0,0,1,1,1,0,1},{1,0,0,1,1,1,1,0}},
								 {{1,1,1,0,0,0,0,1},{1,1,1,1,1,0,0,0},{1,0,0,1,1,0,0,0},{0,0,0,1,0,0,0,1},{0,1,1,0,1,0,0,1},{1,1,0,1,1,0,0,1},{1,0,0,0,1,1,1,0},{1,0,0,1,0,1,0,0},{1,0,0,1,1,0,1,1},{0,0,0,1,1,1,1,0},{1,0,0,0,0,1,1,1},{1,1,1,0,1,0,0,1},{1,1,0,0,1,1,1,0},{0,1,0,1,0,1,0,1},{0,0,1,0,1,0,0,0},{1,1,0,1,1,1,1,1}},
								 {{1,0,0,0,1,1,0,0},{1,0,1,0,0,0,0,1},{1,0,0,0,1,0,0,1},{0,0,0,0,1,1,0,1},{1,0,1,1,1,1,1,1},{1,1,1,0,0,1,1,0},{0,1,0,0,0,0,1,0},{0,1,1,0,1,0,0,0},{0,1,0,0,0,0,0,1},{1,0,0,1,1,0,0,1},{0,0,1,0,1,1,0,1},{0,0,0,0,1,1,1,1},{1,0,1,1,0,0,0,0},{0,1,0,1,0,1,0,0},{1,0,1,1,1,0,1,1},{0,0,0,1,0,1,1,0}}};

static int decSbox[16][16][8] = {{{0,1,0,1,0,0,1,0},{0,0,0,0,1,0,0,1},{0,1,1,0,1,0,1,0},{1,1,0,1,0,1,0,1},{0,0,1,1,0,0,0,0},{0,0,1,1,0,1,1,0},{1,0,1,0,0,1,0,1},{0,0,1,1,1,0,0,0},{1,0,1,1,1,1,1,1},{0,1,0,0,0,0,0,0},{1,0,1,0,0,0,1,1},{1,0,0,1,1,1,1,0},{1,0,0,0,0,0,0,1},{1,1,1,1,0,0,1,1},{1,1,0,1,0,1,1,1},{1,1,1,1,1,0,1,1}},
								 {{0,1,1,1,1,1,0,0},{1,1,1,0,0,0,1,1},{0,0,1,1,1,0,0,1},{1,0,0,0,0,0,1,0},{1,0,0,1,1,0,1,1},{0,0,1,0,1,1,1,1},{1,1,1,1,1,1,1,1},{1,0,0,0,0,1,1,1},{0,0,1,1,0,1,0,0},{1,0,0,0,1,1,1,0},{0,1,0,0,0,0,1,1},{0,1,0,0,0,1,0,0},{1,1,0,0,0,1,0,0},{1,1,0,1,1,1,1,0},{1,1,1,0,1,0,0,1},{1,1,0,0,1,0,1,1}},
								 {{0,1,0,1,0,1,0,0},{0,1,1,1,1,0,1,1},{1,0,0,1,0,1,0,0},{0,0,1,1,0,0,1,0},{1,0,1,0,0,1,1,0},{1,1,0,0,0,0,1,0},{0,0,1,0,0,0,1,1},{0,0,1,1,1,1,0,1},{1,1,1,0,1,1,1,0},{0,1,0,0,1,1,0,0},{1,0,0,1,0,1,0,1},{0,0,0,0,1,0,1,1},{0,1,0,0,0,0,1,0},{1,1,1,1,1,0,1,0},{1,1,0,0,0,0,1,1},{0,1,0,0,1,1,1,0}},
								 {{0,0,0,0,1,0,0,0},{0,0,1,0,1,1,1,0},{1,0,1,0,0,0,0,1},{0,1,1,0,0,1,1,0},{0,0,1,0,1,0,0,0},{1,1,0,1,1,0,0,1},{0,0,1,0,0,1,0,0},{1,0,1,1,0,0,1,0},{0,1,1,1,0,1,1,0},{0,1,0,1,1,0,1,1},{1,0,1,0,0,0,1,0},{0,1,0,0,1,0,0,1},{0,1,1,0,1,1,0,1},{1,0,0,0,1,0,1,1},{1,1,0,1,0,0,0,1},{0,0,1,0,0,1,0,1}},
								 {{0,1,1,1,0,0,1,0},{1,1,1,1,1,0,0,0},{1,1,1,1,0,1,1,0},{0,1,1,0,0,1,0,0},{1,0,0,0,0,1,1,0},{0,1,1,0,1,0,0,0},{1,0,0,1,1,0,0,0},{0,0,0,1,0,1,1,0},{1,1,0,1,0,1,0,0},{1,0,1,0,0,1,0,0},{0,1,0,1,1,1,0,0},{1,1,0,0,1,1,0,0},{0,1,0,1,1,1,0,1},{0,1,1,0,0,1,0,1},{1,0,1,1,0,1,1,0},{1,0,0,1,0,0,1,0}},
								 {{0,1,1,0,1,1,0,0},{0,1,1,1,0,0,0,0},{0,1,0,0,1,0,0,0},{0,1,0,1,0,0,0,0},{1,1,1,1,1,1,0,1},{1,1,1,0,1,1,0,1},{1,0,1,1,1,0,0,1},{1,1,0,1,1,0,1,0},{0,1,0,1,1,1,1,0},{0,0,0,1,0,1,0,1},{0,1,0,0,0,1,1,0},{0,1,0,1,0,1,1,1},{1,0,1,0,0,1,1,1},{1,0,0,0,1,1,0,1},{1,0,0,1,1,1,0,1},{1,0,0,0,0,1,0,0}},
								 {{1,0,0,1,0,0,0,0},{1,1,0,1,1,0,0,0},{1,0,1,0,1,0,1,1},{0,0,0,0,0,0,0,0},{1,0,0,0,1,1,0,0},{1,0,1,1,1,1,0,0},{1,1,0,1,0,0,1,1},{0,0,0,0,1,0,1,0},{1,1,1,1,0,1,1,1},{1,1,1,0,0,1,0,0},{0,1,0,1,1,0,0,0},{0,0,0,0,0,1,0,1},{1,0,1,1,1,0,0,0},{1,0,1,1,0,0,1,1},{0,1,0,0,0,1,0,1},{0,0,0,0,0,1,1,0}},
								 {{1,1,0,1,0,0,0,0},{0,0,1,0,1,1,0,0},{0,0,0,1,1,1,1,0},{1,0,0,0,1,1,1,1},{1,1,0,0,1,0,1,0},{0,0,1,1,1,1,1,1},{0,0,0,0,1,1,1,1},{0,0,0,0,0,0,1,0},{1,1,0,0,0,0,0,1},{1,0,1,0,1,1,1,1},{1,0,1,1,1,1,0,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,0,1},{0,0,0,1,0,0,1,1},{1,0,0,0,1,0,1,0},{0,1,1,0,1,0,1,1}},
								 {{0,0,1,1,1,0,1,0},{1,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1},{0,1,0,0,0,0,0,1},{0,1,0,0,1,1,1,1},{0,1,1,0,0,1,1,1},{1,1,0,1,1,1,0,0},{1,1,1,0,1,0,1,0},{1,0,0,1,0,1,1,1},{1,1,1,1,0,0,1,0},{1,1,0,0,1,1,1,1},{1,1,0,0,1,1,1,0},{1,1,1,1,0,0,0,0},{1,0,1,1,0,1,0,0},{1,1,1,0,0,1,1,0},{0,1,1,1,0,0,1,1}},
								 {{1,0,0,1,0,1,1,0},{1,0,1,0,1,1,0,0},{0,1,1,1,0,1,0,0},{0,0,1,0,0,0,1,0},{1,1,1,0,0,1,1,1},{1,0,1,0,1,1,0,1},{0,0,1,1,0,1,0,1},{1,0,0,0,0,1,0,1},{1,1,1,0,0,0,1,0},{1,1,1,1,1,0,0,1},{0,0,1,1,0,1,1,1},{1,1,1,0,1,0,0,0},{0,0,0,1,1,1,0,0},{0,1,1,1,0,1,0,1},{1,1,0,1,1,1,1,1},{0,1,1,0,1,1,1,0}},
								 {{0,1,0,0,0,1,1,1},{1,1,1,1,0,0,0,1},{0,0,0,1,1,0,1,0},{0,1,1,1,0,0,0,1},{0,0,0,1,1,1,0,1},{0,0,1,0,1,0,0,1},{1,1,0,0,0,1,0,1},{1,0,0,0,1,0,0,1},{0,1,1,0,1,1,1,1},{1,0,1,1,0,1,1,1},{0,1,1,0,0,0,1,0},{0,0,0,0,1,1,1,0},{1,0,1,0,1,0,1,0},{0,0,0,1,1,0,0,0},{1,0,1,1,1,1,1,0},{0,0,0,1,1,0,1,1}},
								 {{1,1,1,1,1,1,0,0},{0,1,0,1,0,1,1,0},{0,0,1,1,1,1,1,0},{0,1,0,0,1,0,1,1},{1,1,0,0,0,1,1,0},{1,1,0,1,0,0,1,0},{0,1,1,1,1,0,0,1},{0,0,1,0,0,0,0,0},{1,0,0,1,1,0,1,0},{1,1,0,1,1,0,1,1},{1,1,0,0,0,0,0,0},{1,1,1,1,1,1,1,0},{0,1,1,1,1,0,0,0},{1,1,0,0,1,1,0,1},{0,1,0,1,1,0,1,0},{1,1,1,1,0,1,0,0}},
								 {{0,0,0,1,1,1,1,1},{1,1,0,1,1,1,0,1},{1,0,1,0,1,0,0,0},{0,0,1,1,0,0,1,1},{1,0,0,0,1,0,0,0},{0,0,0,0,0,1,1,1},{1,1,0,0,0,1,1,1},{0,0,1,1,0,0,0,1},{1,0,1,1,0,0,0,1},{0,0,0,1,0,0,1,0},{0,0,0,1,0,0,0,0},{0,1,0,1,1,0,0,1},{0,0,1,0,0,1,1,1},{1,0,0,0,0,0,0,0},{1,1,1,0,1,1,0,0},{0,1,0,1,1,1,1,1}},
								 {{0,1,1,0,0,0,0,0},{0,1,0,1,0,0,0,1},{0,1,1,1,1,1,1,1},{1,0,1,0,1,0,0,1},{0,0,0,1,1,0,0,1},{1,0,1,1,0,1,0,1},{0,1,0,0,1,0,1,0},{0,0,0,0,1,1,0,1},{0,0,1,0,1,1,0,1},{1,1,1,0,0,1,0,1},{0,1,1,1,1,0,1,0},{1,0,0,1,1,1,1,1},{1,0,0,1,0,0,1,1},{1,1,0,0,1,0,0,1},{1,0,0,1,1,1,0,0},{1,1,1,0,1,1,1,1}},
								 {{1,0,1,0,0,0,0,0},{1,1,1,0,0,0,0,0},{0,0,1,1,1,0,1,1},{0,1,0,0,1,1,0,1},{1,0,1,0,1,1,1,0},{0,0,1,0,1,0,1,0},{1,1,1,1,0,1,0,1},{1,0,1,1,0,0,0,0},{1,1,0,0,1,0,0,0},{1,1,1,0,1,0,1,1},{1,0,1,1,1,0,1,1},{0,0,1,1,1,1,0,0},{1,0,0,0,0,0,1,1},{0,1,0,1,0,0,1,1},{1,0,0,1,1,0,0,1},{0,1,1,0,0,0,0,1}},
								 {{0,0,0,1,0,1,1,1},{0,0,1,0,1,0,1,1},{0,0,0,0,0,1,0,0},{0,1,1,1,1,1,1,0},{1,0,1,1,1,0,1,0},{0,1,1,1,0,1,1,1},{1,1,0,1,0,1,1,0},{0,0,1,0,0,1,1,0},{1,1,1,0,0,0,0,1},{0,1,1,0,1,0,0,1},{0,0,0,1,0,1,0,0},{0,1,1,0,0,0,1,1},{0,1,0,1,0,1,0,1},{0,0,1,0,0,0,0,1},{0,0,0,0,1,1,0,0},{0,1,1,1,1,1,0,1}}};

void InvMixColumns(int stateMatrix[][4][WORDNUMBER]){
	int overflowFlag[3][4];
	int temComplex[4][WORDNUMBER];
	int ansComplex[4][WORDNUMBER];

	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				ansComplex[o][p] = 0;
			}
		}

		for(int o = 0; o < 4; o++){

			//初期化処理
			for(int p = 0; p < 3; p++){
				for(int l = 0; l < 4; l++){
					overflowFlag[p][l] = 0;
				}
			}
			for(int p = 0; p < 4; p++){
				for(int l = 0; l < WORDNUMBER; l++){
					temComplex[p][l] = 0;
				}
			}

			for(int p = 0; p < 4; p++){
				if(invMixColumnsComplex[(p-o+4)%4][0] == 1){
					if(stateMatrix[p][i][0] == 1){
						overflowFlag[0][p]++;
					}
					if(stateMatrix[p][i][1] == 1){
						overflowFlag[1][p]++;
					}
					if(stateMatrix[p][i][2] == 1){
						overflowFlag[2][p]++;
					}
					for(int l = 0; l < WORDNUMBER-3; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l+3];
					}
				}

				if(invMixColumnsComplex[(p-o+4)%4][1] == 1){
					if(stateMatrix[p][i][0] == 1){
						overflowFlag[1][p]++;
					}
					if(stateMatrix[p][i][1] == 1){
						overflowFlag[2][p]++;
					}
					for(int l = 0; l < WORDNUMBER-2; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l+2];
					}
				}

				if(invMixColumnsComplex[(p-o+4)%4][2] == 1){
					if(stateMatrix[p][i][0] == 1){
						overflowFlag[2][p]++;
					}
					for(int l = 0; l < WORDNUMBER-1; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l+1];
					}
				}

				if(invMixColumnsComplex[(p-o+4)%4][3] == 1){
					for(int l = 0; l < WORDNUMBER; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l];
					}
				}
			}

			for(int p = 0; p < 4; p++){
				for(int l = 0; l < WORDNUMBER; l++){
					ansComplex[o][l] = ansComplex[o][l] ^ temComplex[p][l];
					if((overflowFlag[0][p] % 2) == 1){
						ansComplex[o][l] = ansComplex[o][l] ^ mixColumnsConstantComplex[(l+2) % WORDNUMBER];
					}
					if((overflowFlag[1][p] % 2) == 1){
						ansComplex[o][l] = ansComplex[o][l] ^ mixColumnsConstantComplex[(l+1) % WORDNUMBER];
					}
					if((overflowFlag[2][p] % 2) == 1){
						ansComplex[o][l] = ansComplex[o][l] ^ mixColumnsConstantComplex[l];
					}
				}
			}
		}

		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				stateMatrix[o][i][p] = ansComplex[o][p];
			}
		}
	}	
	return;
}

void InvShiftRows(int stateMatrix[][4][WORDNUMBER]){
	int tem[4][WORDNUMBER];
	int i,o,p;

	for(i = 1; i < 4; i++){
		for(o = 0; o < i; o++){
			for(p = 0; p < WORDNUMBER; p++){
				tem[o][p] = stateMatrix[i][3-o][p];
			}
		}

		for(o = 3; o >= i; o--){
			for(p = 0; p < WORDNUMBER; p++){
				stateMatrix[i][o][p] = stateMatrix[i][o-i][p];
			}
		}

		for(o = 0; o < i; o++){
			for(p = 0; p < WORDNUMBER; p++){
				stateMatrix[i][i-1-o][p] = tem[o][p];
			}
		}
	}
	return;
}

void InvSubByte(int *subWord){
	int x = subWord[0]*8+subWord[1]*4+subWord[2]*2+subWord[3];
	int y = subWord[4]*8+subWord[5]*4+subWord[6]*2+subWord[7];
	for(int i = 0; i < WORDNUMBER; i++){
		subWord[i] = decSbox[x][y][i];
	}
	return;
}

void MixColumns(int stateMatrix[][4][WORDNUMBER]){
	int overflowFlag[4];
	int temComplex[4][WORDNUMBER];
	int ansComplex[4][WORDNUMBER];

	for(int i = 0; i < 4; i++){

		//初期化処理
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				ansComplex[o][p] = 0;
			}
		}

		for(int o = 0; o < 4; o++){

			//初期化処理
			for(int p = 0; p < 4; p++){
				overflowFlag[p] = 0;
				for(int l = 0; l < WORDNUMBER; l++){
					temComplex[p][l] = 0;
				}
			}

			for(int p = 0; p < 4; p++){
				if(mixColumnsComplex[(p-o+4)%4][0] == 1){
					if(stateMatrix[p][i][0] == 1){
						overflowFlag[p]++;
					}
					for(int l = 0; l < WORDNUMBER-1; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l+1];
					}
				}

				if(mixColumnsComplex[(p-o+4)%4][1] == 1){
					for(int l = 0; l < WORDNUMBER; l++){
						temComplex[p][l] = temComplex[p][l] ^ stateMatrix[p][i][l];
					}
				}
			}

			for(int p = 0; p < 4; p++){
				for(int l = 0; l < WORDNUMBER; l++){
					ansComplex[o][l] = ansComplex[o][l] ^ temComplex[p][l];
					if((overflowFlag[p] % 2) == 1){
						ansComplex[o][l] = ansComplex[o][l] ^ mixColumnsConstantComplex[l];
					}
				}
			}
		}
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				stateMatrix[o][i][p] = ansComplex[o][p];
			}
		}
	}
	return;
}

void AddRoundKey(int stateMatrix[][4][WORDNUMBER], int subkey[][KEYLENGTH], int roundNumber){
	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				stateMatrix[o][i][p] = stateMatrix[o][i][p] ^ subkey[roundNumber][WORDNUMBER4*i+WORDNUMBER*o+p];
			}
		}
	}
	return;
}

void ShiftRows(int stateMatrix[][4][WORDNUMBER]){
	int tem[4][WORDNUMBER];
	int i,o,p;

	for(i = 1; i < 4; i++){
		for(o = 0; o < i; o++){
			for(p = 0; p < WORDNUMBER; p++){
				tem[o][p] = stateMatrix[i][o][p];
			}
		}

		for(o = 0; o < 4-i; o++){
			for(p = 0; p < WORDNUMBER; p++){
				stateMatrix[i][o][p] = stateMatrix[i][o+i][p];
			}
		}
		for(o = 0; o < i; o++){
			for(p = 0; p < WORDNUMBER; p++){
				stateMatrix[i][4-i+o][p] = tem[o][p];
			}
		}
	}
	return;
}

void Rcon(int subWord[][WORDNUMBER],int *temp,int roundNumber){
	for(int i = 0; i < 4; i++){
		for(int o = 0; o < WORDNUMBER; o++){
			if(i == 0){
				temp[WORDNUMBER*i+o] = subWord[i][o] ^ RconMod[roundNumber-1][o];
			} else {
				temp[WORDNUMBER*i+o] = subWord[i][o] ^ 0;
			}
		}
	}
	return;
}

void SubByte(int *subWord){
	int x = subWord[0]*8+subWord[1]*4+subWord[2]*2+subWord[3];
	int y = subWord[4]*8+subWord[5]*4+subWord[6]*2+subWord[7];
	for(int i = 0; i < WORDNUMBER; i++){
		subWord[i] = encSbox[x][y][i];
	}
	return;
}

//32bit左巡回シフト
void RotWord(int subWord[][WORDNUMBER]){
	int tem[WORDNUMBER];
	int i = 0;
	for(i = 0; i < WORDNUMBER; i++){
		tem[i] = subWord[0][i];
	}
	for(i = 0; i < 3; i++){
		for(int j = 0; j < WORDNUMBER; j++){
			subWord[i][j] = subWord[i+1][j];
		}
	}
	for(i = 0; i < WORDNUMBER; i++){
		subWord[3][i] = tem[i];
	}
	return;
}

//サブ鍵を保存
void SaveSubkey(int subkey[][KEYLENGTH],int savePointer,int w[][WORDNUMBER4]){
	for(int i = 0; i < 4; i++){
		for(int o = 0; o < WORDNUMBER4; o++){
			subkey[savePointer][i*WORDNUMBER4+o] = w[savePointer*4+i][o];
		}
	}

	return;
}

void SubKeyGenerator(int *key,int subkey[][KEYLENGTH]){
	int w[(roundNumber+1)*4][WORDNUMBER4];
	int subWord[4][WORDNUMBER];
	int temp[WORDNUMBER4];
	int tem;
	int i = 0;
	int j = 0;
	int o = 0;

	//ワードの作成(鍵の分割)
	for(i = 0; i < kw; i++){
		for(j = 0; j < WORDNUMBER4; j++){
			w[i][j] = key[i*WORDNUMBER4+j];
		}
	}


	SaveSubkey(subkey,0,w);
	//処理A
	for(i = 1; i <= 10; i++){
		//サブ鍵4i-1を分割
		for(j = 0; j < 4; j++){
			for(o = 0; o < WORDNUMBER; o++){
				subWord[j][o] = w[i*kw-1][j*WORDNUMBER+o];
			}
		}

		RotWord(subWord);

		for(j = 0; j < 4; j++){
			SubByte(subWord[j]);
		}

		Rcon(subWord,temp,i);

		//処理B
		for(j = 0; j < 4; j++){
			//tempとの排他的論理和
			for(o = 0; o < WORDNUMBER4; o++){
				w[i*4+j][o] = w[i*4+j-4][o] ^ temp[o];
			}
			//tempに上書き
			for(o = 0; o < WORDNUMBER4; o++){
				temp[o] = w[i*4+j][o];
			}
		}
		SaveSubkey(subkey,i,w);
	}

	return;
}

void AES(int *plaintext,int subkey[][KEYLENGTH], int *cryptogram){
	int roundCounter = 1;
	int stateMatrix[4][4][WORDNUMBER];

	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				stateMatrix[o][i][p] = plaintext[WORDNUMBER4*i+WORDNUMBER*o+p];
			}
		}
	}

	AddRoundKey(stateMatrix,subkey,0);

	while(roundCounter <= roundNumber){
		for(int i = 0; i < 4; i++){
			for(int o = 0; o < 4; o++){
				SubByte(stateMatrix[i][o]);
			}
		}

		ShiftRows(stateMatrix);

		if(roundCounter != roundNumber){
			MixColumns(stateMatrix);
		}

		AddRoundKey(stateMatrix,subkey,roundCounter);

		roundCounter++;
	}

	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				cryptogram[WORDNUMBER4*i+WORDNUMBER*o+p] = stateMatrix[o][i][p];
			}
		}
	}

	return;
}

void InvAES(int *cryptogram, int subkey[][KEYLENGTH], int *plaintext){
	int counter = 0;
	int roundCounter = roundNumber;
	int stateMatrix[4][4][WORDNUMBER];

	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				stateMatrix[o][i][p] = cryptogram[counter];
				counter++;
			}
		}
	}

	while(roundCounter >= 1){
		AddRoundKey(stateMatrix,subkey,roundCounter);

		if(roundCounter != roundNumber){
			InvMixColumns(stateMatrix);
		}

		InvShiftRows(stateMatrix);

		for(int i = 0; i < 4; i++){
			for(int o = 0; o < 4; o++){
				InvSubByte(stateMatrix[i][o]);
			}
		}
		roundCounter--;
	}

	AddRoundKey(stateMatrix,subkey,0);

	for(int i = 0; i < 4; i++){
		for(int o = 0; o < 4; o++){
			for(int p = 0; p < WORDNUMBER; p++){
				plaintext[WORDNUMBER4*i+WORDNUMBER*o+p] = stateMatrix[o][i][p];
			}
		}
	}
	return;
}

void push(int *plaintext, int *pushComplex, int offset){
	for(int i = 0; i < 4; i++){
		plaintext[i+offset] = pushComplex[i];
	}
	return;
}

void makeComplex(char *input, int *textComplex){
	int pushComplex[16][4] = {{0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},{0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},{1,0,0,1},{1,0,1,0},{1,0,1,1},{1,1,0,0},{1,1,0,1},{1,1,1,0},{1,1,1,1}};
	int counter = 0;
	
	for(int i = 0; i < WORDNUMBER4; i++){
		switch(input[i]){
			case '0':
				push(textComplex,pushComplex[0],counter);
				break;

			case '1':
				push(textComplex,pushComplex[1],counter);
				break;

			case '2':
				push(textComplex,pushComplex[2],counter);
				break;

			case '3':
				push(textComplex,pushComplex[3],counter);
				break;

			case '4':
				push(textComplex,pushComplex[4],counter);
				break;

			case '5':
				push(textComplex,pushComplex[5],counter);
				break;

			case '6':
				push(textComplex,pushComplex[6],counter);
				break;

			case '7':
				push(textComplex,pushComplex[7],counter);
				break;

			case '8':
				push(textComplex,pushComplex[8],counter);
				break;

			case '9':
				push(textComplex,pushComplex[9],counter);
				break;

			case 'a':
			case 'A':
				push(textComplex,pushComplex[10],counter);
				break;

			case 'b':
			case 'B':
				push(textComplex,pushComplex[11],counter);
				break;

			case 'c':
			case 'C':
				push(textComplex,pushComplex[12],counter);
				break;

			case 'd':
			case 'D':
				push(textComplex,pushComplex[13],counter);
				break;

			case 'e':
			case 'E':
				push(textComplex,pushComplex[14],counter);
				break;

			case 'f':
			case 'F':
				push(textComplex,pushComplex[15],counter);
				break;
		}
		counter += 4;
	}
	return;
}

void makeText(int *textComplex, char *output){
	int counter = 0;

	for(int i = 0; i < 128; i += 4){
		int temNumber = textComplex[i]*8+textComplex[i+1]*4+textComplex[i+2]*2+textComplex[i+3];

		switch(temNumber){
			case 0:
				output[counter] = '0';
				break;

			case 1:
				output[counter] = '1';
				break;

			case 2:
				output[counter] = '2';
				break;

			case 3:
				output[counter] = '3';
				break;

			case 4:
				output[counter] = '4';
				break;

			case 5:
				output[counter] = '5';
				break;

			case 6:
				output[counter] = '6';
				break;

			case 7:
				output[counter] = '7';
				break;

			case 8:
				output[counter] = '8';
				break;

			case 9:
				output[counter] = '9';
				break;

			case 10:
				output[counter] = 'a';
				break;

			case 11:
				output[counter] = 'b';
				break;

			case 12:
				output[counter] = 'c';
				break;

			case 13:
				output[counter] = 'd';
				break;

			case 14:
				output[counter] = 'e';
				break;

			case 15:
				output[counter] = 'f';
				break;

		}
		counter++;
	}
	return;
}

int main(){
	char input[33];
	int plaintext[128];
	int cryptogram[128];
	int key[KEYLENGTH];

	size = sizeof(plaintext)/sizeof(plaintext[0]);
	kw = KEYLENGTH / WORDNUMBER4;
	roundNumber = kw + 6;
	
	int subkey[roundNumber+1][KEYLENGTH];

	cout << "平文入力" << endl;
	cin >> input;
	makeComplex(input,plaintext);

	cout << "鍵入力" << endl;
	cin >> input;
	makeComplex(input,key);

	SubKeyGenerator(key,subkey);

	AES(plaintext,subkey,cryptogram);

	makeText(cryptogram,input);

	cout << "暗号文出力" << endl;
	for(int i = 0; i < 32; i++){
		cout << input[i];
	}
	cout << endl;

	InvAES(cryptogram,subkey,plaintext);

	makeText(plaintext,input);

	cout << "平文出力" << endl;
	for(int i = 0; i < 32; i++){
		cout << input[i];
	}
	cout << endl;	

	return 0;
}