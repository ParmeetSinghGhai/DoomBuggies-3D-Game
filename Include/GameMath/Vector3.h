#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED
class GameMath::Vector3
{
public:
float x;
float y;
float z;

static int PrintWidth;
static float EPSILON;

Vector3();
Vector3(const float x, const float y, const float z);

const float Length() const;
GameMath::Vector3 Normalize();

GameMath::Vector3 operator +(float scalar) const;
GameMath::Vector3 operator +(const GameMath::Vector3 vectorB) const;

GameMath::Vector3 operator -() const;
GameMath::Vector3 operator -(float scalar) const;
GameMath::Vector3 operator -(const GameMath::Vector3 vectorB) const;

GameMath::Vector3 operator *(const float scalar) const;
bool operator ==(const GameMath::Vector3 vectorB) const;

GameMath::Vector3 CrossProduct(const GameMath::Vector3 vectorB) const;
float DotProduct(const GameMath::Vector3 vectorB) const;

float FindAngle(const GameMath::Vector3 vectorB) const;
float FindAngle2(const GameMath::Vector3 vectorB) const;

float& operator [](const int Index);

//*************************************************
//THIS IS A FRIEND FUNCTION THAT IS NOT A MEMBER OF THIS CLASS YET IT CAN STILL ACCESS THE MEMBERS OF THE CLASS
//OVERLOADED OPERATOR << ACCEPTS TWO ARGUMENTS : THE FIRST IS THE OUTPUT STREAM AND THE SECOND IS THE OBJECT THAT IS NEEDED TO BE OUTPUT
//WHICH IS WHY THIS OVERLOADED OPERATOR << FUNCTION WAS DECLARED AS A FRIEND FUNCTION SO IT CAN ACCEPT THIS CLASS'S OBJECT AS THE SECOND ARGUMENT
//
//OTHERWISE IF THIS WAS A MEMBER FUNCTION THEN A SECOND ARGUMENT WOULD HAVE TO BE PROVIDED TO THIS FUNCTION WHICH WAS NOT REALLY REQUIRED
//HENCE CREATING THIS UNNECCESSARY NEAD TO PROVIDE A SECOND UNNEEDED ARGUMENT
//*************************************************
friend std::ostream& operator <<(std::ostream &out,GameMath::Vector3 &vectorB)
{
out<<std::fixed<<"[X="<<std::setprecision(2)<<vectorB.x<<",Y="<<std::setw(PrintWidth)<<std::setprecision(2)<<vectorB.y<<",Z="<<std::setw(PrintWidth)<<std::setprecision(2)<<vectorB.z<<"]"<<std::endl;
return out;
}
};
#endif
