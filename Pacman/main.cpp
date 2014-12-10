#include "Pacman.h"
#include <iostream>

//Entry Point for Application
int main(int argc, char* argv[]) {
	int munchieCount;
	cout << "How many munchies?" << endl;
	cin >> munchieCount;

	Pacman* game = new Pacman(argc, argv, munchieCount);
}