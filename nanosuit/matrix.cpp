#pragma once
 
#include"glm\glm.hpp"
#include"Weight.h"
 
class Vertex 
{
public:
	glm::vec3 Position;
	glm::vec3 animPosition;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
 
	Weight			Weights[VERTEX_MAX_BONE];  //限定每个顶点受 VERTEX_MAX_BONE 个骨骼影响;
 
};

    #pragma once
    #include"gles2\gl2.h"

    #define VERTEX_MAX_BONE 10

    class Weight
    {
    public:
    	GLuint		boneid; //骨骼id,要找到对应的Bone，取Bone中的offsetMatrix;
    	float		weight; //权重,用于将多个骨骼的变换组合成一个变换矩阵,一个顶点的所有骨骼权重之和必须为1;



    public:
    	Weight()
    	{
    		weight = 0;
    		boneid = 0;
    	}
    };

#pragma once

#include"glm\glm.hpp"


///model.h
class Bone
{
public:
	char				name[50];   //例如 joint1，与 Scene->Animation->Channels 中的Channel的name对应;
	glm::mat4 offsetMatrix; //顶点坐标做成offsetmatrix 从模型空间到骨骼空间;
	glm::mat4 finalMatrix;
};

for (size_t boneindex = 0; boneindex < mesh->mNumBones; boneindex++)
{
	Bone bone;

	aiBone* bonesrc = mesh->mBones[boneindex];

	memcpy(bone.name, bonesrc->mName.C_Str(), bonesrc->mName.length + 1);

	for (size_t xindex = 0; xindex < 4; xindex++)
	{
		for (size_t yindex = 0; yindex < 4; yindex++)
		{
			bone.offsetMatrix[xindex][yindex] = bonesrc->mOffsetMatrix[yindex][xindex];
		}
	}

	bones.push_back(bone);
}

///animotion
void processAnimation(const aiScene* scene)
{
	for (size_t animationindex = 0; animationindex < scene->mNumAnimations; animationindex++)
	{
		Animation animation;
		aiAnimation* animationsrc = scene->mAnimations[animationindex];
		
		//Animation 名字;
		memcpy(animation.name, animationsrc->mName.C_Str(), animationsrc->mName.length + 1);
 
		animation.duration = animationsrc->mDuration;
 
		animation.ticksPerSecond = animationsrc->mTicksPerSecond;
 
		animation.numChannels = animationsrc->mNumChannels;
 
		//处理这个Animation下的所有的Channel(一个joint的动画集合);
		for (size_t channelindex = 0; channelindex < animationsrc->mNumChannels; channelindex++)
		{
			AnimationChannel animationChannel;
 
			aiNodeAnim* channel = animationsrc->mChannels[channelindex];
 
			memcpy(animationChannel.nodeName, channel->mNodeName.C_Str(), channel->mNodeName.length);
			
 
 
			//位移动画;
			animationChannel.numPositionKeys = channel->mNumPositionKeys;
			for (size_t positionkeyindex = 0; positionkeyindex < channel->mNumPositionKeys; positionkeyindex++)
			{
				AnimationChannelKeyVec3 animationChannelKey;
				
				aiVectorKey vectorKey = channel->mPositionKeys[positionkeyindex];
 
				animationChannelKey.time = vectorKey.mTime;
				animationChannelKey.keyData.x = vectorKey.mValue.x;
				animationChannelKey.keyData.y = vectorKey.mValue.y;
				animationChannelKey.keyData.z = vectorKey.mValue.z;
 
				animationChannel.positionKeys.push_back(animationChannelKey);
			}
 
 
			
 
			//旋转动画;
			animationChannel.numRotationKeys = channel->mNumRotationKeys;
 
			for (size_t rotationkeyindex = 0; rotationkeyindex < channel->mNumRotationKeys; rotationkeyindex++)
			{
				AnimationChannelKeyQuat animationChannelKey;
 
				aiQuatKey quatKey = channel->mRotationKeys[rotationkeyindex];
 
				animationChannelKey.time = quatKey.mTime;
				animationChannelKey.keyData.x = quatKey.mValue.x;
				animationChannelKey.keyData.y = quatKey.mValue.y;
				animationChannelKey.keyData.z = quatKey.mValue.z;
				animationChannelKey.keyData.w = quatKey.mValue.w;
 
				animationChannel.rotationKeys.push_back(animationChannelKey);
			}
 
			//缩放动画;
			animationChannel.numScalingKeys = channel->mNumScalingKeys;
 
			for (size_t scalingindex = 0; scalingindex < channel->mNumScalingKeys; scalingindex++)
			{
				AnimationChannelKeyVec3 animationChannelKey;
 
				aiVectorKey vectorKey = channel->mScalingKeys[scalingindex];
 
				animationChannelKey.time = vectorKey.mTime;
				animationChannelKey.keyData.x = vectorKey.mValue.x;
				animationChannelKey.keyData.y = vectorKey.mValue.y;
				animationChannelKey.keyData.z = vectorKey.mValue.z;
 
				animationChannel.scalingKeys.push_back(animationChannelKey);
			}
 
			animation.channels.push_back(animationChannel);
		}
 
		animations.push_back(animation);
	}
}

