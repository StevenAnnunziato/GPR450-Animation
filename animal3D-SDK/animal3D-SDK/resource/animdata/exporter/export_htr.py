import bpy
import struct
import math
from mathutils import Euler

# Define the file path and name for the exported HTR file
file_path = "C:/Users/steven.annunziato/Desktop/Repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata/exporter/"
file_name = "rig_export.htr"
export_path = file_path + file_name

# Get the active object (the rigged character)
rig_object = bpy.context.view_layer.objects.active

# Set the start and end frames for the animation
start_frame = bpy.context.scene.frame_start
end_frame = bpy.context.scene.frame_end
num_frames = end_frame - start_frame + 1
frame_rate = bpy

# Open the HTR file for writing
with open(export_path, "w") as f:

    # Write the header information
    f.write("[Header]\n")
    f.write("# KeyWord<space>Value<CR>")

    f.write("FileType htr\n")
    f.write("DataType HTRS\n")
    f.write("FileVersion 1.0\n")
    f.write("NumSegments " + str(len(rig_object.data.bones)) + "\n")
    f.write("NumFrames " + str(num_frames) + "\n")
    f.write("DataFrameRate " + str(bpy.context.scene.render.fps) + "\n")
    f.write("EulerRotationOrder ZYX\n")
    f.write("CalibrationUnits m\n")
    f.write("RotationUnits Degrees\n")
    f.write("GlobalAxisofGravity Z\n")
    f.write("BoneLengthAxis Z\n")
    f.write("BoneLengthAxis Z\n")
    f.write("ScaleFactor 1.00\n")

    # Header information
    f.write("[SegmentNames&Hierarchy]\n")
    f.write("# ObjectName<tab>ParentObjectName<CR>\n")
    
    # Write the bone hierarchy information
    for bone in rig_object.pose.bones:
        f.write(bone.name + "\t") # Bone
        if bone.parent:
            f.write(bone.parent.name + "\n") # Parent
        else:
            f.write("GLOBAL\n") # Root

    f.write("[BasePosition]\n")
    f.write("# ObjectName<tab>Tx<tab>Ty<tab>Tz<tab>Rx<tab>Ry<tab>Rz<tab>BoneLength<CR>\n")

    # Write base pose
    for bone in rig_object.pose.bones:
        f.write(bone.name) # Bone
        bone_matrix = rig_object.convert_space( pose_bone=bone,
                                                matrix=bone.matrix, 
                                                from_space="POSE", 
                                                to_space="LOCAL")
        loc = bone_matrix.to_translation()
        rot = bone_matrix.to_euler()
        scale = bone_matrix.to_scale()
        f.write("\t" + str(loc.x) +
                "\t" + str(loc.y) +
                "\t" + str(loc.z))
        f.write("\t" + str(rot.x) +
                "\t" + str(rot.y)+
                "\t" + str(rot.z))
        f.write("\t" + str(scale.x))
        f.write("\n")

    # Write the animation data for each frame
    for i in range(start_frame, end_frame + 1):
        bpy.context.scene.frame_set(i)
        for bone in rig_object.pose.bones:
            bone_matrix = rig_object.convert_space( pose_bone=bone,
                                                    matrix=bone.matrix, 
                                                    from_space="POSE", 
                                                    to_space="LOCAL")
            loc = bone_matrix.to_translation()
            rot = bone_matrix.to_euler()
            scale = bone_matrix.to_scale()
            f.write("\n" + str(i) + 
                    "\t" + str(loc.x) +
                    "\t" + str(loc.y)+
                    "\t" + str(loc.z))
            f.write("\t" + str(rot.x) +
                    "\t" + str(rot.y)+
                    "\t" + str(rot.z))
            f.write("\t" + str(scale.x))
            f.write("\n")
            
    f.write("\n[EndOfFile]")