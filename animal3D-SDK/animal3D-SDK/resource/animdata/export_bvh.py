import bpy
import os

# Define the file path and name for the exported BVH file
file_path = "F:/Repos/GPR450-Animation/animal3D-SDK/animal3D-SDK/resource/animdata"
file_name = "rig_export.bvh"
export_path = os.path.join(file_path, file_name)

# Get the active object (the rigged character)
rig_object = bpy.context.view_layer.objects.active

# Set the start and end frames for the animation
start_frame = bpy.context.scene.frame_start
end_frame = bpy.context.scene.frame_end

# Export the animation data in BVH file format
bpy.ops.export_anim.bvh(filepath=export_path,  
                        frame_start=start_frame, 
                        frame_end=end_frame, 
                        root_transform_only=False)