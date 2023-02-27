import bpy
import mathutils
from mathutils import Matrix
from mathutils import Vector
import math
import os
import bmesh
import sys
import struct
import shutil
from os import system, name

#*************************************************************
TByte=0
bdpath=os.path.splitext(bpy.data.filepath)[0] + ".GameEngine"
cbd=bpy.context.blend_data

mat_rot = mathutils.Matrix.Rotation(math.radians(-90.0), 4, 'X')

def seti(value):
 global TByte    
 TByte=file.write(value.to_bytes(4,sys.byteorder,signed=True)) + TByte

def setf(value):
 global TByte    
 TByte=file.write(bytearray(struct.pack("f", value))) + TByte

def sets(value):
 global TByte    
 TByte=file.write(len(value).to_bytes(4,sys.byteorder,signed=True)) + TByte
 TByte=file.write(value.encode("ascii")) + TByte
 
def setv(value):
 global TByte
 TByte=file.write(bytearray(struct.pack("f", value.x)))+ TByte
 TByte=file.write(bytearray(struct.pack("f", value.y)))+ TByte
 TByte=file.write(bytearray(struct.pack("f", value.z)))+ TByte

def seta(value):
 global TByte
 TByte=file.write(bytearray(struct.pack("f", value[0])))+ TByte
 TByte=file.write(bytearray(struct.pack("f", value[1])))+ TByte
 TByte=file.write(bytearray(struct.pack("f", value[2])))+ TByte

def findarmature():
 for o in bpy.data.objects:
  if(o.type=="ARMATURE"):
   return o

def findmesh():
 for o in bpy.data.objects:
  if(o.type=="MESH"):
   return o

def getworldcoord(object,vec):
 vec=object.matrix_world * vec
 return mat_rot * vec 
#*************************************************************
system('cls')

# step: open file
file=open(bdpath,'wb')
meshfile=open(bdpath,'wt')

# step: armature present or not present flag
if(findarmature() is None):
 seti(0)
else:
 seti(1)  

# step: Extract armature
if(findarmature() is not None):
 print("*****************************")
 print("Armature extraction started ")
 object=findarmature()
 
 #save armature name
 sets(object.data.name)
 print("Armature name = "+object.data.name)
 
 #save total number of bones
 seti(len(object.data.bones))
 print("Total bones = %d"%len(object.data.bones))

 #iterate through all the bones  
 for bone in object.data.bones:
  #save bone name
  sets(bone.name)
  print("Bone name = "+bone.name)
  
  #saving head of the bone
  setv(getworldcoord(object,bone.head_local))
  
  #saving tail of the bone
  setv(getworldcoord(object,bone.tail_local))
  
  #saving x axis of the bone
  setv(getworldcoord(object,bone.x_axis))
  
  #saving y axis of the bone, z axis becomes y axis
  setv(getworldcoord(object,bone.z_axis))
  
  #saving z axis of the bone, y axis becomes z axis
  setv(getworldcoord(object,bone.y_axis))
  
  #save bone parent name
  if(bone.parent is not None):
   sets(bone.parent.name)
   print("   Parent bone ="+bone.parent.name)
  else:
   seti(0)
   print("   No Parent found")
      
  #save total number of child bones      
  seti(len(bone.children))
  print("   Total child bones = %d"%len(bone.children))
          
  #save names of the child bones
  if(len(bone.children) > 0):
   for child in bone.children:
    sets(child.name)
    print("      Child bone = "+child.name)

 print("Armature extraction completed successfully")
else:
 print("No Armature Found")
   

