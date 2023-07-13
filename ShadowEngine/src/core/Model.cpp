#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "graphics/Shader.h"
#include <SOIL/SOIL.h>
#include "graphics/ShaderPrograms.h"
#include "core/Camera.h"
#include <core/Utils.h>
#include <core/Log.h>
#include <graphics/Light.h>
#include <core/Physics.h>
#include "core/Cube.h"
#include <graphics\Shadow.h>

std::map<std::string, const aiScene*> Model::mSceneMapping;
Assimp::Importer* Model::mActiveImporter = nullptr;


Model::Model(Application& application)
	: Drawable(application)
{
	mScene = nullptr;
}


Model::~Model()
{
	mImport.FreeScene();
}

void Model::setPos(const vec3& pos)
{
	Drawable::setPos(pos);
	mPhysicalBody.setTransform(pos, mPhysicalBody.getRotation());
}

void Model::setRotation(const vec3& rotation)
{
	Drawable::setRotation(rotation);
	mPhysicalBody.setTransform(mPhysicalBody.getPos(), mRotation);
}

void Model::update()
{
	mPos = mPhysicalBody.getPos();
	mRotation = mPhysicalBody.getRotation();
}

void Model::initShaders(Shader* shaderProgram)
{
	for (unsigned int i = 0; i < MAX_BONES; i++) // get location all matrices of bones
	{
		std::string name = "bones[" + std::to_string(i) + "]"; // name like in shader
		mBoneLocation[i] = glGetUniformLocation(shaderProgram->id(), name.c_str());
	}
}

void Model::initPhysicalBody(const vec3& miValue, const vec3& maValue, std::vector<Vertex> vertices)
{
	// calculate min and max verticies
	vec3 minValue = vec3(10000000.0f);
	vec3 maxValue = vec3(-1000000.0f);

	for (Vertex vertex : vertices)
	{
		if (vertex.position.x > maxValue.x)
		{
			maxValue.x = vertex.position.x;
		}
		if (vertex.position.y > maxValue.y)
		{
			maxValue.y = vertex.position.y;
		}
		if (vertex.position.z > maxValue.z)
		{
			maxValue.z = vertex.position.z;
		}

		if (vertex.position.x < minValue.x)
		{
			minValue.x = vertex.position.x;
		}
		if (vertex.position.y < minValue.y)
		{
			minValue.y = vertex.position.y;
		}
		if (vertex.position.z < minValue.z)
		{
			minValue.z = vertex.position.z;
		}
	}

	vec3 position = minValue + (maxValue - minValue) / 2.0f; // calculate center of cube
	vec3 scale = maxValue - minValue;

	// add collision box
	rp3d::BoxShape* cubeShape = new rp3d::BoxShape(toReact((scale) / 2.0f));
	rp3d::Transform transform;
	transform.setPosition(toReact(position));
	mPhysicalBody.addCollisionShape(cubeShape, transform, 10.0);

	// draw debug cubes
	/*Cube* cube = &mUpdateableContainer.createUpdateable<Cube>();
	cube->setPos(position);
	cube->setScale(scale);
	cube->getPhysicalBody().setType(BodyType::STATIC);
	mApplication.getWorldLayer().add(*cube);*/


	// TODO Weight, change model


	/*unsigned int nbVertices = vertices.size() * 3;
	unsigned int nbTriangles = indices.size() / 3;

	float* verticesArray = new float[3 * nbVertices];

	for (int i = 0; i < vertices.size(); i++)
	{
		verticesArray[i * 3] = vertices[i].position.x;
		verticesArray[i * 3 + 1] = vertices[i].position.y;
		verticesArray[i * 3 + 2] = vertices[i].position.z;
	}

	int* indicesArray = new int[(int)(3) * nbTriangles];
	for (int i = 0; i < indices.size(); i++)
	{
		indicesArray[i] = indices[i];
	}

	rp3d::TriangleVertexArray* triangleArray =
		new rp3d::TriangleVertexArray(nbVertices, verticesArray, 3 * sizeof(float), nbTriangles,
			indicesArray, 3 * sizeof(int), 
			rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	rp3d::TriangleMesh* triangleMesh = new rp3d::TriangleMesh();

	// Add the triangle vertex array to the triangle mesh 
	triangleMesh->addSubpart(triangleArray);

	// Create the concave mesh shape 
	rp3d::ConcaveMeshShape* concaveMesh = new rp3d::ConcaveMeshShape(triangleMesh);
	mPhysicalBody.addCollisionShape(concaveMesh, rp3d::Transform(), 0);*/
}

