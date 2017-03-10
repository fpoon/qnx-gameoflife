#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <vector>
#include <limits.h>

#include "world.h"
#include "sim.h"

#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#define MAX_CELL_SIZE 16

extern "C" {
	
	int x = 0;
	int y = 0;
	int cellSize = 8;
	int mode = WORLD_MODE_MOVE;
	int timer = 10000;
	int width = 100;
	int height = 100;
	unsigned long min = ULONG_MAX;
	unsigned long avg = 0;
	unsigned long max = 0;
	
	Simulation * sim = NULL;
	
	void updateScreen();
	
	/*uint32_t data[] = {
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
	};*/
	
	std::vector<uint32_t> vdata(width*height, DEAD_CELL);
	
	//setWorld(100,100);
	
	bool inited = false;
	
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
		return width;
	}
	
	int getWorldH() {
		return height;
	}
	
	void setWorld(int w, int h) {
		if (sim)
			return;

		vdata.clear();
		for (int i = 0; i < w*h; i++) {
			vdata.push_back(DEAD_CELL);
		}
		width = w;
		height = h;
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
		if (sim) {
			return sim->cyclic?1:0;
		}
		unsigned long * flags;
		
		PtGetResource(ABW_pbCyclic, Pt_ARG_FLAGS, &flags, 0);
		
		return (*flags & Pt_SET) ? (1) : (0);
	}
	
	int isLinear() {
		unsigned long * flags;
		PtGetResource(ABW_pbMulti, Pt_ARG_FLAGS, &flags, 0);
		
		return (*flags & Pt_SET) ? (0) : (1);
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
			return &vdata[0];
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
			isCyclic(),
			isLinear()
		);
	}
	
	void step() {
		if (!sim)
			return;
		sim->step();
		updateScreen();
	}
	
	void disposeSimulation() {
		stopSimulation();
		if (sim != NULL)
			delete sim;
		sim = NULL;
		
		min = ULONG_MAX;
		avg = 0;
		max = 0;
		
		printf("Simulation disposed\n");
	}
	
	void setTimer(int t) {
		timer = t;
	}
	
	int simFlag = 1;
	std::string popstr;
	std::string minmaxstr;
	int aliveCells;
	
	
	
	void updateScreen() {
		//PtEnter(0);
		PtDamageWidget(ABW_prDrawer);
		std::stringstream ss;
		ss << "Step: " << sim->stepCount << " || Population: " << aliveCells;
		popstr = ss.str();
		
		PtSetResource(ABW_plPopStep, Pt_ARG_TEXT_STRING, popstr.c_str(), 0);
		
		ss.str(std::string());
		
		unsigned long cur = sim->timePerStep/1000L;
		if (cur < min)
			min = cur;
		if (cur > max)
			max = cur;
		
		avg = (avg*((unsigned long)(sim->stepCount-1))+cur)/(unsigned long)sim->stepCount;
		
		ss << "Min: " << min << "us / Avg: " << avg << "us / Max: " << max << "us";
		
		minmaxstr = ss.str();
		
		PtSetResource(ABW_plMinMax, Pt_ARG_TEXT_STRING, minmaxstr.c_str(), 0);
		
		//PtLeave(0);
	}
	
	void * simThread(void * arg) {
		while(simFlag) {
			aliveCells = sim->step();
			PtEnter(0);
			updateScreen();
			PtLeave(0);
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