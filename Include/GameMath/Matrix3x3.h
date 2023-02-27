#ifndef MATRIX3X3_H_INCLUDED
#define MATRIX3X3_H_INCLUDED
class GameMath::Matrix3x3
{
public:
float Matrix[3][3];
static int PrintWidth;
Matrix3x3();
Matrix3x3(GameMath::Matrix4x4 *matrix, int Excludedcol, int Excludedrow);
float Determinant();

//*************************************************
//THIS IS A FRIEND FUNCTION THAT IS NOT A MEMBER OF THIS CLASS YET IT CAN STILL ACCESS THE MEMBERS OF THE CLASS
//OVERLOADED OPERATOR << ACCEPTS TWO ARGUMENTS : THE FIRST IS THE OUTPUT STREAM AND THE SECOND IS THE OBJECT THAT IS NEEDED TO BE OUTPUT
//WHICH IS WHY THIS OVERLOADED OPERATOR << FUNCTION WAS DECLARED AS A FRIEND FUNCTION SO IT CAN ACCEPT THIS CLASS'S OBJECT AS THE SECOND ARGUMENT
//
//OTHERWISE IF THIS WAS A MEMBER FUNCTION THEN A SECOND ARGUMENT WOULD HAVE TO BE PROVIDED TO THIS FUNCTION WHICH WAS NOT REALLY REQUIRED
//HENCE CREATING THIS UNNECCESSARY NEAD TO PROVIDE A SECOND UNNEEDED ARGUMENT
//*************************************************
friend std::ostream& operator <<(std::ostream &out,GameMath::Matrix3x3 &Mat)
{
out<<"MATRIX 3x3:"<<std::endl;
out<<"          ["<<Mat.Matrix[0][0]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][0]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][0]<<"]"<<std::endl;
out<<"          ["<<Mat.Matrix[0][1]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][1]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][1]<<"]"<<std::endl;
out<<"          ["<<Mat.Matrix[0][2]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][2]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][2]<<"]"<<std::endl;
}
};
#endif // MATRIX3X3_H_INCLUDED
