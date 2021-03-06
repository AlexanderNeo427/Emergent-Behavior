#include "GameOfLife.h"

GOL::GameOfLife::GameOfLife(olc::PixelGameEngine* pge)
	:
	SimulationBase(pge), 
	m_grid(nullptr),
	m_numGridCells(0),
	m_timeSinceLastStep(0.f),
	m_isRunning(false)
{}

void GOL::GameOfLife::Init()
{
	m_numGridCells = m_screenWidth * m_screenHeight;

	m_grid = new bool[m_numGridCells];
	m_gridCopy = new bool[m_numGridCells];

	for (int i = 0; i < m_numGridCells; ++i)
	{
		m_grid[i]	  = false;
		m_gridCopy[i] = false;
	}

	m_directions =
	{
		Direction(-1, 1),
		Direction( 0, 1),
		Direction( 1, 1),
		Direction( 1, 0),
		Direction( 1,-1),
		Direction( 0,-1),
		Direction(-1,-1),
		Direction(-1, 0),
	};
}

void GOL::GameOfLife::Update(float deltaTime)
{
	//------------- Input ---------------
	if (m_pge->GetMouse(0).bHeld)
	{
		int gridX = m_pge->GetMouseX();
		int gridY = m_pge->GetMouseY();
		int index = (gridY * m_screenWidth) + gridX;

		//for (int x = 0; x < 10; ++x)
		//{
		//	for (int y = 0; y < 10; ++y)
		//	{
		//		m_grid[index + x + y] = true;

		//		m_gridCopy[index + x + y] = true;
		//	}
		//}
		m_grid[index] = true;
		m_gridCopy[index] = true;
	}
	else if (m_pge->GetMouse(1).bHeld)
	{
		int gridX = m_pge->GetMouseX();
		int gridY = m_pge->GetMouseY();
		int index = (gridY * m_screenWidth) + gridX;

		m_grid[index] = false;
		m_gridCopy[index] = false;
	}

	if (m_pge->GetKey(olc::SPACE).bPressed)
		m_isRunning = !m_isRunning;

	//------------ Pause state -------------
	if (!m_isRunning) return;

	//--------- Simulation Logic ---------
	//m_timeSinceLastStep += deltaTime;

	//if (m_timeSinceLastStep < TIME_STEP)
	//	return;

	//m_timeSinceLastStep = 0.f;

	// Update rules
	// Make copy of grid
	MakeGridCopy(m_grid, m_gridCopy, m_numGridCells);

	for (int y = 0; y < m_screenHeight; ++y)
	{
		for (int x = 0; x < m_screenWidth; ++x)
		{
			int index = (y * m_screenWidth) + x;

			if (m_gridCopy[index] == true)
			{
				int neighbourCount = GetLiveNeighbourCount(x, y, m_gridCopy);

				if (neighbourCount != 2 && neighbourCount != 3)
					m_grid[index] = false;
			}
			else
			{
				int neighbourCount = GetLiveNeighbourCount(x, y, m_gridCopy);

				if (neighbourCount == 3)
					m_grid[index] = true;
			}
		}
	}
}

void GOL::GameOfLife::Render()
{
	m_pge->Clear(olc::DARK_GREY);

	for (int y = 0; y < m_screenHeight; ++y)
	{
		for (int x = 0; x < m_screenWidth; ++x)
		{
			int index = (y * m_screenWidth) + x;
			olc::Pixel fillColor = m_grid[index] ? olc::BLACK : olc::WHITE;
			m_pge->Draw(x, y, fillColor);
		}
	}
}

void GOL::GameOfLife::Exit() {}

int GOL::GameOfLife::GetLiveNeighbourCount(int x, int y, bool* grid)
{
	int numNeighbours = 0;

	for (Direction const& dir : m_directions)
	{
		int nextX = x + dir.x;
		int nextY = y + dir.y;

		if (nextX < 0 || nextX >= m_screenWidth ||
			nextY < 0 || nextY >= m_screenHeight)
		{
			continue;
		}

		if (grid[(nextY * m_screenWidth) + nextX])
			++numNeighbours;
	}

	return numNeighbours;
}

void GOL::GameOfLife::MakeGridCopy(bool* grid, bool* gridCopy, int n)
{
	for (int i = 0; i < n; ++i)
	{
		gridCopy[i] = grid[i];
	}
}
