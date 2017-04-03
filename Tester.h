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
#define  MAINPANEL_GPIBADDRESSRING        15      /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBDEVICERING         16      /* control type: ring, callback function: (none) */
#define  MAINPANEL_GPIBSCANBUTTON         17      /* control type: command, callback function: GPIBScan_CB */
#define  MAINPANEL_LOADLAYOUTBUTTON       18      /* control type: command, callback function: LoadLayout_CB */
#define  MAINPANEL_LOADPROBECARDBUTTON    19      /* control type: command, callback function: LoadProbeCard_CB */
#define  MAINPANEL_PROBECARDNAME          20      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TABS                   21      /* control type: tab, callback function: tabs_CB */
#define  MAINPANEL_RESETRELAYSBUTTON      22      /* control type: command, callback function: resetRelays_CB */
#define  MAINPANEL_DECORATION             23      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG                24      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_2           25      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_3           26      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_4              27      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              28      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_4           29      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_5              30      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_STARTMEASBUTTON        31      /* control type: command, callback function: startAutoMeasure_CB */
#define  MAINPANEL_LAYOUTNAME             32      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_SPLITTER               33      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_AUTOFRAMECHECK         34      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_AUTODEVCHECK           35      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_NEWBUTTON              36      /* control type: command, callback function: newFrame_CB */
#define  MAINPANEL_SAVEALLBUTTON          37      /* control type: command, callback function: saveAllFrames_CB */
#define  MAINPANEL_SAVEBUTTON             38      /* control type: command, callback function: saveFrame_CB */
#define  MAINPANEL_DELETEBUTTON           39      /* control type: command, callback function: deleteFrame_CB */
#define  MAINPANEL_REMEASURECURRENTBUTTO  40      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_REMEASUREVOLTAGEBUTTO  41      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_STATUSBAR              42      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_5           43      /* control type: deco, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_MANUALTABLE           2       /* control type: table, callback function: (none) */


     /* Control Arrays: */

#define  MAN_CON_ARRAY                    1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK deleteFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
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
int  CVICALLBACK tabs_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
