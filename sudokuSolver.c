#include "sudokuSolver.h"

/* sample sudoku from www.oubk.com
0 0 0 0 0 0 4 3 0
0 2 0 9 0 0 0 0 0
0 0 0 0 0 0 0 0 0
3 0 7 0 0 0 0 5 0
6 0 0 8 0 0 0 0 0
0 0 0 2 0 0 0 0 0
0 0 0 0 0 1 2 0 8
0 4 0 0 7 0 0 0 0
5 0 0 0 3 0 0 0 0
*/

/* diabolical sample sudoku from http://www.sudokuwiki.org/sudoku.htm
0 0 0 7 0 4 0 0 5
0 2 0 0 1 0 0 7 0
0 0 0 0 8 0 0 0 2
0 9 0 0 0 6 2 5 0
6 0 0 0 7 0 0 0 8
0 5 3 2 0 0 0 1 0
4 0 0 0 9 0 0 0 0
0 3 0 0 6 0 0 9 0
2 0 0 4 0 7 0 0 0
*/

int len(int bits)
{
	int num, n = 0;

	for (num = 1; num <= N; num ++)
	{
        if (bits & bitMap[num])
        {
            n ++;
        }
	}

	return n;
}

int bit2Num(int bit)
{
	int num;

	for (num = 1; num <= N; num ++)
	{
        if (bit == bitMap[num])
        {
            return num;
        }
	}

    return num;

}


char * bits2Nums(int bits)
{
	static char nums[N + 1];
    int num, n = 0;

    if (0 == bits)
    {
    	nums[n++] = '0';
	}
	
    for (num = 1; num <= N; num ++)
	{
        if (bits & bitMap[num])
        {
            nums[n ++] = num + '0';
        }
	}
	
    nums[n] = '\0';

	return nums;
  
}

void printMatrix(int matrix[][N])
{
    int i, j;
    
    for (i = 0; i < N; i++)
	{             
		for (j = 0; j < N; j++)
		{            
            
            if (matrix[i][j] >= 1 && matrix[i][j] <= N)
            {
                printf("%-2d ",matrix[i][j]);
            }
            else
            {
                printf("%-2s ",bits2Nums(matrix[i][j]));
            }

            if ((j + 1) % 3 == 0)
            {
                printf("   ");
            }
		}

        if ((i < N - 1) && ((i + 1) % 3 == 0))
        {
            printf("\n\n");            
        }

        printf("\n");
        
	}

}

void readMatrix()
{
	int row, column;
	
	printf("input the init matrix: 0 for unknown number\n");
    for (row = 0; row < N; row++)
	{
		for (column= 0; column < N; column ++)
		{
			scanf("%1d",&matrix[row][column]);
		}
	}
    
}

void readMatrixFromFile(char *file)
{
	FILE * fp = NULL;
	int row, column;
	
	//printf("file is %s\n", file);
	fp = fopen(file, "r");
	if (NULL == fp)
    {
        printf("can not open the file\n");
        return;
    }
    
    for (row = 0; row < N; row++)
	{
		for (column= 0; column < N; column ++)
		{
			fscanf(fp, "%1d", &matrix[row][column]);
		}
	}
	
	fclose(fp);
}

void fillCheckReturnStruct(unitType ut, int row, int column, int value, int i1, int i2, int i3, int i4)
{
	int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
	int n = gCheckReturnStruct.srcPointNum;
	gCheckReturnStruct.clearBits = value;
	
	if (n >= N)
	{
		memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
		n = 0;
	}
	
	switch(ut)
	{
		case ROW:
		{		
			if (i1 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = row;
				gCheckReturnStruct.srcPoint[n].column = i1;
				n ++;
			}
			
			if (i2 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = row;
				gCheckReturnStruct.srcPoint[n].column = i2;
				n ++;
			}

			if (i3 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = row;
				gCheckReturnStruct.srcPoint[n].column = i3;
				n ++;
			}

			if (i4 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = row;
				gCheckReturnStruct.srcPoint[n].column = i4;
				n ++;
			}

			gCheckReturnStruct.srcPointNum = n;
			break;
		}

		case COLUMN:
		{
			if (i1 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = i1;
				gCheckReturnStruct.srcPoint[n].column = column;
				n ++;
			}
			
			if (i2 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = i2;
				gCheckReturnStruct.srcPoint[n].column = column;
				n ++;
			}

			if (i3 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = i3;
				gCheckReturnStruct.srcPoint[n].column = column;
				n ++;
			}

			if (i4 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = i4;
				gCheckReturnStruct.srcPoint[n].column = column;
				n ++;
			}

			gCheckReturnStruct.srcPointNum = n;
			break;
		}

		case REGION:
		{
			if (i1 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = regionBeginRow + i1 / 3;
				gCheckReturnStruct.srcPoint[n].column = regionBeginColumn + i1 % 3;
				n ++;
			}
			
			if (i2 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = regionBeginRow + i2 / 3;
				gCheckReturnStruct.srcPoint[n].column = regionBeginColumn + i2 % 3;
				n ++;
			}

			if (i3 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = regionBeginRow + i3 / 3;
				gCheckReturnStruct.srcPoint[n].column = regionBeginColumn + i3 % 3;
				n ++;
			}

			if (i4 < N)
			{
				gCheckReturnStruct.srcPoint[n].row = regionBeginRow + i4 / 3;
				gCheckReturnStruct.srcPoint[n].column = regionBeginColumn + i4 % 3;
				n ++;
			}

			gCheckReturnStruct.srcPointNum = n;
			break;
		}
	}
}


void readLogParameter()
{
	char c[N];

    stepByStep = 0;
	
	//printf("\nshow the log step by step ? default is n.(y/n):");    
    //scanf("%s",c);       
    c[0] = 'n';
    if (c[0] == 'y' || c[0] == 'Y')
    {
        stepByStep = 1;
    }
	else
	{
		stepByStep = 0; 
	}
}

void funLog(const char * funName)
{
	int i;
	
	printf("%s: ", funName);

	for (i = 0; i < gCheckReturnStruct.srcPointNum; i ++)
	{
		printf("(%d,%d) ", gCheckReturnStruct.srcPoint[i].row + 1, gCheckReturnStruct.srcPoint[i].column + 1);
	}

	printf("clear ");

	for (i = 0; i < gCheckReturnStruct.destPointNum; i ++)
	{
		printf("(%d,%d) ", gCheckReturnStruct.destPoint[i].row + 1, gCheckReturnStruct.destPoint[i].column + 1);
	}

	printf("with  %s\n", bits2Nums(gCheckReturnStruct.clearBits));
    
	
	if (stepByStep)
    {
        printMatrix(matrix);
        scanf("%c",&stepByStep);
        stepByStep = 1;
    }
}

#define LOG() 
//#define LOG() funLog(__func__)

int initMatrix()
{
    int i, j;

	unFixedCounter = N * N;               
    memset(unitUnFixedCounter, N, sizeof(unitUnFixedCounter)); 
		
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (matrix[i][j] == 0)
			{
				matrix[i][j] = 0x1FF00;
			}
			else if (unFixed(i, j))
			{
				return 0;
			}
		}
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (fixed(i, j))
			{
				if (checkConflict(i, j))
				{
					return 0;
				}

				clean(i, j);
			}
		}
	}
	
	return 1;
}

time_t saveMatrix()
{
    FILE * file = NULL;
    int row, column;
	char str[80];
	struct tm *ptr;
    time_t t;

	t = time(NULL);
	ptr = localtime(&t);
    strftime(str,100,"%C%m%d%H%M%S.txt",ptr);

    file = fopen(str, "rw");

    if (NULL == file)
    {
        printf("can not create the file\n");
        return 0;
    }

    for (row = 0; row < N; row++)
	{
		for (column= 0; column< N; column++)
		{
			fprintf(file, "%1d", matrix[row][column]);
		}

        fprintf(file, "\n");

	}

    fclose(file);
    return t;
    
}

int checkConflict(int row, int column)
{
	int i, j;
	int regionBeginRow = (row /3) * 3;
	int regionBeginColumn = (column /3) * 3;
	int index;
	int index0 = (row - regionBeginRow) * 3 + (column - regionBeginColumn);
	
	if (unFixed(row, column))
	{
		return 0;
	}

	for (j = 0; j < N; j ++)
	{
		if ((j != column)
			&& (matrix[row][j] == matrix[row][column]))
		{
			return 1;
		}
	}

    for (i = 0; i < N; i ++)
	{
		if ((i != row)
			&& (matrix[i][column] == matrix[row][column]))
		{
			return 1;
		}
	}
	
	for (index = 0; index < N; index ++)
	{
		i = regionBeginRow + index / 3;
		j = regionBeginColumn + index % 3;
		
		if ((index != index0)
			&& (matrix[i][j] == matrix[row][column]))
		{
			return 1;
		}
	}

	return 0;
}

int recursiveSolve(int index)
{
	int  curIndex, value;
	int i, j, num;
	int backupMatrix[N][N];
	int backupUnitUnFixedCounter[BUTT][N];
	int backupUnFixedCounter;
	int ret = 0;

	for (curIndex = index; curIndex < N * N; curIndex ++)
	{
		i = curIndex / N;
		j = curIndex % N;


		if (fixed(i, j))
		{
			continue;
		}

        backupUnFixedCounter = unFixedCounter;
        memcpy(backupMatrix, matrix, sizeof(matrix));		
		memcpy(backupUnitUnFixedCounter, unitUnFixedCounter, sizeof(unitUnFixedCounter));	
        value = matrix[i][j];
		
		for (num = 1; num <= N; num ++)
		{
			if (value & bitMap[num])
			{
				memcpy( matrix, backupMatrix, sizeof(matrix));	
				matrix[i][j] = bitMap[num];
				ret = check();

				if (ret == 1)
				{
					return 1;
				}
				else if (ret == 2)
				{
					unFixedCounter  = backupUnFixedCounter;
                    memcpy( matrix, backupMatrix, sizeof(matrix));
		            memcpy(unitUnFixedCounter, backupUnitUnFixedCounter, sizeof(unitUnFixedCounter));
					continue;
				}
				else
				{
					if (recursiveSolve(index + 1))
					{
						return 1;
					}
				}
			}
		}

        unFixedCounter  = backupUnFixedCounter;
        memcpy( matrix, backupMatrix, sizeof(matrix));
		memcpy(unitUnFixedCounter, backupUnitUnFixedCounter, sizeof(unitUnFixedCounter));	
		return 0;
		
	}

	return 1;
		
}
int  differenceSetInRow(int row, int column1, int column2, int column3, int column4)
{
	int union1 = 0, union2 = 0;
	int column;

	if ((column1 < N)
		&& (unFixed(row, column1)))
	{
		union1 |= matrix[row][column1];
	}

	if ((column2 < N)
		&& (unFixed(row, column2)))
	{
		union1 |= matrix[row][column2];
	}

	if ((column3 < N)
		&& (unFixed(row, column3)))
	{
		union1 |= matrix[row][column3];
	}

	if ((column4 < N)
		&& (unFixed(row, column4)))
	{
		union1 |= matrix[row][column4];
	}

	for (column = 0; column < N; column ++)
	{
		if ((column != column1)
			&& (column != column2)
			&& (column != column3)
			&& (column != column4)
			&& unFixed(row, column))
		{
			union2 |= matrix[row][column];
		}
	}

    return (union1 - (union1 & union2));

}

int differenceSetInColumn(int column, int row1, int row2, int row3, int row4)
{
	int union1 = 0, union2 = 0;
	int row;

	if ((row1 < N)
		&& (unFixed(row1, column)))
	{
		union1 |= matrix[row1][column];
	}

	if ((row2 < N)
		&& (unFixed(row2, column)))
	{
		union1 |= matrix[row2][column];
	}

	if ((row3 < N)
		&& (unFixed(row3, column)))
	{
		union1 |= matrix[row3][column];
	}

	if ((row4 < N)
		&& (unFixed(row4, column)))
	{
		union1 |= matrix[row4][column];
	}

	for (row = 0; row < N; row ++)
	{
		if ((row != row1)
			&& (row != row2)
			&& (row != row3)
			&& (row != row4)
			&& (unFixed(row, column)))
		{
			union2 |= matrix[row][column];
		}
	}

    return (union1 - (union1 & union2));

}


