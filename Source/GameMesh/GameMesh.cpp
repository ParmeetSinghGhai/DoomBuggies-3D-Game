#include <GameMesh/GameMesh.h>
int GameMesh::MaterialIndex=0;

GameMesh::MaterialData::MaterialData() // @suppress("Class members should be properly initialized")
{
Index=MaterialIndex++;
}

//********************************
//  LOG FUNCTION
//********************************
void GameMesh::ShowContent()
{
  std::ofstream Log;
  Log.open ("MeshData.txt");

  int counter=0;
  Log<<"\nName of the Mesh ="<<Name<<"\n";

  Log<<*BoundingBox;

  Log<<"\n***** Materials *********\n";
  counter=0;
  if(Materials.size() > 0)
  {
      Log<<"Total Materials = "<<Materials.size()<<"\n";
      for(GameMesh::MaterialData *material:Materials)
      {
        Log<<"Material Index = "<<material->Index<<"\n";
        Log<<"  Ambient Strength ="<<material->AmbientStrength<<"\n";
        Log<<"  Shininess ="<<material->ShininessIndex<<"\n";
        Log<<"  Diffuse_color="<<material->DiffuseColor;
        Log<<"  Specular_color="<<material->SpecularColor;
        Log<<"  Diffuse Map="<<material->DiffuseMap<<"\n";
        Log<<"  Ambient Map="<<material->AmbientMap<<"\n";
        Log<<"  Specular Map="<<material->SpecularMap<<"\n";
      }
  }
  else
  Log<<"No Material found\n";

  Log<<"\n***** Vertices ************\n";
  Log<<"Total Vertices = "<<Vertices.size()<<"\n";
  counter=0;
  for(GameMath::Vector3 *vec:Vertices)
  Log<<counter++<<" Vertex ="<<*vec;


  Log<<"\n***** Faces ***************\n";
  Log<<"Total Faces = "<<Faces.size()<<"\n";
  counter=0;
  for(GameMesh::FaceData *face:Faces)
  {
    Log<<"Face #"<<counter++<<"*****\n";
    for(int i=0;i<3;i++)
    {
      Log<<"    Vertex index="<<face->Indices[i]<<"\n";
      if(face->UVCoords.size() > 0)
      Log<<"    UV coords="<<*(face->UVCoords[i]);
    }
    Log<<"    Material index="<<face->Material_index<<"\n";
  }

  Log<<"\n***** Vertex Groups *****\n";
  if(VertexGroups.size() > 0)
  {
    Log<<"Total Vertex Groups = "<<VertexGroups.size()<<"\n";
    for(std::pair<std::string,VertexGroupData*> vg:VertexGroups)
    {
      Log<<"Vertex Group name ="<<vg.first<<"\n";
      counter=0;
      for(float Weight:vg.second->Weights)
      {
        Log<<" Weight["<<counter++<<"] ="<<Weight<<"\n";
      }
    }
  }
  else
  {
      Log<<"No Vertex Groups found\n";
  }


Log.close();
}