///update vertex position
void OnDraw()
{
	framecount++;
 
	Node rootNode;
	for (size_t nodeindex = 0; nodeindex < nodes.size(); nodeindex++)
	{
		Node node = nodes[nodeindex];
		if (strcmp(node.parentName,"")==0)
		{
			rootNode = node;
			break;
		}
	};
	
	globalInverseTransform = rootNode.transformation;
	globalInverseTransform=glm::inverse(globalInverseTransform);
 
	transforms.resize(meshes[0].bones.size());
 
	glm::mat4 identity;
	glm::mat4 rootnodetransform;
	TransformNode(rootNode.name, framecount, identity * rootnodetransform);
 
	for (size_t boneindex = 0; boneindex < meshes[0].bones.size(); boneindex++)
	{
		transforms[boneindex] = meshes[0].bones[boneindex].finalMatrix;
	}
 
	//更新Vertex Position;
	for (size_t vertexindex = 0; vertexindex < meshes[0].vertices.size(); vertexindex++)
	{
		Vertex vertex = meshes[0].vertices[vertexindex];
		
		//glm::vec4 animPosition;
		glm::mat4 boneTransform;
		//计算权重;
		for (int weightindex = 0; weightindex < VERTEX_MAX_BONE; weightindex++)
		{
			Weight weight = vertex.Weights[weightindex];
			
			Bone bone = this->meshes[0].bones[weight.boneid];
 
			boneTransform += bone.finalMatrix * weight.weight;
 
			//animPosition += glm::vec4(vertex.Position, 1)* bone.offsetMatrix*weight.weight;
		}
 
		glm::vec4 animPosition(vertex.Position, 1.0f);
		animPosition = boneTransform * animPosition;
 
		vertex.animPosition = glm::vec3(animPosition);
		meshes[0].vertices[vertexindex] = vertex;
	}
}

globalInverseTransform = rootNode.transformation;
globalInverseTransform=glm::inverse(globalInverseTransform);

bone.finalMatrix =globalInverseTransform * parenttransform * nodeTransformation * bone.offsetMatrix ;

for (size_t boneindex = 0; boneindex < meshes[0].bones.size(); boneindex++)
{
	transforms[boneindex] = meshes[0].bones[boneindex].finalMatrix;
}

//更新Vertex Position;
for (size_t vertexindex = 0; vertexindex < meshes[0].vertices.size(); vertexindex++)
{
	Vertex vertex = meshes[0].vertices[vertexindex];

	//glm::vec4 animPosition;
	glm::mat4 boneTransform;
	//计算权重;
	for (int weightindex = 0; weightindex < VERTEX_MAX_BONE; weightindex++)
	{
		Weight weight = vertex.Weights[weightindex];

		Bone bone = this->meshes[0].bones[weight.boneid];

		boneTransform += bone.finalMatrix * weight.weight;

		//animPosition += glm::vec4(vertex.Position, 1)* bone.offsetMatrix*weight.weight;
	}

	glm::vec4 animPosition(vertex.Position, 1.0f);
	animPosition = boneTransform * animPosition;

	vertex.animPosition = glm::vec3(animPosition);
	meshes[0].vertices[vertexindex] = vertex;
}
}

