#include <GameMath/GameMath.h>
int GameMath::Vector2::PrintWidth;
//*************************************************
//DEFAULT CONSTRUCTOR: INITILIAZES THE VALUE OF THE VECTOR X COMPONENT AND Y COMPONENT TO 1.0
//*************************************************
GameMath::Vector2::Vector2()
{
x=1.0f;
y=1.0f;
}
//*************************************************
//CONSTRUCTOR: INITILIAZES THE VALUE OF THE VECTOR X COMPONENT AND Y COMPONENT TO FUNCTION ARGUMENTS X AND Y RESPECTIVELY
//*************************************************
GameMath::Vector2::Vector2(const float x, const float y)
{
this->x=x;
this->y=y;
}
//*************************************************
//OVRLOADED [] OPERATOR: 0 INPUT TO THE [] OPERATORS OUTPUTS THE ADDRESS OF X COMPONENT OF THE VECTOR
//AND 1 INPUT OUTPUTS THE ADDRESS OF THE Y COMPONENT OF THE VECTOR
//*************************************************
float& GameMath::Vector2::operator [](const int Index)
{
    switch(Index)
    {
    case 0:
        return this->x;
        break;
    case 1:
        return this->y;
        break;
    default:
        {
          float* dummy=nullptr;
          return *dummy;
        }

    }
}


