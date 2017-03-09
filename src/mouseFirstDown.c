/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "world.h"

int
mouseFirstDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int x,y,lx,ly;
	PhPointerEvent_t * data;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	data = (PhPointerEvent_t*) PhGetData(cbinfo->event);
	x = data->pos.x;
	y = data->pos.y;
	
	lx = cbinfo->event->translation.x;
	ly = cbinfo->event->translation.y;
	printf("--%d %d\n", lx,ly);
	
	switch(getMode()) {
		case WORLD_MODE_MOVE:
			setMovement(x,y);
			break;
		case WORLD_MODE_DRAW:
			setCell(x+lx,y+ly);
			break;
		default:
			printf("Unsupported right now!\n");
	}
	
	PtDamageWidget(ABW_prDrawer);
	
	return( Pt_CONTINUE );

	}

