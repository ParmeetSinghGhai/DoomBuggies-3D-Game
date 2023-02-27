#include <GameMath/GameMath.h>
//*************************************************
//THIS FUNCTION CONVERTS ANGLE IN RADIANS TO ANGLE IN DEGREES USING THE FORMULAE:
//ANGLE IN DEGREES = ANGLE IN RADIANS * (180 / PI) WHERE PI=3.14159
//*************************************************
float GameMath::Transform::GetDegree(const float &AngleinRadian)
{
	return (AngleinRadian * 180.0f) / PI;
}
//*************************************************
//THIS FUNCTION CONVERTS ANGLE IN DEGREES TO ANGLE IN RADIANS USING THE FORMULAE:
//ANGLE IN RADIANS = ANGLE IN DEGREES * (PI / 180) WHERE PI=3.14159
//*************************************************
float GameMath::Transform::GetRadian(const float &AngleinDegree)
{
	return (AngleinDegree * PI) / 180.0f;
}
//*************************************************
//THIS FUNCTION COPIES THE VALUE OF ALL THE CELLS OF A MATRIX, COLUMN WISE, IN AN ARRAY OF FLOATING POINT TYPE
//IF A NULL POINTER IS PROVIDED AS AN ADDRESS TO THE MATRIX THEN AN IDENTITY MATRIX IS GENERATED AND ITS VALUES ARE COPIED OVER TO THE ARRAY
//*************************************************
void GameMath::Transform::CopyMatrixtoArray(GameMath::Matrix4x4 *Mat,float *output)
{
    if(Mat==nullptr)
    Mat=new GameMath::Matrix4x4();

    *(output++)=Mat->Matrix[0][0];
    *(output++)=Mat->Matrix[0][1];
    *(output++)=Mat->Matrix[0][2];
    *(output++)=Mat->Matrix[0][3];

    *(output++)=Mat->Matrix[1][0];
    *(output++)=Mat->Matrix[1][1];
    *(output++)=Mat->Matrix[1][2];
    *(output++)=Mat->Matrix[1][3];

    *(output++)=Mat->Matrix[2][0];
    *(output++)=Mat->Matrix[2][1];
    *(output++)=Mat->Matrix[2][2];
    *(output++)=Mat->Matrix[2][3];

    *(output++)=Mat->Matrix[3][0];
    *(output++)=Mat->Matrix[3][1];
    *(output++)=Mat->Matrix[3][2];
    *(output++)=Mat->Matrix[3][3];
}
//*************************************************
//THE SCALE TRANSFORMATION BASICALLY MULTIPLES A VECTOR BY A SCALAR VALUE. THE MATRIX USED FOR THIS TRANSFORMATION
//HAS THE SCALAR VALUE SET AT THE FIRST 3 VALUES OF THE DIAGNOL OF THE MATRIX WITH THE 4TH VALUE OF THE DIAGNOL SET TO 1.0 LIKE SHOWN BELOW:
//[S1 ,0  ,0  , 0]     [X]    [S1 * X]
//[0  ,S1 ,0  , 0]  X  [Y]  = [S2 * Y]
//[0  ,0  ,S1 , 0]     [Z]    [S3 * Z]
//[0  ,0  ,0  , 1]     [1]    [   1  ]
//*************************************************
GameMath::Matrix4x4 GameMath::Transform::Scale(const GameMath::Vector3 &ScaleVector)
{
	GameMath::Matrix4x4 output;
	output.Matrix[0][0] = float(ScaleVector.x);
	output.Matrix[1][1] = float(ScaleVector.y);
	output.Matrix[2][2] = float(ScaleVector.z);
	return output;
}
GameMath::Matrix4x4 GameMath::Transform::Scale(const float &x,const float &y,const float &z)
{
	GameMath::Matrix4x4 output;
	output.Matrix[0][0] = float(x);
	output.Matrix[1][1] = float(y);
	output.Matrix[2][2] = float(z);
	return output;
}
//*************************************************
//THE TRANSLATION TRANSFORMATION BASICALLY ADDS SCALAR VALUES TO INDIVIDUAL VECTORS COMPONENTS. THE MATRIX USED FOR THIS TRANSFORMATION
//IS AN IDENTITY MATRIX AND IT HAS THE SCALAR VALUE SET AT THE FIRST 3 VALUES OF THE LAST COLUMN OF THE MATRIX
//WITH THE 4TH VALUE OF THE LAST COLUMN SET TO 1.0 LIKE SHOWN BELOW:
//
//[1  ,0  ,0  , T1]     [X]    [T1 + X]
//[0  ,1  ,0  , T2]  X  [Y]  = [T2 + Y]
//[0  ,0  ,1  , T3]     [Z]    [T3 + Z]
//[0  ,0  ,0  , 1 ]     [1]    [   1  ]
//*************************************************
GameMath::Matrix4x4 GameMath::Transform::Translate(const GameMath::Vector3 &TranslateVector)
{
	GameMath::Matrix4x4 output;
	output.Matrix[3][0] = float(TranslateVector.x);
	output.Matrix[3][1] = float(TranslateVector.y);
	output.Matrix[3][2] = float(TranslateVector.z);
	return output;
}
GameMath::Matrix4x4 GameMath::Transform::Translate(const float &x,const float &y,const float &z)
{
	GameMath::Matrix4x4 output;
	output.Matrix[3][0] = float(x);
	output.Matrix[3][1] = float(y);
	output.Matrix[3][2] = float(z);
	return output;
}
/*************************************************
    +y axis
      |   rotated vector(x',y')
      |    |
      |   |    || original vector(x,y)
      |  |   ||
      | |  ||
      ||_||________ +x axis
     /
    /
   /
 +z axis

OPENGL FOLLOWS THE RIGHT HAND RULE EVEN IT COMES TO ITS COORDINATE SYSTEM. IN THIS SYSTEM,
THE POSITIVE Z AXIS IS ALWAYS FACING YOU AND IT GOING TOWARDS THE SCREEN
THE POSITIVE Y AXIS IS ALWAYS FACING UP
THE POSITIVE X AXIS IS ALWAYS FACING TO THE RIGHT

WITH ROTATION TRANSFORMATION, THE LENGTH OF THE ROTATED VECTOR REMAINS THE SAME. IN THE ABOVE EXAMPLE, THE ROTATION IS BEING TAKEN PLACE ALONG THE POSITIVE
Z AXIS BY AN ANGLE (THETA). THE DERIVATION OF THE ROTATION MATRIX IS AS FOLLOWS:
LET THE ORIGINAL ANGLE THAT THE ORIGINAL VECTOR MAKES WITH THE POSITIVE X AXIS BE (PHI) AND THE VECTOR ITSELF BE DENOTED AS (R).

FOR THE ORIGINAL VECTOR
X=RCOS(PHI)---------(1)
Y=RSIN(PHI)---------(2)

FOR THE ROTATED VECTOR
X'=RCOS(THETA + PHI)
Y'=RSIN(THETA + PHI)

X'=RCOS(THETA)COS(PHI) - RSIN(THETA)SIN(PHI)
X'=COS(THETA) * RCOS(PHI) - SIN(THETA) * RSIN(PHI)
FROM (1) AND (2)
X'=XCOS(THETA) - YSIN(THETA)---------(3)

SIMILARLY
Y'=RSIN(THETA)COS(PHI) - RCOS(THETA)SIN(PHI)
Y'=SIN(THETA)* RCOS(PHI) - COS(THETA) * RSIN(PHI)
FROM (1) AND (2)
Y'=XSIN(THETA) + YCOS(THETA)---------(4)

APPLYING THE VALUES IN (3) AND (4) IN MATRIX FORM, WE GET THE 3X3 ROTATION MATRIX FOR ROTATION ALONG THE POSITIVE Z AXIS

[COS(THETA) -SIN(THETA)  0]
[SIN(THETA)  COS(THETA)  0]
[0             0         1]

SIMILARLY,

THE 3X3 ROTATION MATRIX FOR ROTATION ALONG THE POSITIVE X AXIS IS

[1       0           0    ]
[0  COS(THETA) -SIN(THETA)]
[0  SIN(THETA)  COS(THETA)]

THE 3X3 ROTATION MATRIX FOR ROTATION ALONG THE POSITIVE Y AXIS IS

[COS(THETA)  0  SIN(THETA)]
[ 0          1        0   ]
[-SIN(THETA) 0  COS(THETA)]
*************************************************/
GameMath::Matrix4x4 GameMath::Transform::Rotate(const char &Axis, const float &AngleOfRotationinDegrees)
{
	GameMath::Matrix4x4 output;
	const float CosTheta = cos(GameMath::Transform::GetRadian(AngleOfRotationinDegrees));
    const float SinTheta=sin(GameMath::Transform::GetRadian(AngleOfRotationinDegrees));

	if (Axis == 'x' || Axis == 'X')
	{
		output.Matrix[1][1] = CosTheta;
		output.Matrix[1][2] = SinTheta;
		output.Matrix[2][1] = -SinTheta;
		output.Matrix[2][2] = CosTheta;
	}
	else if (Axis == 'y' || Axis == 'Y')
	{
		output.Matrix[0][0] = CosTheta;
		output.Matrix[0][2] = -SinTheta;
		output.Matrix[2][0] = SinTheta;
		output.Matrix[2][2] = CosTheta;
	}
	else if (Axis == 'z' || Axis == 'Z')
	{
		output.Matrix[0][0] = CosTheta;
		output.Matrix[0][1] = SinTheta;
		output.Matrix[1][0] = -SinTheta;
		output.Matrix[1][1] = CosTheta;
	}
	return output;
}
/**************************************************
QUATERNIONS ARE 4 DIMENSIONAL EXTENSION TO COMPLEX NUMBERS IN SOME WAYS
COMPLEX NUMBERS ARE EXPRESSED AS ONE REAL PART AND ONE IMAGINARY PART LIKE SO : (A + Bi)
QUATERNIONS ARE EXPRESSED AS ONE REAL PART AND 3 IMAGINARY PARTS LIKE SO : (A + Bi + Cj + Dk)
HERE A IS THE REAL PART
AND (Bi + Cj + Dk) CAN BE CONSIDERED AS A VECTOR

QUATERNION MULTIPLICATION INVOLVES DISTRIBUTION OF ONE TERM WITH THE REST OF THE TERMS AND DOING THE SAME WITH OTHER TERMS ALONG
WITH FOLLOWING THE QUATERNION MULTIPLICATIVE FORMULAE:

i * i = -1
j * j = -1
k * k = -1
i * j = k
j * k = i
k * i = j
i * j * k = -1

q1= a + bi + cj + dk
q2= e + fi + gj + hk
q1 * q2 = ae + afi + agj + ahk
          bei + bfi*i + bgij + bhik
          cej + cfji  + cgj*j + chjk
	  dek + dfki  + dgkj  + dhk*k

APPLYING THE ABOVE MULTIPLICATIVE FORMULAES WE GET:
q1 * q2 = ae  + afi  + agj  + ahk +
          bei - bf   + bgK  - bhj +
          cej - cfk  - cg   + chi +
	  dek + dfj  - dgi  - dh

q1 * q2 = (ae - bf - cg - dh) + i(af + be + ch - dg) + j(ag - bh + ce + df) + k(ah + bg - cf + de)

QUATERNION MULTIPLICATION IS NON-COMMUNITATIVE BUT IT IS DISTRIBUTIVE
(q1 * q2) is not equal to (q2 * q1)
(q1q2)q3=q1(q2q3)

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EXTRACTING DOT PRODUCT AND CROSS PRODUCT FROM THE ABOVE QUATERNION MULTIPLICATION
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
(a + bi + cj + dk) * (e + fi + gj + hk) = (ae - bf - cg - dh),
                                          (af + be + ch - dg),
                                          (ag - bh + ce + df),
                                          (ah + bg - cf + de) ------------------(1)

bi + cj + dk CAN BE WRITTEN AS VECTOR X AND fi + gj + hk CAN BE WRITTEN AS VECTOR Y

X=bi + cj + dk
Y=fi + gj + hk

REWRITING (1) ABOVE
(a , X) * (e , Y) = (ae -(bf + cg + dh)),
                    (be + af + ch - dg) ,
                    (ce + df + ag - bh) ,
                    (de - cf + bg + ah)

HERE (bf + cg + dh) IS THE DOT PRODUCT OF THE TWO VECTORS X AND Y
AFTER COMBINING ALL THE e COMPONENTS (be,ce,de) AND THE a COMPONENT TERMS (af,ag,ah) , THE REMAING TERMS (ch - dg, df- bh, bg - cf) FORM THE CROSS PRODUCT OF THE TWO VECTORS
WHICH ALLOW THE QUATERNION MULTIPLICATION TO BE WRITTEN AS DOT AND CROSS PRODUCT OF TWO VECTORS

(a , X) * (e , Y) = (ae - X.DOT(Y), eX + aY + X.CROSS(Y))-----------------------(2)

NOW IF WE MULTIPLY TWO PURE QUATERNIONS WHERE THE REAL PART IS 0 AND THE IMAGINARY PARTS ARE VECTORS THEN THE
MULTIPLICATION OF THOSE TWO QUATERNIONS WILL RESULT INTO THE DOT AND CROSS PRODUCT OF THOSE TWO VECTORS

(0,V1) * (0,V2) = (0 - V1.DOT(V2),0 * V1 + 0 * V2 + V1.CROSS(V2))
(0,V1) * (0,V2) = (-V1.DOT(V2),V1.CROSS(V2))
SCALAR PART IS (-V1.DOT(V2)
VECTOR PART IS (V1.CROSS(V2)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RODRIGUEZ'S ROTATION FORMULAE FOR SPECIAL CASE WHEN VECTOR BEING ROTATED IS PERPENDICUALR TO ROTATION AXIS
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


       n vector(rotation axis)
         *
         *     * n x v(cross product of n and v vector)
         *    *
         *   *
         *  *
         * *
         * *  * * * * V' (rotated vector)
           *
             *
               *
                 * V(original vector)

HERE THE N VECTOR(ROTATION AXIS) AND V (ORIGINAL VECTOR) ARE PERPENDICULAR TO EACH OTHER
N X V IS THE CROSS PRODUCT OF THE N AND THE V VECTORS AND V' IS THE ROTATED VECTOR.
THE N X V VECTOR, THE V' ROTATED VECTOR AND THE V ORIGINAL VECTOR ARE ALL THE SAME PLANE WHILE N VECTOR IS PERPENDICULAR TO ALL OF THEM

WE CAN SPLIT THE V' ROTATED VECTOR INTO ITS V(PERP) PERPERDICULAR COMPONENTS AND V(PARA) PARALLEL COMPONENT
WHRE THE PARALLEL COMPOMENT IS THE ONE ALONG WITH THE ORIGINAL V VECTOR AND THE PERPENDICULAR COMPONENT IS ALONG THE N X V VECTOR
V(PARA) PARALLEL COMPONENT IS THE PROJECTION OF V' ROTATED VECTOR ON THE ORIGINAL VECTOR V = COS(THETA) * V
V(PERP) PERPENDICULAR COMPONENT IS THE PROJECTION OF THE V' ROTATED VECTOR ON THE N X V VECTOR = SIN(THETA) * (N X V)

ROTATED VECTOR V' = V(PARA) + V(PERP)
                  = COS(THETA) * V + SIN(THETA) (N X V) ---------------------------- (3)

WRITING DOWN THESE VECTORS INTO QUATERNION FORM WE GET
V(QUATERION)  = (0,V) WHERE 0 IS THE SCALAR PART AND V IS THE ORIGINAL VECTOR
V'(QUATERION) = (0,V') WHERE 0 IS THE SCALAR PART AND V' IS THE ROTATED VECTOR
N(QUATERION)  = (0,N) WHERE 0 IS THE SCALAR PART AND N IS ROTATION AXIS

N(QUATERION) * V(QUATERION) = (-N.DOT(V),N.CROSS(V))
SINCE N AND V ARE PERPENDICUALR TO EACH OTHER THEIR DOT PRODUCT IS ZERO
N(QUATERION) * V(QUATERION) = (0,N X V)--------------------------------------------- (4)

USING (4) IN (3) ABOVE, WE GET THE (3) IN QUATERNION FORM AS SHOWN BELOW

V'(QUATERION) = COS(THETA) * V(QUATERNION) + SIN(THETA) (N(QUATERION) * V(QUATERION))
              = COS(THETA) * V(QUATERNION) + SIN(THETA) (N(QUATERION) + SIN(THETA) V(QUATERION)
              = (COS(THETA) + SIN(THETA) (N(QUATERION)) * V(QUATERION)
              = e^(THETA)N(QUATERNION) * V(QUATERION) , WHERE e^(THETA)N(QUATERNION) = (COS(THETA) + SIN(THETA) (N(QUATERION))
              = e^(THETA)N(QUATERNION) * V(QUATERION)--------------------------------(5)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RODRIGUEZ'S ROTATION FORMULAE FOR GENERAL CASE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  n vector(rotation axis)
         *
         *         (original vector v)
parallel **********
component*      * *
of       *     *  *
vector v *    *   *
         *   *    *(perpendicular component of the vector v)
         *  *   *
         * *  *
         ** *
         **

HERE WE CAN SAY THAT THE ORIGINAL VECTOR V IS THE SUM OF THE PARALLEL COMPONENT AND THE PERPENDICULAR COMPONENT

V = V(PARA) + V(PERP)---------------------------------------------------------------(6)

V'(ROTATED VECTOR)= V'(ROTATED PARA) + V'(ROTATED PERP)
HOWEVER WITH ROTATION THE PARALLEL COMPONENT WONT BE AFFECTED SINCE ITS PARALLEL TO THE ROTATION AXIS ITSELF SO THE
ROTATED PARALLEL COMPONENT OF THE VECTOR WILL REMAIN THE SAME AND ONLY THE PERPENDICULAR COMPONENT OF THE VECTOR WILL BE ROTATED

V'(ROTATED VECTOR)= V(PARA) + V'(ROTATED PERP)
USING (3) ABOVE WE CAN SAY THAT V'(ROTATED PERP)= COS(THETA) * V(PERP) + SIN(THETA) * (N X V(PERP))

V'(ROTATED VECTOR)= V(PARA) + COS(THETA) * V(PERP) + SIN(THETA) * (N X V(PERP))

USING (5) ABOVE WE CAN SAY THAT V(PERP)= V - V(PARA)
AND N X V = N X (V(PARA) + V(PERP))
          = N X V(PARA) + N X V(PERP)
          SINCE N AND V(PARA) ARE PARELLEL TO EACH OTHER THEIR DOT PRODUCT IS ZERO
          = 0 + N X V(PERP)
    N X V = N X V(PERP)

V'(ROTATED VECTOR)= V(PARA) + COS(THETA) *  (V - V(PARA)) + SIN(THETA) * (N X V)
                  = V(PARA) + COS(THETA) * V - COS(THETA) * V(PARA) + SIN(THETA) * (N X V)
		  = (1 - COS(THETA)) * V(PARA) + COS(THETA) * V + SIN(THETA) * (N X V)
                  V(PARA)= (PROJECTION OF V VECTOR ON THE N VECTOR) * N VECTOR = (V.DOT(N)) * N
                  = (1 - COS(THETA)) * (V.DOT(N)) * N + COS(THETA) * V + SIN(THETA) * (N X V)

RODRIGUEZ'S ROTATION FORMULAE FOR GENERAL CASE = (1 - COS(THETA)) * (V.DOT(N)) * N + COS(THETA) * V + SIN(THETA) * (N X V)------(7)
CONVERTING THE FORMULAE IN QUATERNION FORM WE GET
V(QUATERION)       = (0,V)        WHERE 0 IS THE SCALAR PART AND V IS THE ORIGINAL VECTOR
V(PARA)(QUATERION) = (0,V(PARA))  WHERE 0 IS THE SCALAR PART AND V(PARA) IS THE PARALLEL COMPONENT OF THE ORIGINAL VECTOR
V(PERP)(QUATERION) = (0,V(PERP))  WHERE 0 IS THE SCALAR PART AND V(PERP) IS THE PERPENDICULAR COMPONENT OF THE ORIGINAL VECTOR
V'(PERP)(QUATERION)= (0,V'(PERP)) WHERE 0 IS THE SCALAR PART AND V'(PERP) IS THE ROTATED PERPENDICULAR COMPONENT OF THE ORIGINAL VECTOR
N(QUATERION)       = (0,N)        WHERE 0 IS THE SCALAR PART AND N IS ROTATION AXIS

V'(QUATERNION) = V(PARA)(QUATERION) + V'(PERP)(QUATERION)
USING (5) ABOVE WE KNOW V'(PERP)(QUATERION)= e^(THETA)N(QUATERNION) * V(PERP)(QUATERION)

V'(QUATERNION) = V(PARA)(QUATERION) + e^(THETA)N(QUATERNION) * V(PERP)(QUATERION)

WHEN V IS PERPENDICULAR TO N THEN e^(THETA)N(QUATERNION) * V(QUATERION) = V(QUATERION) * e^(NEGATIVE)(THETA)N(QUATERNION)------(A)
WHEN V IS PARALLEL TO N THEN      e^(THETA)N(QUATERNION) * V(QUATERION) = V(QUATERION) * e^(THETA)N(QUATERNION)----------------(B)

USING (A) AND (B) ABOVE
V'(QUATERNION) = V(PARA)(QUATERION) + e^(THETA)N(QUATERNION) * V(PERP)(QUATERION)
               = e^(THETA/2.0)N(QUATERNION) * e^(-THETA/2.0)N(QUATERNION) * V(PARA)(QUATERION) + e^(THETA/2)N(QUATERNION) * e^(THETA/2)N(QUATERNION) * V(PERP)(QUATERION)
               = e^(THETA/2.0)N(QUATERNION) *  V(PARA)(QUATERION) * e^(-THETA/2.0)N(QUATERNION) + e^(THETA/2)N(QUATERNION) * V(PERP)(QUATERION) * e^(-THETA/2)N(QUATERNION)
               = e^(THETA/2.0)N(QUATERNION) * (V(PARA)(QUATERION) + V(PERP)(QUATERION)) * e^(-THETA/2)N(QUATERNION)

               N VECTOR = Nx + Ny + Nz
               e^(THETA/2.0)N(QUATERNION)=COS(THETA/2) + SIN(THETA/2)*Nx*i + SIN(THETA/2)*Ny*j + SIN(THETA/2)*Nz*K = q
               e^(-THETA/2.0)N(QUATERNION)=COS(THETA/2) - SIN(THETA/2)*Nx*i - SIN(THETA/2)*Ny*j - SIN(THETA/2)*Nz*K = q* (CONJUGATE)

V'(QUATERNION) = q * V * q*
**************************************************/
GameMath::Matrix4x4 GameMath::Transform::RotateQ(char axis, const float &AngleOfRotationinDegrees)
{
    GameMath::Vector3 Axis;
    if(axis=='x'||axis=='X')
    Axis=GameMath::Vector3(1.0f,0.0f,0.0f);
    else if(axis=='y'||axis=='Y')
    Axis=GameMath::Vector3(0.0f,1.0f,0.0f);
    else if(axis=='z'||axis=='Z')
    Axis=GameMath::Vector3(0.0f,0.0f,1.0f);

    GameMath::Matrix4x4 output;
    float sint=sin(GameMath::Transform::GetRadian(AngleOfRotationinDegrees/2.0f));
    float r,i,j,k,s;
    r=cos(GameMath::Transform::GetRadian(AngleOfRotationinDegrees/2.0f));
    i=sint*Axis.x;
    j=sint*Axis.y;
    k=sint*Axis.z;
    s=sqrt(r*r + i*i + j*j + k*k);
    s=1/(s*s);

    output.Matrix[0][0]=1 - (2*s*(j * j)) - (2*s*(k * k));
    output.Matrix[0][1]=2*s*(i*j + k*r);
    output.Matrix[0][2]=2*s*(i*k -j*r);
    output.Matrix[0][3]=0.0f;

    output.Matrix[1][0]=2*s*(i*j - k*r);
    output.Matrix[1][1]=1 - (2*s*(i * i)) - (2*s*(k * k));
    output.Matrix[1][2]=2*s*(j*k + i*r);
    output.Matrix[1][3]=0.0f;

    output.Matrix[2][0]=2*s*(i*k + j*r);
    output.Matrix[2][1]=2*s*(j*k - i*r);
    output.Matrix[2][2]=1 - (2*s*(i * i)) - (2*s*(j * j));
    output.Matrix[2][3]=0.0f;

    output.Matrix[3][0]=0.0f;
    output.Matrix[3][1]=0.0f;
    output.Matrix[3][2]=0.0f;
    output.Matrix[3][3]=1.0f;
    return output;
}

