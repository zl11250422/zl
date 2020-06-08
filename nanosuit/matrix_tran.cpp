bone_transform = inverse(mRootNode->mTransformation) * aiBoneNode->parent->mTransformation * animation->RST matrix * aiBone->mOffsetMatrix; 
gl_Position = MVP * bone_transform * vertex; 

//offsetMatrix
scene->mMeshes[1] -> mBones[index] -> mOffsetMatrix
//transform matrix
parentTransform(transform, scene->mAnimations[idx]->mChannels[cIdx]->mNodeName);

inline void parentTransform(glm::mat4 &result, aiNode* node) { 
	if (node->mParent != 0x0) { 
		glm::mat4 tmp = aiMatrix4x4ToGlm(&node->mParent->mTransformation); 
		result = tmp * result; 
		parentTransform(result, node->mParent); 
	} 
} 

//key frame
glm::quat key_rotate( scene->mAnimations[idx]->mChannels[cIdx]->mRotationKeys[0].mValue.w, scene->mAnimations[idx]->mChannels[cIdx]->mRotationKeys[0].mValue.x, scene->mAnimations[idx]->mChannels[cIdx]->mRotationKeys[0].mValue.y, scene->mAnimations[idx]->mChannels[cIdx]->mRotationKeys[0].mValue.z ); 
glm::mat4 key_translate = glm::translate( glm::mat4(1.0f), glm::vec3( scene->mAnimations[idx]->mChannels[cIdx]->mPositionKeys[0].mValue.x, scene->mAnimations[idx]->mChannels[cIdx]->mPositionKeys[0].mValue.y, scene->mAnimations[idx]->mChannels[cIdx]->mPositionKeys[0].mValue.z ) ); 
