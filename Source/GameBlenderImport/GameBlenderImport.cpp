#include <GameBlenderImport/GameBlenderImport.h>
char* GameBlenderImport::byte=new char(4);
char* GameBlenderImport::bytef=new char(4);
char* GameBlenderImport::bytes=new char(1);
std::ifstream GameBlenderImport::Binaryfile;
std::string GameBlenderImport::FilePath;
//*********************************************
// HELPER FUNCTIONS
//*********************************************

/*************************************************
THIS FUNCTION READS 4 BYTES FROM THE FILE STREAM
AND TYPE CASTS IT TO A INTEGER POINTER. LATER IT RETURNS THE VALUE POINTED TO BY THAT POINTER
*************************************************/
int GameBlenderImport::Geti()
{
    Binaryfile.read(byte,4);
    return *(int*)byte;
}
/*************************************************
THIS FUNCTION READS 4 BYTES FROM THE FILE STREAM
AND TYPE CASTS IT TO A FLOAT POINTER. LATER IT RETURNS THE VALUE POINTED TO BY THAT POINTER
*************************************************/
float GameBlenderImport::Getf()
{
    Binaryfile.read(bytef,4);
    return *(float*)bytef;
}
/*************************************************
THIS FUNCTION READS 3 FLOATING VALUES OF 4 BYTES EACH FROM THE FILE STREAM
AND TYPE CASTS EACH OF THEM TO FLOAT POINTER. LATER IT ASSIGNS THOSE VALUES TO THE X,Y,Z COMPONENT
OF THE VECTOR3 OBJECT PASSED ON IT AS ARGUMENT
*************************************************/
void GameBlenderImport::Getv(GameMath::Vector3* vec)
{
    Binaryfile.read(bytef,4);
    vec->x=*(float*)bytef;

    Binaryfile.read(bytef,4);
    vec->y=*(float*)bytef;

    Binaryfile.read(bytef,4);
    vec->z=*(float*)bytef;
}
/*************************************************
THIS FUNCTION READ THE NUMBERS OF CHARACTERS THAT ARE NEEDED TO BE READ FIRST
AND THEN IT READS THOSE CHARACTERS AND STORES THEM IN A STRING OBJECT
*************************************************/
std::string GameBlenderImport::Gets()
{
    int chars=Geti();
    if(chars<=0)
    return "";

    std::string buffer;
    for(int i=0;i<chars;i++)
    {
    Binaryfile.read(bytes,1);
    buffer+=(*bytes);
    }
    return buffer;
}
/*************************************************
THIS FUNCTION READS 6 FLOATING VALUES WHICH ARE ASSIGNED TO BOUNDING BOX OBJECTS
*************************************************/
void GameBlenderImport::Getbb(float* maxx,float* minx,float* maxy,float* miny,float* maxz,float* minz)
{
*minx=Getf();
*maxx=Getf();
*miny=Getf();
*maxy=Getf();
*minz=Getf();
*maxz=Getf();
}


//*********************************************
// MAIN FUNCTIONS
//*********************************************


