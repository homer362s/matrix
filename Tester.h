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
#define  MAINPANEL_RUNBUTTON              2       /* control type: command, callback function: Run */
#define  MAINPANEL_SENDBUTTON             3       /* control type: command, callback function: SendGPIB */
#define  MAINPANEL_GPIBADDRESSRING        4       /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBDEVICERING         5       /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBSCANBUTTON         6       /* control type: command, callback function: GPIBScan_CB */
#define  MAINPANEL_LOADPROBECARDBUTTON    7       /* control type: command, callback function: LoadProbeCard_CB */
#define  MAINPANEL_TEXTMSG_2              8       /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TABS                   9       /* control type: tab, callback function: (none) */
#define  MAINPANEL_RESETRELAYSBUTTON      10      /* control type: command, callback function: resetRelays_CB */
#define  MAINPANEL_DECORATION             11      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG                12      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_COMMANDBUTTON_3         2       /* control type: command, callback function: (none) */
#define  TABPANEL_TEXTMSG_3               3       /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_2_MAN_CON2_RING         2       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  TABPANEL_2_MAN_CON1_RING         3       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  TABPANEL_2_MANUALTABLE           4       /* control type: table, callback function: (none) */
#define  TABPANEL_2_MEASUREVOLTAGEBUTTON  5       /* control type: command, callback function: ManualMeasure_CB */
#define  TABPANEL_2_IBIASBOX              6       /* control type: string, callback function: (none) */
#define  TABPANEL_2_MEASURECURRENTBUTTON  7       /* control type: command, callback function: ManualMeasure_CB */
#define  TABPANEL_2_VBIASBOX              8       /* control type: string, callback function: (none) */


     /* Control Arrays: */

#define  MAN_CON_ARRAY                    1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK GPIBScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetRelays_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Run(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendGPIB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
