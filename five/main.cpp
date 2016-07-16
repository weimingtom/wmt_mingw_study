#include <iostream>
#include "main.h"
#include "game.h"

// see also : https://github.com/tjumyk/Five_SDL
int main(int argc, char **argv) 
{
	using namespace std;
	
	cout << "[info] Gomoku loading..." << endl;
	Game *game = new Game;
	bool isExit = false;
	while (!isExit)
	{
		fflush(stdin);
		cin.clear();
		int stage = game->getStage();
		switch (stage)
		{
		case 1:
			{
				int currentType = game->chess->getCurrentPieceType();
				int currentPlayer = game->getPlayer(currentType);
				if (currentPlayer == 1)
				{
					cout << "[info][stage 1] AI work..." << endl;
					game->chess->AIWork(currentType);
					game->checkResult(currentType);
				}
				else if (currentPlayer == 0)
				{
					cout << "[warning][stage 1] not AI work turn" << endl;
				}
			}
			break;
		}
		stage = game->getStage();
		switch (stage)
		{
		case 0:
			{
				char ch;
				cout << "[info][stage 0] Start game?(Y/N)" << endl;
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					game->nextStage();
				}
			}
			break;
			
		case 1:
			{
				cout << "[info][stage 1] current piece type : " << game->chess->getCurrentPieceType() << endl;
				cout << "[info][stage 1] current player : " << game->getPlayer(game->chess->getCurrentPieceType()) << endl;
				for (int i = 0; i < 15 ; i++) 
				{
					for (int j = 0; j < 15; j++)
					{
						int type = game->chess->hasPiece(i, j);
						switch (type)
						{
						case 1:
							cout << "x ";
							break;
							
						case 2:
							cout << "o ";
							break;
							
						case 0:
							cout << "_ ";
							break;
						}
					}
					cout << endl;
				}
				cout << endl;

				int currentType = game->chess->getCurrentPieceType();
				int currentPlayer = game->getPlayer(currentType);
				
				if (currentPlayer == 1)
				{
					cout << "[warning][stage 1] not human turn" << endl;
				}
				else if (currentPlayer == 0)
				{
					int x = 0, y = 0;
					cout << "[info][stage 1] Add piece?(x y)" << endl;
					cin >> x >> y;
					cout << "[info][stage 1] You add piece on (" << x << "," << y << ")" << endl;
					
					if (game->chess->addPiece(x, y, currentType))
					{
						game->checkResult(currentType);
					}
				}
			}
			break;
			
		case 2:
			{
				char ch;
				cout << "[info][stage 2] Winner : " << game->getWinner() << endl;
				cout << "[info][stage 2] Restart game?(Y/N)" << endl;
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					game->restart();
				}
				else
				{
					isExit = true;
				}
			}
			break;
		}
	}
	
	cout << "[info] Gomoku exiting..." << endl;	
	delete game;
	
	return 0;
}