void Model::draw(bool shadowPass)
{
	if (mMeshes.size() == 0 && mModelPath != "")
	{
		loadModel();
	}
	
	if (mMeshes.size() > 0 && mVisible)
	{
		// -- setup uniforms --
		// mvp
		Shader* activeShader = shadowPass ? mDepthShader : mShader;
		activeShader->bind();
		glm::mat4 model = createModel(mPos, mScale, mRotation);
		glm::mat4 mvp = Camera::getProjection() * Camera::getView() * model;
		activeShader->addUniform("mvp", mvp);
		activeShader->addUniform("model", model);
		glm::mat4 normalsCube = glm::mat4(glm::transpose(glm::inverse(model)));
		activeShader->addUniform("normalsMatrix", normalsCube);

		// lights
		std::vector<DirLight*> dirLights = Light::getDirLights();
		for (int i = 0; i < dirLights.size(); i++)
		{
			activeShader->addUniform("dirLights[" + std::to_string(i) + "].direction", dirLights[i]->direction);
			activeShader->addUniform("dirLights[" + std::to_string(i) + "].ambient", dirLights[i]->ambient);
			activeShader->addUniform("dirLights[" + std::to_string(i) + "].diffuse", dirLights[i]->diffuse);
			activeShader->addUniform("dirLights[" + std::to_string(i) + "].specular", dirLights[i]->specular);
		}

		std::vector<PointLight*> pointLights = Light::getPointLights();
		for (int i = 0; i < pointLights.size(); i++)
		{
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].ambient", pointLights[i]->ambient);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].constant", pointLights[i]->constant);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i]->diffuse);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].linear", pointLights[i]->linear);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].position", pointLights[i]->position);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i]->quadratic);
			activeShader->addUniform("pointLights[" + std::to_string(i) + "].specular", pointLights[i]->specular);
		}
		
		// draw
		if (mBoneReloadTimes-- > 0 || isAnimationPlaying())
		{
			boneTransform((double)glfwGetTime(), mTransforms);
		}
		for (unsigned int i = 0; i < mTransforms.size(); i++) // move all matrices for actual model position to shader
		{
			glUniformMatrix4fv(mBoneLocation[i], 1, GL_TRUE, (const GLfloat*)&mTransforms[i]);
		}

		activeShader->addUniform("lightSpaceMatrix", Shadow::getLightSpaceMatrix());
		if (!shadowPass)
		{
			mShader->addTexture(3, "depthMap");
			mShader->addUniform("material.shininess", mShininess);
		}

		for (int i = 0; i < mMeshes.size(); i++)
		{
			mMeshes[i].Draw(*activeShader);
		}
	}
}

const aiScene* Model::readFile(const std::string& pFile, unsigned int pFlags)
{
	return mActiveImporter->ReadFile(pFile, pFlags);
}

