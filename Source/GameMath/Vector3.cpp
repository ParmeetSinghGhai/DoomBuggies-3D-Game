#include <GameMath/GameMath.h>
int GameMath::Vector3::PrintWidth;
float GameMath::Vector3::EPSILON=0.001f;
//*************************************************
//DEFAULT CONSTRUCTOR: INITILIAZES THE VALUE OF THE VECTOR X , Y AND Z COMPONENT TO 1.0
//*************************************************
GameMath::Vector3::Vector3()
{
    x=1.0f;y=1.0f;z=1.0f;
}
//*************************************************
//CONSTRUCTOR: INITILIAZES THE VALUE OF THE VECTOR X, Y AND Z COMPONENT OF THE VECTOR TO FUNCTION ARGUMENTS X , Y AND Z RESPECTIVELY
//*************************************************
GameMath::Vector3::Vector3(const float x, const float y, const float z)
{
    this->x=x;this->y=y;this->z=z;
}
//*************************************************
//CALCULATES THE LENGTH OF THE VECTOR USING THE FORMULA, LENGTH = SQUAREROOT(X^2 + Y^2 + Z^2)
//*************************************************
const float GameMath::Vector3::Length() const
{
    return sqrt(x * x + y * y + z * z);
}
//*************************************************
//YOU NORMALIZE A VECTOR BY DIVING EACH COMPONENT OF THE VECTOR BY ITS LENGTH
//SO THE LENGTH OF THE VECTOR AFTER NORMALIZATION IS 1.0
//*************************************************
GameMath::Vector3 GameMath::Vector3::Normalize()
{
    const float Len=Length();
    return GameMath::Vector3(x / Len,y / Len,z / Len);
}
//*************************************************
//THIS FUNCTION ADDS THE FUNCTION ARGUMENT TO EACH COMPONENT OF THE VECTOR INDIVIDUALLY
//*************************************************
GameMath::Vector3 GameMath::Vector3::operator +(float scalar) const
{
    return GameMath::Vector3(x+scalar,y+scalar,z+scalar);
}
//*************************************************
//THIS FUNCTION ADDS TWO VECTORS COMPONENT WISE
//*************************************************
GameMath::Vector3 GameMath::Vector3::operator +(const GameMath::Vector3 vectorB) const
{
    return GameMath::Vector3(x + vectorB.x,y + vectorB.y,z + vectorB.z);
}
//*************************************************
//THIS FUNCTION NEGATES EACH COMPONENT OF THE VECTOR
//*************************************************
GameMath::Vector3 GameMath::Vector3::operator -() const
{
    return GameMath::Vector3(-x,-y,-z);
}
//*************************************************
//THIS FUNCTION SUBTRACTS THE FUNCTION ARGUMENT FROM EACH COMPONENT OF THE VECTOR INDIVIDUALLY
//*************************************************
GameMath::Vector3 GameMath::Vector3::operator -(float scalar) const
{
    return GameMath::Vector3(x-scalar,y-scalar,z-scalar);
}
//*************************************************
//THIS FUNCTION SUBTRACTS TWO VECTORS COMPONENT WISE
//*************************************************
GameMath::Vector3  GameMath::Vector3::operator -(const GameMath::Vector3 vectorB) const
{
    return GameMath::Vector3(x - vectorB.x,y - vectorB.y,z - vectorB.z);
}
//*************************************************
//THIS FUNCTION MULTIPLYIES THE FUNCTION ARGUMENT WITH EACH COMPONENT OF THE VECTOR INDIVIDUALLY
//*************************************************
GameMath::Vector3 GameMath::Vector3::operator *(const float scalar) const
{
    return GameMath::Vector3(x * scalar,y * scalar,z * scalar);
}
//*************************************************
//THE FABS() FUCNTIONS GET THE ABSOLUTE VALUE OF THE ARGUMENT. E.G - FABS(-5.6) = 5.6
//THIS FUNCTON COMPARES IF TWO VECTORS ARE THE SAME BY COMPARING THE FLOATING VALUES OF THE TWO VECTOS COMPOMENT WISE
//SINCE THIS IS A FLOATING POINT COMPARSION, THE FINAL VALUE IS COMPARED TO AN EPSILON WHICH IS THE SMALLEST VALUE USED FOR COMPARSION
//
// EG. VECTOR1.X = 5.03 , VECTOR2.X=5.02 AND EPSILON = 0.1
// FABS(VECTOR1.X - VECTOR2.X) = 0.01
// 0.01 < EPSILON(0.1) = TRUE,  HERE THE TWO VECTORS WOULD BE CONSIDERED TO BE TRUE IF THE SAME HAPPENS WITH Y AND Z COMPONENTS

