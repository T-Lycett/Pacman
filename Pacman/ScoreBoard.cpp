#include "ScoreBoard.h"

#include <fstream>
#include <sstream>

ScoreBoard::ScoreBoard()
{
	for (int iii = 0; iii < 10; iii++)
	{
		_highScores[iii] = new HighScore();
	}
}


ScoreBoard::~ScoreBoard()
{
	for (int iii = 0; iii < 10; iii++)
	{
		delete _highScores[iii];
	}
	delete[] _highScores;
}


void ScoreBoard::Load()
{
	ifstream highScoresFile;
	highScoresFile.open("ScoreBoard/highScores.txt");
	for (int iii = 0; iii < 10; iii++)
	{
		highScoresFile >> _highScores[iii]->name;
		highScoresFile >> _highScores[iii]->score;
	}
	highScoresFile.close();
}


void ScoreBoard::Draw()
{
	std::stringstream stream;
	for (int iii = 0; iii < 10; iii++)
	{
		stream << (iii + 1) << ". " << _highScores[iii]->name << " " << _highScores[iii]->score << endl;
	}


	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), new Vector2(400.0f, 100.0f), Color::Green);
}


void ScoreBoard::AddScore(string name, int score)
{
	int rank = 0;
	for (int iii = 9; iii >= 0; iii--)
	{
		if (score > _highScores[iii]->score)
		{
			for (int jjj = 8; jjj >= iii; jjj--)
			{
				_highScores[jjj + 1]->name = _highScores[jjj]->name;
				_highScores[jjj + 1]->score = _highScores[jjj]->score;
			}
			rank = iii + 1;
		}
	}
	if (rank != 0)
	{
		_highScores[rank - 1]->name = name;
		_highScores[rank - 1]->score = score;
	}

	ofstream highScoresFile;
	highScoresFile.open("ScoreBoard/highScores.txt");
	for (int iii = 0; iii < 10; iii++)
	{
		highScoresFile << _highScores[iii]->name << endl << _highScores[iii]->score << endl;
	}
	highScoresFile.close();
}