int differenceSetInRegion(int row, int column, int r1, int r2, int r3, int r4)
{
	int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
	int union1 = 0, union2 = 0;
	int r;

    row = regionBeginRow + r1 / 3;
	column = regionBeginColumn + r1 % 3;	
	if ((r1 < N)
		&& (unFixed(row, column)))
	{
		union1 |= matrix[row ][column];
	}

	row = regionBeginRow + r2 / 3;
	column = regionBeginColumn + r2 % 3;	
	if ((r2 < N)
		&& (unFixed(row, column)))
	{
		union1 |= matrix[row ][column];
	}

	row = regionBeginRow + r3 / 3;
	column = regionBeginColumn + r3 % 3;	
	if ((r3 < N)
		&& (unFixed(row, column)))
	{
		union1 |= matrix[row ][column];
	}

	row = regionBeginRow + r4 / 3;
	column = regionBeginColumn + r4 % 3;	
	if ((r4 < N)
		&& (unFixed(row, column)))
	{
		union1 |= matrix[row ][column];
	}

	for (r = 0; r < N; r ++)
	{
		row = regionBeginRow + r / 3;
		column = regionBeginColumn + r % 3;	
		if ((r != r1)
			&& (r != r2)
			&& (r != r3)
			&& (r != r4)
			&& (unFixed(row, column)))
		{
			union2 |= matrix[row][column];
		}
	}

	return (union1 - (union1 & union2));
}

int clean(int row, int column)
{
	int ret = 0;
	
    unFixedCounter --;

    if (unFixedCounter == 0)
    {
        return ret;
    }
    
    unitUnFixedCounter[ROW][row] --;
    unitUnFixedCounter[COLUMN][column] --;
    unitUnFixedCounter[REGION][regionLinearMap(row, column)] --;    
    
    if ((unitUnFixedCounter[ROW][row] > 0) 
        && cleanRowWithException(row, num2Bit(matrix[row][column]), N, N, N, N))
    {
		ret = 1;
    }

    if ((unitUnFixedCounter[COLUMN][column] > 0)
        && cleanColumnWithException(column, num2Bit(matrix[row][column]), N, N, N, N))
    {
		ret = 1;
    }

    if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > 0)
        && cleanRegionWithException(row, column, num2Bit(matrix[row][column]), N, N, N, N))
    {
		ret = 1;
    } 
	
    return ret;
	
}


int cleanRowWithIn(int row, int value, int column1, int column2, int column3, int column4)
{
    int ret = 0;    
	
	if ((column1 < N)
		&& unFixed(row, column1)
		&& (matrix[row][column1] & (~value)))
	{
		matrix[row][column1] &= value;
		ret = 1;
	}

	if ((column2 < N)
		&& unFixed(row, column2)
		&& (matrix[row][column2] & (~value)))
	{
		matrix[row][column2] &= value; 
		ret = 1;
	}


	if ((column3 < N)
		&& unFixed(row, column3)
		&& (matrix[row][column3] & (~value)))
	{
		matrix[row][column3] &= value; 
		ret = 1;
	}

	if ((column4 < N)
		&& unFixed(row, column4)
		&& (matrix[row][column4] & (~value)))
	{
		matrix[row][column4] &= value; 
		ret = 1;
	}
	
    return ret;
}


int cleanColumnWithIn(int column, int value, int row1, int row2, int row3, int row4)
{
    int ret = 0;
    	
    if ((row1 < N)
		&& unFixed(row1, column)
		&& (matrix[row1][column] & (~value)))
	{
		matrix[row1][column] &= value;
		ret = 1;
	}

	if ((row2 < N)
		&& unFixed(row2, column)
		&& (matrix[row2][column] & (~value)))
	{
		matrix[row2][column] &= value;
		ret = 1;
	}

	if ((row3 < N)
		&& unFixed(row3, column)
		&& (matrix[row3][column] & (~value)))
	{
		matrix[row3][column] &= value;
		ret = 1;
	}

	if ((row4 < N)
		&& unFixed(row4, column)
		&& (matrix[row4][column] & (~value)))
	{
		matrix[row4][column] &= value;
		ret = 1;
	}
	
    return ret;
}


int cleanRegionWithIn(int row, int column, int value, int i1, int i2, int i3, int i4)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int ret = 0;
	int row1 = row;
	int column1 = column;
    	 
    row = regionBeginRow + i1/3;
	column = regionBeginColumn + i1%3;
    if ((i1 < N)
		&& unFixed(row, column)
		&& (matrix[row][column] & (~value)))
	{
		matrix[row][column] &= value;
		ret = 1;
	}

	row = regionBeginRow + i2/3;
	column = regionBeginColumn + i2%3;
    if ((i2 < N)
		&& unFixed(row, column)
		&& (matrix[row][column] & (~value)))
	{
		matrix[row][column] &= value;
		ret = 1;
	}

	row = regionBeginRow + i3/3;
	column = regionBeginColumn + i3%3;
    if ((i3 < N)
		&& unFixed(row, column)
		&& (matrix[row][column] & (~value)))
	{
		matrix[row][column] &= value;
		ret = 1;
	}

	row = regionBeginRow + i4/3;
	column = regionBeginColumn + i4%3;
    if ((i4 < N)
		&& unFixed(row, column)
		&& (matrix[row][column] & (~value)))
	{
		matrix[row][column] &= value;
		ret = 1;
	}
	
    return ret;
}

int cleanRowWithException(int row, int value, int column1, int column2, int column3, int column4)
{
    int column;
    int ret = 0;	
		
    for (column = 0; column < N; column ++)
    {
        if ((column != column1)
			&& (column != column2)
			&& (column != column3)
			&& (column != column4)
			&& unFixed(row, column)
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);           
            ret = 1;
        }
    }
	
    return ret;
}

int cleanColumnWithException(int column, int value, int row1, int row2, int row3, int row4)
{
    int row;
    int ret = 0;	
	
    for (row = 0; row < N; row ++)
    {
        if ((row != row1)
			&& (row != row2)
			&& (row != row3)
			&& (row != row4)
			&& unFixed(row, column) 
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);
            ret = 1;
        }
    }

    return ret;
}

int cleanRegionWithException(int row, int column, int value, int i1, int i2, int i3, int i4)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i;
    int ret = 0;    
	
    for (i = 0; i < N; i ++)
	{
		row = regionBeginRow + i/3;
		column = regionBeginColumn + i%3;

		if ((i != i1)
			&& (i != i2)
			&& (i != i3)
			&& (i != i4)
			&& unFixed(row, column) 
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);
            ret = 1;
        }
    }
	
    return ret;
}

int check()
{
	int ret = 0;
restart:
		
	ret = checkNakedSingle(1);

    if (ret == 2)
    {
		return ret;
    }
	else if (ret == 1)
	{
		goto restart;
	}
	
	if (unFixedCounter <= 0)
	{
		return 1;
	}
	
	if (checkHiddenSingle(2))
	{
		goto restart;
	}

	if (checkNakedPair(3))
	{
		goto restart;
	}

	if (checkHiddenPair(4))
	{
		goto restart;
	}

	if (checkNakedTriplet(5))
	{
		goto restart;
	}

	if (checkHiddenTriplet(6))
	{
		goto restart;
	}

	if (checkNakedQuad(7))
	{
		goto restart;
	}

	if (checkHiddenQuad(8))
	{
		goto restart;
	}

	if (checkIntersectionRemoval(3))
	{
		goto restart;
	}

	if (checkXWing(4))
	{
		goto restart;
	}

	if (checkXYWing(3))
	{
		goto restart;
	}

	if (checkXYZWing(3))
	{
		goto restart;
	}

	if (checkXYZWing(3))
	{
		goto restart;
	}

	if (checkWXYZWing(4))
	{
		goto restart;
	}

	if (checkSwordfish(6))
	{
		goto restart;
	}

    return 0;
}

int checkNakedPairInRow(int row)
{
    int column, column1;
	int merge;
    int ret = 0;

    for (column = 0; column < N; column ++)
    {
        if (unFixed(row, column) 
			&& (len(matrix[row][column]) == 2))
        {
            for (column1 = column + 1; column1 < N; column1 ++)
            {
                if (matrix[row][column1] == matrix[row][column])
                {
					merge = matrix[row][column];
					if (cleanRowWithException(row, merge, column, column1, N, N))
					{
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(ROW, row, N, merge, column, column1, N, N);
						LOG();
					}
                }
            }
        }
    }

    return ret;
}

int checkNakedPairInColumn(int column)
{
    int row, row1;
	int merge;
    int ret = 0;
	

    for (row = 0; row < N; row ++)
    {
        if (unFixed(row, column) 
			&& (len(matrix[row][column]) == 2))
        {
            for (row1 = row + 1; row1 < N; row1 ++)
            {
                if (matrix[row][column] == matrix[row1][column])
                {
					merge = matrix[row][column];
					
					if (cleanColumnWithException(column, merge, row, row1, N, N))
					{
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(COLUMN, N, column, merge, row, row1, N, N);
						LOG();
					}                   
                }
            }
        }
    }

    return ret;
}


int checkNakedPairInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i, i1;
	int row1, column1;
	int merge;
    int ret = 0;

	for (i = 0; i < N; i ++)
	{
		row = regionBeginRow + i/3;
		column = regionBeginColumn + i%3;

		if (unFixed(row, column)
			&& (len(matrix[row][column]) == 2))
		{
			for (i1 = i + 1; i1 < N; i1 ++)
			{				
				row1 = regionBeginRow + i1/3;
				column1 = regionBeginColumn + i1%3;

				if (matrix[row1][column1] == matrix[row][column])
				{
					merge = matrix[row][column];
					
					if (cleanRegionWithException(row, column, merge, i, i1, N, N))
					{
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(REGION, row, column, merge, i, i1, N, N);
						LOG();
					}   
				}				
			}
		}
	}

    return ret;
}


int checkNakedTripletInRow(int row)
{
	int column, column1, column2;
	int merge;
	int ret = 0;

	for (column = 0; column < N; column ++)
	{
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 3))
		{
			merge = matrix[row][column];
			
			for (column1 = column + 1; column1 < N; column1 ++)
			{
				if (unFixed(row, column1)
					&& (len(matrix[row][column1]) <= 3)
					&& (len((merge |= matrix[row][column1])) <= 3))
				{
					for (column2 = column1 + 1; column2 < N; column2 ++)
					{
						if (unFixed(row, column2)
							&& (len(matrix[row][column2]) <= 3)
							&& (len((merge |= matrix[row][column2])) <= 3)
							&& cleanRowWithException(row, merge, column, column1, column2, N))
						{						
							ret = 1;
							memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
							fillCheckReturnStruct(ROW, row, N, merge, column, column1, column2, N);
							LOG();
						}
					}
				}
			}
		}
	}


	return ret;
}

int checkNakedTripletInColumn(int column)
{
	int row, row1, row2;
	int merge;
	int ret = 0;

	for (row = 0; row < N; row ++)
	{
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 3))
		{
			merge = matrix[row][column];
			
			for (row1 = row + 1; row1 < N; row1 ++)
			{
				if (unFixed(row1, column)
					&& (len(matrix[row1][column]) <= 3)
					&& (len((merge |= matrix[row1][column])) <= 3))
				{
					for (row2 = row1 + 1; row2 < N; row2 ++)
					{
						if (unFixed(row2, column)
							&& (len(matrix[row2][column]) <= 3)
							&& (len((merge |= matrix[row2][column])) <= 3)
							&& cleanColumnWithException(column, merge, row, row1, row2, N))
						{						
							ret = 1;
							memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
							fillCheckReturnStruct(COLUMN, N, column, merge, row, row1, row2, N);
							LOG();
						}
					}
				}
			}
		}
	}


	return ret;
}

int checkNakedTripletInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i, i1, i2;
	int merge;
	int row1, column1, row2, column2;
    int ret = 0;
    
    for (i = 0; i < N; i ++)
	{
		row = regionBeginRow + i / 3;
		column = regionBeginColumn + i % 3;
		
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 3))
		{
			merge = matrix[row][column];
			
			for (i1 = i + 1; i1 < N; i1 ++)
			{
				row1 = regionBeginRow + i1 / 3;
				column1 = regionBeginColumn + i1 % 3;
				
				if (unFixed(row1, column1)
					&& (len(matrix[row1][column1]) <= 3)
					&& (len((merge |= matrix[row1][column1])) <= 3))
				{
					for (i2 = i1 + 1; i2 < N; i2 ++)
					{
						row2 = regionBeginRow + i2 / 3;
						column2 = regionBeginColumn + i2 % 3;
						
						if (unFixed(row2, column2)
							&& (len(matrix[row2][column2]) <= 3)
							&& (len((merge |= matrix[row2][column2])) <= 3)
							&& cleanRegionWithException(row, column, merge, i, i1, i2, N))
						{						
							ret = 1;
							memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
							fillCheckReturnStruct(REGION, row, column, merge, i, i1, i2, N);
							LOG();
						}
					}
				}
			}
		}
	}

    return ret;
}


int checkNakedQuadInRow(int row)
{
	int column, column1, column2, column3;
	int merge;
	int ret = 0;

	for (column = 0; column < N; column ++)
	{
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 4))
		{
			merge = matrix[row][column];
			
			for (column1 = column + 1; column1 < N; column1 ++)
			{
				if (unFixed(row, column1)
					&& (len(matrix[row][column1]) <= 4)
					&& (len((merge |= matrix[row][column1])) <= 4))
				{
					for (column2 = column1 + 1; column2 < N; column2 ++)
					{
						if (unFixed(row, column2)
							&& (len(matrix[row][column2]) <= 4)
							&& (len((merge |= matrix[row][column2])) <= 4))
						{						
							for (column3 = column2 + 1; column3 < N; column3 ++)
							{
								if (unFixed(row, column3)
									&& (len(matrix[row][column3]) <= 4)
									&& (len((merge |= matrix[row][column3])) <= 4)
									&& cleanRowWithException(row, merge, column, column1, column2, column3))
								{						
									ret = 1;
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(ROW, row, N, merge, column, column1, column2, column3);
									LOG();
								}
							}
						}
					}
				}
			}
		}
	}


	return ret;
}

int checkNakedQuadInColumn(int column)
{
	int row, row1, row2, row3;
	int merge;
	int ret = 0;

	for (row = 0; row < N; row ++)
	{
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 4))
		{
			merge = matrix[row][column];
			
			for (row1 = row + 1; row1 < N; row1 ++)
			{
				if (unFixed(row1, column)
					&& (len(matrix[row1][column]) <= 4)
					&& (len((merge |= matrix[row1][column])) <= 4))
				{
					for (row2 = row1 + 1; row2 < N; row2 ++)
					{
						if (unFixed(row2, column)
							&& (len(matrix[row2][column]) <= 4)
							&& (len((merge |= matrix[row2][column])) <= 4))
						{						
							for (row3 = row2 + 1; row3 < N; row3 ++)
							{
								if (unFixed(row3, column)
									&& (len(matrix[row3][column]) <= 4)
									&& (len((merge |= matrix[row3][column])) <= 4)
									&& cleanColumnWithException(column, merge, row, row1, row2, row3))
								{						
									ret = 1;
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(COLUMN, N, column, merge, row, row1, row2, row3);
									LOG();
								}
							}
						}
					}
				}
			}
		}
	}


	return ret;
}


int checkNakedQuadInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i, i1, i2, i3;
	int merge;
	int row1, column1, row2, column2, row3, column3;
    int ret = 0;
    
    for (i = 0; i < N; i ++)
	{
		row = regionBeginRow + i / 3;
		column = regionBeginColumn + i % 3;
		
		if (unFixed(row, column)
			&& (len(matrix[row][column]) <= 4))
		{
			merge = matrix[row][column];
			
			for (i1 = i + 1; i1 < N; i1 ++)
			{
				row1 = regionBeginRow + i1 / 3;
				column1 = regionBeginColumn + i1 % 3;
				
				if (unFixed(row1, column1)
					&& (len(matrix[row1][column1]) <= 4)
					&& (len((merge |= matrix[row1][column1])) <= 4))
				{
					for (i2 = i1 + 1; i2 < N; i2 ++)
					{
						row2 = regionBeginRow + i2 / 3;
						column2 = regionBeginColumn + i2 % 3;
						
						if (unFixed(row2, column2)
							&& (len(matrix[row2][column2]) <= 4)
							&& (len((merge |= matrix[row2][column2])) <= 4))
						{
                            for (i3 = i2 + 1; i3 < N; i3 ++)
							{
								row3 = regionBeginRow + i3 / 3;
								column3 = regionBeginColumn + i3 % 3;
								
								if (unFixed(row3, column3)
									&& (len(matrix[row3][column3]) <= 4)
									&& (len((merge |= matrix[row3][column3])) <= 4)
									&& cleanRegionWithException(row, column, merge, i, i1, i2, i3))
								{								
									ret = 1;
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(REGION, row, column, merge, i, i1, i2, i3);
									LOG();
								}
                            }
						}
					}
				}
			}
		}
	}

    return ret;
}


int checkHiddenSingleInRow(int row)
{
    int column, diff;
    int ret =0;	
	
    for (column = 0; column < N; column++)
    {
        if (unFixed(row, column))
        {
			diff = differenceSetInRow(row, column, N, N, N);       
            if (diff
				&& (len(diff) == 1)
				&& cleanRowWithIn(row, diff, column, N, N, N))
			{
                ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(ROW, row, N, diff, column, N, N, N);
				LOG();              
			}
        }
    }

    return ret;
}

int checkHiddenSingleInColumn(int column)
{
    int row, diff;
    int ret =0;

    for (row = 0; row < N; row++)
    {
        if (unFixed(row, column))
        {
            diff = differenceSetInColumn(column, row, N, N, N);
            if (diff
				&& (len(diff) == 1)
				&& cleanColumnWithIn(column, diff, row, N, N, N))
			{
                ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(COLUMN, N, column, diff, row, N, N, N);
				LOG();                
			}
        }
    }

    return ret;
}

int checkHiddenSingleInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int r1, diff;
	int row1, column1;
    int ret =0;

	for (r1 = 0; r1 < N; r1 ++)
	{
		row1 = regionBeginRow + r1/3;
		column1 = regionBeginColumn + r1%3;

		if (unFixed(row1, column1))
		{
			diff = differenceSetInRegion(row, column, r1, N, N, N);
            if (diff
				&& (len(diff) == 1)
				&& cleanRegionWithIn(row, column, diff, r1, N, N, N))
			{
                ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(REGION, row, column, diff, r1, N, N, N);
				LOG();                                   
			}
		}
	}
	
    return ret;
}


int checkHiddenPairInRow(int row)
{
    int column, column1;
	int diff;
    int ret = 0;

    for (column = 0; column < N; column++)
    {
        if (unFixed(row, column))
        {
            for (column1 = column + 1; column1 < N; column1++)
            {
                if (unFixed(row, column1)
					&& (len(matrix[row][column] & matrix[row][column1]) >= 2)
					&& (len(matrix[row][column] | matrix[row][column1]) > 2))
                {
					diff = differenceSetInRow(row, column, column1, N, N);
                    if (diff
                        && (len(diff) == 2)
                        && cleanRowWithIn(row, diff, column, column1, N, N))
                    {
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(ROW, row, N, diff, column, column1, N, N);
						LOG();
                    }
                }
            }
        }
    }

    return ret;
}



int checkHiddenPairInColumn(int column)
{
    int row, row1;
	int diff;
    int ret =0;

    for (row = 0; row < N; row++)
    {
        if (unFixed(row, column))
        {
            for (row1 = row + 1; row1 < N; row1++)
            {
                if (unFixed(row1, column)
					&& (len(matrix[row][column] & matrix[row1][column]) >= 2)
					&& (len(matrix[row][column] | matrix[row1][column]) > 2))
                {
                    diff = differenceSetInColumn(column, row, row1, N, N);
                    if (diff
                        && (len(diff) == 2)
                        && cleanColumnWithIn(column, diff, row, row1, N, N))
                    {                        
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(COLUMN, N, column, diff, row, row1, N, N);
						LOG();
                    }
                }
            }
        }
    }

    return ret;
}

int checkHiddenPairInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int r1, r2;
    int ret =0;
    int diff;
	int row1, column1, row2, column2;

    for (r1 = 0; r1 < N; r1 ++)
	{
		row1 = regionBeginRow + r1/3;
		column1 = regionBeginColumn + r1%3;
		if (unFixed(row1, column1))
        {
			for (r2 = r1 + 1; r2 < N; r2 ++)
			{
				row2 = regionBeginRow + r2/3;
				column2 = regionBeginColumn + r2%3;

				if (unFixed(row2, column2)
					&& (len(matrix[row2][column2] & matrix[row1][column1]) >= 2)
					&& (len(matrix[row2][column2] | matrix[row1][column1]) > 2))
				{
					diff = differenceSetInRegion(row, column, r1, r2, N, N);
                    if (diff
                        && (len(diff) == 2)
                        && cleanRegionWithIn(row, column, diff, r1, r2, N, N))
                    {
						ret = 1;
						memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
						fillCheckReturnStruct(REGION, row, column, diff, r1, r2, N, N);
						LOG();
                    }
                }
			}
		}
	}
	

    return ret;
}


int checkHiddenTripletInRow(int row)
{
    int column, column1, column2;
	int diff;
    int ret = 0;

    for (column = 0; column < N; column ++)
    {
        if (unFixed(row, column))
        {
            for (column1 = column + 1; column1 < N; column1 ++)
            {
                if (unFixed(row, column1))
                {
					for (column2 = column1+ 1; column2 < N; column2 ++)
					{
						if (unFixed(row, column2)
							&& (len((matrix[row][column] | matrix[row][column1]) | matrix[row][column2]) > 3))
						{
							diff = differenceSetInRow(row, column, column1, column2, N);
		                    if (diff
		                        && (len(diff) == 3)
		                        && cleanRowWithIn(row, diff, column, column1, column2, N))
		                    {
								ret = 1;
								memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
								fillCheckReturnStruct(ROW, row, N, diff, column, column1, column2, N);
								LOG();
		                    }
						}
					}
                }
            }
        }
    }

    return ret;
}



int checkHiddenTripletInColumn(int column)
{
    int row, row1, row2;
	int diff;
    int ret = 0;

    for (row = 0; row < N; row ++)
    {
        if (unFixed(row, column))
        {
            for (row1 = row + 1; row1 < N; row1 ++)
            {
                if (unFixed(row1, column))
                {
					for (row2 = row1 + 1; row2 < N; row2 ++)
					{
						if (unFixed(row2, column)
							&& (len((matrix[row2][column] | matrix[row1][column]) | matrix[row][column]) > 3))
						{
		                    diff = differenceSetInColumn(column, row, row1, row2, N);
		                    if (diff
		                        && (len(diff) == 3)
		                        && cleanColumnWithIn(column, diff, row, row1, row2, N))
		                    {
								ret = 1;
								memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
								fillCheckReturnStruct(COLUMN, N, column, diff, row, row1, row2, N);
								LOG();
		                    }
						}
					}
                }
            }
        }
    }

    return ret;
}

int checkHiddenTripletInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int r1, r2, r3;
    int ret =0;
    int diff;
	int row1, column1, row2, column2, row3, column3;

    for (r1 = 0; r1 < N; r1 ++)
	{
		row1 = regionBeginRow + r1/3;
		column1 = regionBeginColumn + r1%3;
		if (unFixed(row1, column1))
        {
			for (r2 = r1 + 1; r2 < N; r2 ++)
			{
				row2 = regionBeginRow + r2/3;
				column2 = regionBeginColumn + r2%3;

				if (unFixed(row2, column2))
				{
					for (r3 = r2 + 1; r3 < N; r3 ++)
					{
						row3 = regionBeginRow + r3/3;
						column3 = regionBeginColumn + r3%3;

						if (unFixed(row3, column3)
							&& (len((matrix[row3][column3] | matrix[row2][column2]) | matrix[row1][column1]) > 3))
						{
							diff = differenceSetInRegion(row, column, r1, r2, r3, N);
		                    if (diff
		                        && (len(diff) == 3)
		                        && cleanRegionWithIn(row, column, diff, r1, r2, r3, N))
		                    {
		                        ret = 1;
								memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
								fillCheckReturnStruct(REGION, row, column, diff, r1, r2, r3, N);
								LOG();
		                    }
						}
					}
                }
			}
		}
	}
	

    return ret;
}


