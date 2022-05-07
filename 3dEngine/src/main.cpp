#include "Graphics.h"
#include "Game.h"
#include <chrono>
#include <thread>

std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
double delta = 0;
double ns = 1000000.0 / 60.0;
bool firstTime = true;


int main()
{
	Graphics graphics;
	if (graphics.init())
	{
		Game game(graphics);
		/*std::thread th1(&Game::DrawModel, std::ref(game));*/

		while (graphics.isRun())
		{
			graphics.ClearFrame(0x00000);

			if (firstTime)
			{
				end = std::chrono::high_resolution_clock::now();
				firstTime = false;
				continue;
			}

			start = std::chrono::high_resolution_clock::now();
			long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(start - end).count();
			end = start;
			delta += microseconds / ns;

			while (delta >= 1.0)
			{
				game.UpdateModel();
				delta -= 1.0;
			}
			game.DrawModel();

			graphics.ComposeFrame();

			graphics.broadcast((float) 1'000'000 / microseconds);
		}
		//th1.join();
	}
	return 0;
}