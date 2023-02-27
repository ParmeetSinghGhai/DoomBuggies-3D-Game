#ifndef MATRIX4X4_H_INCLUDED
#define MATRIX4X4_H_INCLUDED
class GameBoundingBox;
class GameMath::Matrix4x4
{
public:
float Matrix[4][4];

static int PrintWidth;

Matrix4x4();
Matrix4x4(float *input);
void Clean();

GameMath::Matrix4x4 operator *(const Matrix4x4 &RHS)const;
GameMath::Vector3 operator *(const GameMath::Vector3 &Vector) const;

float* Addressof();
float Determinant();
GameMath::Matrix4x4 Transpose() const;
GameMath::Matrix4x4 Inverse();

//*************************************************
//THIS IS A FRIEND FUNCTION THAT IS NOT A MEMBER OF THIS CLASS YET IT CAN STILL ACCESS THE MEMBERS OF THE CLASS
//OVERLOADED OPERATOR << ACCEPTS TWO ARGUMENTS : THE FIRST IS THE OUTPUT STREAM AND THE SECOND IS THE OBJECT THAT IS NEEDED TO BE OUTPUT
//WHICH IS WHY THIS OVERLOADED OPERATOR << FUNCTION WAS DECLARED AS A FRIEND FUNCTION SO IT CAN ACCEPT THIS CLASS'S OBJECT AS THE SECOND ARGUMENT
//
//OTHERWISE IF THIS WAS A MEMBER FUNCTION THEN A SECOND ARGUMENT WOULD HAVE TO BE PROVIDED TO THIS FUNCTION WHICH WAS NOT REALLY REQUIRED
//HENCE CREATING THIS UNNECCESSARY NEAD TO PROVIDE A SECOND UNNEEDED ARGUMENT
//*************************************************
friend std::ostream& operator <<(std::ostream &out,GameMath::Matrix4x4 &Mat)
{
out<<"MATRIX 4x4:"<<std::endl;
out<<"           ["<<Mat.Matrix[0][0]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][0]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][0]<<","<<std::setw(PrintWidth)<<Mat.Matrix[3][0]<<"]"<<std::endl;
out<<"           ["<<Mat.Matrix[0][1]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][1]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][1]<<","<<std::setw(PrintWidth)<<Mat.Matrix[3][1]<<"]"<<std::endl;
out<<"           ["<<Mat.Matrix[0][2]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][2]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][2]<<","<<std::setw(PrintWidth)<<Mat.Matrix[3][2]<<"]"<<std::endl;
out<<"           ["<<Mat.Matrix[0][3]<<","<<std::setw(PrintWidth)<<Mat.Matrix[1][3]<<","<<std::setw(PrintWidth)<<Mat.Matrix[2][3]<<","<<std::setw(PrintWidth)<<Mat.Matrix[3][3]<<"]"<<std::endl;
return out;
}
};
#endif // MATRIX4X4_H_INCLUDED
