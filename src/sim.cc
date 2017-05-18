#include <algorithm>
#include <iostream>

#include <time.h>

#include "sim.h"

using namespace std;

Simulation::Simulation(uint32_t * feed, uint16_t w, uint16_t h, bool cyclic, bool linear, char * b, char * s) : w(w), h(h), cyclic(cyclic), linear(linear)
{
	timePerStep = 0;
	stepCount = 0;
	data = new uint32_t[w*h];
	tmp_data = new uint32_t[w*h];
	disposable = true;

	for (int i = 0; i < w*h; i++)
	{
		data[i] = feed[i];
		tmp_data[i] = 0;
	}

	for (int i = 0; i < 9; i++)
	{
		born[i] = 0;
		still[i] = 0;
		if (b[i] != 0)
		{
			cout << "b" << i << endl;
			//printf("B%d\n", i);
			born[i] = true;
		}
		if (s[i] != 0)
		{
			cout << "s" << i << endl;
			still[i] = true;
		}
	}
	
	float startline = 0;
	float s = (float)h/(float)SIM_THREADS;
	float endline = s;
	
	for (int i = 0; i < SIM_THREADS; i++) {
		args[i].sim = this;
		args[i].startline = (int)startline;
		args[i].endline = (int)endline;
		startline = endline;
		endline += s;
	}
	args[SIM_THREADS-1].endline = h;
	/*for (int i = 0; i < SIM_THREADS; i++) {
		printf("Startline %d, endline %d\n", args[i].startline, args[i].endline);
	}*/
}

Simulation::~Simulation()
{
	disposable = true;
	if (!linear)
		for (int i = 0; i < SIM_THREADS; i++)
			pthread_kill(threads[i], 0);

	delete data;
	delete tmp_data;
}

int Simulation::stepLines(int startline, int endline)
{
	int ret = 0;
	for (int y = startline; y < endline; y++)
	{
		for(int x = 0; x < w; x++)
		{
			if (disposable)
				return -1;
			ret += setCell(x,y);
		}
	}
	return ret;
}

int Simulation::step()
{
	unsigned long ts;
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	ts = tp.tv_nsec;
	int ret = 0;
	disposable = false;
	if (linear)
	{
		ret = stepLines(0, h);
	} else {
		for (int i = 0; i < SIM_THREADS; i++) {
			pthread_create(&threads[i], NULL, simStepLineThreadFunc, &args[i]);
		}
		for (int i = 0; i < SIM_THREADS; i++) {
			void * alive;
			pthread_join(threads[i], &alive);
			ret += (int)(alive);
		}
	}
	disposable = true;
	std::swap(data, tmp_data);
	
	stepCount++;
	clock_gettime(CLOCK_REALTIME, &tp);
	timePerStep = tp.tv_nsec-ts;
	return ret;
}

uint32_t * Simulation::getData()
{
	return data;
}

int Simulation::setCell(uint16_t x, uint16_t y)
{
	int alive = 0;
	int lx = x-1, by = y+1, rx = x+1, ty = y-1;

	tmp_data[x+(y*w)] = data[x+(y*w)];
	
	if ((tmp_data[x+(y*w)] & ~ALIVE_CELL) > 0)
		tmp_data[x+(y*w)]--;

	if (cyclic)
	{
		if (lx < 0)
			lx = w-1;
		if (rx >= w)
			rx = 0;
		if (ty < 0)
			ty = h-1;
		if (by >= h)
			by = 0;
	}
	
	alive = countAliveCells(x,y,lx,rx,ty,by);

	if ((data[x+(y*w)] & ALIVE_CELL) && !(still[alive]))
		tmp_data[x+(y*w)] = DEAD_CELL | CELL_STEPS;

	if (!(data[x+(y*w)] & ALIVE_CELL) && born[alive])
		tmp_data[x+(y*w)] = ALIVE_CELL | CELL_STEPS;
	
	if (data[x+(y*w)] & ALIVE_CELL)
		return 1;
	return 0;
}

int Simulation::countAliveCells(int x, int y, int lx, int rx, int ty, int by)
{
	//cout << x << " " << y << " " << lx << " " << rx << " " << ty << " " << by << endl;
	int alive = 0;
	if ((ty >= 0) && (data[x+(ty*w)] & ALIVE_CELL))
		alive++;
	if ((ty >= 0 && rx < w) && (data[rx+(ty*w)] & ALIVE_CELL))
		alive++;
	if ((rx < w) && (data[rx+(y*w)] & ALIVE_CELL))
		alive++;
	if ((by < h && rx < w) && (data[rx+(by*w)] & ALIVE_CELL))
		alive++;
	if ((by < h) && (data[x+(by*w)] & ALIVE_CELL))
		alive++;
	if ((by < h && lx >= 0) && (data[lx+(by*w)] & ALIVE_CELL))
		alive++;
	if ((lx >= 0) && (data[lx+(y*w)] & ALIVE_CELL))
		alive++;
	if ((ty >= 0 && lx >= 0) && (data[lx+(ty*w)] & ALIVE_CELL))
		alive++;

	return alive;
}

void * simStepLineThreadFunc(void * args) {
	struct ThreadArg * arg = (struct ThreadArg*)args;
	Simulation * sim = arg->sim;
	int startline = arg->startline;
	int endline = arg->endline;
	int alive = 0;
	alive = sim->stepLines(startline, endline);
	
	return (void*)alive;
}