void Model::loadModel()
{
	// how work skeletal animation in assimp //translated with google =) :
	// node is a separate part of the loaded model (the model is not only a character)
	// for example, the camera, armature, cube, light source, part of the character's body (leg, rug, head).
	// a bone can be attached to the node
	// in the bone there is an array of vertices on which the bone affects (weights from 0 to 1).
	// each mChannels is one aiNodeAnim.
	// In aiNodeAnim accumulated transformations (scaling rotate translate) for the bone with which they have the common name
	// these transformations will change those vertices whose IDs are in the bone with a force equal to the weight.
	// the bone simply contains the ID and the weight of the vertices to which the transformation from aiNodeAnim is moving (with no common name for the bone)
	// (the vertex array and the weight of the transforms for each vertex are in each bone)

	// result: a specific transformation will affect a particular vertex with a certain force.

	mActiveImporter = &mImport;
	mScene = getMappedValue(mSceneMapping, mModelPath, Model::readFile, mModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
	//mScene = mImport.ReadFile(mModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!mScene || mScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !mScene->mRootNode)
	{
		LOG_ERROR(mImport.GetErrorString(), false);
		return;
	}
	mGlobalInverseTransform = mScene->mRootNode->mTransformation;
	mGlobalInverseTransform.Inverse();
	
	if (mScene->HasAnimations() && mScene->mAnimations[0]->mTicksPerSecond != 0.0)
	{
		mTicksPerSecond = (float)mScene->mAnimations[0]->mTicksPerSecond;
	}
	else
	{
		mTicksPerSecond = 25.0f;
	}

	// mDirectory = container for model.obj and textures and other files
	mDirectory = mModelPath.substr(0, mModelPath.find_last_of('/'));

	processNode(mScene->mRootNode, mScene);

	mShader = &ShaderPrograms::getModelShader(false);
	mDepthShader = &ShaderPrograms::getModelShader(true);
	initShaders(mShader);
	initShaders(mDepthShader);
}

void Model::setShininess(float shininess)
{
	mShininess = shininess;
}

void Model::playAnimation(int animationIndex, bool repeat)
{
	mAnimationIndex = animationIndex;
	mAnimationStartTime = (float)glfwGetTime();
	mRepeat = repeat;
}

void Model::stopAnimation()
{
	playAnimation(-1, false);
}

void Model::resetAnimation()
{
	mAnimationStartTime = glfwGetTime();
	mBoneReloadTimes = 2;
}

bool Model::isAnimationPlaying()
{
	if (mAnimationIndex == -1)
	{
		return false;
	}
	double timeInTicks = (glfwGetTime() - mAnimationStartTime) * mTicksPerSecond;
	float animationTime = timeInTicks / mScene->mAnimations[mAnimationIndex]->mDuration;
	return animationTime < mScene->mAnimations[mAnimationIndex]->mDuration;
}

void Model::setModelPath(const std::string& modelPath)
{
	mModelPath = modelPath;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	Mesh mesh;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		mesh = processMesh(aiMesh, scene);
		mMeshes.push_back(mesh); //accumulate all meshes in one vector
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<VertexBoneData> bonesIdWeightsForEachVertex;

	vertices.reserve(mesh->mNumVertices); 
	indices.reserve(mesh->mNumVertices);

	bonesIdWeightsForEachVertex.resize(mesh->mNumVertices);

	vec3 maxVal(-100000);
	vec3 minVal(100000000);

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (vector.x > maxVal.x)
		{
			maxVal.x = vector.x;
		}
		if (vector.y > maxVal.y)
		{
			maxVal.y = vector.y;
		}
		if (vector.z > maxVal.z)
		{
			maxVal.z = vector.z;
		}

		if (vector.x < minVal.x)
		{
			minVal.x = vector.x;
		}
		if (vector.y < minVal.y)
		{
			minVal.y = vector.y;
		}
		if (vector.z < minVal.z)
		{
			minVal.z = vector.z;
		}
		

		if (mesh->mNormals != NULL)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		else
		{
			vertex.normal = glm::vec3();
		}


		// in assimp model can have 8 different texture coordinates
		// we only care about the first set of texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.textCoords = vec;
		}
		else
		{
			vertex.textCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	initPhysicalBody(minVal, maxVal, vertices);

	//material
	if (mesh->mMaterialIndex >= 0)
	{
		//all pointers created in assimp will be deleted automaticaly when we call import.FreeScene();
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		bool exist = false;
		for (int i = 0; (i < textures.size()) && (diffuseMaps.size() != 0); i++)
		{
			if (textures[i].path == diffuseMaps[0].path)
			{
				exist = true;
			}
		}
		if (!exist && diffuseMaps.size() != 0) textures.push_back(diffuseMaps[0]);

		std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		exist = false;
		for (int i = 0; (i < textures.size()) && (specularMaps.size() != 0); i++)
		{
			if (textures[i].path == specularMaps[0].path)
			{
				exist = true;
			}
		}
		if (!exist && specularMaps.size() != 0) textures.push_back(specularMaps[0]);
	}
	
	// load bones
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		unsigned int boneIndex = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		if (mBoneMapping.find(boneName) == mBoneMapping.end())
		{
			// Allocate an index for a new bone
			boneIndex = mNumBones;
			mNumBones++;
			BoneMatrix bi;
			mBoneMatrices.push_back(bi);
			mBoneMatrices[boneIndex].offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
			mBoneMapping[boneName] = boneIndex;
		}
		else
		{
			boneIndex = mBoneMapping[boneName];
		}

		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			unsigned int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bonesIdWeightsForEachVertex[vertex_id].addBoneData(boneIndex, weight);
		}
	}
	
	return Mesh(vertices, indices, textures, bonesIdWeightsForEachVertex);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString aiStr;
		mat->GetTexture(type, i, &aiStr);

		std::string filename = std::string(aiStr.C_Str());
		filename = mDirectory + '/' + filename;

		Texture texture;
		texture.id = generateTexture(filename);
		texture.type = typeName;
		texture.path = aiStr;
		textures.push_back(texture);
	}
	return textures;
}

