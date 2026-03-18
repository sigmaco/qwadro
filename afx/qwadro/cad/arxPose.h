/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_POSE_H
#define ARX_POSE_H

/*
    The arxPose object is a key data structure used in the Qwadro animation subsystems to represent the state of an articulated model 
    or object at a given moment. In essence, a pose encapsulates the configuration of all the joints or bones of a articulated object, 
    including their transformations (position, rotation, scale), and possibly other state information such as weights or constraints.
    A pose typically consists of the following data:

    Joint Transformations (Position, Rotation, Scale):
    These represent how each joint or bone in a skeleton or articulated model is positioned, rotated, and scaled relative to its parent 
    (or in world space, depending on the pose type). These transformations are usually stored as matrices or quaternions for efficient 
    manipulation and interpolation.

    Articulations (Joints/Bones):
    A pose includes information about each articulation (joint or bone) in the system. This typically involves the hierarchical structure 
    of the skeleton, which defines how each bone connects to others, and how those connections affect the transformations of each bone.

    State Information:
    Depending on the system, a pose may include additional state data, such as:
        Weights: Influence of individual bones or joints in a layered pose or animation.
        Constraints: Any movement or position restrictions applied to the pose, such as limits on joint rotation.
        Time: In an animation system, the pose may also carry timing information (e.g., frame number or timestamp).

    Pose Metadata: 
    Some poses may include metadata such as:
        A tag (e.g., "idle", "run", "jump") for categorization.
        A weight indicating the contribution of the pose to a composite pose (if the pose is blended with others).
        A timestamp for real-time systems, tracking when the pose was generated.
*/

#include "qwadro/iris/arxIrisDefs.h"
#include "qwadro/math/afxTransform.h"

AFX_DEFINE_STRUCT(arxPoseArticulation)
// The arxPoseArticulation structure is designed to represent the articulation of a pose.
// This articulation refers to the individual joints or bones of a object in 3D space. 
// It contains relevant information about how each articulation is transformed and how much influence it has in a pose.
{
    // The influence or weight of the articulation in the pose.
    afxReal         weight;

    // The count of articulations or joints involved.
    afxUnit         cnt;

    // The transformation (position, rotation, scale) of the articulation.
    afxTransform    xform;

    // A unique identifier for the traversal of this articulation.
    afxUnit         traversalId;
};

AFX_DEFINE_STRUCT(arxPoseInfo)
// The arxPoseInfo struct is a data structure used to define specific information related to a pose.
// The struct contains parameters that help describe the characteristics of a pose and provide the necessary information to retrieve or manipulate it.
{
    // The number of articulations (joints/bones) involved in this pose.
    afxUnit     artCnt;

    // A threshold for filling or blending the pose (for interpolation).
    afxReal     fillThreshold;

    // A debugging tag associated with the pose, potentially for identification or categorization.
    afxString   tag;

    // A pointer to user-defined data, allowing for flexible extensions.
    void*       udd;
};

/*
    The function ArxAcquirePoses() acquires multiple poses associated with a specific scenario in a system, potentially for an animation 
    or simulation context. The function interacts with a scenario (a predefined situation or context, such as a character's animation or 
    pose sequence) and provides a set of poses that can be used within that scenario.

    The function returns an error code to indicate whether the operation was successful.

    The function begins by understanding the scenario (scio) context. This could involve fetching poses related to a specific animation, 
    simulation state, or character configuration.
*/

