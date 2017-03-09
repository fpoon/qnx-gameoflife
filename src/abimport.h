/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 22 ];


#ifdef __cplusplus
extern "C" {
#endif
int newClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int openClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int saveClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int stepClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int stopClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rebootClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int drawClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int moveClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int zoomInClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int zoomOutClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int mouseUp( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void drawWorld( PtWidget_t *widget, PhTile_t *damage ) 

;
int speedLessClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int speedMoreClicked( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int mouseDown( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int mouseFirstDown( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
