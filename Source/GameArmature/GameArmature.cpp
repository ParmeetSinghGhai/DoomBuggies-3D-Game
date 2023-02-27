#include<GameArmature/GameArmature.h>
int GameArmature::BoneIndex=0;

GameArmature::Bone::Bone() // @suppress("Class members should be properly initialized")
{
this->Index=BoneIndex++;
}
/*************************************************
THIS FUNCTION CONNECTS THE BONES TOGETHER USING THEIR PARENT AND CHILD RELATIONSHIP. ITS WORKING IS AS FOLLOWS:
1) INITIALLY WHEN A BONE IS INITIALIZED:
  A) IT CONTAINS STRING VALUES (BONE NAME) FOR ITS PARENT BONE.IF PARENT IS PRESENT OTHERWISE ITS ""
  B) IT CONTAINS STRING VALUES (BONE NAME) FOR ITS CHILD BONES.IF NO CHILD BONES ARE PRESENT THEN ITS "" FOR THAT CHILD BONE
  C) POINTER TO THE BONE THAT IS THE PARENT BONE. ITS INITIALLY SET TO NULLPTR
  D) POINTER(S) TO THE CHILD BONES WHICH ARE SET TO NULLPTR INITIALLY

2) THE FUNCTION ITERATES THROUGH ALL THE BONES ONE BY ONE. FOR EACH BONE IT DOES THE FOLLOWING:
  A) CHECKS IF THE BONE HAS A PARENT BONE. IF PARENT BONE IS PRESENT THEN SETS ITS PARENT BONE POINTER TO THE
     BONE WHICH HAS THE SAME NAME AS THE PARENT BONE. ALL BONES CAN BE UNIQUELY IDENTIFIED USING THEIR NAMES
  B) IT GOES THROUGH ALL THE CHILD BONES ONE BY ONE AND DOES THE FOLLOWING:
     LOCATES THE CHILD BONE USING ITS NAME
     SETS THE CHILD POINTER TO THE BONE THAT HAS THE SAME NAME AS THE CHILD BONE AS  ALL BONES CAN BE UNIQUELY IDENTIFIED USING THEIR NAMES

THIS IS HOW THE PARENT CHILD RELATIONSHIP IS ESTABLISHED IN THE ARMATURE
AND THIS FUNCTION IS RUN AFTER ALL THE BONES IN THE ARMATURE HAVE BEEN IMPORTED
*************************************************/
void GameArmature::ConnectBones()
{
for(std::map<std::string,Bone*>::iterator it =Bones.begin();it !=Bones.end();it++)
{
  if(((it->second)->Parent.first).size() > 0)
  (it->second)->Parent.second=Bones[(it->second)->Parent.first];

  for(std::map<std::string,Bone*>::iterator child_it=(it->second)->Child.begin();child_it!=(it->second)->Child.end();child_it++)
  child_it->second=Bones[child_it->first];
}
}

void GameArmature::ShowContent()
{
std::ofstream Log;
Log.open ("ArmatureData.txt");
Log<<"Name of the Armature ="<<Name<<"\n";
for(std::map<std::string,Bone*>::iterator it =Bones.begin();it !=Bones.end();it++)
{
  Log<<"\n";
  Log<<"Bone Name ="<<it->first<<"\n";
  if((it->second)->Parent.second!=nullptr)
  Log<<"Bone Parent ="<<(it->second)->Parent.second->Name<<"\n";
  for(std::map<std::string,Bone*>::iterator child_it=(it->second)->Child.begin();child_it!=(it->second)->Child.end();child_it++)
  Log<<"Bone Child ="<<child_it->second->Name<<"\n";
}
Log.close();
Log<<"Log file created\n";
}

