/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CONFIG                           1       /* callback function: ke64__configPanel_CB */
#define  CONFIG_DIGITALFILTERENABLE       2       /* control type: LED, callback function: ke64__toggleDigitalFilter_CB */
#define  CONFIG_DIGITALCOUNT              3       /* control type: string, callback function: ke64__digitalCount_CB */
#define  CONFIG_MEDIANFILTERENABLE        4       /* control type: LED, callback function: ke64__toggleMedianFilter_CB */
#define  CONFIG_MEDIANRANK                5       /* control type: string, callback function: ke64__medianRank_CB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ke64__configPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ke64__digitalCount_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ke64__medianRank_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ke64__toggleDigitalFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ke64__toggleMedianFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
