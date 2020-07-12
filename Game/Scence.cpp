#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath, LPCWSTR releaseScene)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->releaseScene = releaseScene;
}