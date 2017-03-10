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

int fillPercent = 10;

int
fillSliderMoved( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int * val;
	int i;
	char buf[8];
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	for (i = 0; i < 8; i++)
		buf[i] = 0;
	
	PtGetResource(ABW_psFillSlider, Pt_ARG_GAUGE_VALUE, &val, 0);
	sprintf(buf, "%d%%", *val);
	fillPercent = *val;
	
	PtSetResource(ABW_plFillLabel, Pt_ARG_TEXT_STRING, buf, 0);

	return( Pt_CONTINUE );

	}

