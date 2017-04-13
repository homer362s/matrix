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
#define  MAINPANEL_MEASURECURRENTBUTTON   7       /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_VBIASBOX               8       /* control type: string, callback function: (none) */
#define  MAINPANEL_MEASADDRESSRING        9       /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_MEASDEVICERING         10      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_SOURCEADDRESSRING      11      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_SOURCEDEVICERING       12      /* control type: ring, callback function: deviceChanged_CB */
#define  MAINPANEL_GPIBSCANBUTTON         13      /* control type: command, callback function: DevScan_CB */
#define  MAINPANEL_LOADLAYOUTBUTTON       14      /* control type: command, callback function: LoadLayout_CB */
#define  MAINPANEL_LOADPROBECARDBUTTON    15      /* control type: command, callback function: LoadProbeCard_CB */
#define  MAINPANEL_PROBECARDNAME          16      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_RESETRELAYSBUTTON      17      /* control type: command, callback function: resetRelays_CB */
#define  MAINPANEL_TEXTMSG                18      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_2           19      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              20      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_4           21      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_6              22      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_5              23      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_STARTREMEASBUTTON      24      /* control type: command, callback function: startAutoReMeasure_CB */
#define  MAINPANEL_STARTMEASBUTTON        25      /* control type: command, callback function: startAutoMeasure_CB */
#define  MAINPANEL_LAYOUTNAME             26      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_SPLITTER               27      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_REMEASURECURRENTBUTTO  28      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_STATUSBAR              29      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_MEASCOEFF              30      /* control type: string, callback function: (none) */
#define  MAINPANEL_INPUTCOEFF             31      /* control type: string, callback function: (none) */
#define  MAINPANEL_MATRIXADDRRING         32      /* control type: ring, callback function: addressChanged_CB */
#define  MAINPANEL_SINGLEAUTOMEASBUTTON   33      /* control type: command, callback function: singleAutoMeasure_CB */
#define  MAINPANEL_AUTOMEASDEVLIST        34      /* control type: ring, callback function: (none) */
#define  MAINPANEL_DECORATION             35      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_7           36      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_2              37      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_6           38      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_5           39      /* control type: deco, callback function: (none) */
#define  MAINPANEL_COMMANDBUTTON          40      /* control type: command, callback function: showResistanceWindow_CB */
#define  MAINPANEL_TEXTMSG_7              41      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_8              42      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_SPLITTER_5             43      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_SPLITTER_3             44      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_SPLITTER_4             45      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_SPLITTER_2             46      /* control type: splitter, callback function: (none) */
#define  MAINPANEL_SINGLEIVMEASBUTTON     47      /* control type: command, callback function: singleIVMeasure_CB */
#define  MAINPANEL_MEASUREIVBUTTON        48      /* control type: command, callback function: ManualMeasure_CB */
#define  MAINPANEL_VCOUNT                 49      /* control type: string, callback function: ivChanged_CB */
#define  MAINPANEL_VSTEP                  50      /* control type: string, callback function: ivChanged_CB */
#define  MAINPANEL_VSTOP                  51      /* control type: string, callback function: ivChanged_CB */
#define  MAINPANEL_TEXTMSG_12             52      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_13             53      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_VSTART                 54      /* control type: string, callback function: ivChanged_CB */
#define  MAINPANEL_TEXTMSG_4              55      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_9              56      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_8           57      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_10             58      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_11             59      /* control type: textMsg, callback function: (none) */

#define  P_PANEL                          2       /* callback function: pPanel_CB */
#define  P_PANEL_GRAPH                    2       /* control type: graph, callback function: (none) */
#define  P_PANEL_COMMANDBUTTON            3       /* control type: command, callback function: (none) */
#define  P_PANEL_TEXTMSG                  4       /* control type: textMsg, callback function: (none) */

#define  R_PANEL                          3       /* callback function: rPanel_CB */
#define  R_PANEL_TABS                     2       /* control type: tab, callback function: tabs_CB */
#define  R_PANEL_AUTOFRAMECHECK           3       /* control type: radioButton, callback function: (none) */
#define  R_PANEL_NEWBUTTON                4       /* control type: command, callback function: newFrame_CB */
#define  R_PANEL_SAVEALLBUTTON            5       /* control type: command, callback function: saveAllFrames_CB */
#define  R_PANEL_RENAMEFRAMEBUTTON        6       /* control type: command, callback function: renameFrame_CB */
#define  R_PANEL_UPDATEDEVIDBUTTON        7       /* control type: command, callback function: renameRow_CB */
#define  R_PANEL_DEVIDBOX                 8       /* control type: string, callback function: (none) */
#define  R_PANEL_SAVEBUTTON               9       /* control type: command, callback function: saveFrame_CB */
#define  R_PANEL_DELETEBUTTON             10      /* control type: command, callback function: deleteFrame_CB */
#define  R_PANEL_FRAMEIDBOX               11      /* control type: string, callback function: (none) */
#define  R_PANEL_DECORATION_5             12      /* control type: deco, callback function: (none) */
#define  R_PANEL_AUTODEVCHECK             13      /* control type: radioButton, callback function: (none) */
#define  R_PANEL_MAXBOX                   14      /* control type: string, callback function: updateHighlights_CB */
#define  R_PANEL_MINBOX                   15      /* control type: string, callback function: updateHighlights_CB */
#define  R_PANEL_COLORSCHECKBOX           16      /* control type: radioButton, callback function: toggleColors_CB */

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
int  CVICALLBACK DevScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ivChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadLayout_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK mainPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK newFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK renameRow_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetRelays_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK rPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveAllFrames_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK showResistanceWindow_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK singleAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK singleIVMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startAutoReMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK tabs_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK toggleColors_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK updateHighlights_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
