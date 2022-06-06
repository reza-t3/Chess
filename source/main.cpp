#include "Board.h"

using namespace sf;
using namespace std;

int main() {
	RenderWindow window(VideoMode(1080, 800), "Chess", Style::Titlebar | Style::Close);
	Board board(&window);
	board.run();
	return 0;
}