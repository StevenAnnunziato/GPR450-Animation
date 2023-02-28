import bpy
import math
from mathutils import Euler

# Define the file path and name for the exported HTR file
#file_path = "E:/~School/SP23/Animation Programming/repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata/exporter/"
#file_path = "F:/Repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata/exporter/"
file_path = "C:/Users/steven.annunziato/Desktop/Repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata/exporter/"
#file_name = "rig_export_simple.htr"
file_name = "rig_export.htr"
export_path = file_path + file_name

# Get the active object (the rigged character)
rig_object = bpy.data.objects["Armature"]

# Set the start and end frames for the animation
start_frame = bpy.context.scene.frame_start
end_frame = bpy.context.scene.frame_end
num_frames = end_frame - start_frame + 1

# determine euler order
order = 'XYZ'
for pose_bone in rig_object.pose.bones:
    pose_bone.rotation_mode = order

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
    f.write("EulerRotationOrder " + order + "\n")
    f.write("CalibrationUnits m\n")
    f.write("RotationUnits Degrees\n")
    f.write("GlobalAxisofGravity Z\n")
    f.write("BoneLengthAxis Z\n")
    f.write("ScaleFactor " + str(rig_object.scale.x) + "\n")

    # Hierarchy information
    f.write("[SegmentNames&Hierarchy]\n")
    f.write("# ObjectName<tab>ParentObjectName<CR>\n")
    
    # Write the bone hierarchy information
    for bone in rig_object.pose.bones:
        f.write(bone.name + "\t") # Bone
        if bone.parent:
            f.write(bone.parent.name + "\n") # Parent
        else:
            f.write("GLOBAL\n") # Root

    # Base position info
    f.write("[BasePosition]\n")
    f.write("# ObjectName<tab>Tx<tab>Ty<tab>Tz<tab>Rx<tab>Ry<tab>Rz<tab>BoneLength<CR>\n")

    # Write base pose
    # NOTE:
    # this assumes the base pose is frame 1
    bpy.context.scene.frame_set(1)
    parent_bone = None
    for bone in rig_object.pose.bones:
        f.write(bone.name) # Bone
        
        if bone.parent:
            bone_matrix = rig_object.convert_space(pose_bone=bone.parent, matrix=(bone.matrix), from_space='POSE', to_space='LOCAL')    # bone matrix                   object -> local, relative to parent
        else: #root
            bone_matrix = rig_object.convert_space(pose_bone=bone, matrix=(bone.matrix_basis), from_space='LOCAL', to_space='POSE')     # basis matrix, or ofset        local -> object
        
        # write location and rotation as Eulers
        loc = bone_matrix.to_translation()
        rot = bone_matrix.to_euler()
        f.write("\t" + str('%.6f' % loc.x) +
                "\t" + str('%.6f' % loc.y) +
                "\t" + str('%.6f' % loc.z))
        f.write("\t" + str('%.6f' % (rot.x * 180 / math.pi)) +
                "\t" + str('%.6f' % (rot.y * 180 / math.pi)) +
                "\t" + str('%.6f' % (rot.z * 180 / math.pi)))
        f.write("\t" + str('%.6f' % bone.length))
        f.write("\n")

    # Write the animation data for each frame
    for bone in rig_object.pose.bones:
        f.write("[" + bone.name + "]\n")
        f.write("# Tx<tab>Ty<tab>Tz<tab>Rx<tab>Ry<tab>Rz<tab>BoneScaleFactor<CR>\n")

        # for each frame...
        for i in range(start_frame, end_frame + 1):
            bpy.context.scene.frame_set(i)

            # grab deltas as keyframed by the animator
            # this describes a bone's transform relative to its rest position
            loc = bone.matrix_basis.to_translation()
            rot = bone.matrix_basis.to_euler()
            scale = bone.matrix_basis.to_scale()
            f.write(str(i) + 
                    "\t" + str('%.6f' % loc.x) +
                    "\t" + str('%.6f' % loc.y)+
                    "\t" + str('%.6f' % loc.z))
            f.write("\t" + str('%.6f' % (rot.x * 180 / math.pi)) +
                    "\t" + str('%.6f' % (rot.y * 180 / math.pi)) +
                    "\t" + str('%.6f' % (rot.z * 180 / math.pi)))
            f.write("\t" + str('%.6f' % scale.x))
            f.write("\n")
            
    # done
    f.write("[EndOfFile]")