int checkHiddenQuadInRow(int row)
{
    int column, column1, column2, column3;
	int diff;
    int ret = 0;

    for (column = 0; column < N; column ++)
    {
        if (unFixed(row, column))
        {
            for (column1 = column + 1; column1 < N; column1 ++)
            {
                if (unFixed(row, column1))
                {
					for (column2 = column1+ 1; column2 < N; column2 ++)
					{
						if (unFixed(row, column2))
						{
                            for (column3 = column2 + 1; column3 < N; column3 ++)
                            {
								if (unFixed(row, column3)
									&& (len(matrix[row][column] | matrix[row][column1] | matrix[row][column2] | matrix[row][column3]) > 4))
								{
				                   diff = differenceSetInRow(row, column, column1, column2, column3);
				                    if (diff
				                        && (len(diff) == 4)
				                        && cleanRowWithIn(row, diff, column, column1, column2, column3))
				                    {
										ret = 1;
										memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
										fillCheckReturnStruct(ROW, row, N, diff, column, column1, column2, column3);
										LOG();
				                    }
								}
                            }
						}
					}
                }
            }
        }
    }

    return ret;
}



int checkHiddenQuadInColumn(int column)
{
    int row, row1, row2, row3;
	int diff;
    int ret = 0;

    for (row = 0; row < N; row ++)
    {
        if (unFixed(row, column))
        {
            for (row1 = row + 1; row1 < N; row1 ++)
            {
                if (unFixed(row1, column))
                {
					for (row2 = row1 + 1; row2 < N; row2 ++)
					{
						if (unFixed(row2, column))
						{
                            for (row3 = row2 + 1; row3 < N; row3 ++)
                            {
								if (unFixed(row3, column)
									&& (len(matrix[row3][column] | matrix[row2][column] | matrix[row1][column] | matrix[row][column]) > 4))
								{
				                    diff= differenceSetInColumn(column, row, row1, row2, row3);
				                    if (diff
				                        && (len(diff) == 4)
				                        && cleanColumnWithIn(column, diff, row, row1, row2, row3))
				                    {
										ret = 1;
										memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
										fillCheckReturnStruct(COLUMN, N, column, diff, row, row1, row2, row3);
										LOG();
				                    }
								}
                            }
						}
					}
                }
            }
        }
    }

    return ret;
}



int checkHiddenQuadInRegion(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int r1, r2, r3, r4;
    int ret =0;
    int diff;
	int row1, column1, row2, column2, row3, column3, row4, column4;

    for (r1 = 0; r1 < N; r1 ++)
	{
		row1 = regionBeginRow + r1/3;
		column1 = regionBeginColumn + r1%3;
		if (unFixed(row1, column1))
        {
			for (r2 = r1 + 1; r2 < N; r2 ++)
			{
				row2 = regionBeginRow + r2/3;
				column2 = regionBeginColumn + r2%3;

				if (unFixed(row2, column2))
				{
					for (r3 = r2 + 1; r3 < N; r3 ++)
					{
						row3 = regionBeginRow + r3/3;
						column3 = regionBeginColumn + r3%3;

						if (unFixed(row3, column3))
						{
							for (r4 = r3 + 1; r4 < N; r4 ++)
							{
								row4 = regionBeginRow + r4 /3;
								column4 = regionBeginColumn + r4 %3;
											
								if (unFixed(row4, column4)
									&& (len(matrix[row4][column4] | matrix[row3][column3] | matrix[row2][column2] | matrix[row1][column1]) > 4))
								{
									diff = differenceSetInRegion(row, column, r1, r2, r3, r4);
				                    if (diff
				                        && (len(diff) == 4)
				                        && cleanRegionWithIn(row, column, diff, r1, r2, r3, r4))
				                    {
				                        ret = 1;
										memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
										fillCheckReturnStruct(REGION, row, column, diff, r1, r2, r3, r4);
										LOG();
				                    }
								}
							}
						}
					}
                }
			}
		}
	}	

    return ret;
}
int checkIntersectionRemovalRegionVsRow(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i, r1;
    int ret = 0;
    int diff;

    for (i = 0; i < 3; i++)
    {
        r1 = i * 3;
		diff = differenceSetInRegion(row, column, r1, r1 + 1, r1 + 2, N);
        if (diff)
        {
			 
			 if (cleanRowWithException(i + regionBeginRow, diff, regionBeginColumn, regionBeginColumn+1, regionBeginColumn+2, N))
			 {
			 	ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(ROW, i + regionBeginRow, N, diff, regionBeginColumn, regionBeginColumn+1, regionBeginColumn+2, N);
				LOG();
			 }
        }
    }

    return ret;
}

int checkIntersectionRemovalRegionVsColumn(int row, int column)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int j, r1;
    int ret = 0;
    int diff;

    
    for (j = 0; j < 3; j++)
    {
		r1 = j;
		diff = differenceSetInRegion(row, column, r1, r1 + 3, r1 + 6, N);
        if (diff)
        {
			 
			 if (cleanColumnWithException(j + regionBeginColumn, diff, regionBeginRow, regionBeginRow+1, regionBeginRow+2, N))
			 {
			 	ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(COLUMN, N, j + regionBeginColumn, diff, regionBeginRow, regionBeginRow+1, regionBeginRow+2, N);
				LOG();
			 }         
        }
    }

    return ret;
}

int checkIntersectionRemovalRowVsRegion(int row)
{
    int column;
    int diff;
    int r1;
    int ret = 0;

    for (column = 0; column < N; column += 3)
    {
		diff = differenceSetInRow(row, column, column + 1, column + 2, N);
        if (diff)
        {
			
			r1 = (row % 3) * 3;
			if (cleanRegionWithException(row, column, diff, r1, r1 + 1, r1 + 2, N))
			{
				ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(REGION, row, column, diff,  r1, r1 + 1, r1 + 2, N);
				LOG();
			}      
        }

    }

	return ret;
}


int checkIntersectionRemovalColumnVsRegion(int column)
{
    int row;
    int diff;
    int r1;
    int ret = 0;

    for (row = 0; row < N; row += 3)
    {
		diff = differenceSetInColumn(column, row, row + 1, row + 2, N);
        if (diff)
        {
			
			r1 = (column % 3);
			if (cleanRegionWithException(row, column, diff, r1, r1 + 3, r1 + 6, N))
			{
				ret = 1;
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(REGION, row, column, diff, r1, r1 + 3, r1 + 6, N);
				LOG();
			}     
        }

    }

	return ret;
}