unsigned int Model::findPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

unsigned int Model::findRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

unsigned int Model::findScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
	for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

aiVector3D Model::calcInterpolatedPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumPositionKeys == 1)
	{
		return nodeAnim->mPositionKeys[0].mValue;
	}

	unsigned int positionIndex = findPosition(animationTime, nodeAnim);
	unsigned int nextPositionIndex = positionIndex + 1;
	assert(nextPositionIndex < nodeAnim->mNumPositionKeys);
	
	float deltaTime = (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
	
	float factor = (animationTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
	
	aiVector3D start = nodeAnim->mPositionKeys[positionIndex].mValue;
	aiVector3D end = nodeAnim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Model::calcInterpolatedRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumRotationKeys == 1)
	{
		return nodeAnim->mRotationKeys[0].mValue;
	}

	unsigned int rotationIndex = findRotation(animationTime, nodeAnim);
	unsigned int nextRotationIndex = rotationIndex + 1;
	assert(nextRotationIndex < nodeAnim->mNumRotationKeys);
	
	float deltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
	float factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;

	aiQuaternion startQuat = nodeAnim->mRotationKeys[rotationIndex].mValue;
	aiQuaternion endQuat = nodeAnim->mRotationKeys[nextRotationIndex].mValue;

	return nlerp(startQuat, endQuat, factor);
}

