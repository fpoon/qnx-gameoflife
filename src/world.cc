#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <vector>
#include <limits.h>
#include <fstream>

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
	
	uint32_t aliveColors[CELL_STEPS+1];
	uint32_t deadColors[CELL_STEPS+1];
	
	void updateScreen();
	void getRules(char * target, char * rules, bool born);
	
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
	
	void centerScreen(int lastSize, int curSize) {
		/*PhRect_t canvas;
		PtCalcCanvas(ABW_prDrawer, &canvas);
		int mx = canvas.lr.x*0.25;
		int my = canvas.lr.y*0.25;
		if (lastSize < curSize) {
			x += mx;
			y +=  my;
		}
		else if (lastSize > curSize) {
			x -= mx;
			y -= my;
		}
		
		printf("XY %d %d\n", x, y);*/
	}
	
	void increaseCellSize() {
		int lastSize = cellSize;
		if (cellSize < MAX_CELL_SIZE)
			cellSize*=2;
		else
			cellSize = MAX_CELL_SIZE;
		centerScreen(lastSize, cellSize);
		printf("Cell size increased! Size: %d\n", cellSize);
	}
	
	
	void decreaseCellSize() {
		int lastSize = cellSize;
		if (cellSize > 1)
			cellSize/=2;
		else
			cellSize = 1;
		centerScreen(lastSize, cellSize);
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
	
	void loadCellColors(const char * path) {
		if (!path) {
			for (int i = 0; i < CELL_STEPS+1; i++) {
				aliveColors[i] = 0x00FFFFFF;
				deadColors[i] = 0x00000000;
			}
			return;
		}
		std::string p("colors/");
		p += path;
		std::fstream f(p.c_str(), ios::in);
		for (int i = 0; i < CELL_STEPS+1; i++)
			f >> std::hex >> aliveColors[i];
		for (int i = 0; i < CELL_STEPS+1; i++)
			f >> std::hex >> deadColors[i];
		f.close();
	}
	
	int getCellColor(int cell) {
		if (cell & ALIVE_CELL)
			return aliveColors[cell & CELL_STEPS_MASK];
		else
			return deadColors[cell & CELL_STEPS_MASK];
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
		if (getWorldData()[sy*w+(sx)] & ALIVE_CELL)
			getWorldData()[sy*w+(sx)] |= CELL_STEPS;
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
		char * rules;
		char bornRules[9];
		char stillRules[9];
		disposeSimulation();
		PtGetResource(ABW_ptRules, Pt_ARG_TEXT_STRING, &rules, 0);
		getRules(bornRules, rules, true);
		PtGetResource(ABW_ptRules, Pt_ARG_TEXT_STRING, &rules, 0);
		getRules(stillRules, rules, false);
		sim = new Simulation(
			getWorldData(),
			getWorldW(),
			getWorldH(),
			isCyclic(),
			isLinear(),
			bornRules,
			stillRules
		);
	}
	
	void step() {
		if (!sim)
			return;
		sim->step();
		updateScreen();
	}
	
	pthread_t simTh;
	
	void disposeSimulation() {
		stopSimulation();
		if (sim != NULL) {
			if (!sim->linear) {
				pthread_kill(simTh, 0);
				printf("SimThread killed\n");
			}
			delete sim;
			sim = NULL;
		}
		
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
	
	void getRules(char * target, char * ruless, bool born) {
		char * rules = ruless;
		bool readMode = false;
		for (int i = 0; i < 9; i++) {
			target[i] = 0;
		}
		
		
		for(;*rules != 0; rules++) {
			//printf("%c ", *rules);
			if (*rules == 'b' || *rules == 'B')
			{
				readMode = born;
				continue;
			}
			
			if (*rules == 's' || *rules == 'S')
			{
				readMode = !born;
				continue;
			}
			
			if (readMode) {
				if (*rules-48 >= 0 && *rules-48 <= 8)
					target[*rules-48]=*rules;
			}
		}
		return;
	}
	
	
	void updateScreen() {
		if (!sim)
			return;
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
		if (!sim || simFlag)
			return;
		simFlag = 1;
		pthread_create(&simTh, NULL, simThread, NULL); 
	}
	
	void stopSimulation() {
		if (!sim)
			return;
		simFlag = 0;
	}
	
	void saveWorld(const char * path) {
		char * rules;
		std::fstream f(path, ios::out);
		PtGetResource(ABW_ptRules, Pt_ARG_TEXT_STRING, &rules, 0);
		f << rules << std::endl;
		f << getWorldW() << std::endl;
		f << getWorldH() << std::endl;
		for (int i = 0; i < getWorldH(); i++) {
			for (int j = 0; j < getWorldW(); j++)
				f << (getWorldData()[i*getWorldW()+j]&ALIVE_CELL?1:0) << " ";
			f << "\n";
		}
		f.close();
	}
	
	void openWorld(const char *path) {
		char * rules;
		char * wstr;
		char * hstr;
		int w,h;
		int cell;
		std::fstream f(path, ios::in);
		PtGetResource(ABW_ptRules, Pt_ARG_TEXT_STRING, &rules, 0);
		PtGetResource(ABW_ptWidth, Pt_ARG_TEXT_STRING, &wstr, 0);
		PtGetResource(ABW_ptHeight, Pt_ARG_TEXT_STRING, &hstr, 0);
		f >> rules;
		f >> w;
		f  >> h;
		itoa(w,wstr,10);
		itoa(h,hstr,10);
		setWorld(w,h);
		for (int i = 0; i < getWorldW()*getWorldH(); i++) {
			f >> cell;
			getWorldData()[i] = cell * (NEWBORN_CELL);
		}
		f.close();
	}
}