/*************************************************
THIS FUNCTION READ A ".GAMEEFFECTS" FILE AND RETURNS A FLOAT POINTER TO THE MESH DATA CONTAINED IN THAT
.GAMEEFFECTS, WHICH CONTAINS VERTEX DATA AND UV COORDINATE DATA FOR TEXTURE MAPPING.
IT TAKES THE FILEPATH OF THE ".GAMEEFFECTS" FILE AS ONE OF ITS ARGUMENT AND AN INTEGER POINTER WHICH IS USED
TO SAVE THE TRIANGLE COUNT IN THE GAMEEFFECTS OBJECTS LATER CREATED USING THIS MESH DATA
*************************************************/
float* GameBlenderImport::ImportEffects(std::string EffectsFilePath,int* TriangleCount)
{
Binaryfile.open(EffectsFilePath,std::ios::in | std::ios::binary);
int trianglecount;
float *vertexdata;
if(Binaryfile.is_open())
{
    trianglecount=Geti();//TOTAL NUMBER OF TRIANGLES
    //EACH TRIANGLE HAS 3 VERTICES AND EACH VERTEX HAS 3 FLOATING VALUES FOR X,Y,Z COORDINATES AND 2 FLOATING VALUES FOR UV COORDINATES
    //HENCE THE DATA READ = TRIANGLE COUNT * 3 VERTICES PER TRIANGLE * 5 FLOATING VALUE PER VERTEX
    vertexdata=new float[trianglecount * 3 * 5];
    (*TriangleCount)=trianglecount;
    int count=0;
    for(int i=0;i<trianglecount;i++)
    {
      for(int j=0;j<3;j++)//3 VERTICES PER TRIANGLE
      {
      vertexdata[count++]=Getf();//X coordinate
      vertexdata[count++]=Getf();//Y coordinate
      vertexdata[count++]=Getf();//Z coordinate
      vertexdata[count++]=Getf();//UV.X coordinate
      vertexdata[count++]=Getf();//UV.Y coordinate
      }
    }
    Binaryfile.close();
    return vertexdata;
}
return nullptr;
}


