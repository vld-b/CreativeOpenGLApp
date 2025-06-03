#ifndef GAMEMANAGER_CLASS_H
#define GAMEMANAGER_CLASS_H

class GameManager {
private:
	double deltaTime_;
	double lastFrameTime_;

public:
	const double& deltaTime;

	GameManager();

	void startFrame(double time);
	void endFrame(double time);
};

#endif // !GAMEMANAGER_CLASS_H
