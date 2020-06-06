#include <stdio.h>

#include "ShivourEngine.h"

#include "daveTime.h"



class myNoise : public ShivourNoiseFunc {
public:
	virtual float noise(float x, float y, float z) { 
		//return (sinf(x) + sinf(z) - y)*2 + 4;
		//return (rand()%100)/50. - 1 ;
		return (x - 5.1) * (x - 5.1) + (y - 5.1) * (y - 5.1) + (z - 5.1) * (z - 5.1) - 25;
		//return sinf(1 / (x-2.2))-y+2.2;
	};
};

int main(int iargs, char** argv) 
{

	printf("hi\n");
	if (!shivourInit()) {
		printf("err in init\n");
		shivourDeInit();
		return -1;
	}

	{//extra variable scope to ensure shiv deconstructs before sdl is shut down
		ShivourEngine shiv = ShivourEngine(1600, 900);

		if (!shiv.isWindowValid())
		{
			return -1;
		}

		std::vector<int> positions;

		myNoise noise;

		int shader = shiv.addShader("res\\shad\\basic.vs", "res\\shad\\light.fs");
		float modDat[] = { 0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f };
		int model1 = shiv.addModelMarchingSquares(220, 220, 220, 0.05f, &noise);
		int model2 = shiv.addModel("res\\models\\cube2.obj");
		int model = model2;
		int textures = shiv.addTextures("res\\textures\\ains.dds");
		int camera = shiv.addCamera();
		int light = shiv.addLightingScene(1, 1.3, 0.8, 1, 1, 1);

		for (int i = 0; i < 1; ++i) {
			int pos = shiv.addPosition();
			positions.push_back(pos);
			shiv.setPositionPos(pos, i/10, 0, i % 10);
			//shiv.setPositionScale(pos, 0.25f, 0.25f, 0.25f);
			shiv.addRenderable(shader, model, textures, camera, pos, light);
		}

		shiv.setCameraPos(camera, 0, 0, 5.0f);
		shiv.setCameraProjection(camera, 0.5f);

		float position = 0;

		SDL_GameController* gamec = SDL_GameControllerOpen(0);
		Timer t;
		t.init();
		while (!shiv.windowShouldClose())
		{

			SDL_GameControllerUpdate();
			int xoff = SDL_GameControllerGetAxis(gamec, SDL_CONTROLLER_AXIS_RIGHTX);
			int yoff = SDL_GameControllerGetAxis(gamec, SDL_CONTROLLER_AXIS_RIGHTY);
			int xmov = SDL_GameControllerGetAxis(gamec, SDL_CONTROLLER_AXIS_LEFTX);
			int ymov = SDL_GameControllerGetAxis(gamec, SDL_CONTROLLER_AXIS_LEFTY);

			float xRot = abs(xoff) > 10000 ? xoff / 1000000.0f : 0;
			float yRot = abs(yoff) > 10000 ? yoff / 1000000.0f : 0;
			float xmvv = abs(xmov) > 10000 ? xmov / 1000000.0f : 0;
			float ymvv = abs(ymov) > 10000 ? ymov / 1000000.0f : 0;

			position += 0.01;

			for (int &pos : positions) {
				//shiv.addPositionRotation(pos, 0.01f/3.141592f, 0.01, 0);
			}
			//printf("%f %f %f %f \n", ymvv, xmvv, yRot, yRot);
			shiv.addCameraRotation(camera, yRot, xRot, 0);
			shiv.moveCamera(camera, xmvv, 0, ymvv);
			shiv.doEngineUpdate();
			//printf("%f\n", 1/t.getElapsedTime());
		}
		SDL_GameControllerClose(0);
	}

	shivourDeInit();
	return 0;
}