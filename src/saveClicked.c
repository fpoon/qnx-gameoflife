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

	PtFileSelectionInfo_t saveinfo;
char const * savbtns[] = {"OK"};
int
saveClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if (isSimulation()) {
		PtAlert(ABW_base, NULL, "Warning!", NULL, "Please, reboot simualtion first!",
				NULL, 1, savbtns, NULL, 1, 3, Pt_BLOCK_ALL);
			return( Pt_CONTINUE );
	}
	
	PtFileSelection(
		ABW_base,
		NULL,
		"Save file...",
		"projects",
		"*", // extension
		"Save",
		"Cancel",
		"nsd",
		&saveinfo,
		Pt_FSR_NO_FCHECK
	);
	
	if (*saveinfo.path != 0) {
		printf("Saving %s\n", saveinfo.path);
		saveWorld(saveinfo.path);
	}
	return( Pt_CONTINUE );

	}