ARX afxError ArxAcquirePoses
(
    // This represents a scenario, which could be a predefined or runtime context in the system. 
    // It represents a particular sequence or situation where poses are needed, such as a character animation, a game state, or a simulation.
    // A scenario might include specific parameters or settings that determine how the poses are acquired. 
    // For example, the scenario might define the time range, the conditions for each pose, or specific animations to retrieve.
    arxScenario scio,

    // This is the count of poses to acquire. It specifies how many poses you want to retrieve for the scenario.
    // The cnt parameter allows you to request multiple poses at once, depending on the number of poses 
    // required for the scenario (e.g., to populate an animation sequence with a series of frames).
    afxUnit cnt,

    // This is an array of pose information structures, each of which contains metadata or settings related to the individual poses that are 
    // being acquired. The information could include details like the time, the type of pose, any specific parameters (like pose indices or 
    // articulation information), or other contextual data required to fetch a pose.
    // This array allows the user to specify the specific criteria for each pose, and the function will use this information to determine which 
    // poses to retrieve.
    arxPoseInfo const cfgs[],

    // This is an array of poses that will be populated by the function. 
    // After calling ArxAcquirePoses, the function will fill this array with the retrieved poses based on the scenario and the provided pose information(info[]).
    // Each entry in the poses[] array will correspond to a specific pose based on the information provided in the cfgs[] array. 
    // These poses could represent different frames or key poses in a sequence(like animation keyframes) or various states of an object or character.
    arxPose poses[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The ArxGetPoseCapacity() function queries the articulatory capacity of a pose from a given starting base articulation.
*/

ARX afxUnit ArxGetPoseCapacity
(
    // The arxPose object to be queried.
    arxPose pose, 
    // The index of the base articulation.
    afxUnit base
);

/*
    The function ArxDescribePose() retrieves detailed information about a pose and storing that information in an arxPoseInfo structure. 
    This allows you to describe or inspect the properties of a given pose (such as its structure, content, or how it's organized), 
    which can be useful for tasks like debugging, analysis, animation blending, or procedural adjustments.
*/

ARX void ArxDescribePose
(
    // The pose that you want to describe. 
    // The arxPose object contains the joint transformations (position, rotation, scale) for a character or object.
    arxPose pose,

    // This is a pointer to an arxPoseInfo structure where the function will store the descriptive information about the pose.
    arxPoseInfo* info
);

/*
    The function ArxDescribePoseArticulations() describes and extracts detailed articulation information for a given pose. 
    It takes a pose and returns information about the articulations (joints or bones) involved in that pose, filling an array 
    of arxPoseArticulation structures.

    ArxDescribePoseArticulations is used to describe multiple articulations (joints or bones) of a pose. This function provides 
    detailed information about each articulation, which is useful in tasks like understanding how each articulation (joint) contributes 
    to a pose, especially in animation blending or procedural animation systems. Or extracting detailed articulation data to analyze the 
    structure of a pose or to debug pose transformations. Or when working with motion capture data, you might want to retrieve specific 
    articulation information, such as the position and rotation of individual joints. Or in procedural animation, you may need to modify 
    or apply transformations to specific articulations, and having access to detailed articulation data helps with this.
*/

ARX void ArxDescribePoseArticulations
(
    // This is the pose that contains the articulation data.
    // It likely represents a specific configuration or frame of animation for a character, object, or articulated model.
    // The pose includes joint positions, rotations, and other transformations.
    arxPose pose,

    // The base index or starting point for the articulations that need to be described. 
    // This could be used to offset the articulation description, particularly when you want to retrieve 
    // articulations starting from a specific joint index.
    afxUnit base,
    
    // This represents the count of articulations (joints or bones) that you want to describe starting from the base index. 
    // This allows you to specify how many articulations should be described.
    afxUnit cnt,

    // This is an optional array of indices that can be used to remap or select specific articulations in a non-linear way. 
    // If provided, the remap array could define a specific selection of articulations to be described, rather than linearly.
    afxUnit const remap[],

    // This is an array of arxPoseArticulation structures where the articulation data will be stored. After calling this function, 
    // the array will be populated with information about the specified articulations (joints or bones).
    arxPoseArticulation infos[]
);

/*
    The ArxGetPoseTransform() method retrieves the transformation (position, rotation, scale) of a specific articulation in a given pose.
    You provide the pose and the index of the articulation you're interested in, and the function returns the corresponding transformation.
    It helps to get the current state of a specific joint or part of an articulated object when you need to know how a pose is 
    affecting a particular articulation.

    Suppose you have a character (or some other articulated object) with a series of bones or joints. You want to know the position and 
    rotation of one of the bones (say, the elbow) after applying a particular pose.
*/

ARX afxTransform* ArxGetPoseTransform
(
    arxPose pose, 
    afxUnit artIdx
);

/*
    The ArxCopyPose() method copies pose data from one pose (@from) to another (@pose), optionally using a mapping array (artMap) 
    that helps transfer or convert specific elements of the pose. The parameters @toBaseArtIdx and fromBaseArtIdx specifies 
    base articulation for each pose.

    The artMap[] array allows for non-linear selection of articulations during the pose copy. 
    This means that instead of copying pose data in a straight, one-to-one fashion (e.g., from one joint to the corresponding joint), 
    the artMap[] lets you remap or select articulations in a custom order or pattern. It's optional, so if not provided, 
    the pose may be copied in a linear fashion, from one articulation to another, based on default indexing or structure.

    Imagine you're working with a character in a 3D environment, and you have a pose on one character (the "source") that you want to 
    apply to another (the "target"). The articulations (or bones) in these two characters might not be aligned or indexed in the same way, 
    so the artMap[] lets you define how each articulation from the source should be copied over to the target. Without artMap[], 
    the copy would follow a linear, index-by-index mapping.
*/

ARX void ArxCopyPose
(
    // The target pose. 
    // This is the destination articulation (or body part) where the copied pose will be applied.
    arxPose pose, 

    // The base index of the target articulation. 
    // This might correspond to a specific starting point in the target's hierarchy of articulations.
    afxUnit toBaseIdx, 

    // The source pose. 
    // This is where the pose data will come from (likely another articulated object or a different set of articulation states).
    arxPose from, 

    // The base index for the source articulation. 
    // This marks the starting point in the source's articulation hierarchy from which the data will be copied.
    afxUnit fromBaseIdx, 

    // The number of articulations (or body parts) involved in the copy. 
    // This is a way to limit or define how much of the pose is copied (e.g., you may only want to copy the position of a 
    // certain number of joints rather than all of them).
    afxUnit cnt, 

    // An optional array that specifies how the articulations in the source pose are mapped to the target pose. 
    // Since this array is optional, it gives you flexibility. If provided, it allows you to copy the pose data from arbitrary or 
    // non-sequential articulations, rather than copying them one by one in the default order.
    afxUnit const remap[]
);

/*
    The ArxModulatePose() method provides a way to blend or modulate one pose (composite) into another (pose) with fine-grained control 
    over which articulations are affected and to what extent. You can apply this modulation with varying weights, using masks to affect 
    only specific joints, and even remap joints between different articulation structures.

    The purpose of ArxModulatePose is to blend or modulate a composite pose into a target pose (pose). The modulation will affect only 
    certain parts of the target pose, as controlled by the various parameters. The weights (weightNone and weightAll) allow you to control 
    how much of the composite pose is applied to the target pose. A weight of 0 means no influence, and a weight of 1 means full influence.

    You can selectively apply the modulation to certain joints, and the joint remap array allows you to align articulations from one rig to another, 
    so this function works even with rigs that have different bone structures.
*/

ARX void ArxModulatePose
(
    // This is the target pose to which the modulation or blending will be applied. 
    // The final result of the modulation will be stored in this pose.
    arxPose pose, 

    // The base index of the target articulations in the pose. 
    // This is where the modulation will start or where the base reference for the target pose lies.
    afxUnit toBaseArtIdx, 

    // This is the composite pose that you want to blend with the target pose. 
    // This pose will be mixed into the target pose (pose) according to the weights and conditions set by the other parameters.
    arxPose composite, 

    // This refers to the base index of the articulations in the composite pose. 
    // It marks the starting point for articulations in the composite pose from which the modulation will be applied.
    afxUnit fromBaseArtIdx, 

    // The count of articulations to modulate. This could be the number of joints or bones to affect when blending the composite pose into the target pose. 
    // For example, if artCnt = 3, you would blend the first 3 articulations from composite to pose.
    afxUnit artCnt, 

    // This parameter controls the weight or influence of parts of the pose when there is no modulation (or no blending). 
    // It sets the baseline behavior for the parts of the pose that should remain unchanged, 
    // effectively ensuring that some articulations are not influenced by the blending.
    afxReal weightNone, 

    // This is the weight that applies when all parts of the pose should be modulated to their fullest extent. 
    // If the articulation is fully blended, this would be the value that defines how strongly the composite pose influences the target pose.
    afxReal weightAll, 

    // This is a masking mechanism that may allow you to specify which parts of the pose should be affected by the modulation. 
    // This could be used to selectively modulate certain parts of the pose (for example, only the arms or legs, and not the head or torso). 
    // It could be a bitmask or similar structure to enable/disable the modulation for certain bones or joints.
    arxTrackMask* mask, 

    // This is an optional joint remapping array. It allows you to remap articulations from the composite pose to different articulations in the target pose. 
    // This is useful if the articulations of the source and target poses don't match directly (e.g., different rigs with different bone structures). 
    // The jntRemap[] would provide the correct indices of which articulations in the target pose should receive data from which articulations in the composite pose.
    afxUnit const jntRemap[]
);

/*
    The ArxComputeAttachmentWorldMatrix() method calculates the world-space transformation (world matrix) for an attachment based on the articulated pose and skeleton. 
    The world matrix represents the position, rotation, and scale of the attachment in the world space, which is typically used to determine the attachment's 
    final placement relative to the environment (e.g., to position an accessory, weapon, or any other item attached to a character).

    The pose provides the current transformation of the articulated object (e.g., a character), and the skeleton provides the hierarchical structure of the joints or bones. 
    The function will take into account how each bone is transformed in the pose to compute the world matrix of the attachment.

    If the pose doesn't exactly match the skeleton's joint structure (e.g., due to a custom rig or non-standard articulation setup), 
    the sparseJntMap[] and sparseJntMapRev[] provide a way to translate between the pose's joint indices and the skeleton's joint indices, 
    allowing the function to properly handle mismatched joint structures.

    The displacement matrix allows for an additional transformation to be applied after computing the world matrix. For example, 
    if the attachment has a fixed offset relative to the joint (like a sword held in the hand), the displacement matrix would shift the attachment's 
    position appropriately after computing the world matrix.

    The resulting world matrix will include all of the transformations (pose, skeleton, displacement) combined. It represents the final 
    transformation of the attachment in world space, which can then be used to render the attachment or to compute interactions with the 
    environment (e.g., checking for collisions or determining visibility).
*/

ARX void ArxComputeAttachmentWorldMatrix
(
    // The pose of the articulated object (e.g., a character or a skeletal structure). 
    // The pose contains the transformations (like position, rotation, and possibly scale) of the individual articulations or bones of the skeleton.
    arxPose pose, 
    
    // This is the skeleton of the articulated object. It contains the hierarchical structure of bones or articulations (joints), 
    // with data about each bone's local transformations and relationships (parent-child relationships between bones).
    arxSkeleton skl, 

    // This refers to the index of a specific joint or bone. 
    // This joint might be the one to which the attachment is associated or the joint that is the focus for computing the world matrix for the attachment. 
    // It could refer to the joint that is moving, rotating, or transforming the attachment.
    afxUnit jntIdx, 

    // This array is a sparse joint map. It provides a mapping between the joints of the pose and the joints of the skeleton. 
    // This can be useful if the skeleton and the pose are not perfectly aligned in terms of indexing, or if there are joints that are not included in the pose. 
    // The array could map joint indices in the pose to their corresponding joints in the skeleton.
    afxUnit const sparseJntMap[], 

    // This is the reverse mapping of sparseJntMap[]. It allows for a reverse lookup to map joint indices from the skeleton back to the pose. 
    // This could be used for ensuring the consistency of joint data between the pose and skeleton, especially when working with complex or non-standard mappings.
    afxUnit const sparseJntMapRev[], 

    // This parameter is a displacement matrix. It represents an offset or transformation that needs to be applied to the attachment after computing the world matrix. 
    // For example, if an attachment is offset by a certain amount from the bone to which it is attached (such as an accessory or weapon), 
    // this displacement matrix would apply that transformation to the final world matrix.
    afxM4d const displace, 

    // This is the output matrix that will contain the world matrix of the attachment after the computation. 
    // The world matrix represents the final transformation of the attachment in the global/world space, 
    // taking into account the pose, skeleton, and any displacement.
    afxM4d m
);

/*
    The ArxComputeAttachmentOffset() method calls ArxComputeAttachmentWorldMatrix(), but then it inverts the result to convert the attachment's 
    position from world space to the local space of the specified joint. This gives you the attachment's relative position to the joint 
    (e.g., how far the weapon is from the hand joint in the hand's local space), which is useful for precise placement and adjustment of 
    attachments during animations or other transformations.
    
    The purpose of ArxComputeAttachmentOffset is to compute the relative transformation (offset) of an attachment in relation to a specific joint 
    or bone in the skeleton. This is different from computing the world matrix, as it focuses on the relative position, rotation, and scale 
    of the attachment with respect to the joint. While the world matrix places the attachment in the global/world space, the attachment offset 
    keeps the attachment localized to a specific joint or bone in the pose, which can then be further transformed into the world space if needed.

    The key difference between ArxComputeAttachmentOffset and ArxComputeAttachmentWorldMatrix is that while ArxComputeAttachmentWorldMatrix 
    computes the world matrix of an attachment (placing it in global space), ArxComputeAttachmentOffset computes the relative offset of the 
    attachment with respect to a specific joint, which is essentially the inverse of the transformation applied in ArxComputeAttachmentWorldMatrix.

    In ArxComputeAttachmentOffset, after calling ArxComputeAttachmentWorldMatrix, the function takes the resulting world matrix and inverts it.
    Inverting the world matrix essentially means transforming the world space coordinates back to the local space of the specified joint. 
    The result is the offset matrix that tells you where the attachment is located relative to the joint, not in the global/world space.

    The world matrix positions the attachment in global space. If you know the world matrix of both the joint (bone) and the attachment, 
    you can compute the relative offset by "reversing" the global transformations and computing the difference between the two.

    Inverting the world matrix effectively gives you the inverse transformation that would take the attachment's world coordinates back into 
    the local coordinates of the joint.
*/

ARX void ArxComputeAttachmentOffset
(
    // The pose of the articulated object (like a character), which holds the transformations (position, rotation, scale) of the individual joints or bones.
    arxPose pose, 

    // The skeleton of the articulated object, which defines the structure and hierarchy of bones or joints in the model.
    arxSkeleton skl, 

    // The index of the joint to which the attachment is associated. This joint will serve as the reference point for computing the offset. 
    // The offset is the relative transformation between the attachment and this joint.
    afxUnit jntIdx, 

    // This is a sparse joint map, which likely provides a mapping between the joints in the pose and the joints in the skeleton. 
    // This is useful if the skeleton and the pose do not have identical joint structures or indices. 
    // This array allows you to translate joint indices between the two systems.
    afxUnit const sparseJntMap[], 

    // This is the reverse mapping of the sparseJntMap[], allowing you to look up joint indices in the opposite direction (from skeleton to pose). 
    // This can help ensure that transformations are correctly applied even when joint mappings are complex.
    afxUnit const sparseJntMapRev[], 

    // This is a displacement matrix that represents an additional offset or transformation to be applied to the attachment relative to the 
    // specified joint. For example, this could be used to apply an offset that accounts for the attachment being located at a specific position 
    // relative to the joint (e.g., a weapon held in the hand at a certain distance and orientation).
    afxM4d const displace, 

    // This is the output matrix that will store the offset transformation of the attachment relative to the specified joint. 
    // The matrix will represent the final transformation required to place the attachment at the correct position and orientation relative to the joint.
    afxM4d m
);

/*
    The ArxRebuildRestPose() method rebuilds the rest pose of an articulated object (such as a character or a skeleton) based on the current 
    pose and the skeleton. The rest pose is a neutral or reference pose, typically used as a base for creating animations, rigging, 
    or manipulating the skeleton.

    The main purpose of ArxRebuildRestPose is to rebuild the rest pose for the articulated object’s skeleton based on its current pose.
    The function will update the transformations (position, rotation, etc.) of the skeleton’s joints to match the current pose.
    It could be used to reset or reconstruct the neutral pose or bind pose, which serves as a reference pose for animation or deformation.

    In a typical rigging or animation system, the rest pose (sometimes called the bind pose) is the pose in which the skeleton is initially 
    posed before any deformations or animations. It’s essentially a neutral reference pose from which all animations start.
*/

ARX void ArxRebuildRestPose
(
    // The current pose of the articulated object.     
    // This pose contains the transformations (position, rotation, scale) of the bones or joints in their current state. 
    // This is what you would typically modify during animations or while adjusting a character's posture.
    arxPose pose, 
    
    // The skeleton of the articulated object. The skeleton defines the hierarchical structure of the bones (joints), 
    // their relationships, and how they should move relative to each other. The skeleton is the underlying framework that the pose works with.
    arxSkeleton skl, 

    // The index of the base joint or starting joint in the skeleton. This is the joint from which the rebuilding of the rest pose might begin, 
    // especially if you are only modifying a part of the skeleton or updating certain joints.
    afxUnit baseJntIdx, 

    // The count of joints that will be rebuilt starting from @baseJntIdx. This allows you to specify how many joints, starting from the base joint, 
    // should be included in the process of rebuilding the rest pose. It can be used to selectively rebuild a portion of the skeleton or the entire skeleton.
    afxUnit jntCnt
);

/*
    The ArxApplyPoseRootMotionVectors() method applies root motion to a character or articulated model's pose. 
    In animation systems, root motion refers to the movement and rotation of the character's root bone (often the pelvis or a similar joint), 
    which drives the overall translation and rotation of the character in the world space.

    It does apply translation and rotation vectors to a character's pose, which would typically affect the root joint or bone, 
    causing the whole character to move and rotate accordingly.

    The main purpose of ArxApplyPoseRootMotionVectors is to apply root motion to the character's pose by updating the root bone's position and 
    rotation according to the provided translation and rotation vectors. This can be used in situations where the character needs to move or 
    rotate as a result of animation, such as when the root bone moves in space during an animation sequence (e.g., walking, running, or any 
    movement animation).

    In animation systems that use root motion, the movement of the character is driven by the movement of the root bone. 
    Root motion is often used in character animation when the character's position and rotation are meant to be controlled by the animation itself, 
    rather than by external input like user controls or physics simulations.

    Rather than just applying the rotation directly to the root joint, the function seems to compute the angular velocity as a quaternion, 
    which is a more robust and efficient way to handle rotational motion, especially for interpolating rotations in animation systems.
*/

ARX void ArxApplyPoseRootMotionVectors
(
    // The pose of the articulated object (such as a character). 
    // This pose contains the current transformations (position, rotation, scale) of the joints or bones in their current state. 
    // In the context of root motion, the pose is the starting point where the root bone’s transformation is modified by the root motion vectors.
    arxPose pose, 

    // This is a translation vector that represents the movement of the character's root bone (usually the pelvis or base joint) in world space. 
    // It typically contains the changes in the x, y, and z coordinates that the character should move by. 
    // This vector will apply the overall displacement to the character.
    afxV3d const translation, 

    // The rotation vector is interpreted as an angular velocity vector. 
    // The function will use this to create a quaternion that represents the rotational velocity of the root joint.
    afxV3d const rotation
);

/*
    The ArxCommencePoseAccumulation() method initiates accumulating pose data for a set of joints in a character's skeleton. 
    Pose accumulation generally involves collecting or summing up transformations (such as position, rotation, or scale) for specific 
    joints over time, which is part of a Qwadro subsystem that supports things like animation blending, motion retargeting, or other complex 
    pose manipulation operations.

    The main purpose of ArxCommencePoseAccumulation is to begin the process of accumulating pose data for a specific set of joints in the skeleton, 
    starting from a given joint index (baseArtIdx) and including a number of joints (artCnt). Accumulating pose data often refers to combining or 
    blending pose transformations over time or multiple iterations.

    If you are blending multiple animations together (e.g., blending a walking animation with a running animation), you might accumulate the 
    transforms of various joints to create a smooth transition. This could be used when adjusting a character’s pose procedurally over time, 
    such as when modifying the pose based on AI inputs or other systems. And if you are retargeting an animation from one skeleton to 
    another (e.g., transferring an animation from a human character to a monster), you might accumulate poses for specific joints to 
    adapt the animation to the new skeleton.

    In other words, this function might set up the system to accumulate transformations for certain joints in a skeleton over time or 
    across multiple iterations.
*/

ARX void ArxCommencePoseAccumulation
(
    // This is the current pose of the articulated object (such as a character). 
    // The pose contains the current transformations (position, rotation, scale) of the joints or bones. 
    // It is essentially the state of the skeleton at a particular moment in time.
    arxPose pose, 

    // This is the index of the base articulation (often a base joint or root bone). 
    // It indicates where the pose accumulation will start. 
    // The base articulation index is important because it determines which joint is considered the "starting point" for the pose accumulation.
    afxUnit baseArtIdx, 

    // The count of articulations (joints or bones) to be accumulated starting from baseArtIdx. 
    // This parameter allows you to specify how many joints or bones should be included in the accumulation process. 
    // You can accumulate transformations for a specific portion of the skeleton (e.g., just the arms or legs), or for the entire skeleton, 
    // depending on the count.
    afxUnit artCnt, 

    // This is an optional joint map array that might be used to map joint indices in a non-linear way. 
    // It could be used when you want to accumulate pose data for specific joints in a non-sequential order, or when the skeleton's joint 
    // indices are not the same as the indices used by the pose. The array could provide a mapping between joint indices in the pose and the 
    // indices in the skeleton, allowing for more flexible control over which joints are involved in the accumulation.
    afxUnit const jntMap[]
);

/*
    The ArxConcludePoseAccumulation() method finalizes the pose accumulation process that was initiated by ArxCommencePoseAccumulation. 
    This might involve applying the accumulated pose data to the skeleton, integrating or updating the transformations (position, rotation, 
    scale) of the joints in the skeleton, and handling tolerances or errors in the application of the accumulated pose, using the allowedErr 
    parameter to specify an acceptable level of deviation when updating the joints.

    In simple terms, this function takes the accumulated pose data and finalizes the transformation of the skeleton, allowing the result to 
    be applied to the model's final pose.
*/

ARX void ArxConcludePoseAccumulation
(
    // This is the current pose of the articulated object (character or model). 
    // The pose represents the current transformations (position, rotation, and scale) of the joints or bones in the skeleton.
    arxPose pose, 

    // This is the index of the base articulation (joint) where the accumulation process should end. 
    // In the context of concluding the accumulation, this is the point where the function will finish processing or applying the 
    // accumulated data to the skeleton's joints, starting from the base index.
    afxUnit baseArtIdx, 

    // The count of joints or bones to apply the accumulated pose data to. 
    // This is the same as the artCnt used in ArxCommencePoseAccumulation, and it specifies how many joints will be affected by the accumulation process. 
    // The function will apply the accumulated pose data to these joints starting from the baseArtIdx.
    afxUnit artCnt, 

    // The skeleton model (skl) associated with the character or object. 
    // The skeleton model defines the hierarchical structure of the joints or bones and their relationships. 
    // The accumulated pose data is applied to the corresponding joints in the skeleton.
    arxModel skl, 

    // This is the allowed error or tolerance during the pose application. 
    // It is a threshold that specifies how much error or deviation is acceptable when applying the accumulated pose data. 
    // This could be useful in cases where approximate precision is acceptable, such as when blending animations or adjusting 
    // poses in a way that doesn't require perfect accuracy.
    afxReal allowedErr, 

    // This is an optional joint mapping array, which might be used to map joints in a non-linear way or to adjust which joints are affected by the accumulation process.
    // It allow for flexibility in handling different joint indices or for selecting a subset of joints to apply the accumulated pose to.
    afxUnit const jntMap[]
);

/*
    The function ArxAccumulateLocalTransform() is designed to accumulate or apply a local transformation to a specific joint in a skeleton. 
    The transformation could be a new movement, rotation, or scaling applied to the joint, or it could be a part of an animation blending or 
    procedural animation system.

    This function might be useful in scenarios where you are blending animations or modifying the pose of a character using multiple transformations. 
    The blending operation (blendOp) allows for smooth transitions between transformations, while the weight (weight) allows for controlling the 
    intensity of the applied transformation.

    The function first identifies which joint to modify based on the artIdx (or possibly sklJntIdx). 
    This joint will be the target for the accumulated transformation.

    Using the transformation t (which includes translation, rotation, and scaling), the function applies the transformation to the selected joint. 
    The actual transformation applied to the joint depends on the blending operation (blendOp) and the weight (weight).
    After applying the transformation, the joint's new local transformation is updated in the skeleton's pose (pose). 
    This means the final pose will reflect the accumulated transformations for the affected joint.
*/

ARX void ArxAccumulateLocalTransform
(
    // This is the current pose of the character or model, which holds the joint transformations (position, rotation, and scale) for all 
    // the joints in the skeleton. The transformations here are relative to the parent joints in the skeleton hierarchy.
    arxPose pose, 

    // This is the articulation index or a specific identifier for the articulation (joint or bone) in the skeleton to which the transformation 
    // should be applied. It's an index into the joint hierarchy, specifying which joint's transformation is being modified.
    afxUnit artIdx, 

    // This is the skeleton joint index, which corresponds to the joint in the skeleton model (skl) where the transformation will be applied. 
    // It's similar to the artIdx but might be used for a specific skeleton structure. 
    // This index may allow you to map different skeletal structures or target joints differently within different poses.
    afxUnit sklJntIdx, 

    // This is the weight factor that controls how much of the transformation should be applied. 
    // The weight allows for blending or partial influence, meaning the transformation is not applied fully but is weighted according to this 
    // value (ranging from 0.0 to 1.0, where 0.0 means no influence and 1.0 means full influence).
    afxReal weight, 

    // The skeleton model associated with the character. It defines the joint hierarchy and structure of the model. 
    // The skeleton holds the relative transformations of all joints and is used to apply the local transformation data to the right joint in the skeleton.
    arxModel skl, 

    // This is the blending operation to be used for combining the new transformation (t) with the existing transformation of the joint.
    afxQuatBlend blendOp, 

    // This is a pointer to the transformation that you want to apply to the joint. 
    // The transformation is represented by a combination of position (translation), rotation (typically a quaternion), 
    // and scale (if applicable). The transformation will be applied to the joint in the skeleton based on the specified weight and the 
    // blending operation (blendOp).
    afxTransform const* t
);

#endif//ARX_POSE_H