// EG. VECTOR1.X = 5.3 , VECTOR2.X=5.2 AND EPSILON = 0.1
// FABS(VECTOR1.X - VECTOR2.X) = 0.1
// 0.1 < EPSILON(0.1) = FALSE,  HERE THE TWO VECTORS WOULD BE CONSIDERED TO BE FALSE REGARDLESS OF WHAT HAPPENS WITH Y AND Z COMPONENTS
//*************************************************
bool GameMath::Vector3::operator ==(const GameMath::Vector3 vectorB) const
{
    if(fabs(x - vectorB.x) < EPSILON && fabs(x - vectorB.y) < EPSILON  && fabs(x - vectorB.z) < EPSILON)
    return true;
    else
    return false;
}
//*************************************************
//THE VECTOR CROSS PRODUCT OF TWO VECTORS IS ANOTHER VECTOR THAT IS PERPENDICULAR TO BOTH THE VECTORS
//IF VECTOR1= X1,Y1,Z2 AND VECTOR2=X2,Y2,Z2 THEN THEIR CROSS PRODUCT IS VECTOR3 WHERE
// X3= (Y1 * Z2) - (Z1 * Y2)
// Y3= (Z1 * X2) - (X1 * Z2)
// Z3= (X1 * Y2) - (Y1 * X2)
// VECTORS USED IN THIS FUNCTION ARE NEEDED TO BE UNIT VECTORS
//*************************************************
GameMath::Vector3 GameMath::Vector3::CrossProduct(const GameMath::Vector3 vectorB) const
{
    return GameMath::Vector3(y * vectorB.z - z * vectorB.y,z * vectorB.x - x * vectorB.z,x * vectorB.y - y * vectorB.x);
}
//*************************************************
//THE VECTOR DOT PRODUCT GIVES OUT THE PROJECTION OF ONE VECTOR ONTO THE OTHER VECTOR WHERE THE RESULT IS A SCALAR SPECIFYING THE LENGTH
//OF THE PROJECTION OF VECTOR ONE ONTO VECTOR TWO
//THE GENERAL FORMULAE FOR DOT PRODUCT BETWEEN TWO VECTORS A AND B ARE:
//A.B = LENGTH(A) * LENGTH(B) * COS(ANGLE BETWEEN A AND B) WHERE A.B=(A.X * B.X) + (A.Y * B.Y) + (A.Z*B.Z)
//
//IF A AND B ARE UNIT VECTORS WHEN THIS FORMULAE IS FURTHER SIMPLIFIED TO :
//A.B =COS(ANGLE BETWEEN A AND B) WHERE A.B=(A.X * B.X) + (A.Y * B.Y) + (A.Z*B.Z)
//*************************************************
float GameMath::Vector3::DotProduct(const GameMath::Vector3 vectorB) const
{
    return x * vectorB.x + y * vectorB.y + z * vectorB.z;
}
//*************************************************
//THE BELOW TWO FUNCTIONS CALCULATE THE ANGLE BETWEEN THE TWO VECTORS BY COMPUTING THE DOT PRODUCT OF THE TWO VECTORS
//AND THEN PASSING IT ON TO THE INVERSE COSINE FUNCTION TO THE GET THE ANGLE BETWEEN THE TWO VECTORS IN RADIANS
//FORMULAE USED IS :
//ANGLE = INVERSE COSINE(A.B / LENGTH(A) * LENGTH(B)) , WHERE A.B=(A.X * B.X) + (A.Y * B.Y) + (A.Z*B.Z)
//*************************************************
float GameMath::Vector3::FindAngle(const GameMath::Vector3 vectorB) const
{
    return acos(this->DotProduct(vectorB)/(this->Length() * vectorB.Length()));
}
float GameMath::Vector3::FindAngle2(const GameMath::Vector3 vectorB) const
{
    return acos(x * vectorB.x + y * vectorB.y + z * vectorB.z);
}
//*************************************************
//OVRLOADED [] OPERATOR:
//0 INPUT TO THE [] OPERATORS OUTPUTS THE ADDRESS OF X COMPONENT OF THE VECTOR
//1 INPUT OUTPUTS THE ADDRESS OF THE Y COMPONENT OF THE VECTOR
//2 INPUT OUTPUTS THE ADDRESS OF THE Z COMPONENT OF THE VECTOR
//*************************************************
float& GameMath::Vector3::operator [](const int Index)
{
    switch(Index)
    {
    case 0:
        return this->x;
        break;
    case 1:
        return this->y;
        break;
    case 2:
        return this->z;
        break;
    default:
        {
          float* dummy=nullptr;
          return *dummy;
        }
        break;
    }
}

