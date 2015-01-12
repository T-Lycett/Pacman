#pragma once
#include <string>
#include "S2D\S2D.h"

using namespace std;
using namespace S2D;

struct HighScore
{
	string name;
	int score = 0;
};

class ScoreBoard
{
private:
	HighScore* _highScores[10];
	int _score;
public:
	ScoreBoard();
	~ScoreBoard();

	void Load();
	void Draw();

	void AddScore(string name, int score);
};

