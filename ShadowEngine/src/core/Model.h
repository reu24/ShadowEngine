#pragma once

#include "GL/glew.h"
#include "graphics/Mesh.h"
#include <vector>
#include <string>
#include <map>
#include "glm/gtc/quaternion.hpp"
#include "Drawable.h"
#include "graphics/Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <core/PhysicalBody.h>

// TODO Multiple animations
/*
	A 3D Model that can do animations and that can be rendered. 
	Add it to the game using mApplication.getWorldLayer().add()
*/
class Model : public Drawable
{
public:
	Model(Application& application);
	~Model();
	static const unsigned int MAX_BONES = 100;

	virtual void setPos(const vec3& pos) override;
	virtual void setRotation(const vec3& rotation) override;

	// Set how shiny the model should look like
	void setShininess(float shininess);
	// Plays the animation at animationIndex starting at 0.
	void playAnimation(int animationIndex, bool repeat);
	// Stops the currently playing animation and remains in the position.
	void stopAnimation();
	// Resets the animation to the base position.
	void resetAnimation();
	// Returns wheather an animation is currently playing or not.
	bool isAnimationPlaying();

	// Set the path where the animation is.
	void setModelPath(const std::string& modelPath);

	/*
		Returns the physical body of the object. 
		Use this body to perform physical actions 
		and to change the physical properties of the object.
	*/
	PhysicalBody& getPhysicalBody()
	{
		return mPhysicalBody;
	}

	void update() override;
	void draw(bool shadowPass) override;

protected:
	std::string mModelPath = "";
	PhysicalBody mPhysicalBody;
	float mShininess = 5.0f;

private:
	Assimp::Importer mImport;
	const aiScene* mScene;
	std::vector<Mesh> mMeshes;
	std::string mDirectory;

	std::map<std::string, unsigned int> mBoneMapping;
	unsigned int mNumBones = 0;
	std::vector<BoneMatrix> mBoneMatrices;
	aiMatrix4x4 mGlobalInverseTransform;

	GLuint mBoneLocation[MAX_BONES];
	float mTicksPerSecond = 0.0f;

	float mAnimationStartTime = -1.0f;
	int mAnimationIndex = -1;
	bool mRepeat = false;
	int mBoneReloadTimes = 2;
	std::vector<aiMatrix4x4> mTransforms;

	static std::map<std::string, const aiScene*> mSceneMapping;

	static Assimp::Importer* mActiveImporter;
	static const aiScene* readFile(const std::string& pFile, unsigned int pFlags);

	Shader* mShader = nullptr;
	Shader* mDepthShader = nullptr;

	void loadModel();

	void initShaders(Shader* shaderProgram);

	void initPhysicalBody(const vec3& minValue, const vec3& maxValue, std::vector<Vertex> vertices);

	glm::mat4 aiToGlm(aiMatrix4x4 aiMatr);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend); // super super n lerp =)

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int findPosition(float animationTime, const aiNodeAnim* nodeAnim);
	unsigned int findRotation(float animationTime, const aiNodeAnim* nodeAnim);
	unsigned int findScaling(float animationTime, const aiNodeAnim* nodeAnim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string node_name);
	// calculate transform matrix
	aiVector3D calcInterpolatedPosition(float animationTime, const aiNodeAnim* nodeAnim);
	aiQuaternion calcInterpolatedRotation(float animationTime, const aiNodeAnim* nodeAnim);
	aiVector3D calcInterpolatedScaling(float animationTime, const aiNodeAnim* nodeAnim);

	unsigned int generateTexture(const std::string& path);

	void readNodeHierarchy(float animationTime, const aiNode* node, const aiMatrix4x4 parentTransform);
	void boneTransform(double timeInSec, std::vector<aiMatrix4x4>& transforms);

	// rotate Head
	glm::quat mRotateHeadXZ = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
};

