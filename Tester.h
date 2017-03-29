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

#define  MAINPANEL                        1       /* callback function: panelCB */
#define  MAINPANEL_RunButton              2       /* control type: command, callback function: Run */
#define  MAINPANEL_COMMANDBUTTON          3       /* control type: command, callback function: SendGPIB */
#define  MAINPANEL_GPIBADDRESSRING        4       /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBDEVICERING         5       /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBSCANBUTTON         6       /* control type: command, callback function: GPIBScan_CB */
#define  MAINPANEL_DECORATION             7       /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK GPIBScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Run(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendGPIB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
