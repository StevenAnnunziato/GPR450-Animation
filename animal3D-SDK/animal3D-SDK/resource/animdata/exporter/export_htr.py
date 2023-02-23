import bpy
import struct

# Define the file path and name for the exported HTR file
file_path = "C:/Users/steven.annunziato/Desktop/Repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata/exporter"
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
with open(export_path, "wb") as f:

    # Write the header information
    f.write("[Header]\n")
    f.write("# KeyWord<space>Value<CR>")
    f.write("FileType htr\n")
    f.write("DataType HTRS\n")
    f.write("FileVersion 1.0\n")
    f.write("NumSegments " + len(rig_object.data.bones) + "\n")
    f.write("NumFrames " + num_frames + "\n")
    f.write("DataFrameRate " + bpy.context.scene.render.fps + "\n")
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
        f.write(bone.name.encode("ascii")) # Bone
        if bone.parent:
            f.write( bone.parent.name.encode("ascii\n")) # Parent
        else:
            f.write(b"Main\n") # Root

    f.write("[BasePosition]\n")
    f.write("# ObjectName<tab>Tx<tab>Ty<tab>Tz<tab>Rx<tab>Ry<tab>Rz<tab>BoneLength<CR>\n")

    # Write base pose
    for bone in rig_object.pose.bones:
        f.write("\n" + bone.name.encode("ascii")) # Bone
        bone_matrix = rig_object.convert_space( bone,
                                                bone.matrix, 
                                                from_space="POSE", 
                                                to_space="LOCAL")
        loc = bone_matrix.to_translation()
        rot = bone_matrix.to_quaternion().to_axis_angle();
        scale = bone_matrix.to_scale()
        f.write("\t" +loc.x +"\t"+loc.y+"\t" +"\t"+ loc.z))
        f.write("\t" +rot.x +"\t"+rot.y+"\t" +"\t"+ rot.z))
        f.write("\t" +scale.x))

    # Write the animation data for each frame
    for i in range(start_frame, end_frame + 1):
        bpy.context.scene.frame_set(i)
        for bone in rig_object.pose.bones:
            bone_matrix = rig_object.convert_space( bone,
                                                    bone.matrix, 
                                                    from_space="POSE", 
                                                    to_space="LOCAL")
            loc = bone_matrix.to_translation()
            rot = bone_matrix.to_quaternion().to_axis_angle();
            scale = bone_matrix.to_scale()
            f.write("\n"+i + "\t" +loc.x +"\t"+loc.y+"\t" +"\t"+ loc.z))
            f.write("\t" +rot.x +"\t"+rot.y+"\t" +"\t"+ rot.z))
            f.write("\t" +scale.x))
            
    f.write("\n[EndOfFile]")