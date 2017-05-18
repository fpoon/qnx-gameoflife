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
PtFileSelectionInfo_t openinfo;

char const * opnbtns[] = {"OK"};

int
openClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
		
	PhPoint_t p = {100, 100};

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if (isSimulation()) {
		PtAlert(ABW_base, NULL, "Warning!", NULL, "Please, reboot simualtion first!",
				NULL, 1, opnbtns, NULL, 1, 3, Pt_BLOCK_ALL);
			return( Pt_CONTINUE );
	}
	
	PtFileSelection(
		ABW_base,
		NULL,
		"Open file...",
		"projects",
		"*", // extension
		"Open",
		"Cancel",
		"nsd",
		&openinfo,
		0
	);
	
	if (*openinfo.path != 0) {
		printf("Opening %s\n", openinfo.path);
		openWorld(openinfo.path);
		PtDamageWidget(ABW_base);
	}
	
	return( Pt_CONTINUE );

	}