/*************************************************
THIS FUNCTION READS THE CONTENT OF A ".GAMEENGINE" FILE AND CREATES A "GAMEOBJECT" OBJECT USING THE DATA CONTAINED IN THAT FILE.
IT IMPORTS THE ARMATURE DATA AND THE MESH DATA INTO THE GAMEOBJECT. IN SOME GAMEOBJECTS, ARMATURE IS SKIPPED.

ARMATURE'S INTERNAL DATA STRUCTURE IS AS FOLLOWS:
1) ARMATURE NAME THAT UNIQUELY IDENTIFIES THE ARMATURE
2) COLLECTION OF BONES WHERE EACH BONE IS UNIQUELY INDENTIFIED WITH ITS NAME. EACH BONE HAS THE FOLLOWING DATA STRUCTURE:
   A) NAME OF THE BONE THAT UNIQUELY IDENTIFIES THE BONE
   B) 3 DIMENSIONAL VECTOR CONTAINING THE HEAD COORDINATES OF THE BONE
   C) 3 DIMENSIONAL VECTOR CONTAINING THE TAIL COORDINATES OF THE BONE
   D) 3 DIMENSIONAL VECTOR CONTAINING THE X AXIS COORDINATES OF THE BONE
   E) 3 DIMENSIONAL VECTOR CONTAINING THE Y AXIS COORDINATES OF THE BONE
   F) 3 DIMENSIONAL VECTOR CONTAINING THE Z AXIS COORDINATES OF THE BONE
   G) PARENT BONE UNIQUELY IDENTIFIED BY IT'S NAME. IF NOT PARENT BONE IS PRESENT THEN ITS POINTS TO NULLPTR
   H) COLLECTION OF CHILD BONES UNIQUELY INDENTIFIED BY THEIR NAMES


MESH'S INTERNAL DATA STRUCTURE IS AS FOLLOWS:
1) NAME OF THE MESH THAT UNIQUELY INDENTIFIES THE MESH
2) COLLECTION OF ALL THE VERTICES WHERE EACH VERTEX IS 3 DIMENSIONAL VECTOR
3) COLLECTION OF ALL THE TRIANGLES/FACES. EACH FACE HAS THE FOLLOWING DATA STRUCTURE:
   A) AN ARRAY OF 3 INDICES WHERE EACH INDEX IS FOR THE VERTEX THAT MAKES UP THE TRIANGLE
   B) AN COLLECTION OF UV COORDINATES USED FOR TEXTURE MAPPING WHERE EACH VERTEX HAS ITS OWN UV COORDINATES
      UV COORDINATES ARE BASICALLY 2 DIMENSIONAL COMPONENTS WITH X AND Y COMPONENTS
   C) A MATERIAL INDEX FOR THE TRIANGLE THAT UNIQUELY IDENTIFIES A MATERIAL
4) COLLECTION OF MATERIAL OBJECT. EACH MATERIAL HAS THE FOLLOWING DATA STRUCTURE:
   A) AM INDEX THAT UNIQUELY IDENTIFIES THE MATERIAL
   B) FLOATING VALUE THAT SIGNIFIES THE AMBIENT STRENGTH OF THE MATERIAL
   C) FLOATING VALUE THAT SIGNIFIES THE SHININESS INDEX OF THE MATERIAL
   D) 3 DIMENSIONAL VECTOR FOR DIFFUSE COLOR OF THE MATERIAL
   E) 3 DIMENSIONAL VECTOR FOR SPECULAR COLOR OF THE MATERIAL
   F) A STRING THAT SPECIFIES THE FILEPATH TO THE DIFFUSEMAP FOR THIS MATERIAL, WHICH IS A BMP FILE
   G) A STRING THAT SPECIFIES THE FILEPATH TO THE AMBIENTMAP FOR THIS MATERIAL, WHICH IS A BMP FILE
   H) A STRING THAT SPECIFIES THE FILEPATH TO THE SPECULARMAP FOR THIS MATERIAL, WHICH IS A BMP FILE
5) COLLECTION OF VERTEX GROUP DATA. EACH VERTEX GROUP HAS THE FOLLOWING DATA STRUCTURE:
   A) COLLECTION OF FLOATING POINT VALUES WHERE EACH FLOATING VALUE CORRESPONDS TO THE WEIGHT OF A VERTEX IN THAT VERTEX GROUP
   EXAMPLE: WE HAVE 2 VERTEX GROUPS NAMELY VG1 AND VG2 AND 5 VERTICES IN THE ORDER: V1,V2,V3,V4,V5
   IN THIS SENARIO EACH VERTEX GROUP WILL HAVE THE FOLLOWING DATA STRUCTURE:
   VG1:
       V1:W1,V2:W2,V3:W3,V4:W4,V5:W5
   VG2:
       V1:W1,V2:W2,V3:W3,V4:W4,V5:W5
   HERE W REPRESENT THE WEIGHT OF THE VERTEX IN THAT VERTEX GROUP
*************************************************/
GameObject* GameBlenderImport::Import(std::string filepath)
{
FilePath=filepath;
GameMath::Vector3* vec3=nullptr;
GameMath::Vector2* vec2=nullptr;
GameMesh::FaceData* face=nullptr;
GameMesh::MaterialData* material=nullptr;
GameMesh::VertexGroupData *vertexgroup=nullptr;
GameBoundingBox* boundingbox=nullptr;
std::string buffer;

Binaryfile.open(FilePath,std::ios::in | std::ios::binary);
if(Binaryfile.is_open())
{
    try
    {
	GameObject *Gobject=new GameObject();//CREATE NEW GAME OBJECT FILE IF THE FILE OPENED UP SUCCESSFULLY
	if(Geti()==1)//ARMATURE IS PRESENT SO EXTRACT ARMATURE
	{
	   Gobject->Armature.Name=Gets();//GET ARMATURE NAME
           int totalbones=Geti();//GET TOTAL NUMBER OF BONES
	   for(int i=0;i<totalbones;i++)//ITERATE THROUGH ALL THE BONES
	   {
	     GameArmature::Bone* Gbone=new GameArmature::Bone();//CREATE A NEW BONE
	     Gbone->Name=Gets();//GET BONE NAME
	     Getv(&(Gbone->Head));//GET BONE HEAD VECTOR
	     Getv(&(Gbone->Tail));//GET BONE TAIL VECTOR
	     Getv(&(Gbone->XAxis));//GET BONE XAXIS
	     Getv(&(Gbone->YAxis));//GET BONE YAXIS
	     Getv(&(Gbone->ZAxis));//GET BONE ZAXIS
	     std::string parent=Gets();//GET BONE'S PARENT BONE NAME
	     if(parent!="")//IF BONE'S PARENT BONE IS NOT PRESENT THEN THE STRING WILL BE ""
	     Gbone->Parent.first=parent;//IF THE BONE'S PARENT BONE IS PRESENT THEN SAVE THAT NAME AS PARENT BONE'S NAME
	     Gbone->Parent.second=nullptr;//SET THE PARENT BONE POINTER TO NULL FOR NOW AS NOT ALL THE BONES HAVE BEEN SAVED YET

	     int totalchildbones=Geti();// GET TOTAL NUMBER OF CHILD BONES
	     for(int q=0;q<totalchildbones;q++)
	     Gbone->Child[Gets()]=nullptr;//SAVE NAME OF THE CHILD BONES

	     Gobject->Armature.Bones[Gbone->Name]=Gbone;//SAVE THIS NEW BONE IN THE BONES COLLECTION OF THE ARMATURE
	   }
	   Gobject->Armature.ConnectBones();//AFTER ALL BONE DATA HAS BEEN COLLECTED THEN WE NEED TO LINK THEM
	                                    //TOGETHER AS PER THEIR PARENT AND CHILD RELATIONSHIP USING THEIR NAMES
	}

       //EXTRACT MESH DATA
       Gobject->Mesh.Name=Gets();//GET MESH NAME
       int totalvertices=Geti();//GET TOTAL NUMBER OF VERTICES IN THE MESH
       for(int i=0;i<totalvertices;i++)//SAVES ALL VERTICES IN THE MESH'S VERTICES COLLECTION
       {
	 vec3=new GameMath::Vector3();
	 vec3->x=Getf();
	 vec3->y=Getf();
	 vec3->z=Getf();
	 Gobject->Mesh.Vertices.push_back(vec3);
       }

       float maxx,minx,maxy,miny,maxz,minz;
       Getbb(&maxx,&minx,&maxy,&miny,&maxz,&minz);
       boundingbox=new GameBoundingBox(maxx,minx,maxy,miny,maxz,minz);//GET BOUNDING BOX'S DATA
       Gobject->Mesh.BoundingBox=boundingbox;//SAVE MESH'S BOUNDING BOX'S DATA

       int totalfaces=Geti();//GET TOTAL NUMBER OF TRIANGLES PRESENT
       int uvpresentflag=Geti();//GET THE UV PRESENT FLAG, IF SET T0 0 THEN UV COORDINATES ARE NOT PRESENT ELSE THEY ARE PRESENT
       if(uvpresentflag==1)//UV COORDINATES ARE PRESENT SO EXTRACT VERTEX COORDINATES AND UV COORDINATES
       {
	   for(int i=0;i<totalfaces;i++)
	   {
	     face=new GameMesh::FaceData();
	     face->Material_index=Geti();
	     face->Indices[0]=Geti();
	     face->Indices[1]=Geti();
	     face->Indices[2]=Geti();

	     for(int j=0;j<3;j++)
	     {
		 vec2=new GameMath::Vector2();
		 vec2->x=Getf();
		 vec2->y=Getf();
		 face->UVCoords.push_back(vec2);
	     }
	     Gobject->Mesh.Faces.push_back(face);
	   }
       }
       else//UV COORDINATES ARE NOT PRESENT SO DO NOT EXTRACT UV COORDINATES.ONLY EXTRACT VERTEX COORDINATES
       {
	   for(int i=0;i<totalfaces;i++)
	   {
	     face=new GameMesh::FaceData();
	     face->Material_index=Geti();
	     face->Indices[0]=Geti();
	     face->Indices[1]=Geti();
	     face->Indices[2]=Geti();
	     Gobject->Mesh.Faces.push_back(face);
	   }
       }

       int totalmaterial=Geti();//GET TOTAL NUMBER OF MATERIALS
       for(int i=0;i<totalmaterial;i++)//ITERATE THROUGH ALL THE MATERIALS
       {
	 material=new GameMesh::MaterialData();//CREATE A NEW MATERIAL OBJECT
	 material->AmbientStrength=Getf();//GET MATERIAL AMBIENT STRENGTH
	 material->ShininessIndex=Getf();//GET MATERIAL SHININESS INDEX
	 material->DiffuseColor.x=Getf();//GET MATERIAL DIFFUSE COLOR X COMPONENT
	 material->DiffuseColor.y=Getf();//GET MATERIAL DIFFUSE COLOR Y COMPONENT
	 material->DiffuseColor.z=Getf();//GET MATERIAL DIFFUSE COLOR Z COMPONENT
	 material->SpecularColor.x=Getf();//GET MATERIAL SPECULAR COLOR X COMPONENT
	 material->SpecularColor.y=Getf();//GET MATERIAL SPECULAR COLOR Y COMPONENT
	 material->SpecularColor.z=Getf();//GET MATERIAL SPECULAR COLOR Z COMPONENT
	 if(Gets()!="")//IF THE NAME OF THE DIFFUSE MAP IS PRESENT THEN SET THE NAME TO "MESHNAME + DIFFUSEMAP.BMP"
	 material->DiffuseMap=GameWindow::CWD+"\\Payload\\Textures\\"+Gobject->Mesh.Name+"DiffuseMap.bmp";
	 else
	 material->DiffuseMap="";
	 if(Gets()!="")//IF THE NAME OF THE AMBIENT MAP IS PRESENT THEN SET THE NAME TO "MESHNAME + AMBIENTMAP.BMP"
	 material->AmbientMap=GameWindow::CWD+"\\Payload\\Textures\\"+Gobject->Mesh.Name+"AmbientMap.bmp";
	 else
	 material->AmbientMap="";
	 if(Gets()!="")//IF THE NAME OF THE SPECULAR MAP IS PRESENT THEN SET THE NAME TO "MESHNAME + SPECULARMAP.BMP"
	 material->SpecularMap=GameWindow::CWD+"\\Payload\\Textures\\"+Gobject->Mesh.Name+"SpecularMap.bmp";
	 else
	 material->SpecularMap="";
	 Gobject->Mesh.Materials.push_back(material);//SAVE THE MATERIAL IN MESH'S MATERIAL COLLECTION
       }

       int totalvertexgroups=Geti();//GET TOTAL NUMBER OF VERTEX GROUPS
       for(int i=0;i<totalvertexgroups;i++)//ITERATE THROUGH ALL THE VERTEX GROUPS
       {
	 vertexgroup=new GameMesh::VertexGroupData();//CREATE A NEW VERTEX GROUP OBJECT
	 buffer=Gets();//GET NAME OF THE VERTEX GROUP
	 for(int q=0;q<totalvertices;q++)//ITERATE THROOUGH ALL THE VERTICES OF THE MESH TO MAP ALL THEIR WEIGHTS FOR THIS VERTEX GROUP
	 vertexgroup->Weights.push_back(Getf());
	 Gobject->Mesh.VertexGroups[buffer]=vertexgroup;//SAVE THE VERTEX GROUP IN THE MESH'S VERTEX GROUP COLLECTION
       }

       Gobject->FilePath=filepath.substr(0, filepath.find_last_of("\\/"));//SAVE THE FILE PATH
       std::cout<<"Object "<<Gobject->Mesh.Name<<" imported successfully\n";
       fflush(NULL);
       Binaryfile.close();
       return Gobject;//RETURN THE NEWLY CREATED GAME OBJECT
    }
    catch(std::ifstream::failure e)
    {
    std::cout<<"ERROR::RUNTIME EXCEPTION IN GAME OBJECT EXTRACT MODULE\n";
    Binaryfile.close();
    return nullptr;
    }
}
else
std::cout<<"ERROR:: IN GAME OBJECT EXTRACT MODULE: FILE: "<<FilePath<<" DID NOT OPEN\n";
Binaryfile.close();
return nullptr;
}

