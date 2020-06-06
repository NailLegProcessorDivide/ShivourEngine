#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <set>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glm/gtx/quaternion.hpp>

#include "JMem.h"
#include "JLogging.h"

struct ShivourModelData;
class ShivourModelPosition;
class ShivourTextures;
class ShivourModel;
class ShivourCamera;
class ShivourShader;
struct ShivourRenderable;
struct ShivourRenderableCompare;
class ShivourRenderer;

enum SHIV_MODEL_COMPONENTS
{
	SHIV_MODEL_VERTS = 0x1,
	SHIV_MODEL_TEXCOS = 0x2,
	SHIV_MODEL_VERTCOLS = 0x4,
	SHIV_MODEL_NORMALS = 0x8,
	SHIV_MODEL_TEXTURE = 0x10,
	SHIV_MODEL_NOMAL_TEXTURE = 0x20,
	SHIV_MODEL_REFLECT_TEXTURE = 0x40,
};

struct ShivBasicLight {
	glm::vec3 lightPos;
	glm::vec3 lightCol;
};

class ShivourLights {
private:
	ShivBasicLight light;
public:
	ShivourLights(glm::vec3 pos, glm::vec3 col);
	~ShivourLights();

	void load(ShivourShader*);
};

struct ShivourModelData {
public:
	uint32_t modelInfo;
	uint32_t triCount;
	struct {
		GLuint vertexBufferID;
		GLuint normalBufferID;
		GLuint texBufferID;
	}buffers;
};

class ShivourModelPosition {
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
public:
	ShivourModelPosition();
	void load(ShivourShader* currentShader);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void addPosition(float x, float y, float z);
	void addRotation(float x, float y, float z);
	void addScale(float x, float y, float z);
	void mulScale(float x, float y, float z);
};

class ShivourTextures {
private:
	GLuint textureID;
public:
	ShivourTextures();
	ShivourTextures(const char *);

	static GLuint readTexture(const char*);

	~ShivourTextures();

	void bind(ShivourShader* currentShader);
};

class ShivourModel {
private:
	ShivourModelData modelData;
public:
	ShivourModel(const float*, int);
	ShivourModel(const float* verticies, const float* normals, const float* textureCoords, int triCount);
	ShivourModel(const char* fileName);
	~ShivourModel();
	void load();
	uint32_t getNumTris();
};

class ShivourCamera {
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::mat4 projection;
public:
	ShivourCamera();
	~ShivourCamera();
	void load(ShivourShader* currentShader);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void addPosition(float x, float y, float z);
	void addRotation(float x, float y, float z);
	void move(float x, float y, float z);
	void setProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
};

class ShivourShader {
private:
	uint32_t shaderInfo;
	GLuint program;
public:
	GLuint modelUniform;
	GLuint cameraUniform;
	GLuint camPos;
	GLuint lightPos;
	GLuint lightCol;
public:
	ShivourShader(uint32_t, GLuint);
	~ShivourShader();
	void bind();
};

struct ShivourRenderable {
	int shader;
	int model;
	int texture;
	int camera;
	int positionData;
	int lighting;
};
struct ShivourRenderableCompare {
	bool operator()(const ShivourRenderable& left, const ShivourRenderable& right) const;
};

class ShivourNoiseFunc {
public:
	virtual float noise(float x, float y, float z) { return 0; };
};

struct ShivRenderLog {
public:
	int rendererID;
	int rendererShaderID;
	int rendererPositionID;
	int rendererCameraID;
	int rendererModelID;
	int rendererTextureID;
	int mCubeLoggerID;
	int renderLightingID;
};

class ShivourRenderer {
private:
	jstl::Ticketer<ShivourShader*> shaders;
	jstl::Ticketer<ShivourModel*> models;
	jstl::Ticketer<ShivourTextures*> textures;
	jstl::Ticketer<ShivourCamera*> cameras;
	jstl::Ticketer<ShivourModelPosition*> positions;
	jstl::Ticketer<ShivourLights*> lighting;

private:
	ShivRenderLog renderLogger;

public:
	ShivourRenderer();
	~ShivourRenderer();

	int addShader(const char* , const char*);
	void removeShader(int);

	int addModel(const float*, int);
	int addModel(const float* verticies, const float* normals, const float* textureCoords, int triCount);
	int addModelMarchingSquares(int width, int height, int depth, float resolution, ShivourNoiseFunc* noise);
	int addModel(const char* file);
	void removeModel(int);

	int addTextures();
	int addTextures(const char* fileName);
	void removeTextures(int);

	int addCamera();
	void removeCamera(int);
	void setCameraPos(int id, float x, float y, float z);
	void setCameraRotation(int id, float x, float y, float z);
	void addCameraPos(int id, float x, float y, float z);
	void addCameraRotation(int id, float x, float y, float z);
	void moveCamera(int id, float x, float y, float z);
	void setCameraProjection(int id, float fov, float aspectRatio, float nearPlane, float farPlane);

	int addPosition();
	void removePosition(int id);
	void setPositionPos(int id, float x, float y, float z);
	void setPositionRotation(int id, float x, float y, float z);
	void setPositionScale(int id, float x, float y, float z);
	void addPositionPos(int id, float x, float y, float z);
	void addPositionRotation(int id, float x, float y, float z);
	void addPositionScale(int id, float x, float y, float z);
	void scalePositionScale(int id, float x, float y, float z);


	int addLightingScene(float x, float y, float z, float r, float g, float b);
	void removeLightingScene(int id);

	void init();
	GLuint makeShaderFromSource(const char* vs, const char* fs);

	void render(std::set<ShivourRenderable, ShivourRenderableCompare>& renderables);
};