int checkXWingInRow()
{
	int i, i1, j, j1;
	int diff, diff1, diff2;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {		
		for (j = 0; j < N; j ++)
		{
			if (unFixed(i, j))
			{
				for (j1 = j + 1; j1 < N; j1 ++)
				{
					if (unFixed(i, j1)
						&& (matrix[i][j1] & matrix[i][j]))
					{
						diff = differenceSetInRow(i, j, j1, N, N);
						diff &= (matrix[i][j1] & matrix[i][j]);
						if (diff)
						{
							for (i1 = i + 1; i1 < N; i1 ++)
							{
								if (unFixed(i1, j1)
									&& unFixed(i1, j)
									&& ((matrix[i1][j1] & matrix[i1][j]) & diff))
								{
									diff1 = differenceSetInRow(i1, j, j1, N, N);
									diff2 = diff1 & diff;
									if (diff2 > 0)
									{
										
										
										if (cleanColumnWithException(j, diff2 , i, i1, N, N))
										{
											ret = 1;											
										}

										if (cleanColumnWithException(j1, diff2 , i, i1, N, N))
										{
											ret = 1;
										}

										if (ret)
										{
											memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
											fillCheckReturnStruct(COLUMN, N, j, diff2, i, i1, N, N);
											fillCheckReturnStruct(COLUMN, N, j1, diff2, i, i1, N, N);
											LOG();
										}
									}
									
								}
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}

int checkXWingInColumn()
{
	int i, i1, j, j1;
	int diff, diff1, diff2;
	int ret = 0;

    for (j = 0; j < N; j ++)
    {
		for (i = 0; i < N; i ++)
		{
			if (unFixed(i, j))
			{
				for (i1 = i + 1; i1 < N; i1 ++)
				{
					if (unFixed(i1, j)
						&& (matrix[i1][j] & matrix[i][j]))
					{
						diff= differenceSetInColumn(j, i, i1, N, N);
						diff &= (matrix[i1][j] & matrix[i][j]);
						if (diff)
						{
							for (j1 = j + 1; j1 < N; j1 ++)
							{
								if (unFixed(i1, j1)
									&& unFixed(i, j1)
									&& ((matrix[i1][j1] & matrix[i][j1]) & diff))
								{
									diff1 = differenceSetInColumn(j1, i, i1, N, N);
									diff2 = diff1 & diff;
									if (diff2 > 0)
									{
										
										
										if (cleanRowWithException(i, diff2 , j, j1, N, N))
										{
											ret = 1;											
										}

										if (cleanRowWithException(i1, diff2 , j, j1, N, N))
										{
											ret = 1;
										}

										if (ret)
										{
											memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
											fillCheckReturnStruct(ROW, i, N, diff2, j, j1, N, N);
											fillCheckReturnStruct(ROW, i1, N, diff2, j, j1, N, N);
											LOG();
										}
									}									
								}
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}


int checkXYWingInRowColumn()
{
	int i, i1, j, j1;
	int merge, merge1;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {
		for (j = 0; j < N; j ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 2))
			{
				for (j1 = 0; j1 < N; j1 ++)
				{
					if (unFixed(i, j1)
						&& (matrix[i][j1] & matrix[i][j])
						&& (matrix[i][j1] != matrix[i][j])
						&& (len(matrix[i][j1]) == 2))
					{
						merge = (matrix[i][j1] | matrix[i][j]) - (matrix[i][j1] & matrix[i][j]);
						merge1 = matrix[i][j1] - (matrix[i][j1] & matrix[i][j]);
						
						for (i1 = 0; i1 < N; i1 ++)
						{
							if (unFixed(i1, j)
								&& (matrix[i1][j] == merge)
								&& unFixed(i1, j1)
								&& (matrix[i1][j1] & merge1))
							{								
								matrix[i1][j1] &= (~merge1);
								ret = 1;
								memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
								fillCheckReturnStruct(ROW, i, N, merge1, j, j1, N, N);
								fillCheckReturnStruct(ROW, i1, N, merge1, j, N, N, N);
								LOG();
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}

int checkXYWingInRowRegion()
{
	int i, i1, i2, j, j1, j2, j3;
	int merge, merge1;
	int regionBeginRow;
    int regionBeginColumn, regionBeginColumn1;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {
		for (j = 0; j < N; j ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 2))
			{
				for (j1 = 0; j1 < N; j1 ++)
				{					
					if (unFixed(i, j1)
						&& (matrix[i][j1] & matrix[i][j])
						&& (matrix[i][j1] != matrix[i][j])
						&& (len(matrix[i][j1]) == 2))
					{
						merge = (matrix[i][j1] | matrix[i][j]) - (matrix[i][j1] & matrix[i][j]);
						merge1 = matrix[i][j1] - (matrix[i][j1] & matrix[i][j]);
						regionBeginRow = (i / 3) * 3;
						regionBeginColumn = (j / 3) * 3;
						
						for (i1 = 0; i1 < N; i1 ++)
						{
							i2 = regionBeginRow + i1 / 3;
							j2 = regionBeginColumn + i1 % 3;
							
							if ((i2 != i)
								&& unFixed(i2, j2)
								&& (matrix[i2][j2] == merge))
							{
								
								
								for (j3 = regionBeginColumn; j3 < regionBeginColumn + 3; j3  ++)
								{
									if (unFixed(i, j3)
										&& (matrix[i][j3] & merge1))
									{
										matrix[i][j3] &= (~merge1);
										ret = 1;
									}
								}

								regionBeginColumn1 = (j1 / 3) * 3;

								for (j3 = regionBeginColumn1; j3 < regionBeginColumn1 + 3; j3  ++)
								{
									if (unFixed(i2, j3)
										&& (matrix[i2][j3] & merge1))
									{
										matrix[i2][j3] &= (~merge1);
										ret = 1;
									}
								}

								if (ret)
								{
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(ROW, i, N, merge1, j, j1, N, N);
									fillCheckReturnStruct(ROW, i2, N, merge1, j2, N, N, N);
									LOG();
								}
								
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}

int checkXYWingInColumnRegion()
{
	int i, i1, i2, j, j1, j2, i3;
	int merge, merge1;
	int regionBeginRow, regionBeginRow1;
    int regionBeginColumn;
	int ret = 0;

    for (j = 0; j < N; j ++)
    {
		for (i = 0; i < N; i ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 2))
			{
				for (i1 = 0; i1 < N; i1 ++)
				{					
					if (unFixed(i1, j)
						&& (matrix[i1][j] & matrix[i][j])
						&& (matrix[i1][j] != matrix[i][j])
						&& (len(matrix[i1][j]) == 2))
					{
						merge = (matrix[i1][j] | matrix[i][j]) - (matrix[i1][j] & matrix[i][j]);
						merge1 = matrix[i1][j] - (matrix[i1][j] & matrix[i][j]);
						regionBeginRow = (i / 3) * 3;
						regionBeginColumn = (j / 3) * 3;
						
						for (j1 = 0; j1 < N; j1 ++)
						{
							i2 = regionBeginRow + j1 / 3;
							j2 = regionBeginColumn + j1 % 3;
							
							if ((j2 != j)
								&& unFixed(i2, j2)
								&& (matrix[i2][j2] == merge))
							{
								
								
								for (i3 = regionBeginRow; i3 < regionBeginRow + 3; i3  ++)
								{
									if (unFixed(i3, j)
										&& (matrix[i3][j] & merge1))
									{
										matrix[i3][j] &= (~merge1);
										ret = 1;
									}
								}

								regionBeginRow1 = (i1 / 3) * 3;

								for (i3 = regionBeginRow1; i3 < regionBeginRow1 + 3; i3  ++)
								{
									if (unFixed(i3, j2)
										&& (matrix[i3][j2] & merge1))
									{
										matrix[i3][j2] &= (~merge1);
										ret = 1;
									}
								}

								if (ret)
								{
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(COLUMN, N, j, merge1, i, i1, N, N);
									fillCheckReturnStruct(ROW, i2, N, merge1, j2, N, N, N);
									LOG();
								}
								
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}


int checkSwordfishInRow()
{
	int i, i1, i2, j, j1, j2;
	int diff, diff1, diff2, diff3, diff4;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {		
		for (j = 0; j < N; j ++)
		{			
			if (unFixed(i, j))
			{
				for (j1 = j + 1; j1 < N; j1 ++)
				{
					if (unFixed(i, j1)
						&& (matrix[i][j1] & matrix[i][j]))
					{
						diff = differenceSetInRow(i, j, j1, N, N);
						diff &= (matrix[i][j1] & matrix[i][j]);

						if (diff)
						{
							for (i1 = i + 1; i1 < N; i1 ++)
							{								
								if ((unFixed(i1, j1) && (matrix[i1][j1] & diff))
									|| (unFixed(i1, j) && (matrix[i1][j] & diff)))
								{
									for (j2 = j1 + 1; j2 < N; j2 ++)
									{
										if (unFixed(i1, j2)
											&& (matrix[i1][j2] & diff))
										{
											diff1 = differenceSetInRow(i1, j, j1, j2, N);
											diff2 = diff & diff1;
											if (diff2)
											{
												for (i2 = i1 + 1; i2 < N; i2 ++)
												{
                                                    diff3 = differenceSetInRow(i2, j, j1, j2, N);
													diff4 = diff3 & diff2;
													
													if (diff4)
													{
														
														
														if(cleanColumnWithException(j, diff4, i, i1, i2, N))
														{
															ret = 1;
														}

														if (cleanColumnWithException(j1, diff4, i, i1, i2, N))
														{
															ret = 1;
														}

														if (cleanColumnWithException(j2, diff4, i, i1, i2, N))
														{
															ret = 1;
														}													

														if (ret)
														{
															memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
															fillCheckReturnStruct(ROW, i, N, diff4, j, j1, j2, N);
															fillCheckReturnStruct(ROW, i1, N, diff4, j, j1, j2, N);
															fillCheckReturnStruct(ROW, i2, N, diff4, j, j1, j2, N);
															LOG();
														}
													}
												}												
											}
										}										
									}							
								}
							}
						}
						else
						{
							for (j2 = j1 + 1; j2 < N; j2 ++)
							{
								if (unFixed(i, j2)
									&& (matrix[i][j2] & matrix[i][j1] & matrix[i][j]))
								{
									diff = differenceSetInRow(i, j, j1, j2, N);
									diff &= (matrix[i][j2] & matrix[i][j1] & matrix[i][j]);

									if (diff)
									{
										for (i1 = i + 1; i1 < N; i1 ++)
										{											
                                            diff1 = differenceSetInRow(i1, j, j1, j2, N);
											diff2 = diff1 & diff;
											
											if (diff2)
											{
												for (i2 = i1 + 1; i2 < N; i2 ++)
												{
                                                    diff3 = differenceSetInRow(i2, j, j1, j2, N);
													diff4 = diff3 & diff2;
													
													if (diff4)
													{
														
														
														if(cleanColumnWithException(j, diff4, i, i1, i2, N))
														{
															ret = 1;
														}

														if (cleanColumnWithException(j1, diff4, i, i1, i2, N))
														{
															ret = 1;
														}

														if (cleanColumnWithException(j2, diff4, i, i1, i2, N))
														{
															ret = 1;
														}

														if (ret)
														{
															memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
															fillCheckReturnStruct(ROW, i, N, diff4, j, j1, j2, N);
															fillCheckReturnStruct(ROW, i1, N, diff4, j, j1, j2, N);
															fillCheckReturnStruct(ROW, i2, N, diff4, j, j1, j2, N);
															LOG();
														}
													}
												}
												
											}
											
										}
									}
								}
							}
						}
						
					}
				}
			}
		}
    }

	return ret;
	
}


int checkSwordfishInColumn()
{
	int i, i1, i2, j, j1, j2;
	int diff, diff1, diff2, diff3, diff4;
	int ret = 0;

    for (j = 0; j < N; j ++)
    {		
		for (i = 0; i < N; i ++)
		{			
			if (unFixed(i, j))
			{
				for (i1 = i + 1; i1 < N; i1 ++)
				{
					if (unFixed(i1, j)
						&& (matrix[i1][j] & matrix[i][j]))
					{
						diff = differenceSetInColumn(j, i, i1, N, N);
						diff &= (matrix[i][j] & matrix[i1][j]);

						if (diff)
						{
							for (j1 = j + 1; j1 < N; j1 ++)
							{								
								if ((unFixed(i1, j1) && (matrix[i1][j1] & diff))
									|| (unFixed(i, j1) && (matrix[i][j1] & diff)))
								{
									for (i2 = i1 + 1; i2 < N; i2 ++)
									{
										if (unFixed(i2, j1)
											&& (matrix[i2][j1] & diff))
										{
											diff1 = differenceSetInColumn(j1, i, i1, i2, N);
											diff2 = diff1 & diff;
											if (diff2)
											{
												for (j2 = j1 + 1; j2  < N; j2  ++)
												{
                                                    diff3 = differenceSetInColumn(j2, i, i1, i2, N);
													diff4 = diff3 & diff2;
													
													if (diff4)
													{
														
														
														if(cleanRowWithException(i, diff4, j, j1, j2, N))
														{
															ret = 1;
														}

														if (cleanRowWithException(i1, diff4, j, j1, j2, N))
														{
															ret = 1;
														}

														if (cleanRowWithException(i2, diff4, j, j1, j2, N))
														{
															ret = 1;
														}													

														if (ret)
														{
															memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
															fillCheckReturnStruct(COLUMN, N, j, diff4, i, i1, i2, N);
															fillCheckReturnStruct(COLUMN, N, j1, diff4, i, i1, i2, N);
															fillCheckReturnStruct(COLUMN, N, j2, diff4, i, i1, i2, N);
															LOG();
														}
													}
												}												
											}
										}										
									}							
								}
							}
						}
						else
						{
							for (i2 = i1 + 1; i2 < N; i2 ++)
							{
								if (unFixed(i2, j)
									&& (matrix[i2][j] & matrix[i1][j] & matrix[i][j]))
								{
									diff = differenceSetInColumn(j, i, i1, i2, N);
									diff &= (matrix[i2][j] & matrix[i1][j] & matrix[i][j]);

									if (diff)
									{
										for (j1 = j + 1; j1 < N; j1 ++)
										{											
                                            diff1 = differenceSetInColumn(j1, i, i1, i2, N);
											diff2 = diff1 & diff;											
											if (diff2)
											{
												for (j2 = j1 + 1; j2  < N; j2  ++)
												{
                                                    diff3 = differenceSetInColumn(j2, i, i1, i2, N);
													diff4 = diff3 & diff2;
													
													if (diff4)
													{
														
														
														if(cleanRowWithException(i, diff4, j, j1, j2, N))
														{
															ret = 1;
														}

														if (cleanRowWithException(i1, diff4, j, j1, j2, N))
														{
															ret = 1;
														}

														if (cleanRowWithException(i2, diff4, j, j1, j2, N))
														{
															ret = 1;
														}

														if (ret)
														{
															memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
															fillCheckReturnStruct(COLUMN, N, j, diff4, i, i1, i2, N);
															fillCheckReturnStruct(COLUMN, N, j1, diff4, i, i1, i2, N);
															fillCheckReturnStruct(COLUMN, N, j2, diff4, i, i1, i2, N);
															LOG();
														}
													}
												}												
											}
											
										}
									}
								}
							}
						}
						
					}
				}
			}
		}
    }

	return ret;
	
}


int checkXYZWingInRowRegion()
{
	int i, i2, j, j1, j2, j3;
	int merge1;
	int regionBeginRow, regionBeginColumn;
    int r;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {
		for (j = 0; j < N; j ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 3)) /* XYZ */
			{
				for (j1 = 0; j1 < N; j1 ++)
				{					
					if (unFixed(i, j1)
						&& (len(matrix[i][j1]) == 2)
						&& (len(matrix[i][j1] & matrix[i][j]) == 2)) /* XZ */
					{
						regionBeginRow = (i / 3) * 3;
						regionBeginColumn = (j / 3) * 3;
						
						for (r = 0; r < N; r ++)
						{
							i2 = regionBeginRow + r / 3;
							j2 = regionBeginColumn + r % 3;
							
							if ((i2 != i)
								&& unFixed(i2, j2)
								&& (matrix[i2][j2] != matrix[i][j1])
								&& (len(matrix[i2][j2]) == 2)
								&& (len(matrix[i2][j2] & matrix[i][j]) == 2)) /* YZ */
							{
								merge1 = matrix[i][j] & matrix[i][j1] & matrix[i2][j2]; /* Z */
								
								
								for (j3 = regionBeginColumn; j3 < regionBeginColumn + 3; j3  ++)
								{
									if ((j3 != j)
										&& (j3 != j1)
										&& unFixed(i, j3)
										&& (matrix[i][j3] & merge1))
									{
										matrix[i][j3] &= (~merge1);
										ret = 1;
									}
								}

								if (ret)
								{
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(ROW, i, N, merge1, j, j1, N, N);
									fillCheckReturnStruct(ROW, i2, N, merge1, j2, N, N, N);
									LOG();
								}
								
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}



int checkXYZWingInColumnRegion()
{
	int i, i1, i2, j, j2, i3;
	int merge1;
	int regionBeginRow, regionBeginColumn;
    int r;
	int ret = 0;

    for (j = 0; j < N; j ++)
    {
		for (i = 0; i < N; i ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 3)) /* XYZ */
			{
				for (i1 = 0; i1 < N; i1 ++)
				{					
					if (unFixed(i1, j)
						&& (len(matrix[i1][j]) == 2)
						&& (len(matrix[i1][j] & matrix[i][j]) == 2)) /* XZ */
					{
						regionBeginRow = (i / 3) * 3;
						regionBeginColumn = (j / 3) * 3;
						
						for (r = 0; r < N; r ++)
						{
							i2 = regionBeginRow + r / 3;
							j2 = regionBeginColumn + r % 3;
							
							if ((j2 != j)
								&& unFixed(i2, j2)
								&& (matrix[i2][j2] != matrix[i1][j])
								&& (len(matrix[i2][j2]) == 2)
								&& (len(matrix[i2][j2] & matrix[i][j]) == 2)) /* YZ*/
							{
								merge1 = matrix[i][j] & matrix[i1][j] & matrix[i2][j2]; /* Z */
								
								
								for (i3 = regionBeginRow; i3 < regionBeginRow + 3; i3  ++)
								{
									if ((i3 != i)
										&& (i3 != i1)
										&& unFixed(i3, j)
										&& (matrix[i3][j] & merge1))
									{
										matrix[i3][j] &= (~merge1);
										ret = 1;
									}
								}

								if (ret)
								{
									memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
									fillCheckReturnStruct(COLUMN, N, j, merge1, i, i1, N, N);
									fillCheckReturnStruct(ROW, i2, N, merge1, j2, N, N, N);
									LOG();
								}
								
							}
						}
					}
				}
			}
		}
    }

	return ret;
	
}


int checkWXYZWingInRowRegion()
{
	int i, j, j1, j2, j3;
	int merge1, merge2;
	int regionBeginRow, regionBeginColumn;
    int r, row, column;
	int ret = 0;

    for (i = 0; i < N; i ++)
    {
		for (j = 0; j < N; j ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 4)) /* WXYZ */
			{
				for (j1 = 0; j1 < N; j1 ++)
				{					
					if (unFixed(i, j1)
						&& (len(matrix[i][j1]) == 2)
						&& (len(matrix[i][j1] & matrix[i][j]) == 2)) /* XZ */
					{
						for (j2 = j1 + 1; j2 < N; j2 ++)
						{
							if (unFixed(i, j2)
								&& (len(matrix[i][j2]) == 2)
								&& (len(matrix[i][j2] & matrix[i][j]) == 2)
								&& (len(matrix[i][j2] & matrix[i][j1]) == 1)) /* YZ */
							{
								merge1 = matrix[i][j] - (matrix[i][j2] | matrix[i][j1]) + (matrix[i][j2] & matrix[i][j1]); /* WZ */
								merge2 = matrix[i][j2] & matrix[i][j1]; /* Z */
								regionBeginRow = (i / 3) * 3;
								regionBeginColumn = (j / 3) * 3;
								
								for (r = 0; r < N; r ++)
								{
									row = regionBeginRow + r / 3;
									column = regionBeginColumn + r % 3;
									
									if ((row != i)
										&& unFixed(row, column)
										&& (matrix[row][column] == merge1)) /* WZ */
									{										
										
										
										for (j3 = regionBeginColumn; j3 < regionBeginColumn + 3; j3  ++)
										{
											if ((j3 != j)
												&& (j3 != j1)
												&& (j3 != j2)
												&& unFixed(i, j3)
												&& (matrix[i][j3] & merge2))
											{
												matrix[i][j3] &= (~merge2);
												ret = 1;
											}
										}

										if (ret)
										{
											memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
											fillCheckReturnStruct(ROW, i, N, merge2, j, j1, j2, N);
											fillCheckReturnStruct(ROW, row, N, merge2, column, N, N, N);
											LOG();
										}
										
									}
								}
							}
						}			
					}
				}
			}
		}
    }

	return ret;
	
}

int checkWXYZWingInColumnRegion()
{
	int i, i1, i2, i3, j;
	int merge1, merge2;
	int regionBeginRow, regionBeginColumn;
    int r, row, column;
	int ret = 0;

    for (j = 0; j < N; j ++)
    {
		for (i = 0; i < N; i ++)
		{
			if (unFixed(i, j)
				&& (len(matrix[i][j]) == 4)) /* WXYZ */
			{
				for (i1 = 0; i1 < N; i1 ++)
				{					
					if (unFixed(i1, j)
						&& (len(matrix[i1][j]) == 2)
						&& (len(matrix[i1][j] & matrix[i][j]) == 2)) /* XZ */
					{
						for (i2 = i1 + 1; i2 < N; i2 ++)
						{
							if (unFixed(i2, j)
								&& (len(matrix[i2][j]) == 2)
								&& (len(matrix[i2][j] & matrix[i][j]) == 2)
								&& (len(matrix[i2][j] & matrix[i1][j]) == 1)) /* YZ */
							{
								merge1 = matrix[i][j] - (matrix[i1][j] | matrix[i2][j]) + (matrix[i1][j] & matrix[i2][j]); /* WZ */
								merge2 = matrix[i1][j] & matrix[i2][j]; /* Z */
								regionBeginRow = (i / 3) * 3;
								regionBeginColumn = (j / 3) * 3;
								
								for (r = 0; r < N; r ++)
								{
									row = regionBeginRow + r / 3;
									column = regionBeginColumn + r % 3;
									
									if ((column != j)
										&& unFixed(row, column)
										&& (matrix[row][column] == merge1)) /* WZ */
									{										
										
										
										for (i3 = regionBeginRow; i3 < regionBeginRow + 3; i3  ++)
										{
											if ((i3 != i)
												&& (i3 != i1)
												&& (i3 != i2)
												&& unFixed(i3, j)
												&& (matrix[i3][j] & merge2))
											{
												matrix[i3][j] &= (~merge2);
												ret = 1;
											}
										}

										if (ret)
										{
											memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
											fillCheckReturnStruct(COLUMN, N, j, merge2, i, i1, i2, N);
											fillCheckReturnStruct(ROW, row, N, merge2, column, N, N, N);
											LOG();
										}
										
									}
								}
							}
						}			
					}
				}
			}
		}
    }

	return ret;
	
}

int checkNakedSingle(int control)
{
	int row, column;
	int ret = 0;

	if (unFixedCounter < control)
	{
		return ret;
	}

	for (row = 0; row < N; row ++)
	{
		for (column = 0; column < N; column ++)
		{
			if (matrix[row][column] == 0)
			{
				return 2;
			}
			
			if (unFixed(row, column)
				&& (matrix[row][column] & (matrix[row][column] - 1)) == 0)
			{
				memset(&gCheckReturnStruct, 0, sizeof(gCheckReturnStruct));
				fillCheckReturnStruct(ROW, row, N, matrix[row][column], column, N, N, N);
				LOG();
				
				matrix[row][column] = bit2Num(matrix[row][column]);								
				clean(row, column);
				ret = 1;
				
			}
		}
	}
	
	return ret;
}


int checkHiddenSingle(int control)
{
	int row, column;
	int ret = 0;
	
	for (row = 0; row < N; row ++)
	{
        if ((unitUnFixedCounter[ROW][row] > control)
            && checkHiddenSingleInRow(row))
        {
            ret = 1;
        }
	}
 
	for (column = 0; column < N; column ++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkHiddenSingleInColumn(column))
        {
             ret = 1;
        }
	}
	
    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkHiddenSingleInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;
	
}


int checkNakedPair(int control)
{
	int row, column;
	int ret = 0;

	for (row = 0; row < N; row++)
	{
	    if ((unitUnFixedCounter[ROW][row] > control)
	        && checkNakedPairInRow(row))
	    {
	         ret = 1;
	    }
	}
   
      
	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkNakedPairInColumn(column))
        {
             ret = 1;
        }
	}

	
    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkNakedPairInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;		
}


int checkHiddenPair(int control)
{
	int row, column;
	int ret = 0;
	
	for (row = 0; row < N; row++)
	{
        if ((unitUnFixedCounter[ROW][row] > control)
            && checkHiddenPairInRow(row))
        {
             ret = 1;
        }
	}

	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkHiddenPairInColumn(column))
        {
             ret = 1;
        }
	}


    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkHiddenPairInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;	
}


int checkNakedTriplet(int control)
{
	int row, column;
	int ret = 0;

	for (row = 0; row < N; row++)
	{
	    if ((unitUnFixedCounter[ROW][row] > control)
	        && checkNakedTripletInRow(row))
	    {
	         ret = 1;
	    }
	}
   
      
	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkNakedTripletInColumn(column))
        {
             ret = 1;
        }
	}

	
    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkNakedTripletInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;		
}

int checkHiddenTriplet(int control)
{
	int row, column;
	int ret = 0;
	
	for (row = 0; row < N; row++)
	{
        if ((unitUnFixedCounter[ROW][row] > control)
            && checkHiddenTripletInRow(row))
        {
             ret = 1;
        }
	}

	for (column = 0; column < N ; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkHiddenTripletInColumn(column))
        {
             ret = 1;
        }
	}


    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkHiddenTripletInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;	
}

int checkNakedQuad(int control)
{
	int row, column;
	int ret = 0;

	for (row = 0; row < N; row++)
	{
	    if ((unitUnFixedCounter[ROW][row] > control)
	        && checkNakedQuadInRow(row))
	    {
	         ret = 1;
	    }
	}
   
      
	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkNakedQuadInColumn(column))
        {
             ret = 1;
        }
	}

	
    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkNakedQuadInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;		
}

int checkHiddenQuad(int control)
{
	int row, column;
	int ret = 0;
	
	for (row = 0; row < N; row++)
	{
        if ((unitUnFixedCounter[ROW][row] > control)
            && checkHiddenQuadInRow(row))
        {
             ret = 1;
        }
	}

	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkHiddenQuadInColumn(column))
        {
             ret = 1;
        }
	}


    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkHiddenQuadInRegion(row, column))
            {
                 ret = 1;
            }
    	}
    }

	return ret;	
}	


int checkIntersectionRemoval(int control)
{
	int row, column;
	int ret = 0;
	
    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkIntersectionRemovalRegionVsRow(row, column))
            {
                 ret = 1;
            }
    	}
    }

    for (row = 0; row < N; row += 3)
    {
    	for (column = 0; column < N; column += 3)
    	{
            if ((unitUnFixedCounter[REGION][regionLinearMap(row, column)] > control)
                && checkIntersectionRemovalRegionVsColumn(row, column))
            {
                 ret = 1;
            }
    	}
    }



    for (row = 0; row < N; row++)
	{
        if ((unitUnFixedCounter[ROW][row] > control)
            && checkIntersectionRemovalRowVsRegion(row))
        {
            ret = 1;
        }
	}




	for (column = 0; column < N; column++)
	{
        if ((unitUnFixedCounter[COLUMN][column] > control)
            && checkIntersectionRemovalColumnVsRegion(column))
        {
            ret = 1;
        }
	}

	return ret;	
}



int checkXWing(int control)
{
	int ret = 0;
	
	if ((unFixedCounter > control)
		&& checkXWingInRow())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkXWingInColumn())
	{
		ret = 1;
	}

	return ret;
}



int checkXYWing(int control)
{
	int ret = 0;
	
	if ((unFixedCounter > control)
		&& checkXYWingInRowColumn())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkXYWingInRowRegion())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkXYWingInColumnRegion())
	{
		ret = 1;
	}

	return ret;
}





int checkSwordfish(int control)
{
	int ret = 0;
	
	if ((unFixedCounter > control)
		&& checkSwordfishInRow())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkSwordfishInColumn())
	{
		ret = 1;
	}

	return ret;
}



int checkXYZWing(int control)
{
	int ret = 0;
	
	if ((unFixedCounter > control)
		&& checkXYZWingInRowRegion())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkXYZWingInColumnRegion())
	{
		ret = 1;
	}

	return ret;
}


int checkWXYZWing(int control)
{
	int ret = 0;
	
	if ((unFixedCounter > control)
		&& checkWXYZWingInRowRegion())
	{
		ret = 1;
	}

	if ((unFixedCounter > control)
		&& checkWXYZWingInColumnRegion())
	{
		ret = 1;
	}

	return ret;
}



int cleanRow(int row, int value)
{
    int column;
    int ret = 0;

    for (column = 0; column < N; column ++)
    {
        if (unFixed(row, column) 
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);           
            ret = 1;
        }
    }

    return ret;
}

int cleanColumn(int column, int value)
{
    int row;
    int ret = 0;

    for (row = 0; row < N; row ++)
    {
        if (unFixed(row, column) 
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);
            ret = 1;
        }
    }

    return ret;
}

int cleanRegion(int row, int column, int value)
{
    int regionBeginRow = (row/3) * 3;
    int regionBeginColumn = (column/3) * 3;
    int i;
    int ret = 0;

    for (i = 0; i < N; i ++)
	{
		row = regionBeginRow + i/3;
		column = regionBeginColumn + i%3;

		if (unFixed(row, column) 
            && (matrix[row][column] & value))
        {
            matrix[row][column] &= (~value);
            ret = 1;
        }
    }

    return ret;
}


int checkXYZWingInRowRegionTest()
{	
	int round = 0;
	char *fun = "checkXYZWingInRowRegion";
	
	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0x700; /* 123*/
	matrix[1][4] = 0x300; /* 12*/
	
	matrix[2][2] = 0x500; /* 13*/

	matrix[1][0] = 0xF100; /* 15678*/
	matrix[1][2] = 0xF100; /* 15678*/
	matrix[1][3] = 0xF100; /* 15678*/
	
	if (checkXYZWingInRowRegion()
		&& matrix[1][0] == 0xF000
		&& matrix[1][2] == 0xF000
		&& matrix[1][3] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", __func__, ++ round);
		return 0;
	}

	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0x700; /* 123*/
	matrix[1][4] = 0x300; /* 12*/
	
	matrix[2][2] = 0x300; /* 12*/

	matrix[1][0] = 0xF100; /* 15678*/
	matrix[1][2] = 0xF100; /* 15678*/
	matrix[1][3] = 0xF100; /* 15678*/
	
	if (!checkXYZWingInRowRegion()
		&& matrix[1][0] == 0xF100
		&& matrix[1][2] == 0xF100
		&& matrix[1][3] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
}

int checkXYZWingInColumnRegionTest()
{	
	int round = 0;
	char *fun = "checkXYZWingInColumnRegion";
	
	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0x700; /* 123*/
	matrix[4][1] = 0x300; /* 12*/
	
	matrix[2][2] = 0x500; /* 13*/

	matrix[0][1] = 0xF100; /* 15678*/
	matrix[2][1] = 0xF100; /* 15678*/
	matrix[3][1] = 0xF100; /* 15678*/
	
	if (checkXYZWingInColumnRegion()
		&& matrix[0][1] == 0xF000
		&& matrix[2][1] == 0xF000
		&& matrix[3][1] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0x700; /* 123*/
	matrix[4][1] = 0x300; /* 12*/
	
	matrix[2][2] = 0x300; /* 12*/

	matrix[0][1] = 0xF100; /* 15678*/
	matrix[2][1] = 0xF100; /* 15678*/
	matrix[3][1] = 0xF100; /* 15678*/
	
	if (!checkXYZWingInColumnRegion()
		&& matrix[0][1] == 0xF100
		&& matrix[2][1] == 0xF100
		&& matrix[3][1] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
}

int checkWXYZWingInRowRegionTest()
{	
	int round = 0;
	char *fun = "checkWXYZWingInColumnRegion";
	
	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0xF00; /* 1234*/
	matrix[1][4] = 0x300; /* 12*/
	matrix[1][5] = 0x500; /* 13*/	
	matrix[2][2] = 0x900; /* 14*/

	matrix[1][0] = 0xF100; /* 15678*/
	matrix[1][2] = 0xF100; /* 15678*/
	matrix[1][3] = 0xF100; /* 15678*/
	
	if (checkWXYZWingInRowRegion()
		&& matrix[1][0] == 0xF000
		&& matrix[1][2] == 0xF000
		&& matrix[1][3] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0xF00; /* 1234*/
	matrix[1][4] = 0x300; /* 12*/
	matrix[1][5] = 0x500; /* 13*/	
	matrix[2][2] = 0xF00; /* 12344*/

	matrix[1][0] = 0xF100; /* 15678*/
	matrix[1][2] = 0xF100; /* 15678*/
	matrix[1][3] = 0xF100; /* 15678*/
	
	if (!checkWXYZWingInRowRegion()
		&& matrix[1][0] == 0xF100
		&& matrix[1][2] == 0xF100
		&& matrix[1][3] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkWXYZWingInColumnRegionTest()
{	
	int round = 0;
	char *fun = "checkWXYZWingInColumnRegion";
	
	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0xF00; /* 1234*/
	matrix[4][1] = 0x300; /* 12*/
	matrix[5][1] = 0x500; /* 13*/	
	matrix[2][2] = 0x900; /* 14*/

	matrix[0][1] = 0xF100; /* 15678*/
	matrix[2][1] = 0xF100; /* 15678*/
	matrix[3][1] = 0xF100; /* 15678*/
	
	if (checkWXYZWingInColumnRegion()
		&& matrix[0][1] == 0xF000
		&& matrix[2][1] == 0xF000
		&& matrix[3][1] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	memset(matrix, 0, sizeof(matrix));
	matrix[1][1] = 0xF00; /* 1234*/
	matrix[4][1] = 0x300; /* 12*/
	matrix[5][1] = 0x500; /* 13*/	
	matrix[2][2] = 0x300; /* 12*/

	matrix[0][1] = 0xF100; /* 15678*/
	matrix[2][1] = 0xF100; /* 15678*/
	matrix[3][1] = 0xF100; /* 15678*/
	
	if (!checkWXYZWingInColumnRegion()
		&& matrix[0][1] == 0xF100
		&& matrix[2][1] == 0xF100
		&& matrix[3][1] == 0xF100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}



int checkXWingInRowTest()
{	
	int round = 0;
	char *fun = "checkXWingInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRow(1, 0x400);
	cleanRow(4, 0x400);
	
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][5] = 0x600; /* 23*/
	
	matrix[4][2] = 0x600; /* 23*/
	matrix[4][5] = 0x600; /* 23*/

    matrix[2][2] = 0x700; /* 123*/
	matrix[2][5] = 0x700; /* 123*/
	
	if (checkXWingInRow()
		&& matrix[2][2] == 0x300
		&& matrix[2][5] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}



int checkXWingInColumnTest()
{	
	int round = 0;
	char *fun = "checkXWingInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumn(2, 0x400);
	cleanColumn(5, 0x400);
	
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][5] = 0x600; /* 23*/
	
	matrix[4][2] = 0x600; /* 23*/
	matrix[4][5] = 0x600; /* 23*/

    matrix[1][6] = 0x700; /* 123*/
	matrix[4][6] = 0x700; /* 123*/
	
	if (checkXWingInColumn()
		&& matrix[1][6] == 0x300
		&& matrix[4][6] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}


int checkXYWingInRowColumnTest()
{	
	int round = 0;
	char *fun = "checkXYWingInRowColumn";
	
	memset(matrix, 0, sizeof(matrix));
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][6] = 0x300; /* 12*/
	
	matrix[5][2] = 0x500; /* 13*/

	matrix[5][6] = 0xF100; /* 15678*/
	
	if (checkXYWingInRowColumn()
		&& matrix[5][6] == 0xF000)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;

	
}


int checkXYWingInRowRegionTest()
{	
	int round = 0;
	char *fun = "checkXYWingInRowRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][6] = 0x300; /* 12*/
	
	matrix[2][2] = 0x500; /* 13*/

	if (checkXYWingInRowRegion()
		&& matrix[1][0] == 0x1FE00
		&& matrix[1][1] == 0x1FE00
		&& matrix[2][6] == 0x1FE00
		&& matrix[2][7] == 0x1FE00
		&& matrix[2][8] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;

	
}

int checkXYWingInColumnRegionTest()
{	
	int round = 0;
	char *fun = "checkXYWingInColumnRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	matrix[1][2] = 0x600; /* 23*/
	matrix[6][2] = 0x300; /* 12*/
	
	matrix[0][0] = 0x500; /* 13*/

	if (checkXYWingInColumnRegion()
		&& matrix[6][0] == 0x1FE00
		&& matrix[7][0] == 0x1FE00
		&& matrix[8][0] == 0x1FE00
		&& matrix[0][2] == 0x1FE00
		&& matrix[2][2] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

    return 1;
	
}


int checkSwordfishInRowTest()
{	
	int round = 0;
	char *fun = "checkSwordfishInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRow(1, 0x400);
	cleanRow(4, 0x400);
	cleanRow(7, 0x400);
	
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][5] = 0x600; /* 23*/
	matrix[1][8] = 0x600; /* 23*/
	
	matrix[4][2] = 0x600; /* 23*/
	matrix[4][5] = 0x600; /* 23*/
	matrix[4][8] = 0x600; /* 23*/

	matrix[7][2] = 0x600; /* 23*/
	matrix[7][5] = 0x600; /* 23*/
	matrix[7][8] = 0x600; /* 23*/

    matrix[2][2] = 0x700; /* 123*/
	matrix[2][5] = 0x700; /* 123*/
	matrix[2][8] = 0x700; /* 123*/
	
	if (checkSwordfishInRow()
		&& matrix[2][2] == 0x300
		&& matrix[2][5] == 0x300
		&& matrix[2][8] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}


int checkSwordfishInColumnTest()
{	
	int round = 0;
	char *fun = "checkSwordfishInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumn(2, 0x400);
	cleanColumn(5, 0x400);
	cleanColumn(8, 0x400);
	
	matrix[1][2] = 0x600; /* 23*/
	matrix[1][5] = 0x600; /* 23*/
	matrix[1][8] = 0x600; /* 23*/
	
	matrix[4][2] = 0x600; /* 23*/
	matrix[4][5] = 0x600; /* 23*/
	matrix[4][8] = 0x600; /* 23*/

	matrix[7][2] = 0x600; /* 23*/
	matrix[7][5] = 0x600; /* 23*/
	matrix[7][8] = 0x600; /* 23*/

    matrix[1][6] = 0x700; /* 123*/
	matrix[4][6] = 0x700; /* 123*/
	matrix[7][6] = 0x700; /* 123*/
	
	if (checkSwordfishInColumn()
		&& matrix[1][6] == 0x300
		&& matrix[4][6] == 0x300
		&& matrix[7][6] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkIntersectionRemovalRegionVsRowTest()
{	
	int round = 0;
	char *fun = "checkIntersectionRemovalRegionVsRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0x100, 0, 1, 2, N);
	
	
	
	if (checkIntersectionRemovalRegionVsRow(0, 0)
		&& matrix[0][2] == 0x1FF00
		&& matrix[0][4] == 0x1FE00
		&& matrix[0][8] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkIntersectionRemovalRegionVsColumnTest()
{	
	int round = 0;
	char *fun = "checkIntersectionRemovalRegionVsColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0x100, 0, 3, 6, N);
	
	
	
	if (checkIntersectionRemovalRegionVsColumn(0, 0)
		&& matrix[2][0] == 0x1FF00
		&& matrix[4][0] == 0x1FE00
		&& matrix[8][0] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkIntersectionRemovalRowVsRegionTest()
{	
	int round = 0;
	char *fun = "checkIntersectionRemovalRowVsRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithException(0, 0x100, 0, 1, 2, N);
	
	
	
	if (checkIntersectionRemovalRowVsRegion(0)
		&& matrix[0][2] == 0x1FF00
		&& matrix[1][1] == 0x1FE00
		&& matrix[2][2] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkIntersectionRemovalColumnVsRegionTest()
{	
	int round = 0;
	char *fun = "checkIntersectionRemovalColumnVsRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithException(0, 0x100, 0, 1, 2, N);
	
	
	
	if (checkIntersectionRemovalColumnVsRegion(0)
		&& matrix[2][0] == 0x1FF00
		&& matrix[1][1] == 0x1FE00
		&& matrix[2][2] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkNakedSingleTest()
{	
	int round = 0;
	char *fun = "checkNakedSingle";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithIn(0, 0x100, 0, N, N, N);
	
	
	
	if (checkNakedSingle(0)
		&& matrix[0][0] == 1
		&& matrix[0][8] == 0x1FE00
		&& matrix[8][0] == 0x1FE00
		&& matrix[2][2] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

    cleanRowWithIn(0, 0x100, 0, 8, N, N);
	
	if (2 == checkNakedSingle(0)
		&& matrix[0][0] == 1
		&& matrix[0][8] == 0
		&& matrix[8][0] == 0x1FE00
		&& matrix[2][2] == 0x1FE00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}

	return 1;
	
}

int checkNakedPairInRowTest()
{	
	int round = 0;
	char *fun = "checkNakedPairInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithIn(0, 0x300, 0, 1, N, N);
	
	
	if (checkNakedPairInRow(0)
		&& matrix[0][0] == 0x300
		&& matrix[0][2] == 0x1FC00
		&& matrix[0][8] == 0x1FC00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedPairInColumnTest()
{	
	int round = 0;
	char *fun = "checkNakedPairInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithIn(0, 0x300, 0, 1, N, N);
	
	
	if (checkNakedPairInColumn(0)
		&& matrix[0][0] == 0x300
		&& matrix[2][0] == 0x1FC00
		&& matrix[8][0] == 0x1FC00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedPairInRegionTest()
{	
	int round = 0;
	char *fun = "checkNakedPairInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithIn(0, 0, 0x300, 0, 1, N, N);
	
	
	if (checkNakedPairInRegion(0, 0)
		&& matrix[0][0] == 0x300
		&& matrix[0][2] == 0x1FC00
		&& matrix[1][1] == 0x1FC00
		&& matrix[2][2] == 0x1FC00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedTripletInRowTest()
{	
	int round = 0;
	char *fun = "checkNakedTripletInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithIn(0, 0x700, 0, 1, 2, N);
	
	
	if (checkNakedTripletInRow(0)
		&& matrix[0][2] == 0x700
		&& matrix[0][4] == 0x1F800
		&& matrix[0][8] == 0x1F800)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedTripletInColumnTest()
{	
	int round = 0;
	char *fun = "checkNakedTripletInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithIn(0, 0x700, 0, 1, 2, N);
	
	
	if (checkNakedTripletInColumn(0)
		&& matrix[2][0] == 0x700
		&& matrix[4][0] == 0x1F800
		&& matrix[8][0] == 0x1F800)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedTripletInRegionTest()
{	
	int round = 0;
	char *fun = "checkNakedTripletInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithIn(0, 0, 0x700, 0, 1, 2, N);
	
	
	if (checkNakedTripletInRegion(0, 0)
		&& matrix[0][0] == 0x700
		&& matrix[1][1] == 0x1F800
		&& matrix[2][2] == 0x1F800)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}


int checkNakedQuadInRowTest()
{	
	int round = 0;
	char *fun = "checkNakedQuadInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithIn(0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkNakedQuadInRow(0)
		&& matrix[0][3] == 0xF00
		&& matrix[0][4] == 0x1F000
		&& matrix[0][8] == 0x1F000)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedQuadInColumnTest()
{	
	int round = 0;
	char *fun = "checkNakedQuadInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithIn(0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkNakedQuadInColumn(0)
		&& matrix[2][0] == 0xF00
		&& matrix[4][0] == 0x1F000
		&& matrix[8][0] == 0x1F000)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkNakedQuadInRegionTest()
{	
	int round = 0;
	char *fun = "checkNakedQuadInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithIn(0, 0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkNakedQuadInRegion(0, 0)
		&& matrix[0][0] == 0xF00
		&& matrix[1][1] == 0x1F000
		&& matrix[2][2] == 0x1F000)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenSingleInRowTest()
{	
	int round = 0;
	char *fun = "checkHiddenSingleInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithException(0, 0x100, 0, N, N, N);
	
	
	if (checkHiddenSingleInRow(0)
		&& matrix[0][0] == 0x100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenSingleInColumnTest()
{	
	int round = 0;
	char *fun = "checkHiddenSingleInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithException(0, 0x100, 0, N, N, N);
	
	
	if (checkHiddenSingleInColumn(0)
		&& matrix[0][0] == 0x100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenSingleInRegionTest()
{	
	int round = 0;
	char *fun = "checkHiddenSingleInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0x100, 0, N, N, N);
	
	
	if (checkHiddenSingleInRegion(0, 0)
		&& matrix[0][0] == 0x100)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenPairInRowTest()
{	
	int round = 0;
	char *fun = "checkHiddenPairInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithException(0, 0x300, 0, 1, N, N);
	
	
	if (checkHiddenPairInRow(0)
		&& matrix[0][0] == 0x300
		&& matrix[0][1] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenPairInColumnTest()
{	
	int round = 0;
	char *fun = "checkHiddenPairInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithException(0, 0x300, 0, 1, N, N);
	
	
	if (checkHiddenPairInColumn(0)
		&& matrix[0][0] == 0x300
		&& matrix[1][0] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenPairInRegionTest()
{	
	int round = 0;
	char *fun = "checkHiddenPairInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0x300, 0, 1, N, N);
	
	
	if (checkHiddenPairInRegion(0, 0)
		&& matrix[0][0] == 0x300
		&& matrix[0][1] == 0x300)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenTripletInRowTest()
{	
	int round = 0;
	char *fun = "checkHiddenTripletInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithException(0, 0x700, 0, 1, 2, N);
	
	
	if (checkHiddenTripletInRow(0)
		&& matrix[0][0] == 0x700
		&& matrix[0][1] == 0x700
		&& matrix[0][2] == 0x700)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenTripletInColumnTest()
{	
	int round = 0;
	char *fun = "checkHiddenTripletInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithException(0, 0x700, 0, 1, 2, N);
	
	
	if (checkHiddenTripletInColumn(0)
		&& matrix[0][0] == 0x700
		&& matrix[1][0] == 0x700
		&& matrix[2][0] == 0x700)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenTripletInRegionTest()
{	
	int round = 0;
	char *fun = "checkHiddenTripletInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0x700, 0, 1, 2, N);
	
	
	if (checkHiddenTripletInRegion(0, 0)
		&& matrix[0][0] == 0x700
		&& matrix[0][1] == 0x700
		&& matrix[0][2] == 0x700)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenQuadInRowTest()
{	
	int round = 0;
	char *fun = "checkHiddenQuadInRow";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRowWithException(0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkHiddenQuadInRow(0)
		&& matrix[0][0] == 0xF00
		&& matrix[0][1] == 0xF00
		&& matrix[0][2] == 0xF00
		&& matrix[0][3] == 0xF00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenQuadInColumnTest()
{	
	int round = 0;
	char *fun = "checkHiddenQuadInColumn";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanColumnWithException(0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkHiddenQuadInColumn(0)
		&& matrix[0][0] == 0xF00
		&& matrix[1][0] == 0xF00
		&& matrix[2][0] == 0xF00
		&& matrix[3][0] == 0xF00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int checkHiddenQuadInRegionTest()
{	
	int round = 0;
	char *fun = "checkHiddenQuadInRegion";
	
	memset(matrix, 0, sizeof(matrix));
	initMatrix();
	cleanRegionWithException(0, 0, 0xF00, 0, 1, 2, 3);
	
	
	if (checkHiddenQuadInRegion(0, 0)
		&& matrix[0][0] == 0xF00
		&& matrix[0][1] == 0xF00
		&& matrix[0][2] == 0xF00
		&& matrix[1][0] == 0xF00)
	{
		
	}
	else
	{
		printf("%s test %d failed!\n", fun, ++ round);
		return 0;
	}
	
	return 1;
	
}

int test()
{
    if(!checkWXYZWingInRowRegionTest())
    {
		return 0;
    }

	if(!checkWXYZWingInColumnRegionTest())
    {
		return 0;
    }

	if(!checkXYZWingInRowRegionTest())
    {
		return 0;
    }

	if(!checkXYZWingInColumnRegionTest())
    {
		return 0;
    }

	if(!checkXYWingInRowColumnTest())
    {
		return 0;
    }

	if(!checkXYWingInRowRegionTest())
    {
		return 0;
    }

	if(!checkXYWingInColumnRegionTest())
    {
		return 0;
    }

	if(!checkSwordfishInRowTest())
    {
		return 0;
    }

	if(!checkSwordfishInColumnTest())
    {
		return 0;
    }

	if(!checkXWingInRowTest())
    {
		return 0;
    }

	if(!checkXWingInColumnTest())
    {
		return 0;
    }

    if(!checkIntersectionRemovalRegionVsRowTest())
    {
		return 0;
    }

	if(!checkIntersectionRemovalRegionVsColumnTest())
    {
		return 0;
    }

	if(!checkIntersectionRemovalRowVsRegionTest())
    {
		return 0;
    }

	if(!checkIntersectionRemovalColumnVsRegionTest())
    {
		return 0;
    }	

    if(!checkNakedSingleTest())
    {
		return 0;
    }

    if(!checkNakedPairInRowTest())
    {
		return 0;
    }

	if(!checkNakedPairInColumnTest())
    {
		return 0;
    }

	if(!checkNakedPairInRegionTest())
    {
		return 0;
    }		

	if(!checkNakedTripletInRowTest())
    {
		return 0;
    }

	if(!checkNakedTripletInColumnTest())
    {
		return 0;
    }

	if(!checkNakedTripletInRegionTest())
    {
		return 0;
    }		
    
    if(!checkNakedQuadInRowTest())
    {
		return 0;
    }

	if(!checkNakedQuadInColumnTest())
    {
		return 0;
    }

	if(!checkNakedQuadInRegionTest())
    {
		return 0;
    }		
				

	if(!checkHiddenSingleInRowTest())
    {
		return 0;
    }

	if(!checkHiddenSingleInColumnTest())
    {
		return 0;
    }

	if(!checkHiddenSingleInRegionTest())
    {
		return 0;
    }

	if(!checkHiddenPairInRowTest())
    {
		return 0;
    }

	if(!checkHiddenPairInColumnTest())
    {
		return 0;
    }

	if(!checkHiddenPairInRegionTest())
    {
		return 0;
    }

	if(!checkHiddenTripletInRowTest())
    {
		return 0;
    }

	if(!checkHiddenTripletInColumnTest())
    {
		return 0;
    }

	if(!checkHiddenTripletInRegionTest())
    {
		return 0;
    }

	if(!checkHiddenQuadInRowTest())
    {
		return 0;
    }

	if(!checkHiddenQuadInColumnTest())
    {
		return 0;
    }

	if(!checkHiddenQuadInRegionTest())
    {
		return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    char c[N] = {0};
    int backupMatrix[N][N];
    int saved = 0;
    int unFixedCounterBackup;
	int ret = 0;
	
	if (!test())
	{
		printf("fix the bug!!!\n");
		//scanf("%s",c);
		return 1;
	}
	else
	{
		//printf("self test succeed!!!!\n");
	}
    
    //while(1)
    {    
        
        if (!saved)
        {
		    memset(matrix, 0, sizeof(matrix)); 
		    if (argc ==1)
			{
				readMatrix();
			}
			else
			{
				readMatrixFromFile(argv[1]);
			}      	
            
            printf("read the matrix as:\n");
            printMatrix(matrix);
            memcpy(backupMatrix, matrix, sizeof(matrix));
            //saveMatrix();
            saved = 1;
        }
        else
        {
            //printf("replay the matrix again ? default is n.(y/n):");
            //scanf("%s",c);            
            c[0] = 'n';
            if (c[0] == 'y' || c[0] == 'Y')
            {
                memcpy(matrix, backupMatrix, sizeof(matrix));
            }
            else
            {
                saved = 0;
                //continue;
            }           
        }       
        
        if (!initMatrix())
        {
			printf("the init puzzle is wrong!!!\n\n");
			return 1;
        }
		
    	readLogParameter();
		ret = check();
     
        if (ret == 1)
        {
			printMatrix(matrix);
			printf("\nthe puzzle has been successfully solved!!!\n");
			return 0;	        
        }
		else if (ret == 2)
		{
			printMatrix(matrix);
			printf("\nthe puzzle has no solution!!!\n");
			return 1;
		}
		else
		{
			printf("\nUsing defined algorithms to narrow down the puzzle to.\n");
	        printMatrix(matrix);
	        printf("\nTry to solve the puzzle with recursive.\n");

			if (recursiveSolve(0))
			{
				printf("\none of the solutions is :\n");
				printMatrix(matrix);
				return 0;
			}
			else
			{
				printf("\nsorry, the puzzle can not be successfully solved, please report the puzzle to the author, thanks!\n");
				return 1;
			}
		}
    }

	return 0;
}

