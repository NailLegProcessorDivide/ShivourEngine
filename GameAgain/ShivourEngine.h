#pragma once
#include <SDL.h>
#include <cstdint>

#include "ShivourRenderer.h"

bool shivourInit();
void shivourDeInit();

class ShivourController;

class ShivourEngine {
private:
	SDL_Window * window;
	SDL_GLContext glContext;

	ShivourRenderer renderer;

	std::set<ShivourRenderable, ShivourRenderableCompare> renderables;

	bool windowValid;
	bool shouldClose;
private:
	int loggerInfo;
public:
	ShivourEngine();
	ShivourEngine(uint32_t width, uint32_t height);
	ShivourEngine(const char*, uint32_t width, uint32_t height);
	~ShivourEngine();

	void doEngineUpdate();

	int addShader(const char*, const char*);
	void removeShader(int);

	int addModel(const float* verticies, int triCount);
	int addModel(const float* verticies, const float* normals, const float* textureCoords, int triCount);
	int addModel(const char* fileName);
	int addModelMarchingSquares(int width, int height, int depth, float resolution, ShivourNoiseFunc* noise);
	void removeModel(int);

	int addTextures();
	int addTextures(const char*);
	void removeTextures(int);

	int addCamera();
	void removeCamera(int);
	void setCameraPos(int id, float x, float y, float z);
	void setCameraRotation(int id, float x, float y, float z);
	void addCameraPos(int id, float x, float y, float z);
	void addCameraRotation(int id, float x, float y, float z);
	void moveCamera(int id, float x, float y, float z);
	void setCameraProjection(int id, float angle = 0.5f , float aspectRatio = 16.f/9, float minDepth = 0.1f, float maxDepth = 100);

	int addPosition();
	void removePosition(int);
	void setPositionPos(int id, float x, float y, float z);
	void setPositionRotation(int id, float x, float y, float z);
	void setPositionScale(int id, float x, float y, float z);
	void addPositionPos(int id, float x, float y, float z);
	void addPositionRotation(int id, float x, float y, float z);
	void addPositionScale(int id, float x, float y, float z);
	void scalePositionScale(int id, float x, float y, float z);

	int addLightingScene(float x, float y, float z, float r, float g, float b);
	void removeLightingScene(int id);

	void addRenderable(int shader, int model, int textures, int camera, int position, int light);

	bool isWindowValid();
	bool windowShouldClose();

};