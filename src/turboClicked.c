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

char const *turbobtns[] = {"Understood"};

int
turboClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	unsigned long * flags;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtGetResource(ABW_pbTurbo, Pt_ARG_FLAGS, &flags, 0);
		
	if (!(*flags & Pt_SET))
		return Pt_CONTINUE;
	
	PtAlert(ABW_base, NULL, "Warning!", NULL, "Rule B0 doesn't work in Turbo mode",
				NULL, 1, turbobtns, NULL, 1, 3, Pt_BLOCK_ALL);
	
	return( Pt_CONTINUE );

	}