#step: Extract Mesh
if(findmesh() is not None):
 print("*****************************")
 print("Mesh extraction started ")
 object=findmesh()

 #save mesh name
 sets(object.data.name)
 print("Mesh name = "+ object.data.name)
 
 #triangulate mesh
 bm = bmesh.new()
 bm.from_mesh(object.data)
 bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method="BEAUTY",  ngon_method="BEAUTY")
 bm.to_mesh(object.data)
 bm.free()
 tmesh=object.data
 
 #save total number of vertices
 seti(len(tmesh.vertices))
 print("Total vertices in mesh = %d"%len(tmesh.vertices))
 
 #save all vertices coordinates 
 for vertex in tmesh.vertices:
  vec=object.matrix_world @ vertex.co
  vec=mat_rot @ vec
  setv(vec)
  
 #save bounding box of the object
 #format = minx, maxx, miny, maxy, minz, maxz
 bb=[]
 bytes=0
 for bbv in object.bound_box:
  vec=object.matrix_world @ Vector((bbv[0],bbv[1],bbv[2],1.0))
  vec=mat_rot @ vec
  bb.append(vec)
 
 #the world mesh will be bumpy but its bounding box needs to 
 #have its height set to as low as possible
 #if the mesh is named "World" then the bounding box's y 
 #coordinates will be set to 0.0 and 0.001 
 setf(bb[0].x)#minx
 setf(bb[4].x)#maxx
 if(object.data.name=="World"):
  print("world BB values saved")
  setf(0.0)#miny
  setf(0.001)#maxy
 else:
  print("object BB values saved")
  setf(bb[0].y)#miny
  setf(bb[1].y)#maxx
 setf(bb[2].z)#minz  
 setf(bb[0].z)#maxz
 
 #save total polygons
 seti(len(tmesh.polygons))
 print("Total triangles = %d"%len(tmesh.polygons))

 #save flag for UV coordinates
 if(tmesh.uv_layers.active is not None):
  seti(1)#UV coordinates are present 
 else:
  seti(0)#UV coordinates are not present  
   
 #save triangle data where triangle has its vertice indices saved
 #along with uv coordinates for those vertices, if the uv coordinates 
 #are present
 uv_layer=None
 if(tmesh.uv_layers.active is not None):
  uv_layer=tmesh.uv_layers.active.data
 for poly in tmesh.polygons: 
  seti(poly.material_index)
  for vertexindex in poly.vertices:
   seti(vertexindex)
  for loopindex in poly.loop_indices:
   if(uv_layer is not None):
    setf(uv_layer[loopindex].uv.x) 
    setf(uv_layer[loopindex].uv.y)
      
 #save total materials
 seti(len(tmesh.materials))
 print("Total materials = %d"%len(tmesh.materials))
 
 #save material data 
 index=0
 for mat in tmesh.materials:
  setf(mat.ambient)
  setf(mat.specular_hardness)
  seta(mat.diffuse_color)
  seta(mat.specular_color)
  print("   Material = "+ mat.name)
  print("      Index = "+str(index)) 
  index+=1
  #save material maps per materials
  diffmappath=""
  ambmappath=""
  specmappath=""
  for textureindex in range(0,len(mat.texture_slots)):
   mat.active_texture_index=textureindex
   if(mat.active_texture is not None and mat.active_texture.type=="IMAGE"):
    if(mat.active_texture.name=="DiffuseMap" and diffmappath is ""):
     diffmappath=mat.active_texture.image.filepath
    if(mat.active_texture.name=="AmbientMap" and ambmappath is ""):
     ambmappath=mat.active_texture.image.filepath
    if(mat.active_texture.name=="SpecularMap" and specmappath is ""):
     specmappath=mat.active_texture.image.filepath

  sets(diffmappath)
  print("       Diffuse Map = "+diffmappath)   
  sets(ambmappath)
  print("       Ambient Map = "+ambmappath)   
  sets(specmappath)
  print("       Specular Map = "+specmappath)   
                  
 #save total vertext group data 
 seti(len(object.vertex_groups))
 print("Total vertex groups = %d"%len(object.vertex_groups))
    
 #save vertex groups data
 #a vertex can be part of multiple vertex groups
 #we start with a list of all vertex groups
 #for each vertex group, we iterate through all the vertices of the mesh
 #and find out which vertex belongs to that vertex group
 #if the vertex is found then its weight is recorded else 
 #the default weight of 0.0 if recorded 
 for vertgrp in object.vertex_groups:
  sets(vertgrp.name)#save group name
  print("   Vertex group = "+ vertgrp.name)
  for vertex in tmesh.vertices:   
   found=0#marker
   for vertexgroup in vertex.groups:
    if(vertexgroup.group is vertgrp.index):
     setf(vertexgroup.weight)#save vertex weight in that group
     found=1
     break
   if(found==0):#save default weight 
    setf(0.0)   

 print("Mesh extraction completed successfully")
else:
 print("No Mesh found")
    
#step: closing the file 
file.close()  
  