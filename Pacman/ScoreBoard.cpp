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
	SpriteBatch::DrawString(stream.str().c_str(), new Vector2(600.0f, 200.0f), Color::Green);
}


void ScoreBoard::AddScore(string name, int score)
{
	int rank = 11;
	for (int iii = 9; iii >= 0; iii--)
	{
		if (score > _highScores[iii]->score)
		{
			
			rank = iii;
		}
	}
	for (int iii = 8; iii >= rank; iii--)
	{
		_highScores[iii + 1]->name = _highScores[iii]->name;
		_highScores[iii + 1]->score = _highScores[iii]->score;
	}
	if (rank < 10)
	{
		_highScores[rank]->name = name;
		_highScores[rank]->score = score;
	}

	ofstream highScoresFile;
	highScoresFile.open("ScoreBoard/highScores.txt");
	for (int iii = 0; iii < 10; iii++)
	{
		highScoresFile << _highScores[iii]->name << endl << _highScores[iii]->score << endl;
	}
	highScoresFile.close();
}