aiVector3D Model::calcInterpolatedScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumScalingKeys == 1)
	{
		return nodeAnim->mScalingKeys[0].mValue;
	}

	unsigned int scalingIndex = findScaling(animationTime, nodeAnim);
	unsigned int nextScalingIndex = scalingIndex + 1;
	assert(nextScalingIndex < nodeAnim->mNumScalingKeys);
	
	float deltaTime = (float)(nodeAnim->mScalingKeys[nextScalingIndex].mTime - nodeAnim->mScalingKeys[scalingIndex].mTime);
	
	float  factor = (animationTime - (float)nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
	
	aiVector3D start = nodeAnim->mScalingKeys[scalingIndex].mValue;
	aiVector3D end = nodeAnim->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

unsigned int Model::generateTexture(const std::string& path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	int width, height, channels;
	unsigned char* data;
	data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (!data)
	{
		LOG_ERROR("Failed to load texture " + path, false);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones

	if (animation == nullptr)
	{
		return nullptr;
	}

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = animation->mChannels[i];
		if (std::string(node_anim->mNodeName.data) == nodeName)
		{
			return node_anim;
		}
	}

	return nullptr;
}
// start from RootNode
void Model::readNodeHierarchy(float animationTime, const aiNode* node, const aiMatrix4x4 parentTransform)
{
	std::string nodeName(node->mName.data);

	const aiAnimation* animation = nullptr;
	if (mScene->HasAnimations())
	{
		if (mAnimationIndex == -1)
		{
			animation = mScene->mAnimations[0];
		}
		else
		{
			animation = mScene->mAnimations[mAnimationIndex];
		}
	}

	aiMatrix4x4 nodeTransform = node->mTransformation;

	const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

	if (nodeAnim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scalingVector = calcInterpolatedScaling(animationTime, nodeAnim);
		aiMatrix4x4 scalingMatr;
		aiMatrix4x4::Scaling(scalingVector, scalingMatr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotateQuat = calcInterpolatedRotation(animationTime, nodeAnim);
		aiMatrix4x4 rotateMatr = aiMatrix4x4(rotateQuat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translateVector = calcInterpolatedPosition(animationTime, nodeAnim);
		aiMatrix4x4 translateMatr;
		aiMatrix4x4::Translation(translateVector, translateMatr);

		if (std::string(nodeAnim->mNodeName.data) == std::string("Head"))
		{
			aiQuaternion rotateHead = aiQuaternion(mRotateHeadXZ.w, mRotateHeadXZ.x, mRotateHeadXZ.y, mRotateHeadXZ.z);

			nodeTransform = translateMatr * (rotateMatr * aiMatrix4x4(rotateHead.GetMatrix())) * scalingMatr;
		}
		else
		{
			nodeTransform = translateMatr * rotateMatr * scalingMatr;
		}

	}

	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

	if (mBoneMapping.find(nodeName) != mBoneMapping.end()) // true if node_name exist in bone_mapping
	{
		unsigned int bone_index = mBoneMapping[nodeName];
		mBoneMatrices[bone_index].finalWorldTransform = mGlobalInverseTransform * globalTransform * mBoneMatrices[bone_index].offsetMatrix;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		readNodeHierarchy(animationTime, node->mChildren[i], globalTransform);
	}

}

void Model::boneTransform(double timeInSec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identityMatrix;

	double timeInTicks = (timeInSec - mAnimationStartTime) * mTicksPerSecond;
	float animationTime;

	if (mRepeat && mAnimationIndex != -1)
	{
		animationTime = fmod((float)timeInTicks, mScene->mAnimations[mAnimationIndex]->mDuration);
	}
	else if (mAnimationIndex == -1)
	{
		animationTime = 0.0f;
	}
	else
	{
		animationTime = (float)timeInTicks / mScene->mAnimations[mAnimationIndex]->mDuration;
	}

	if (mAnimationStartTime == -1.0f)
	{
		animationTime = 0.0f;
	}
	else if (mAnimationIndex == -1)
	{
	}
	else if (animationTime >= mScene->mAnimations[mAnimationIndex]->mDuration)
	{
		animationTime = (float)mScene->mAnimations[mAnimationIndex]->mDuration - 1;
	}

	readNodeHierarchy(animationTime, mScene->mRootNode, identityMatrix);

	transforms.resize(mNumBones);

	for (unsigned int i = 0; i < mNumBones; i++)
	{
		transforms[i] = mBoneMatrices[i].finalWorldTransform;
	}
}

glm::mat4 Model::aiToGlm(aiMatrix4x4 aiMatr)
{
	glm::mat4 result;
	result[0].x = aiMatr.a1; result[0].y = aiMatr.b1; result[0].z = aiMatr.c1; result[0].w = aiMatr.d1;
	result[1].x = aiMatr.a2; result[1].y = aiMatr.b2; result[1].z = aiMatr.c2; result[1].w = aiMatr.d2;
	result[2].x = aiMatr.a3; result[2].y = aiMatr.b3; result[2].z = aiMatr.c3; result[2].w = aiMatr.d3;
	result[3].x = aiMatr.a4; result[3].y = aiMatr.b4; result[3].z = aiMatr.c4; result[3].w = aiMatr.d4;

	return result;
}

aiQuaternion Model::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}
