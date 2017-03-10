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

char const *newbtns[] = {"OK"};

int
newClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int x,y;
	char * str;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if (isSimulation()) {
		disposeSimulation();
	}
	
	//PtMessageBox(ABW_base, "Warning!", "Current cells will be wiped out!", NULL, NULL);
	PtAlert(ABW_base, NULL, "Warning!", NULL, "Current cells will be wiped out!",
				NULL, 1, newbtns, NULL, 1, 3, Pt_BLOCK_ALL);
	PtGetResource(ABW_ptWidth, Pt_ARG_TEXT_STRING, &str, 0);
	x = atoi(str);
	PtGetResource(ABW_ptHeight, Pt_ARG_TEXT_STRING, &str, 0);
	y = atoi(str);
	
	if (x > 0 && y > 0)
		setWorld(x,y);
	else
		PtMessageBox(ABW_base, "Error!", "Incorrect size!", NULL, NULL);
	
	return rebootClicked(widget, apinfo, cbinfo);

	}