GameMath::Matrix4x4 GameMath::Transform::RotateQ(GameMath::Vector3 Axis, const float &AngleOfRotationinDegrees)
{
    Axis=Axis.Normalize();
    GameMath::Matrix4x4 output;
    float sint=sin(GameMath::Transform::GetRadian(AngleOfRotationinDegrees/2.0f));
    float r,i,j,k,s;
    r=cos(GameMath::Transform::GetRadian(AngleOfRotationinDegrees/2.0f));
    i=sint*Axis.x;
    j=sint*Axis.y;
    k=sint*Axis.z;
    s=sqrt(r*r + i*i + j*j + k*k);
    s=1/(s*s);

    output.Matrix[0][0]=1 - (2*s*(j * j)) - (2*s*(k * k));
    output.Matrix[0][1]=2*s*(i*j + k*r);
    output.Matrix[0][2]=2*s*(i*k -j*r);
    output.Matrix[0][3]=0.0f;

    output.Matrix[1][0]=2*s*(i*j - k*r);
    output.Matrix[1][1]=1 - (2*s*(i * i)) - (2*s*(k * k));
    output.Matrix[1][2]=2*s*(j*k + i*r);
    output.Matrix[1][3]=0.0f;

    output.Matrix[2][0]=2*s*(i*k + j*r);
    output.Matrix[2][1]=2*s*(j*k - i*r);
    output.Matrix[2][2]=1 - (2*s*(i * i)) - (2*s*(j * j));
    output.Matrix[2][3]=0.0f;

    output.Matrix[3][0]=0.0f;
    output.Matrix[3][1]=0.0f;
    output.Matrix[3][2]=0.0f;
    output.Matrix[3][3]=1.0f;
    return output;
}
GameMath::Matrix4x4 GameMath::Transform::RotateQ(float w,float x,float y,float z)
{
    GameMath::Matrix4x4 output;
    float r,i,j,k,s;
    r=w;
    i=x;
    j=y;
    k=z;
    s=sqrt(r*r + i*i + j*j + k*k);
    s=1/(s*s);

    output.Matrix[0][0]=1 - (2*s*(j * j)) - (2*s*(k * k));
    output.Matrix[0][1]=2*s*(i*j + k*r);
    output.Matrix[0][2]=2*s*(i*k -j*r);
    output.Matrix[0][3]=0.0f;

    output.Matrix[1][0]=2*s*(i*j - k*r);
    output.Matrix[1][1]=1 - (2*s*(i * i)) - (2*s*(k * k));
    output.Matrix[1][2]=2*s*(j*k + i*r);
    output.Matrix[1][3]=0.0f;

    output.Matrix[2][0]=2*s*(i*k + j*r);
    output.Matrix[2][1]=2*s*(j*k - i*r);
    output.Matrix[2][2]=1 - (2*s*(i * i)) - (2*s*(j * j));
    output.Matrix[2][3]=0.0f;

    output.Matrix[3][0]=0.0f;
    output.Matrix[3][1]=0.0f;
    output.Matrix[3][2]=0.0f;
    output.Matrix[3][3]=1.0f;
    return output;
}
/**************************************************
IN OPENGL THE FOLLOWING TRANSFORMATIONS TAKE PLACE THAT ALLOWS A 3D OBJECT TO BE PROJECTED ONTO THE SCREEN

LOCAL SPACE => [MODEL MATRIX] => WORLD SPACE => [VIEW MATRIX] => VIEW SPACE => [PROJECTION MATRIX] => CLIP SPACE => [VIEWPORT TRANSFORMATION] => SCREEN SPACE


OPENGL USES THE RIGHT HANDED COORDINATE SYSTEM WHERE THE +Z AXIS IS FACING TOWARDS THE USER,
THE POSITIVE X AXIS IS ON THE RIGHT AND THE POSITIVE Y AXIS IS AT THE TOP LIKE SHOWN BELOW

            +Y
             *
             *
             *
             *
             ********* +X
            *
           *
          *
        +Z

NDC(NORMALIZED DEVICE COORDINATES) USE THE LEFT HANDED COORDINATE SYSTEM WHERE THE +Z AXIS IS FACING TOWARDS THE SCREEN,
THE POSITIVE X AXIS IS ON THE RIGHT AND THE POSITIVE Y AXIS IS AT THE TOP LIKE SHOWN BELOW


            +Y    +Z
             *    *
             *   *
             *  *
             * *
             ********* +X

THE PROJECTION MATRIX CONVERTS THE COORDINATES FROM THE PROJECTIVE SPACE TO NDC SPACE WHERE LATER ON
THE COORDINATES FROM THE NDC SPACE ARE CONVERTED INTO SCREEN SPACE

THE DERIVATION OF THIS MATRIX IS EXPLAINED AT: http://www.songho.ca/opengl/gl_projectionmatrix.html
**************************************************/
GameMath::Matrix4x4 GameMath::Transform::PerspectiveView(float Top,float Bottom,float Right,float Left,float Near,float Far)
{
    GameMath::Matrix4x4 output;
    output.Matrix[0][0] = (2 * Near) / (Right - Left);
    output.Matrix[0][1] = 0.0f;
    output.Matrix[0][2] = 0.0f;
    output.Matrix[0][3] = 0.0f;

    output.Matrix[1][0] = 0.0f;
    output.Matrix[1][1] = (2 * Near) / (Top - Bottom);
    output.Matrix[1][2] = 0.0f;
    output.Matrix[1][3] = 0.0f;

    output.Matrix[2][0] = (Right + Left) / (Right - Left);
    output.Matrix[2][1] = (Top + Bottom) / (Top - Bottom);
    output.Matrix[2][2] = -(Far + Near) / (Far - Near);
    output.Matrix[2][3] = -1.0f;

    output.Matrix[3][0] = 0.0f;
    output.Matrix[3][1] = 0.0f;
    output.Matrix[3][2] = -(2.0f * Far * Near) / (Far - Near);
    output.Matrix[3][3] = 0.0f;

    return output;
}
GameMath::Matrix4x4 GameMath::Transform::PerspectiveView(const float AspectRatio,const float &Near, const float &Far, const float &AngleofViewinDegrees)
{
    GameMath::Matrix4x4 output;
    const float TanTheta=tan(GameMath::Transform::GetRadian(AngleofViewinDegrees/2.0f));

    output.Matrix[0][0] = 1.0f / (TanTheta* AspectRatio);
    output.Matrix[0][1] = 0.0f;
    output.Matrix[0][2] = 0.0f;
    output.Matrix[0][3] = 0.0f;

    output.Matrix[1][0] = 0.0f;
    output.Matrix[1][1] = 1.0f / TanTheta;
    output.Matrix[1][2] = 0.0f;
    output.Matrix[1][3] = 0.0f;

    output.Matrix[2][0] = 0.0f;
    output.Matrix[2][1] = 0.0f;
    output.Matrix[2][2] = -(Far + Near) / (Far - Near);
    output.Matrix[2][3] = -1.0f;

    output.Matrix[3][0] = 0.0f;
    output.Matrix[3][1] = 0.0f;
    output.Matrix[3][2] = -(2.0f * Far * Near) / (Far - Near);
    output.Matrix[3][3] = 0.0f;

    return output;
}
/**************************************************
THIS FUNCTIONS TAKES YAW ANGLE, PITCH ANGLE AND THE CAMERA POSITION AS THE INPUT AND COVERTS IT INTO THE VIEW MATRIX
DETAILED WORKING OF THIS FUNCTION IS EXPLAINED IN THE GAMECAMERA/FPS.CPP SOURCE FILE
**************************************************/
GameMath::Matrix4x4 GameMath::Transform::LookAt(const float &YawAngleinDegrees,const float &PitchAngleinDegrees,const GameMath::Vector3 &CameraPosition)
{
    GameMath::Matrix4x4 output;

    const float YCos {cos(GameMath::Transform::GetRadian(YawAngleinDegrees))};
    const float YSin {sin(GameMath::Transform::GetRadian(YawAngleinDegrees))};
    const float XCos {cos(GameMath::Transform::GetRadian(PitchAngleinDegrees))};
    const float XSin {sin(GameMath::Transform::GetRadian(PitchAngleinDegrees))};

    output.Matrix[0][0]=YCos;
    output.Matrix[0][1]=XSin * YSin;
    output.Matrix[0][2]=XCos * YSin;
    output.Matrix[0][3]=0.0f;

    output.Matrix[1][0]=0.0f;
    output.Matrix[1][1]=XCos;
    output.Matrix[1][2]=-XSin;
    output.Matrix[1][3]=0.0f;

    output.Matrix[2][0]=-YSin;
    output.Matrix[2][1]=XSin * YCos;
    output.Matrix[2][2]=XCos * YCos;
    output.Matrix[2][3]=0.0f;

    output.Matrix[3][0]=(-CameraPosition.x * YCos) + (-CameraPosition.z * -YSin);
    output.Matrix[3][1]=(-CameraPosition.x * XSin * YSin) + (-CameraPosition.y * XCos) + (-CameraPosition.z * XSin * YCos);
    output.Matrix[3][2]=(-CameraPosition.x * XCos * YSin) + (-CameraPosition.y * -XSin) + (-CameraPosition.z * XCos * YCos);
    output.Matrix[3][3]=1.0f;

    return output;
}




