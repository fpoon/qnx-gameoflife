#include <stdio.h>
#include <stdint.h>
#include <pthread.h>


#include "world.h"
#include "sim.h"

#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#define MAX_CELL_SIZE 16

extern "C" {
	
	int x = 0;
	int y = 0;
	int cellSize = 4;
	int mode = WORLD_MODE_MOVE;
	int timer = 10000;
	
	Simulation * sim = NULL;
	
	uint32_t data[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, ALIVE_CELL, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, ALIVE_CELL, ALIVE_CELL, 0, 0, 0, 0,
		0, 0, 0, ALIVE_CELL, ALIVE_CELL, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	
	void setMode(int m) {
		mode = m;
	}
	int getMode() {
		return mode;
	}
	
	int getWorldX() {
		return x;
	}
	
	int getWorldY() {
		return y;
	}
	
	int getWorldW() {
		return 10;
	}
	
	int getWorldH() {
		return 10;
	}
	
	int getCellSize() {
		return cellSize;
	}
	
	void increaseCellSize() {
		if (cellSize < MAX_CELL_SIZE)
			cellSize*=2;
		else
			cellSize = MAX_CELL_SIZE;
		printf("Cell size increased! Size: %d\n", cellSize);
	}
	
	void decreaseCellSize() {
		if (cellSize > 1)
			cellSize/=2;
		else
			cellSize = 1;
		printf("Cell size decreased! Size: %d\n", cellSize);
	}
	
	int isCyclic() {
		return 1;
	}
	
	uint32_t getCell(int x, int y) {
		int w,h;
		w = getWorldW();
		h = getWorldH();
		if (isCyclic()) 
		{
			x %= getWorldW();
			y %= getWorldH();
			x+=(x)<(0)?(getWorldW()):(0);
			y+=(y)<(0)?(getWorldH()):(0);
		}
		if (x < 0 || x >= w || y < 0 || y >= h)
			return OUTSIDE_CELL;
		return getWorldData()[y*w+(x)];
	}
	
	int getCellColor(int cell) {
		if (cell & ALIVE_CELL)
			return 0x0000FF00;
		else
			return 0x0;
	}
	
	void setCell(int absx, int absy) {
		int sx, sy;
		int w,h;
		sx = ((getWorldX()+absx)/cellSize);
		sy = ((getWorldY()+absy)/cellSize);
				
		w = getWorldW();
		h = getWorldH();
		if (isCyclic()) 
		{
			sx %= getWorldW();
			sy %= getWorldH();
			sx+=(sx)<(0)?(getWorldW()):(0);
			sy+=(sy)<(0)?(getWorldH()):(0);
		}
		printf("-->%d %d; %d %d\n",absx,absy, sx, sy);
		if (sx < 0 || sx >= w || sy < 0 || sy >= h)
			return;
		getWorldData()[sy*w+(sx)] ^= ALIVE_CELL ;
		getWorldData()[sy*w+(sx)] &= ALIVE_CELL;
	}
	
	uint32_t * getWorldData() {
		if (!sim)
			return data;
		return sim->getData();
	}
	
	int sposx, sposy;
	
	void moveWorld(int mx, int my) {
		mx -= sposx;
		my -= sposy;
		x -= mx;
		y -= my;
	}
	
	void setMovement(int px, int py) {
		sposx = px;
		sposy = py;
	}
	
	int isSimulation() {
		if (sim)
			return 1;
		else
			return 0;
	}
	
	void newSimulation() {
		disposeSimulation();
		sim = new Simulation(
			getWorldData(),
			getWorldW(),
			getWorldH(),
			isCyclic());
	}
	
	void step() {
		if (!sim)
			return;
		sim->step();
	}
	
	void disposeSimulation() {
		if (sim != NULL)
			delete sim;
		sim = NULL;
		printf("Simulation disposed\n");
	}
	
	void setTimer(int t) {
		timer = t;
	}
	
	int simFlag = 1;
	
	void updateScreen() {
		PtEnter(0);
		PtDamageWidget(ABW_prDrawer);
		PtLeave(0);
	}
	
	void * simThread(void * arg) {
		while(simFlag) {
			sim->step();
			updateScreen();
			usleep(timer);
		}
		return NULL;
	}
	
	void startSimulation() {
		if (!sim)
			return;
		simFlag = 1;
		pthread_create(NULL, NULL, simThread, NULL); 
	}
	
	void stopSimulation() {
		if (!sim)
			return;
		simFlag = 0;
	}
}