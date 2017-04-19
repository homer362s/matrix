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

#define  MAINPANEL                        1       /* callback function: mainPanel_CB */
#define  MAINPANEL_MAN_CON5_RING          2       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON4_RING          3       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON3_RING          4       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON2_RING          5       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_MAN_CON1_RING          6       /* control type: ring, callback function: ManConnectionChanged_CB */
#define  MAINPANEL_RENAMEFRAMEBUTTON      7       /* control type: command, callback function: renameFrame_CB */
#define  MAINPANEL_UPDATEDEVIDBUTTON      8       /* control type: command, callback function: renameRow_CB */
#define  MAINPANEL_FRAMEIDBOX             9       /* control type: string, callback function: (none) */
#define  MAINPANEL_DEVIDBOX               10      /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASURECURRENTBUTTON   11      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_VBIASBOX               12      /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASADDRRING           13      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_MEASDEVRING            14      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_SOURCEADDRRING         15      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_SOURCEDEVRING          16      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_GPIBSCANBUTTON         17      /* control type: command, callback function: DevScan_CB */
#define  MAINPANEL_LOADLAYOUTBUTTON       18      /* control type: command, callback function: LoadLayout_CB */
#define  MAINPANEL_LOADPROBECARDBUTTON    19      /* control type: command, callback function: LoadProbeCard_CB */
#define  MAINPANEL_PROBECARDNAME          20      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TABS                   21      /* control type: tab, callback function: tabs_CB */
#define  MAINPANEL_RESETRELAYSBUTTON      22      /* control type: command, callback function: resetRelays_CB */
#define  MAINPANEL_TEXTMSG                23      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              24      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_4           25      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_5              26      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_STARTREMEASBUTTON      27      /* control type: command, callback function: startAutoReMeasure_CB */
#define  MAINPANEL_STARTMEASBUTTON        28      /* control type: command, callback function: startAutoMeasure_CB */
#define  MAINPANEL_LAYOUTNAME             29      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_SPLITTER               30      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_AUTOFRAMECHECK         31      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_AUTODEVCHECK           32      /* control type: radioButton, callback function: (none) */
#define  MAINPANEL_NEWBUTTON              33      /* control type: command, callback function: newFrame_CB */
#define  MAINPANEL_SAVEALLBUTTON          34      /* control type: command, callback function: saveAllFrames_CB */
#define  MAINPANEL_SAVEBUTTON             35      /* control type: command, callback function: saveFrame_CB */
#define  MAINPANEL_DELETEBUTTON           36      /* control type: command, callback function: deleteFrame_CB */
#define  MAINPANEL_REMEASURECURRENTBUTTO  37      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_STATUSBAR              38      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_MEASCOEFF              39      /* control type: string, callback function: (none) */
#define  MAINPANEL_INPUTCOEFF             40      /* control type: string, callback function: (none) */
#define  MAINPANEL_MATRIXADDRRING         41      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_SINGLEAUTOMEASBUTTON   42      /* control type: command, callback function: singleAutoMeasure_CB */
#define  MAINPANEL_AUTOMEASDEVLIST        43      /* control type: ring, callback function: (none) */
#define  MAINPANEL_MAXBOX                 44      /* control type: string, callback function: updateHighlights_CB */
#define  MAINPANEL_MINBOX                 45      /* control type: string, callback function: updateHighlights_CB */
#define  MAINPANEL_DECORATION_5           46      /* control type: deco, callback function: (none) */
#define  MAINPANEL_COLORSCHECKBOX         47      /* control type: radioButton, callback function: toggleColors_CB */
#define  MAINPANEL_DECORATION             48      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_7           49      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_2              50      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_PRODUCTVERSION         51      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_2           52      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_6           53      /* control type: deco, callback function: (none) */
#define  MAINPANEL_AUTODISCONNBUTTON      54      /* control type: command, callback function: AutoConnect_CB */
#define  MAINPANEL_AUTOCONNBUTTON         55      /* control type: command, callback function: AutoConnect_CB */
#define  MAINPANEL_AUTOCONNLED            56      /* control type: LED, callback function: (none) */
#define  MAINPANEL_DECORATION_3           57      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_8           58      /* control type: deco, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_MANUALTABLE           2       /* control type: table, callback function: (none) */


     /* Control Arrays: */

#define  MAN_CON_ARRAY                    1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK addressChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AutoConnect_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK deleteFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK deviceChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DevScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadLayout_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK mainPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK newFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameRow_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetRelays_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveAllFrames_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK singleAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoReMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK tabs_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK toggleColors_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK updateHighlights_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
