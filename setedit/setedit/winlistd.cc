/* Copyright (C) 1996,1997,1998,1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#define Uses_TRect
#define Uses_TDialog
#define Uses_TScrollBar
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TApplication
#define Uses_TProgram
#define Uses_TCollection
#define Uses_TListBox
#define Uses_MsgBox
#define Uses_TCEditWindow
#define Uses_limits
#define Uses_TSHzGroup
#define Uses_TSButton
#define Uses_TSListBox
#include <easydia1.h>
#include <ceditor.h>
#include <easydiag.h>

#include <dskwin.h>
#define Uses_TSListEditors
#include <edcollec.h>
#include <editcoma.h>
#define Uses_TSetEditorApp
#define Uses_SETAppConst
#include <setapp.h>
#include <advice.h>

TView *setFocusTo;
Boolean focusChanged;
TEditorCollection *edHelper=NULL;

class TListWindowsDiag : public TDialog
{
public:
 TListWindowsDiag();
 void handleEvent(TEvent &event);
 TListEditors *tl;
};

TListWindowsDiag::TListWindowsDiag() :
 TDialog(TRect(1,1,1,1),_("Windows List")),
 TWindowInit(TListWindowsDiag::initFrame)
{
}

void TListWindowsDiag::handleEvent(TEvent &event)
{
 TDialog::handleEvent(event);
 if (event.what==evCommand || event.what==evBroadcast)
   {
    ccIndex pos = tl->focused;
    TDskWin *obj = (TDskWin *)(edHelper->at(pos));
 
    switch( event.message.command )
      {
       case cmListItemSelected:
       case cmGo:
            if (obj->GoAction(pos))
               delete obj;
            tl->setRange(edHelper->getCount());
            break;
 
       case cmDelete:
            if (obj->DeleteAction(pos,False))
               delete obj;
            tl->setRange(edHelper->getCount());
            break;
 
       case cmInsert:
            message(editorApp,evCommand,cmeOpen,0);
            break;
 
       case cmDelFile:
            if (obj->CanBeDeletedFromDisk)
              {
               if (GiveAdvice(gadvDeleteFile0)==cmYes)
                 {
                  if (obj->DeleteAction(pos,True))
                     delete obj;
                  tl->setRange(edHelper->getCount());
                 }
               else
                 {
                  clearEvent(event);
                  return;
                 }
              }
            else
              {
               messageBox(_("You can't delete it from disk"),mfOKButton);
               clearEvent(event);
               return;
              }
            break;
 
       default:
            return;
      }
    if (state & sfModal)
      {
       endModal(event.message.command);
       clearEvent(event);
      }
   }
}

void BringListOfWindows(void)
{
 TRect r=TApplication::deskTop->getExtent();
 int h=r.b.y-r.a.y-10;
 int w=r.b.x-r.a.x-15;

 TListWindowsDiag *d=new TListWindowsDiag();
 TSViewCol *col=new TSViewCol(d);
 TSListEditors *list=new TSListEditors(w,h,tsslbVertical | tsslbHorizontal,1,PATH_MAX);
 d->tl=(TListEditors *)list->view;
 TSHzGroup *buttons=MakeHzGroup(
   new TSButton(_("~G~o"),cmGo,bfDefault),
   new TSButton(_("~D~elete"),cmDelete),
   new TSButton(_("~O~pen"),cmInsert),
   new TSButton(_("Cancel"),cmCancel),0);
 col->insert(xTSLeft,yTSUp,list);
 col->insert(xTSCenter,yTSUnder,buttons,0,list);
 col->doItCenter(hcListWin);
 delete col;

 TListBoxRec box;
 box.items=edHelper;
 box.selection=0;
 d->setData(&box);

 focusChanged=False;
 int i;

 do
  {
   TProgram::deskTop->insert(d);
   d->setState(sfModal,True);
   i = d->execute();
   TProgram::deskTop->remove(d);
  }
 while (i==cmDelete || i==cmDelFile);
 delete d;

 if (focusChanged)
   {
    setFocusTo->select();
    setFocusTo->show();
    TProgram::deskTop->unlock();
   }
}
