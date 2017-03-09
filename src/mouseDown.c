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
mouseDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int x,y;
	PhPointerEvent_t * data;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	data = (PhPointerEvent_t*) PhGetData(cbinfo->event);
	x = data->pos.x;
	y = data->pos.y;
	
	switch(getMode()) {
		case WORLD_MODE_MOVE:
			moveWorld(x,y);
			setMovement(x,y);
			break;
		case WORLD_MODE_DRAW:
		default:
			printf("Unsupported right now!\n");
	}
	
	PtDamageWidget(ABW_prDrawer);
	
	
	return( Pt_CONTINUE );

	}

