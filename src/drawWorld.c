/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "world.h"

#define GRID_OFFSET_X 0
#define GRID_OFFSET_Y 0

void drawGrid(PhRect_t * canvas)
{
	int i,j;
	int cell;
	int x,y,w,h;
	int startx, starty, cellx, celly, size;
	size = getCellSize();
	w = canvas->lr.x-canvas->ul.x;
	h = canvas->lr.y-canvas->ul.y;
	
	cellx = (w/size)+2;
	celly = (h/size)+2;
	
	//startx = ((w-getWorldW()*size)/2)-getWorldX();
	//starty = ((h-getWorldH()*size)/2)-getWorldY();
	
	startx = ((getWorldX()/size)-1)*size-getWorldX();
	starty = ((getWorldY()/size)-1)*size-getWorldY();

	y = starty;
	PgSetStrokeColor(0x00666666);
	for (j = (getWorldY()/size)-1; y <= canvas->lr.y; y+=size)
	{
		x = startx;
		for (i = (getWorldX()/size)-1; x <= canvas->lr.x; x+=size)
		{
			cell = getCell(i, j);
			PgSetFillColor(getCellColor(cell));
			if (cell != -1)
			{
				if (size > 4)
					PgDrawIRect(x,y,x+size,y+size, Pg_DRAW_FILL_STROKE);
				else
					PgDrawIRect(x,y,x+size,y+size, Pg_DRAW_FILL);
			}
			i++;
		}
		j++;
	}
	
	/*PgSetStrokeColor(0x000000ff);
	PgDrawILine(0,starty,w,starty);
	PgDrawILine(startx,0,startx,h);
	
	PgSetStrokeColor(0x00ff0000);
	PgDrawILine(0,h/2,w,h/2);
	PgDrawILine(w/2,0,w/2,h);*/
}

void drawWorld( PtWidget_t *widget, PhTile_t *damage ) 
{
	
	PhRect_t canvas;
	PtSuperClassDraw( PtBasic, widget, damage );
	PtCalcCanvas(widget, &canvas);
	
	
	
	PtClipAdd(widget, &canvas);
	PgSetFillColor(Pg_BLACK);
	PgDrawRect(&canvas, Pg_DRAW_FILL);
	
	//PgDrawILine(0,0,167,167);
	
	drawGrid(&canvas);
	
	PtClipRemove();
	//printf("Pomalowano (%d; %d), (%d; %d)\n", canvas.ul.x, canvas.ul.y, canvas.lr.x, canvas.lr.y);
}

