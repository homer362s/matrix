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
#define  MAINPANEL_MAN_CON5_RING          2       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON4_RING          3       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON3_RING          4       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON2_RING          5       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON1_RING          6       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_RENAMEFRAMEBUTTON      7       /* control type: command, callback function: renameFrame_CB */
#define  MAINPANEL_UPDATEDEVIDBUTTON      8       /* control type: command, callback function: renameRow_CB */
#define  MAINPANEL_FRAMEIDBOX             9       /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASUREVOLTAGEBUTTON   10      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_DEVIDBOX               11      /* control type: string, callback function: (none) */
#define  MAINPANEL_IBIASBOX               12      /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASURECURRENTBUTTON   13      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_VBIASBOX               14      /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASADDRESSRING        15      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_MEASDEVICERING         16      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_SOURCEADDRESSRING      17      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_SOURCEDEVICERING       18      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_GPIBSCANBUTTON         19      /* control type: command, callback function: GPIBScan_CB */
#define  MAINPANEL_LOADLAYOUTBUTTON       20      /* control type: command, callback function: LoadLayout_CB */
#define  MAINPANEL_LOADPROBECARDBUTTON    21      /* control type: command, callback function: LoadProbeCard_CB */
#define  MAINPANEL_PROBECARDNAME          22      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TABS                   23      /* control type: tab, callback function: tabs_CB */
#define  MAINPANEL_RESETRELAYSBUTTON      24      /* control type: command, callback function: resetRelays_CB */
#define  MAINPANEL_TEXTMSG                25      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_2           26      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_3           27      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_4              28      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              29      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_4           30      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_5              31      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_STARTREMEASBUTTON      32      /* control type: command, callback function: startAutoReMeasure_CB */
#define  MAINPANEL_STARTMEASBUTTON        33      /* control type: command, callback function: startAutoMeasure_CB */
#define  MAINPANEL_LAYOUTNAME             34      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_SPLITTER               35      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_AUTOFRAMECHECK         36      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_AUTODEVCHECK           37      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_NEWBUTTON              38      /* control type: command, callback function: newFrame_CB */
#define  MAINPANEL_SAVEALLBUTTON          39      /* control type: command, callback function: saveAllFrames_CB */
#define  MAINPANEL_SAVEBUTTON             40      /* control type: command, callback function: saveFrame_CB */
#define  MAINPANEL_DELETEBUTTON           41      /* control type: command, callback function: deleteFrame_CB */
#define  MAINPANEL_REMEASURECURRENTBUTTO  42      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_STATUSBAR              43      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_REMEASUREVOLTAGEBUTTO  44      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_MEASCOEFF              45      /* control type: string, callback function: (none) */
#define  MAINPANEL_INPUTCOEFF             46      /* control type: string, callback function: (none) */
#define  MAINPANEL_DECORATION_5           47      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION             48      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_6           49      /* control type: deco, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_MANUALTABLE           2       /* control type: table, callback function: (none) */


     /* Control Arrays: */

#define  MAN_CON_ARRAY                    1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK addressChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK deleteFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK deviceChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GPIBScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadLayout_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK newFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameRow_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetRelays_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveAllFrames_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoReMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK tabs_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
