#ifndef __UI_H__
#define __UI_H__

#include "Module.h"
#include "Globals.h"

class ModuleUi : public Module
{
public:
	ModuleUi(Application* app, bool start_enabled = true);
	virtual ~ModuleUi();

	bool Start();
	update_status PreUpdate();

	bool CleanUp();

	void AddScore(int value);
	void Draw();
	void DrawHighScore();
	void UpdateScores();

public:
	int score;
	int prevScore;
	int highScore;

	int comboCount;
	int specialCount;

private:
	
};

#endif // !__UI_H__
