#include <SFML/Graphics.hpp>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <Windows.h>
using namespace sf;

int iWidth = GetSystemMetrics(SM_CXSCREEN);  // разрешение экрана по горизонтали
int iHeight = GetSystemMetrics(SM_CYSCREEN); // разрешение экрана по вертикали

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 900
#define CELL_SIZE 5
#define Game_w WINDOW_WIDTH/CELL_SIZE
#define Game_h WINDOW_HEIGHT/CELL_SIZE

bool Generation[Game_w][Game_h];
bool OldGeneration[Game_w][Game_h];

int ckor;
int x;
int y;

void Generate()
{
	for (int y = 0; y < Game_h; y++) {
		for (int x = 0; x < Game_w; x++) {

			if (rand() % 2 == 1) {

				Generation[x][y] = OldGeneration[x][y] = true;
			}
			else {

				Generation[x][y] = OldGeneration[x][y] = false;
			}
		}
	}
	ckor = 0;
}

void UpdateOldGeneration()
{
	for (int y = 0; y < Game_h; y++) {
		for (int x = 0; x < Game_w; x++) {

			OldGeneration[x][y] = Generation[x][y];
		}
	}
}

int GetCell(int x, int y)
{
	if ((x < 0) || (x > Game_w - 1) || (y < 0) || (y > Game_h - 1)) {

		return 0;
	}
	else {

		return OldGeneration[x][y];
	}
}

int GetCellsCount(int x, int y)
{
	return GetCell(x - 1, y) + GetCell(x - 1, y - 1) + GetCell(x, y - 1) + GetCell(x + 1, y - 1) +
		GetCell(x + 1, y) + GetCell(x + 1, y + 1) + GetCell(x, y + 1) + GetCell(x - 1, y + 1);
}

void Update()
{
	UpdateOldGeneration();

	for (int y = 0; y < Game_h; y++) {
		for (int x = 0; x < Game_w; x++) {

			int cellsCount = GetCellsCount(x, y);

			if (OldGeneration[x][y] == false) {

				if (cellsCount == 3) {

					Generation[x][y] = true;

					ckor++;
				}
			}
			else {

				if ((cellsCount < 2) || (cellsCount > 3)) {

					Generation[x][y] = false;

				}
			}
		}
	}
}

void Draw(RenderWindow* window, RectangleShape* rectangle)
{
	for (int y = 0; y < Game_h; y++) {

		for (int x = 0; x < Game_w; x++) {

			if (Generation[x][y]) {

				rectangle->setPosition(x * CELL_SIZE, y * CELL_SIZE);

				window->draw(*rectangle);
			}
		}
	}
}

void mouseGenerate(RenderWindow *window, RectangleShape *rectangle, int x, int y) {
	
	Generation[x][y] = OldGeneration[x][y] = true;

	rectangle->setPosition(x * CELL_SIZE, y * CELL_SIZE);

	window->draw(*rectangle);
}

int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life", Style::Close);
	window.setFramerateLimit(22);

	Font font;
	font.loadFromFile("C:\\AdobeClean-Bold.ttf");
	Text text("", font, 35);

	RectangleShape rectangle;
	rectangle.setFillColor(Color(100, 80, 510));
	rectangle.setSize(Vector2f(CELL_SIZE, CELL_SIZE));

	Generate();

	Vector2i pos = Mouse::getPosition(window);
	x = pos.x / Game_w;
	y = pos.y / Game_h;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed)

				window.close();

			if (Keyboard::isKeyPressed(Keyboard::Escape))

				window.close();
		}

		if (event.type == Event::MouseButtonPressed) {  

			mouseGenerate(&window, &rectangle, x, y);

			Generate();

		}
		Update();

		window.clear();

		Draw(&window, &rectangle);

		std::ostringstream playerCkor;
		playerCkor << ckor;
		text.setFillColor(Color::White);
		text.setString("GEN: " + playerCkor.str());
		text.setPosition(10, 10);
		window.draw(text);

		text.setString("press LBM to new Generate");
		text.setPosition(590, 850);
		window.draw(text);

		window.display();
	}
	return 0;
}