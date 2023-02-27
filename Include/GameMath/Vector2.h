#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED
class GameMath::Vector2
{
public:
float x;
float y;
static int PrintWidth;
Vector2();
Vector2(const float x, const float y);

float& operator [](const int Index);

//*************************************************
//THIS IS A FRIEND FUNCTION THAT IS NOT A MEMBER OF THIS CLASS YET IT CAN STILL ACCESS THE MEMBERS OF THE CLASS
//OVERLOADED OPERATOR << ACCEPTS TWO ARGUMENTS : THE FIRST IS THE OUTPUT STREAM AND THE SECOND IS THE OBJECT THAT IS NEEDED TO BE OUTPUT
//WHICH IS WHY THIS OVERLOADED OPERATOR << FUNCTION WAS DECLARED AS A FRIEND FUNCTION SO IT CAN ACCEPT THIS CLASS'S OBJECT AS THE SECOND ARGUMENT
//
//OTHERWISE IF THIS WAS A MEMBER FUNCTION THEN A SECOND ARGUMENT WOULD HAVE TO BE PROVIDED TO THIS FUNCTION WHICH WAS NOT REALLY REQUIRED
//HENCE CREATING THIS UNNECCESSARY NEAD TO PROVIDE A SECOND UNNEEDED ARGUMENT
//*************************************************
friend std::ostream& operator <<(std::ostream &out,GameMath::Vector2 &vectorB)
{
out<<"[X="<<vectorB.x<<",Y="<<std::setw(PrintWidth)<<vectorB.y<<"]"<<std::endl;
return out;
}
};
#endif
