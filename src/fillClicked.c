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

#include "sim.h"

extern int fillPercent;

int
fillClicked( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int amount;
	int i;
	int r,size;
		
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;	
		
	if (isSimulation())
		return (Pt_CONTINUE);
	
	size = getWorldW()*getWorldH();
	
	//printf("randmax %d size %d*%d=%d\n", RAND_MAX, getWorldW(), getWorldH(), size);
	
	amount = (int)(((float)size*(float)fillPercent)/100.0f);
	
	printf("Filling %d cells\n", amount);
	
	for (i = 0; i < size; i++)
		getWorldData()[i] = 0;
	
	for (i = 0; i < amount; i++) {
		do { r = (rand()*rand())%size; }	while(getWorldData()[r]);
		getWorldData()[r] = NEWBORN_CELL;
	}
	
	PtDamageWidget(ABW_prDrawer);
	
	return( Pt_CONTINUE );

	}

