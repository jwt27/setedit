/* Copyright (C) 1996,1997,1998,1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#if defined(Uses_TDialogAID) && !defined(__TDialogAID__)
#define __TDialogAID__

class TDialogAID : public TGrowDialog
{
public:
 TDialogAID(const TRect& bounds, const char *aTitle,
            TScrollBar *sb, TStringableListBox *slb);
 virtual void handleEvent(TEvent& event);
 int (*AddAction)(void);
 int (*InsAction)(int);
 int (*DelAction)(int);
 int (*OkAction)(void);
 int (*CancelAction)(void);
 TStringableListBox *List;
 unsigned flags;
};

const int
      cmAddKey=0x2240,
      cmDeleteKey=0x2241,
      cmInsertKey=0x2242,
      cmAddCommand=0x2243,
      cmAddMacro=0x2244,
      cmOKApply=0x2245,
      cmCancelApply=0x2246;
const int aidInsert=1, aidComMac=2, aidOKEnabled=4;

#if 1
// Ugh! nasty, no? Fix me ... some day
#define Uses_SETAppConst
#include <../../setedit/include/setapp.h>
#else
#define cmeZoom cmZoom
#endif

// Used to create the dialogs
TDialogAID *CreateAddInsDelDialog(int x, int y, char *name, int h, int w,
                                  int flags);
TDialog *CreateChooseDialog(int x, int y, char *name, int h, int w);

#endif
