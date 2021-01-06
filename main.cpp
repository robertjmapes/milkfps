#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <fstream>
#include <string>
#include <iostream>

// Override base class with your custom functionality
class UltimateFPS : public olc::PixelGameEngine
{

public:
	UltimateFPS()
	{
		sAppName = "MilkFPS";
	}

	bool OnUserCreate() override
	{
		LoadMap();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::A).bHeld)
			fPlayerA -= (0.5f)*fElapsedTime;
		if (GetKey(olc::Key::D).bHeld)
			fPlayerA += (1.0f)*fElapsedTime;

		if (GetKey(olc::Key::W).bHeld)
		{
			fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}
		// Handle Strafe Right movement & collision
		if (GetKey(olc::Key::E).bHeld)
		{
			fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
				fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
			}
		}


		
		for (int x = 0; x < ScreenWidth(); x++)
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
			float fDistanceToWall = 0;

			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.01f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
				
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
					}
				}

			}

			int nCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
			int nFloor = ScreenHeight() - nCeiling;

			olc::Pixel colour;
			if (fDistanceToWall <= fDepth / 4.0f) colour = olc::WHITE;
			else if (fDistanceToWall <= fDepth / 2.0f) colour = olc::GREY;
			else if (fDistanceToWall <= fDepth) colour = olc::DARK_GREY;


			for (int y = 0; y < ScreenHeight(); y++)
			{
				if (y < nCeiling)
					Draw(x, y, olc::BLACK);
				else if (y > nCeiling && y <= nFloor)
					Draw(x, y, colour);
				else
					Draw(x, y, olc::BLACK);					
			}

		}

		// Display Map & Player
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
				Draw(nx + 1, ny + 1, olc::GREY);
		Draw(1 + (int)fPlayerY, 1 + (int)fPlayerX, olc::BLUE);
		
		
		return true;
	}

private:
	float fPlayerX = 3.0f;
	float fPlayerY = 3.0f;
	float fPlayerA = 0.0f;

	float fFOV = 3.14159 / 4.0;
	float fDepth = 16.0;
	float fSpeed = 1.0f;

	std::wstring map;
	int nMapWidth = 16;
	int nMapHeight = 16;

	void LoadMap()
	{
		std::wstring line;
		std::wifstream MapFile("map.txt");
		while (std::getline(MapFile, line))
			map += line;
		std::wcout << map << std::endl;
		
	}

	
};

int main()
{
	UltimateFPS demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}