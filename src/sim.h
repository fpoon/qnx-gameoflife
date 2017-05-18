#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>
#include <pthread.h>

#define DEAD_CELL      0x00000000
#define ALIVE_CELL      0x80000000
#define OUTSIDE_CELL 0xFFFFFFFF

#define CELL_STEPS              0x0000000F
#define CELL_STEPS_MASK    0x0000000F

#define NEWBORN_CELL ALIVE_CELL | CELL_STEPS

#define SIM_THREADS 20

#ifdef __cplusplus

class Simulation;

struct ThreadArg {
	Simulation * sim;
	int startline;
	int endline;
};

class Simulation {
	private:
		bool born[9];
		bool still[9];
		
		uint16_t w, h;

		uint32_t * data;
		uint32_t * tmp_data;
		
		struct ThreadArg args[SIM_THREADS];
		pthread_t threads[SIM_THREADS];
		
		int setCell(uint16_t x, uint16_t y);
		
		//int countAliveCells(uint16_t x, uint16_t y, uint16_t lx, uint16_t rx, uint16_t ty, uint16_t by);
		int countAliveCells(int x, int y, int lx, int rx, int ty, int by);
	
	public:
		bool linear;
		bool cyclic;
		bool disposable;
		uint32_t stepCount;
		Simulation(uint32_t * feed, uint16_t w, uint16_t h, bool cyclic, bool linear=true, char * b="\0\0\03\0\0\0\0\0", char * s="\0\023\0\0\0\0\0");
		~Simulation();
		int step();
		int stepLines(int startline, int endline);
		uint32_t * getData();
		unsigned long timePerStep;
};

#endif

void * simStepLineThreadFunc(void * args);

#endif
