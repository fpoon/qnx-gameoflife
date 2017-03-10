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

char const *resbtns[] = {"OK"};

int
resizeClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
		
	int x,y;
	char * str;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if (isSimulation()) {
		PtAlert(ABW_base, NULL, "Warning!", NULL, "Please, reboot simualtion first!",
				NULL, 1, resbtns, NULL, 1, 3, Pt_BLOCK_ALL);
			return( Pt_CONTINUE );
	}
	
	return newClicked(widget, apinfo, cbinfo);

	}

