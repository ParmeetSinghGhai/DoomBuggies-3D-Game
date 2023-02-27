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

def seti(value):
 file.write(value.to_bytes(4,sys.byteorder,signed=True))

def setf(value):
 file.write(bytearray(struct.pack("f", value)))

def sets(value):
 file.write(len(value).to_bytes(4,sys.byteorder,signed=True))
 file.write(value.encode("ascii")) 
 
def setv(value):
 file.write(bytearray(struct.pack("f", value.x)))
 file.write(bytearray(struct.pack("f", value.y)))
 file.write(bytearray(struct.pack("f", value.z)))

def setuv(value):
 file.write(bytearray(struct.pack("f", value.x)))
 file.write(bytearray(struct.pack("f", value.y)))
   
def findmesh():
 for o in bpy.data.objects:
  if(o.type=="MESH"):
   return o 

mat_rot = mathutils.Matrix.Rotation(math.radians(-90.0), 4, 'X')
bdpath=os.path.splitext(bpy.data.filepath)[0] + ".GameEffects"
cbd=bpy.context.blend_data
vertexcollection=[]
uvcollection=[]

object=findmesh()
bm = bmesh.new()
bm.from_mesh(object.data)
bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=3,  ngon_method=1)
bm.to_mesh(object.data)
bm.free()
tmesh=object.data

#open the file 
system('cls') 
file=open(bdpath,'wb')

#save total triangle count
seti(len(tmesh.polygons))

#gather vertex and uv data
uv_layer=None
if(tmesh.uv_layers.active is not None):
 uv_layer=tmesh.uv_layers.active.data
 print("Total faces ="+str(len(tmesh.polygons)))
 print("Total vertices in mesh "+str(len(tmesh.vertices)))  
 for poly in tmesh.polygons: 
  for vertexindex in poly.vertices:
   for vertex in tmesh.vertices:
    if(vertex.index == vertexindex):
     vec=object.matrix_world * vertex.co
     vec=mat_rot * vec
     vertexcollection.append(vec)
  for loopindex in poly.loop_indices:
   if(uv_layer is not None):
    uvcollection.append(uv_layer[loopindex].uv)
  
print("Saving "+str(len(vertexcollection))+ " vertices")  

#save vertex and uv data
i=0
for vertex in vertexcollection:
 setv(vertex)
 setuv(uvcollection[i])
 i+=1

#close file 
file.close() 