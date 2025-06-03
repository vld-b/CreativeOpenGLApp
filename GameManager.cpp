#include "GameManager.h"

GameManager::GameManager() : deltaTime(deltaTime_) {
	this->lastFrameTime_ = 0.0f;
	this->deltaTime_ = 0.0f;
}

void GameManager::startFrame(double time) {
	this->lastFrameTime_ = time;
}

void GameManager::endFrame(double time) {
	this->deltaTime_ = time - this->lastFrameTime_;
}