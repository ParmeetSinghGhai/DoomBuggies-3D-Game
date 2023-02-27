#include <GameMath/GameMath.h>
#include <GameBoundingBox/GameBoundingBox.h>
int GameMath::Matrix4x4::PrintWidth=5;


//*************************************************
//DEFAULT CONSTRUCTOR: THIS CREATES A 4X4 IDENTITY MATRIX WHERE THE VALUES OF THE DIAGNOL OF THE MATRIX ARE SET TO 1.0
//WHILE THE REST OF THE VALUES ARE SET TO 0.0
//*************************************************
GameMath::Matrix4x4::Matrix4x4()
{
Matrix[0][0]=1.0f;
Matrix[0][1]=0.0f;
Matrix[0][2]=0.0f;
Matrix[0][3]=0.0f;

Matrix[1][0]=0.0f;
Matrix[1][1]=1.0f;
Matrix[1][2]=0.0f;
Matrix[1][3]=0.0f;

Matrix[2][0]=0.0f;
Matrix[2][1]=0.0f;
Matrix[2][2]=1.0f;
Matrix[2][3]=0.0f;

Matrix[3][0]=0.0f;
Matrix[3][1]=0.0f;
Matrix[3][2]=0.0f;
Matrix[3][3]=1.0f;
}
//*************************************************
//CONSTRUCTOR: THIS CONSTRUCTOR ACCEPTS THE ADDRESS OF THE FIRST VALUE IN AN ARRAY OF FLOATING POINTS OF SIZE 16
//THE ARRAY IS EXPECTED TO CONTAIN 16 FLOATING POINT VALUES ALIGNED AS 4 COLUMNS OF SIZE 4 EACH
//*************************************************
GameMath::Matrix4x4::Matrix4x4(float *input)
{
Matrix[0][0]=*(input++);
Matrix[0][1]=*(input++);
Matrix[0][2]=*(input++);
Matrix[0][3]=*(input++);

Matrix[1][0]=*(input++);
Matrix[1][1]=*(input++);
Matrix[1][2]=*(input++);
Matrix[1][3]=*(input++);

Matrix[2][0]=*(input++);
Matrix[2][1]=*(input++);
Matrix[2][2]=*(input++);
Matrix[2][3]=*(input++);

Matrix[3][0]=*(input++);
Matrix[3][1]=*(input++);
Matrix[3][2]=*(input++);
Matrix[3][3]=*(input++);
}

