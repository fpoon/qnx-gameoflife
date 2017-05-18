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
colorsSelected( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	short * sel;
	char ** items;
	char * item;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PtGetResource(ABW_pcbColors, Pt_ARG_CBOX_SEL_ITEM, &sel, 0);
	PtGetResource(ABW_pcbColors, Pt_ARG_ITEMS, &items, 0);
	printf("Selected %d ", *sel);
	item = items[*sel==0?0:*sel-1];
	printf(" - %s\n", item);
	loadCellColors(item);
	PtDamageWidget(ABW_prDrawer);
	
	return( Pt_CONTINUE );

	}

