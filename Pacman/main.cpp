#include "Pacman.h"
#include <iostream>

//Entry Point for Application
int main(int argc, char* argv[]) {
	string playerName;
	cout << "Enter your name:" << endl;
	cin >> playerName;

	Pacman* game = new Pacman(argc, argv, playerName);
}