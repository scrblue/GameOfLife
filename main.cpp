#include <iostream>
#include <SFML/Graphics.hpp>

constexpr int cellLength = 40;
constexpr int lineWidth = 1;
constexpr int windowX = 800;
constexpr int windowY = 800;
constexpr int numColumns = windowX / cellLength;
constexpr int numRows = windowY / cellLength;

struct position
{
	int x, y;
};

class cell
{
	private:
		position p;
		bool isActive = false;
		bool isActiveNext = false;
		sf::RectangleShape lines[4];
		sf::RectangleShape fill;

	public:
		cell(int x, int y);
		void configShapes();
		sf::RectangleShape returnLine(int d);
		sf::RectangleShape returnFill();
		void setActive(bool set);
		bool returnActive();
		void processTurn();
		void setActiveNext(bool set);
		bool returnActiveNext();
};

bool cell::returnActiveNext()
{
	return isActiveNext;
}

cell::cell (int x, int y)
{
	p.x = x;
	p.y = y;
	configShapes();
}

void cell::configShapes()
{
	for (int i = 0; i < 4; i++)
	{
		lines[i].setSize(sf::Vector2f(cellLength, lineWidth));
		lines[i].setFillColor(sf::Color::White);
		if (i == 0 || i == 1)
		{
			lines[i].setPosition(p.x*cellLength, p.y*cellLength);
		}
		else if (i == 1 || i == 3)
		{
			lines[i].setRotation(90.f);
		}
	}

	lines[2].setPosition(p.x*cellLength, (p.y+1)*cellLength);	
	lines[3].setPosition((p.x+1)*cellLength, p.y*cellLength);

	fill.setSize(sf::Vector2f(cellLength, cellLength));
	fill.setFillColor(sf::Color::White);
	fill.setPosition(p.x*cellLength, p.y*cellLength);
}

sf::RectangleShape cell::returnLine(int d)
{
	if (d == 0)
	{
		return lines[0];
	}
	else if (d == 1)
	{
		return lines[1];
	}
	else if (d == 2)
	{
		return lines[2];
	}
	else if (d == 3)
	{
		return lines[3];
	}
}

sf::RectangleShape cell::returnFill()
{
	return fill;
}

void cell::setActive(bool set)
{
	isActive = set;
}

bool cell::returnActive()
{
	return isActive;
}

void cell::processTurn()
{
	isActive = isActiveNext;
}

void cell::setActiveNext(bool set)
{
	isActiveNext = set;
}

int countAliveNeighbors(std::vector<std::vector<cell>> cellMap, int x, int y)
{
	int aliveCount = 0;
	if (y > 0 && cellMap[x][y-1].returnActive())
	{
		aliveCount++;
	}

	if (y > 0 && x < numColumns-1 && cellMap[x+1][y-1].returnActive())
	{
		aliveCount++;
	}

	if (x < numColumns-1 && cellMap[x+1][y].returnActive())
	{
		aliveCount++;
	}

	if (x < numColumns-1 && y < numRows-1 && cellMap[x+1][y+1].returnActive())
	{
		aliveCount++;
	}

	if (y < numRows-1 && cellMap[x][y+1].returnActive())
	{
		aliveCount++;
	}
	
	if (x > 0 && y < numRows-1 && cellMap[x-1][y+1].returnActive())
	{
		aliveCount++;
	}

	if (x > 0 && cellMap[x-1][y].returnActive())
	{
		aliveCount++;
	}

	if (x > 0 && y > 0 && cellMap[x-1][y-1].returnActive())
	{
		aliveCount++;
	}

	return aliveCount;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Game of Life");

	std::vector<std::vector<cell>> cellMap;

	bool playing = false;

	for (int i = 0; i < numColumns; i++)
	{
		cellMap.push_back(std::vector<cell>());
		for (int j = 0; j < numRows; j++)
		{
			cell newCell(i, j);
			cellMap[i].push_back(newCell);
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && !playing)
				{
					int cellX = event.mouseButton.x/cellLength;
					int cellY = event.mouseButton.y/cellLength;
					std::cout << cellX << " " << cellY << std::endl;
					if (!cellMap[cellX][cellY].returnActive())
					{
						cellMap[cellX][cellY].setActive(true);
					}
					else
					{
						cellMap[cellX][cellY].setActive(false);
					}
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					if (playing)
					{
						std::cout << "Not playing" << std::endl;
						playing = false;
					}
					else
					{
						std::cout << "Playing" << std::endl;
						playing = true;
					}
				}
			}
		}
		

		if (playing)
		{
			for (int i = 0; i < numColumns; i++)
			{
				for (int j = 0; j < numRows; j++)
				{
					//Count alive neighbors
					int numAliveNeighbors = countAliveNeighbors(cellMap, i, j);
					//Set active next
					if (cellMap[i][j].returnActive() && numAliveNeighbors < 2)
					{
						std::cout << "Cell (" << i << ", " << j << ") died" << std::endl;
						cellMap[i][j].setActiveNext(false);
					}
					else if (cellMap[i][j].returnActive() && numAliveNeighbors < 4)
					{	
						std::cout << "Cell (" << i << ", " << j << ") remains alive" << std::endl;
						cellMap[i][j].setActiveNext(true);
					}
					else if (cellMap[i][j].returnActive() && numAliveNeighbors > 3)
					{
						std::cout << "Cell (" << i << ", " << j << ") died" << std::endl;
						cellMap[i][j].setActiveNext(false);
					}
					else if (!cellMap[i][j].returnActive() && numAliveNeighbors == 3)
					{
						std::cout << "Cell (" << i << ", " << j << ") is born" << std::endl;
						cellMap[i][j].setActiveNext(true);
					}
					else
					{
						cellMap[i][j].setActiveNext(false);
					}

				}
			}
			//Process turn
			for (int i = 0; i < numColumns; i++)
			{
				for (int j = 0; j < numRows; j++)
				{
					cellMap[i][j].processTurn();
				}
			}
		}

		window.clear();

		for (int i = 0; i < numColumns; i++)
		{
			for (int j = 0; j < numRows; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					window.draw(cellMap[i][j].returnLine(k));
				}
				
				if (cellMap[i][j].returnActive())
				{
					window.draw(cellMap[i][j].returnFill());
				}
			}
		}

		window.display();
	}

	return 0;
}
