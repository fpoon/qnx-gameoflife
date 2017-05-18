#ifndef _WORLD_H_
#define _WORLD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WORLD_MODE_DRAW 1
#define WORLD_MODE_MOVE 0

int getWorldX();
int getWorldY();
int getWorldW();
int getWorldH();
void setMovement(int px, int py);
void moveWorld(int mx, int my);
void setWorld(int w, int h);
uint32_t getCell(int x, int y);
uint32_t * getWorldData();
int getCellSize();
void setCell(int x, int y);
void increaseCellSize();
void decreaseCellSize();
int getCellColor(int cell);

void newSimulation();
void startSimulation();
void stopSimulation();
int isSimulation();
void disposeSimulation();
void step();
void loadCellColors(const char * path);

void setMode(int mode);
int getMode();

void saveWorld(const char * path);
void openWorld(const char * path);

#ifdef __cplusplus
}
#endif

extern int timer;

#endif //_WORLD_H_
