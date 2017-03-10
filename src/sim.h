#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>
#include <pthread.h>

#define DEAD_CELL      0x00000000
#define ALIVE_CELL      0x80000000
#define OUTSIDE_CELL 0xFFFFFFFF

#define CELL_STEPS 0x00000010

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
		bool linear;

		bool born[8];
		bool still[8];
		
		uint16_t w, h;

		uint32_t * data;
		uint32_t * tmp_data;
		
		struct ThreadArg args[SIM_THREADS];
		pthread_t threads[SIM_THREADS];
		
		int setCell(uint16_t x, uint16_t y);
		
		//int countAliveCells(uint16_t x, uint16_t y, uint16_t lx, uint16_t rx, uint16_t ty, uint16_t by);
		int countAliveCells(int x, int y, int lx, int rx, int ty, int by);
	
	public:
		bool cyclic;
		uint32_t stepCount;
		Simulation(uint32_t * feed, uint16_t w, uint16_t h, bool cyclic, bool linear=true, uint8_t b=0x08, uint8_t s=0x0C);
		~Simulation();
		int step();
		int stepLines(int startline, int endline);
		uint32_t * getData();
		unsigned long timePerStep;
};

#endif

void * simStepLineThreadFunc(void * args);

#endif
