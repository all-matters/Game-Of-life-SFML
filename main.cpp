#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <iostream>

// Taken from [https://stackoverflow.com/a/42840334]
sf::Vector2i WrapAround(sf::Vector2i gridsize, sf::Vector2i xy) 
{
	int x0 {xy.x % gridsize.x};
	if (x0 < 0) {x0 += gridsize.x;};
	int y0 {xy.y % gridsize.y};
	if (y0 < 0) {y0 += gridsize.y;}; 
	return sf::Vector2i{x0,y0};
}

int CountNeighbors(const std::array<std::array<sf::RectangleShape,18>,18>& grid,int WhereX,int WhereY)
{
	int count {0};
	for (int x {WhereX-1};x<=WhereX+1;x++)
	{
		for (int y {WhereY-1};y<=WhereY+1;y++)
		{
			if (x != WhereX || y != WhereY ) //really cant visualize on how this works lol
			{
				int gridsize {static_cast<int>(grid.size())};
				sf::Vector2i wraparound {WrapAround(sf::Vector2i{gridsize,gridsize}, sf::Vector2i{x,y})};
				if ( grid[wraparound.x][wraparound.y].getFillColor() == sf::Color::Black )
				{
					++count;
					std::cout << count << '\n';
				}
			}
			
		}
	} 
	return count;
}
// std::array<std::array<sf::RectangleShape,18>,18> ReturnNextState(const std::array<std::array<sf::RectangleShape,18>,18>& grid) TOO LONG!!
auto ReturnNextState(const std::array<std::array<sf::RectangleShape,18>,18>& grid)
{
	auto temp {grid}; //I don't know if auto keyword is practical here. I just got tired of copy and typing that long thing lol
	for (int x {};x<grid.size();++x)
	{
		for (int y {};y<grid[y].size();++y)
		{
			const int count {CountNeighbors(grid,x,y)}; 
			if (grid[x][y].getFillColor() == sf::Color::Black)
			{
				if (count < 2 || count > 3) 
				{
					//Any live cell with more than three live neighbours dies, as if by overpopulation.
					// Any live cell with fewer than two live neighbours dies, as if by underpopulation.
					temp[x][y].setFillColor(sf::Color::Transparent);
				}
				//Any live cell with two or three live neighbours lives on to the next generation.
			}
			if (grid[x][y].getFillColor() == sf::Color::Transparent) 
			{
				if (count == 3) // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
				{
					temp[x][y].setFillColor(sf::Color::Black);
					std::cout << "new cell";
				}
			}
		}
	}
	return temp;
}

int main(int argc, char const *argv[])
{
	//HELLO WORLD
	bool state {false};
	std::array<std::array<sf::RectangleShape,18>,18> grid;
	const float gridsize {30.f};
	for (int x {};x<grid.size();++x)
	{
		for (int y {};y<grid[y].size();++y)
		{
			grid[x][y].setSize(sf::Vector2f(gridsize,gridsize));
			grid[x][y].setPosition(gridsize*x + 10,gridsize*y + 10);
			grid[x][y].setFillColor(sf::Color::Transparent);
			grid[x][y].setOutlineColor(sf::Color(128,128,128));
			grid[x][y].setOutlineThickness(2);
		}
	}

	sf::RenderWindow window (sf::VideoMode(560,560), "Game Of Life",sf::Style::Titlebar);

	sf::Clock clock {};
	sf::Clock twosecClock {};
	float dt {};

	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		dt = clock.restart().asSeconds();
		sf::Event event {};
		while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Space)
            {
            	state = !state;
            	// grid = ReturnNextState(grid);
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			const sf::Vector2f mouse {sf::Mouse::getPosition(window)};
			for (int x {};x<grid.size();++x)
			{
				for (int y {};y<grid[y].size();++y)
				{
					if ( grid[x][y].getGlobalBounds().contains(mouse) )
					{
						grid[x][y].setFillColor(sf::Color::Black);
					}
				}
			}
		} else if ( sf::Mouse::isButtonPressed(sf::Mouse::Right) )
		{
			const sf::Vector2f mouse {sf::Mouse::getPosition(window)};
			for (int x {};x<grid.size();++x)
			{
				for (int y {};y<grid[y].size();++y)
				{
					if ( grid[x][y].getGlobalBounds().contains(mouse) )
					{
						grid[x][y].setFillColor(sf::Color::Transparent);
					}
				}
			}
		}

		//I'm dumb. I could possibly shorten this but I dont know how
		if (state)
		{
			if (twosecClock.getElapsedTime() > sf::Time(sf::seconds(0.1f)))
			{
				twosecClock.restart();
				grid = ReturnNextState(grid);
			}
		}

        window.clear(sf::Color::White);
        for (int x {};x<grid.size();++x)
		{
			for (int y {};y<grid[y].size();++y)
			{
				window.draw(grid[x][y]);
			}
		}
		window.display();
	}	
	return 0;
}