//*************************************************
//THIS FUNCTION RESETS THE MATRIX TO AN IDENTITY MATRIX BY SETTING THE DIAGNOL VALUE AS 1.0
//AND THE REST OF THE VALUES AS 0.0
//*************************************************
void GameMath::Matrix4x4::Clean()
{
Matrix[0][0]=1.0f;
Matrix[0][1]=0.0f;
Matrix[0][2]=0.0f;
Matrix[0][3]=0.0f;

Matrix[1][0]=0.0f;
Matrix[1][1]=1.0f;
Matrix[1][2]=0.0f;
Matrix[1][3]=0.0f;

Matrix[2][0]=0.0f;
Matrix[2][1]=0.0f;
Matrix[2][2]=1.0f;
Matrix[2][3]=0.0f;

Matrix[3][0]=0.0f;
Matrix[3][1]=0.0f;
Matrix[3][2]=0.0f;
Matrix[3][3]=1.0f;
}
//*************************************************
//THIS FUNCTION MULTIPLES TWO 4X4 MATRICES LIKE SO
//IMPORTANT RULE IN MATRIX MULTIPLICATION IS THAT THE NUMBER OF COLUMNS OF FIRST MATRIX MUST EQUAL THE NUMBER OF ROWS OF THE SECOND MATRIX
//
//   MATRIX 1	    MATRIX 2
//[A1,B1,C1,D1]   [E1,F1,G1,H1]    [ A1*E1 + B1*E2 + C1*E3 + D1*E4 , A1*F1 + B1*F2 + C1*F3 + D1*F4 , A1*G1 + B1*G2 + C1*G3 + D1*G4 , A1*H1 + B1*H2 + C1*H3 + D1*H4 ]
//[A2,B2,C2,D2] X [E2,F2,G2,H2] =  [															           ]
//[A3,B3,C3,D3]   [E3,F3,G3,H3]    [       															   ]
//[A4,B4,C4,D4]   [E4,F4,G4,H4]    [    															   ]
//*************************************************
GameMath::Matrix4x4 GameMath::Matrix4x4::operator*(const Matrix4x4 &RHS) const
{
GameMath::Matrix4x4 Output;
float buffer;
for(int LHSrow=0;LHSrow<=3;LHSrow++)
{
    for (int RHScol=0;RHScol<=3;RHScol++)
    {
       buffer=0.0f;
       for(int INCnum=0;INCnum<=3;INCnum++)
       {
           buffer= Matrix[INCnum][LHSrow] * RHS.Matrix[RHScol][INCnum] + buffer;
       }
       Output.Matrix[RHScol][LHSrow]=buffer;
    }
}
return Output;
}
//*************************************************
//THIS FUNCTION MULTIPLES A 4X4 MATRIX WITH A VECTOR OF SIZE 3 BY TREATING THAT VECTOR OF SIZE 3 AS A VECTOR OF SIZE 4 WITH THE LAST COMPONENT OF THAT VECTOR SET AS 1.0
//VECTOR 3 IS CONVERTED INTO VECTOR 4 AND THEN INTO A 1X4 MATRIX WITH 1 COLUMN AND 4 ROWS SO IT CAN BE MULTIPLIED WITH ANOTHER MATRIX LIKE SO
//
//IMPORTANT RULE IN MATRIX MULTIPLICATION IS THAT THE NUMBER OF COLUMNS OF FIRST MATRIX MUST EQUAL THE NUMBER OF ROWS OF THE SECOND MATRIX
//
//VECTOR(X,Y,Z) = VECTOR(X,Y,Z,1.0)
//THE RESULT OF MATRIX4X4 MULTIPLICATION WITH A VECTOR 4 GIVES OUT ANOTHER VECTOR OF SIZE 4
//
//      MATRIX 	                      VECTOR 4
//[A1,B1,C1,D1]   [V1]    [ A1*V1 + B1*V2 + C1*V3 + D1*V4 ]
//[A2,B2,C2,D2] X [V2] =  [ A2*V1 + B2*V2 + C2*V3 + D2*V4 ]															           ]
//[A3,B3,C3,D3]   [V3]    [ A3*V1 + B3*V2 + C3*V3 + D3*V4 ]     															   ]
//[A4,B4,C4,D4]   [V4]    [ A4*V1 + B4*V2 + C4*V3 + D4*V4 ]  															   ]
//*************************************************
GameMath::Vector3 GameMath::Matrix4x4::operator*(const GameMath::Vector3 &Vector) const
{
float buffer;
float VectorInputBuffer[4]={Vector.x,Vector.y,Vector.z,1.0f};
float VectorOutputBuffer[4]={0.0f};
for(int row=0;row<=3;row++)
{
    buffer=0.0f;
    for(int col=0;col<=3;col++ )
    {
        buffer=Matrix[col][row] * VectorInputBuffer[col] + buffer;
    }
    VectorOutputBuffer[row]=buffer;
}
return GameMath::Vector3(VectorOutputBuffer[0],VectorOutputBuffer[1],VectorOutputBuffer[2]);
}
//*************************************************
//THIS FUNCTION GIVES OUT THE ADDRESS OF THE FIRST ELEMENT OF THE MATRIX
//SO OPENGL CAN READ TEH VALUES AS COLUM MAJOR MATRIX WHEN ITS PASSED ON AS AN ARGUMENT TO OPENGL FUNCTIONS
//*************************************************
float* GameMath::Matrix4x4::Addressof()
{
    return &Matrix[0][0];
}
//*************************************************
//THIS FUNCTION CALCULATES TRANSPOSE OF AN MATRIX BY COVERTING ROWS INTO COLUMNS
//*************************************************
GameMath::Matrix4x4 GameMath::Matrix4x4::Transpose() const
{
GameMath::Matrix4x4 obj;
obj.Matrix[0][0] = this->Matrix[0][0];
obj.Matrix[1][0] = this->Matrix[0][1];
obj.Matrix[2][0] = this->Matrix[0][2];
obj.Matrix[3][0] = this->Matrix[0][3];

obj.Matrix[0][1] = this->Matrix[1][0];
obj.Matrix[1][1] = this->Matrix[1][1];
obj.Matrix[2][1] = this->Matrix[1][2];
obj.Matrix[3][1] = this->Matrix[1][3];

obj.Matrix[0][2] = this->Matrix[2][0];
obj.Matrix[1][2] = this->Matrix[2][1];
obj.Matrix[2][2] = this->Matrix[2][2];
obj.Matrix[3][2] = this->Matrix[2][3];

obj.Matrix[0][3] = this->Matrix[3][0];
obj.Matrix[1][3] = this->Matrix[3][1];
obj.Matrix[2][3] = this->Matrix[3][2];
obj.Matrix[3][3] = this->Matrix[3][3];
return obj;
}
//*************************************************
//THIS FUNCTION CALCULATES THE DETERMINANT OF A 4X4 MATRIX BY USING THE FOLLOWING FORMULAE:
//
//   MATRIX
// [A1,B1,C1,D1]     A1 * DETERMINANT OF [B2,C2,D2]  - B1 * DETERMINANT OF [A2,C2,D2]  +  C1 * DETERMINANT OF [A2,B2,D2]   +   D1 * DETERMINANT OF [A2,B2,C2]
// [A2,B2,C2,D2]  =                      [B3,C3,D3]                        [A3,C3,D3]                         [A3,B3,D3]                           [A3,B3,C3]
// [A3,B3,C3,D3]                         [B4,C4,D4]                        [A4,C4,D4]                         [A4,B4,D4]                           [A4,B4,C4]
// [A4,B4,C4,D4]
//*************************************************
float GameMath::Matrix4x4::Determinant()
{
float D[4];
GameMath::Matrix3x3 *buf;
buf=new GameMath::Matrix3x3(this,0,0);
D[0]=this->Matrix[0][0] * buf->Determinant();

buf=new GameMath::Matrix3x3(this,1,0);
D[1]=this->Matrix[1][0] * buf->Determinant();

buf=new GameMath::Matrix3x3(this,2,0);
D[2]=this->Matrix[2][0] * buf->Determinant();

buf=new GameMath::Matrix3x3(this,3,0);
D[3]=this->Matrix[3][0] * buf->Determinant();

return D[0]-D[1]+D[2]-D[3];
}
//*************************************************
//THIS FUNCTION CALCULATES THE INVERSE OF A MATRIX BY USING THE FOLLOWING PROCEDURE
//1) CREATE A MATRIX OF MINORS BY IGNORING THE VALUES OF THE CURRENT ROW AND COLUMN AND CALCULATING THE DETERMINANT OF THE REMAINING VALUES
//2) CREATE A MATRIX OF COFACTORS BY APPLYING A "CHECKBOARD" OF MINUSES TO THE MATRIX OF MINORS
//3) CREATE ADJUGATE OF MATRIX OF COFACTORS BY TRANSPOSING THE MATRIX
//4) FIND THE DETERMINANT OF THE ORIGINAL 4X4 MATRIX
//5) MUTIPLY THE ADJUGATE BY 1.0 / DETERMINANT
//*************************************************
GameMath::Matrix4x4 GameMath::Matrix4x4::Inverse()
{
Matrix4x4 MatrixofMinors;
Matrix3x3 *buf;
bool toggle=true;

//STEPS 1 , 2 AND  3 ARE BEING PERFORMED HERE
for(int col=0;col< 4;col++)
{
  for(int row=0;row < 4;row++)
  {
   buf=new GameMath::Matrix3x3(this,col,row);
   if(toggle==true)
   MatrixofMinors.Matrix[col][row]=buf->Determinant();
   else
   MatrixofMinors.Matrix[col][row]=-buf->Determinant();
   toggle=!toggle;
  }
}

//THIS IS STEP 4
float MajorDeterminant = this->Matrix[0][0] * MatrixofMinors.Matrix[0][0] + this->Matrix[1][0] * MatrixofMinors.Matrix[1][0] + this->Matrix[2][0] * MatrixofMinors.Matrix[2][0] + this->Matrix[3][0] * MatrixofMinors.Matrix[3][0];

if(MajorDeterminant<=0)
MatrixofMinors = GameMath::Matrix4x4();
else
{
 //THIS IS STEP 3
 MatrixofMinors = MatrixofMinors.Transpose();
 //THIS IS STEP 5
 for (int col=0; col<4; col++)
 {
   for (int row=0; row<4; row++)
   MatrixofMinors.Matrix[col][row] = (1.0f / MajorDeterminant) * MatrixofMinors.Matrix[col][row];
 }
}

return MatrixofMinors;
}



