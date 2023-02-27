#include <GameMath/GameMath.h>
int GameMath::Matrix3x3::PrintWidth=5;
//*************************************************
//ALL MATRICES USED IN THIS PROGRAM ARE COLUMN MAJOR MATRICES SINCE OPENGL HANDLES COLUM MAJOR MATRICES INTERNALLY
//FOR ACCESSING THE VALUE OF A PARTICULAR CELL OF THE MATRIX, THE COLUMN IS NEEDED TO BE PROVIDED FIRST
//AND THE ROW IS NEEDED TO BE PROVIED SECOND ARGUMENT LIKE SO  MATRIX[COL][ROW]
//*************************************************


//*************************************************
//DEFAULT CONSTRUCTOR: THIS CREATES A 3X3 IDENTITY MATRIX WHERE THE VALUES OF THE DIAGNOL OF THE MATRIX ARE SET TO 1.0
//WHILE THE REST OF THE VALUES ARE SET TO 0.0
//*************************************************
GameMath::Matrix3x3::Matrix3x3()
{
Matrix[0][0] = 1.0f;
Matrix[0][1] = 0.0f;
Matrix[0][2] = 0.0f;

Matrix[1][0] = 0.0f;
Matrix[1][1] = 1.0f;
Matrix[1][2] = 0.0f;

Matrix[2][0] = 0.0f;
Matrix[2][1] = 0.0f;
Matrix[2][2] = 1.0f;
}
//*************************************************
//CONSTRUCTOR: THAT CREATES A 3X3 MATRIX USING A 4X4 MATRIX BY SPECIFYING THE ROW AND THE COLUMN OF THE
//4X4 MATRIX THAT IS NEEDED TO BE SKIPPED
//*************************************************
GameMath::Matrix3x3::Matrix3x3(GameMath::Matrix4x4 *matrix, int Excludedcol, int Excludedrow)
{
int row = 0;
int col = 0;

for (int i=0;i<4;i++)
{
    for (int j=0;j<4;j++)
    {
        if (i!=Excludedcol && j!=Excludedrow)
        {
            Matrix[col][row] = matrix->Matrix[i][j];
            row+= 1;
        }
    }
    if (i != Excludedcol)
    {
     row=0;
     col+= 1;
    }
}
}
//*************************************************
//The area of the parallelogram is the absolute value of the determinant of the matrix formed by the vectors representing the parallelogram's side
//The volume of this parallelepiped is the absolute value of the determinant of the matrix formed by the columns constructed from the vectors r1, r2, and r3.
//
//THIS FUNCTION CALCULATES THE DETERMINANT OF A MATRIX BY USING THE FOLLOWING FORMULAE
//[A , B , C]
//[D , E , F]    = A * [E , F] - B * [D , F] + C * [D , E]   = AEI + BFG + CDH - CEG - BDI - AFH
//[G , H , I]          [H , I]       [G , I]       [G , H]
//*************************************************
float GameMath::Matrix3x3::Determinant()
{
return Matrix[0][0] * (Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1]) - Matrix[1][0] * (Matrix[0][1] * Matrix[2][2] - Matrix[0][2] * Matrix[1][1]) + Matrix[2][0] * (Matrix[0][1] * Matrix[1][2] - Matrix[0][2] * Matrix[1][1]);
}
