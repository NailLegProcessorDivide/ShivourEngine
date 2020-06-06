#include "ShivourEngine.h"

#include <stdio.h>

#include <windows.h>//yay
#include <gl/GL.h>
#include <gl/GLU.h>

#include "daveTime.h"

jstl::LogMaster jstl::logger;
int jstl::timerLogID;

bool shivourInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("error in sdl init: %s\n", SDL_GetError());
		return false;
	}
	printf("initialised sdl\n");
	return true;
}
void shivourDeInit() {
	SDL_Quit();
	printf("shutdown sdl\n");
}

ShivourEngine::ShivourEngine() :
	ShivourEngine("ShivourGame", 640, 480) 
{

}

ShivourEngine::ShivourEngine(uint32_t width, uint32_t height) :
	ShivourEngine("ShivourGame", width, height)
{

}

ShivourEngine::ShivourEngine(const char * title, uint32_t width, uint32_t height) :
	window(nullptr), glContext(NULL), shouldClose(false), windowValid(false), 
	renderables()
{
	ADDLOG(loggerInfo, "shiv.info", stdout);
	if (SDL_WasInit(0) != SDL_INIT_EVERYTHING)
	{
		shivourInit();
		if (SDL_WasInit(0) != SDL_INIT_EVERYTHING)
		{
			return;
		}
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		JLOG(loggerInfo, "unable to create window: %s\n", SDL_GetError());
		return;
	}
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) 
	{
		JLOG(loggerInfo, "error initialising opengl context: %s\n", SDL_GetError());
		return;
	}
	glewInit();
	windowValid = true;
	renderer.init();


	JLOG(loggerInfo, "max uniforms: %i\n", GL_MAX_UNIFORM_LOCATIONS);

	JLOG(loggerInfo, "num joysticks connected: %i\n", SDL_NumJoysticks());

	SDL_Joystick* controller = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		controller = SDL_JoystickOpen(i);
		JLOG(loggerInfo, "controller %i: %s\n", i, SDL_JoystickName(controller));
		SDL_JoystickClose(controller);
	}
}

ShivourEngine::~ShivourEngine()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	window = nullptr;
	JLOG(loggerInfo, "closed window\n");
}


void ShivourEngine::doEngineUpdate()
{
	jstl::Timer t("engine loop took %.3f ms\n");
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			shouldClose = true;
		}
	}
	glClearColor(0.5f, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	{
		jstl::Timer rt("render loop took %.3f ms\n");
		renderer.render(renderables);
	}
	SDL_GL_SwapWindow(window);
	JLOG(loggerInfo, "rendered\n");
}


int ShivourEngine::addShader(const char* vsn, const char* fsn) {
	return renderer.addShader(vsn, fsn);
}
void ShivourEngine::removeShader(int shaderID) {
	renderer.removeShader(shaderID);
}

int ShivourEngine::addModel(const float* data, int triCount) {
	return renderer.addModel(data, triCount);
}
int ShivourEngine::addModel(const float* verticies, const float* normals, const float* textureCoords, int triCount) {
	return renderer.addModel(verticies, normals, textureCoords, triCount);
}

int ShivourEngine::addModelMarchingSquares(int width, int height, int depth, float resolution, ShivourNoiseFunc* noise) {
	return renderer.addModelMarchingSquares(width, height, depth, resolution, noise);
}
int ShivourEngine::addModel(const char* fileName) {
	return renderer.addModel(fileName);
}
void ShivourEngine::removeModel(int modelID) {
	renderer.removeModel(modelID);
}

int ShivourEngine::addTextures() {
	return renderer.addTextures();
}
int ShivourEngine::addTextures(const char* fileName) {
	return renderer.addTextures(fileName);
}
void ShivourEngine::removeTextures(int textureID) {
	renderer.removeTextures(textureID);
}

int ShivourEngine::addCamera() {
	return renderer.addCamera();
}
void ShivourEngine::removeCamera(int cameraID) {
	renderer.removeCamera(cameraID);
}
void ShivourEngine::setCameraPos(int id, float x, float y, float z) {
	renderer.setCameraPos(id, x, y, z);
}
void ShivourEngine::setCameraRotation(int id, float x, float y, float z) {
	renderer.setCameraRotation(id, x, y, z);
}
void ShivourEngine::addCameraPos(int id, float x, float y, float z) {
	renderer.addCameraPos(id, x, y, z);
}
void ShivourEngine::addCameraRotation(int id, float x, float y, float z) {
	renderer.addCameraRotation(id, x, y, z);
}
void ShivourEngine::moveCamera(int id, float x, float y, float z) {
	renderer.moveCamera(id, x, y, z);
}



int ShivourEngine::addPosition() {
	return renderer.addPosition();
}
void ShivourEngine::removePosition(int positionID) {
	renderer.removePosition(positionID);
}
void ShivourEngine::setPositionPos(int id, float x, float y, float z) {
	renderer.setPositionPos(id, x, y, z);
}
void ShivourEngine::setPositionRotation(int id, float x, float y, float z) {
	renderer.setPositionRotation(id, x, y, z);
}
void ShivourEngine::setPositionScale(int id, float x, float y, float z) {
	renderer.setPositionScale(id, x, y, z);
}
void ShivourEngine::addPositionPos(int id, float x, float y, float z) {
	renderer.addPositionPos(id, x, y, z);
}
void ShivourEngine::addPositionRotation(int id, float x, float y, float z) {
	renderer.addPositionRotation(id, x, y, z);
}
void ShivourEngine::addPositionScale(int id, float x, float y, float z) {
	renderer.addPositionScale(id, x, y, z);
}
void ShivourEngine::scalePositionScale(int id, float x, float y, float z) {
	renderer.scalePositionScale(id, x, y, z);
}

void ShivourEngine::setCameraProjection(int id, float fov, float aspect, float nearP, float farP) {
	renderer.setCameraProjection(id, fov, aspect, nearP, farP);
}

void ShivourEngine::addRenderable(int shader, int model, int textures, int camera, int position, int light) {
	renderables.insert({ shader, model, textures, camera, position, light });
}

int ShivourEngine::addLightingScene(float x, float y, float z, float r, float g, float b) {
	return renderer.addLightingScene(x, y, z, r, g, b);
}

void ShivourEngine::removeLightingScene(int id) {
	renderer.removeLightingScene(id);
}

bool ShivourEngine::isWindowValid() 
{
	return windowValid;
}

bool ShivourEngine::windowShouldClose() 
{
	return shouldClose;
}