#ifndef _CODEH
#define _CODEH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

#define N 9
int matrix[N][N];
static int bitMap[N + 1] =
{
    0x0,     /* none */
    0x100,   /* 1 */
    0x200,   /* 2 */
    0x400,   /* 3 */
    0x800,   /* 4 */
    0x1000,  /* 5 */
    0x2000,  /* 6 */
    0x4000,  /* 7 */
    0x8000,  /* 8 */
    0x10000  /* 9 */
};

int unFixedCounter;
int stepByStep;

//#define num2Bit(n) (1 << (n + 7))
#define num2Bit(n) bitMap[n]
#define unFixed(i,j) (matrix[i][j] > N)
#define fixed(i,j) (matrix[i][j] >= 1 && matrix[i][j] <= N)
#define regionLinearMap(row, column) (((row) / 3) * 3 + (column) / 3)

/************************************************
region index:
0 1 2
3 4 5
6 7 8

region inner index:
0 1 2
3 4 5
6 7 8
************************************************/

/******************************* 
空集 void 子集 subset
交集 intersection 并集 union
补集 complement 映射 mapping
差集   difference of set 
*******************************/ 

typedef enum 
{
    UNION1,  
    UNION2,
    DIFF1, /* A - B*/
    DIFF2, /* B - A*/
    INTERSECTION,
    SET_OP_TYPE_BUTT
}setOpType;

typedef enum 
{
    ROW,
    COLUMN,
    REGION,
    BUTT
}unitType;

int unitUnFixedCounter[BUTT][N];

	
enum candidateRemovalType
{
	SINGLE = 1,
	PAIR = 2,
	TRIPLET = 3,
	QUAD = 4,
};

typedef struct
{
    int row;
    int column;
}POINT;

typedef struct
{
    int srcPointNum;
    POINT srcPoint[N];
	
	int destPointNum;
    POINT destPoint[N];
	
    int clearBits;
}checkReturnStruct;

static checkReturnStruct gCheckReturnStruct = {0};

int len(int bits);
int bit2Num(int bit);
char * bits2Nums(int bits);
void printMatrix(int matrix[][N]);
void readMatrix();
void readMatrixFromFile(char *file);
void readLogParameter();
void funLog(const char * funName);
int initMatrix();
time_t saveMatrix();
void fillCheckReturnStruct(unitType ut, int row, int column, int value, int i1, int i2, int i3, int i4);

int recursiveSolve(int index);
int differenceSetInRow(int row, int column1, int column2, int column3, int column4);
int differenceSetInColumn(int column, int row1, int row2, int row3, int row4);
int differenceSetInRegion(int row, int column, int r1, int r2, int r3, int r4);

int cleanRowWithIn(int row, int value, int column1, int column2, int column3, int column4);
int cleanColumnWithIn(int column, int value, int row1, int row2, int row3, int row4);
int cleanRegionWithIn(int row, int column, int value, int i1, int i2, int i3, int i4);
int cleanRowWithException(int row, int value, int column1, int column2, int column3, int column4);
int cleanColumnWithException(int column, int value, int row1, int row2, int row3, int row4);
int cleanColumnWithException(int column, int value, int row1, int row2, int row3, int row4);


int clean(int row, int column);

int check();
int checkNakedPairInRow(int row);
int checkNakedPairInColumn(int column);
int checkNakedPairInRegion(int row, int column);
int checkNakedTripletInRow(int row);
int checkNakedTripletInColumn(int column);
int checkNakedTripletInRegion(int row, int column);
int checkNakedQuadInRow(int row);
int checkNakedQuadInColumn(int column);
int checkNakedQuadInRegion(int row, int column);



int checkHiddenSingleInRow(int row);
int checkHiddenSingleInColumn(int column);
int checkHiddenSingleInRegion(int row, int column);
int checkHiddenPairInRow(int row);
int checkHiddenPairInColumn(int column);
int checkHiddenPairInRegion(int row, int column);
int checkHiddenTripletInRow(int row);
int checkHiddenTripletInColumn(int column);
int checkHiddenTripletInRegion(int row, int column);
int checkHiddenQuadInRow(int row);
int checkHiddenQuadInColumn(int column);
int checkHiddenQuadInRegion(int row, int column);


int checkIntersectionRemovalRegionVsRow(int row, int column);
int checkIntersectionRemovalRegionVsColumn(int row, int column);
int checkIntersectionRemovalRowVsRegion(int row);
int checkIntersectionRemovalColumnVsRegion(int column);

int checkXWingInRow();
int checkXWingInColumn();
int checkXYWingInRowColumn();
int checkXYWingInRowRegion();
int checkXYWingInColumnRegion();
int checkSwordfishInRow();
int checkSwordfishInColumn();
int checkXYZWingInRowRegion();
int checkXYZWingInColumnRegion();
int checkWXYZWingInRowRegion();
int checkWXYZWingInColumnRegion();

int checkNakedSingle(int control);
int  checkHiddenSingle(int control);
int  checkNakedPair(int control);
int  checkHiddenPair(int control);
int  checkNakedTriplet(int control);
int  checkHiddenTriplet(int control);
int  checkNakedQuad(int control);
int  checkHiddenQuad(int control);
int  checkIntersectionRemoval(int control); 
int checkXWing(int control);
int checkXYWing(int control);
int checkSwordfish(int control);
int checkXYZWing(int control);
int checkWXYZWing(int control);

#endif

