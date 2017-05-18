/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "world.h"

//char * colorSchemes[32];
char * test[] = {"abc", "def"};
short colorN = 0;

int
baseOpened( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
		char line[32];
		char * item[1];
	int i = 0;
	FILE * index;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	srand(time(NULL));
	loadCellColors(NULL);
	item[0] = line;
	index = fopen("./colors/index", "r");
	for(i = 0; i < 32 && fscanf(index, "%s", line) != EOF; i++)
		//printf("%s\n", colorSchemes[i]);
		PtListAddItems(ABW_pcbColors, item, 1, 0);
	fclose(index);
	colorN = i;
	//PtListAddItems(ABW_pcbColors,test,2, 0);
	
	
	return( Pt_CONTINUE );

	}

