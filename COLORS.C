/************** Dialog2 C Sample Program Source Code File (.C) ****************/
#define INCL_GPI
#define INCL_BASE
#define INCL_WIN
#include <os2.h>
#include "colors.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
MRESULT EXPENTRY ColorProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY fnwpColorSample( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
/***************** Start of main procedure ***************************/
SHORT RGBColors[3];
LONG  Color,Colors[100];
CHAR ProfileColors[12];
HAB hab;
/*********************************************************************/
/*                                                                   */
/*  FUNCTION: main                                                   */
/*                                                                   */
int main( int argc, char *argv[] )
{
  HMQ     hmq;                        /* Message queue handle         */
  hab   = WinInitialize( 0 );         /* Initialize PM                */
  hmq   = WinCreateMsgQueue( hab, 0 );/* Create application msg queue */
  /* Invoke a modal dialog with the main window frame as owner       */
  WinRegisterClass( hab,              /* Register a second class      */
                    "Color",          /*  for color sample child      */
                    fnwpColorSample,  /*  window of the Client Area   */
                    0L,
                    0 );
  WinDlgBox( HWND_DESKTOP,              /* Parent                    */
             HWND_DESKTOP,              /* Owner                     */
             ColorProc,                 /* Address of dialog proc    */
             NULLHANDLE,                /* Module handle             */
             ID_COLORS,                 /* ID of dialog in resource  */
             NULL );                    /* Initialization data       */
  WinDestroyMsgQueue( hmq );
  WinTerminate( hab );
  return 0;
}
/*********************************************************************/

MRESULT EXPENTRY ColorProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  ULONG Index;
  HPOINTER Hptr;
#define SCROLLPAGE 16
  SHORT Slider;
  ULONG SliderId;

  switch (msg)
  {
case WM_VSCROLL:
     SliderId = (ULONG)SHORT1FROMMP(mp1);
     Slider= (SHORT) WinSendDlgItemMsg(hwndDlg,SliderId,
                                      SBM_QUERYPOS,MPVOID,MPVOID);

     switch (SHORT2FROMMP(mp2))
       {
       case SB_SLIDERTRACK:
          Slider=SHORT1FROMMP(mp2);
       break;
    case SB_LINEUP:
        if (Slider>0) Slider--;
       break;
    case SB_LINEDOWN:
        if (Slider<255) Slider++;
       break;
    case SB_PAGEUP:
        if ((Slider-SCROLLPAGE)>=0) Slider-=SCROLLPAGE;
       break;
    case SB_PAGEDOWN:
        if ((Slider+SCROLLPAGE)<=255) Slider+=SCROLLPAGE;
       break;
       default:
       break;
       }
       RGBColors[SliderId-ID_COLOR_BASE]=255-Slider;
       WinSendDlgItemMsg(hwndDlg,SliderId,
                         SBM_SETPOS,MPFROMSHORT(Slider),MPVOID);
       WinInvalidateRect(WinWindowFromID(hwndDlg,ID_MIX),NULL,TRUE);
       break;
    case WM_INITDLG:
       WinLoadMenu(hwndDlg,NULLHANDLE,ID_MENU);
       WinSendMsg(hwndDlg,WM_UPDATEFRAME,MPFROMLONG(FCF_MENU),MPVOID);
       Hptr=WinLoadPointer(HWND_DESKTOP,NULLHANDLE,ID_COLORS);
       if (Hptr==(HPOINTER)NULL) {
         DosBeep(400,100);
       } /* endif */
       WinSendMsg(hwndDlg,WM_SETICON,MPFROMLONG(Hptr),MPVOID);
       WinSendDlgItemMsg(hwndDlg,ID_RED,
                  SBM_SETSCROLLBAR,MPFROMSHORT(255),MPFROM2SHORT(0,255));
       WinSendDlgItemMsg(hwndDlg,ID_BLUE,
                  SBM_SETSCROLLBAR,MPFROMSHORT(255),MPFROM2SHORT(0,255));
       WinSendDlgItemMsg(hwndDlg,ID_GREEN,
                  SBM_SETSCROLLBAR,MPFROMSHORT(255),MPFROM2SHORT(0,255));
      for (Index=0;Index<NUM_PMCOLORS;Index++) {
          Color=WinQuerySysColor(HWND_DESKTOP,PMColors[Index].SystemColor,0L);
          PMColors[Index].Red  =LOUCHAR(HIUSHORT(Color));
          PMColors[Index].Green=HIUCHAR(LOUSHORT(Color));
          PMColors[Index].Blue =LOUCHAR(LOUSHORT(Color));
          WinSendDlgItemMsg(hwndDlg,ID_LIST, LM_INSERTITEM,
                            MPFROMSHORT(LIT_END), MPFROMP(PMColors[Index].Label) );
      } /* endfor */
       WinSendDlgItemMsg(hwndDlg,ID_LIST, LM_SETTOPINDEX,MPFROMSHORT(1),MPVOID);
      break;
    case WM_CONTROL:
      switch( SHORT2FROMMP( mp1 ) )
      {
       case LN_ENTER:                           /* Catch double click on */
          Index=(USHORT)WinSendMsg( WinWindowFromID(hwndDlg,ID_LIST), /* list box item, and    */
                      LM_QUERYSELECTION,     /* get current selection */
                      MPFROMSHORT(LIT_FIRST),
                      MPVOID );
          Color=WinQuerySysColor(HWND_DESKTOP,PMColors[Index].SystemColor,0L);
          PMColors[Index].Red  =LOUCHAR(HIUSHORT(Color));
          PMColors[Index].Green=HIUCHAR(LOUSHORT(Color));
          PMColors[Index].Blue =LOUCHAR(LOUSHORT(Color));
          RGBColors[RED]=PMColors[Index].Red;
          WinSendDlgItemMsg(hwndDlg,ID_RED, SBM_SETPOS,     /* get current selection */
                      MPFROMSHORT(255-RGBColors[RED]), (MPARAM)NULL );
          RGBColors[GREEN]=PMColors[Index].Green;
          WinSendDlgItemMsg(hwndDlg,ID_GREEN, SBM_SETPOS,     /* get current selection */
                      MPFROMSHORT(255-RGBColors[GREEN]), (MPARAM)NULL );
          RGBColors[BLUE]=PMColors[Index].Blue;
          WinSendDlgItemMsg(hwndDlg,ID_BLUE, SBM_SETPOS,     /* get current selection */
                      MPFROMSHORT(255-RGBColors[BLUE]), MPVOID );
          WinInvalidateRect(WinWindowFromID(hwndDlg,ID_MIX),(PRECTL)NULL,TRUE);
          break;
        default:
          break;
      }
      break;
    case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
      {
        case DID_OK:    /* Enter key pressed or pushbutton selected  */
          Index=(USHORT)WinSendMsg( WinWindowFromID(hwndDlg,ID_LIST), /* list box item, and    */
                      LM_QUERYSELECTION,     /* get current selection */
                      (MPARAM)LIT_FIRST, (MPARAM)NULL );
          if (Index==LIT_NONE) break;
          Color=RGBColors[BLUE]+(256L*(LONG)RGBColors[GREEN])+(65536L*(LONG)RGBColors[RED]);
          Colors[0]=Color;
          WinSetSysColors(HWND_DESKTOP, 0L, LCOLF_CONSECRGB,
                       PMColors[Index].SystemColor, 1L, Colors);
          Color=WinQuerySysColor(HWND_DESKTOP,PMColors[Index].SystemColor,0L);
          PMColors[Index].Red  =LOUCHAR(HIUSHORT(Color));
          PMColors[Index].Green=HIUCHAR(LOUSHORT(Color));
          PMColors[Index].Blue =LOUCHAR(LOUSHORT(Color));
          sprintf(ProfileColors,"%3.3d %3.3d %3.3d",PMColors[Index].Red,PMColors[Index].Green,PMColors[Index].Blue);
          PrfWriteProfileData( HINI_SYSTEMPROFILE, "PM_Colors", PMColors[Index].Key , ProfileColors, 12);
          return FALSE;
          break;
        case ID_EXIT:    /* Enter key pressed or pushbutton selected  */
             WinDismissDlg( hwndDlg, TRUE ); /* Finished with dialog box    */
          break;
        default:
          return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
      }
      break;

    default:  /* Pass all other messages to the default dialog proc  */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return FALSE;
}

/******************* End of first dialog procedure *******************/

/*********************************************************************/

MRESULT EXPENTRY fnwpColorSample( HWND hwnd, ULONG msg,
                                  MPARAM mp1, MPARAM mp2 )
{
  HPS    hps;
  RECTL  rcl,rcl1;
  if( msg == WM_PAINT )
  {
       hps = WinBeginPaint( hwnd, NULLHANDLE, &rcl );
       Color=RGBColors[BLUE]+(256L*(LONG)RGBColors[GREEN])+(65536L*(LONG)RGBColors[RED]);
       GpiCreateLogColorTable( hps, LCOL_RESET,
                              LCOLF_RGB,0L,1L,&Color);
       rcl1.xLeft=rcl.xLeft;
       rcl1.xRight=rcl.xRight;
       rcl1.yTop=rcl.yTop;
       rcl1.yBottom=rcl.yTop-4;
       WinFillRect( hps, &rcl1, CLR_BLACK );
       rcl1.xLeft=rcl.xLeft;
       rcl1.xRight=rcl.xRight;
       rcl1.yTop=rcl.yBottom+4;
       rcl1.yBottom=rcl.yBottom;
       WinFillRect( hps, &rcl1, CLR_BLACK );
       rcl1.xLeft=rcl.xLeft;
       rcl1.xRight=rcl.xLeft+4;
       rcl1.yTop=rcl.yTop-4;
       rcl1.yBottom=rcl.yBottom+4;
       WinFillRect( hps, &rcl1, CLR_BLACK );
       rcl1.xLeft=rcl.xRight-4;
       rcl1.xRight=rcl.xRight;
       rcl1.yTop=rcl.yTop-4;
       rcl1.yBottom=rcl.yBottom+4;
       WinFillRect( hps, &rcl1, CLR_BLACK );
       rcl.xLeft+=5;
       rcl.yBottom+=5;
       rcl.xRight-=5;
       rcl.yTop-=5;
       WinFillRect( hps, &rcl, Color );
       WinEndPaint( hps );                      /* Drawing is complete   */
    return FALSE;
  }
  else
    /* Pass all other messages to the default window procedure       */
    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

