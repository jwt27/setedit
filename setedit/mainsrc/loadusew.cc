/* Copyright (C) 1996,1997,1998,1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#include <stdio.h>
#define Uses_string

#define Uses_TSOSStringCollection
#define Uses_TCEditor_External
#define Uses_TCEditor_Internal
#define Uses_TCEditor
#define Uses_TPoint
#define Uses_TDialog
#define Uses_TDialogAID
#define Uses_TStringable

#define Uses_TSSortedListBox
#define Uses_TSButton
#define Uses_TSHzGroup
#define Uses_TSInputLine
#define Uses_TSHzLabel

// First include creates the dependencies
#include <easydia1.h>
#include <ceditor.h>
// Second request the headers
#include <easydiag.h>

#include <pathtool.h>
#include <loadshl.h>

static const char *UserWordsFileName="userword.txt";
static const int   maxUserWordLen=80;
static const int   stateLookingName=0,stateCollecting=1,stateExitLoop=2;
static char *destFile=0;
static unsigned localCtxHelp;

static void ReplaceCRby0(char *s)
{
 for (; *s && *s!='\n' && *s!='\r'; s++);
 *s=0;
}

/**[txh]********************************************************************

  Description:
  Loads the user words for the desired syntax hl. The ExpandFileName... hook
is used to find the file containing the words. The routine marks the
FG1_UserWordsTried flag even if didn't succeed.

***************************************************************************/

void LoadUserWords(strSHL *s, int id)
{
 // Mark are already tried
 s->Flags1|=FG1_UserWordsTried;

 char *fileName=ExpandHome(UserWordsFileName);
 FILE *f=fopen(fileName,"rt");
 if (!f)
    return;
 char *name=SHLNameOf(id);
 char line[maxUserWordLen];
 int  state=stateLookingName;
 TStringCollection *col=0;

 while (fgets(line,maxUserWordLen,f) && state!=stateExitLoop)
   {
    ReplaceCRby0(line);
    switch (state)
      {
       case stateLookingName:
            if (line[0]=='.' && strcasecmp(name,line+1)==0)
              {
               state=stateCollecting;
               s->UserWords=col=new TStringCollection(12,6);
              }
            break;
       case stateCollecting:
            if (line[0]=='.')
               state=stateExitLoop;
            else
              if (line[0]=='+')
                 col->insert(newStr(line+1));
            break;
      }
   }
 fclose(f);

 if (col)
    SETSECreateTables(s->SearchUserWords,s->Flags1 & FG1_CaseSensitive,col);
}

/**[txh]********************************************************************

  Description:
  Returns the name of the file (without path) for the user words file. Is
just because the constant is local.

  Return:
  A pointer to the internal constant.

***************************************************************************/

const char *GetNameOfUserWordsFile()
{
 return UserWordsFileName;
}

static
void WriteWords(char *name, TStringCollection *col, FILE *f)
{
 fprintf(f,".%s\n",name);
 int i,c=col->getCount();
 for (i=0; i<c; i++)
     fprintf(f,"+%s\n",(char *)col->at(i));
}

/**[txh]********************************************************************

  Description:
  Updates the user's word file deleting the old definitions and adding the
new ones. The arguments are the name of the syntax hl and the list of user
words.

***************************************************************************/

static
void UpdateFile(char *name, TStringCollection *col)
{
 char *origFile=ExpandHome(UserWordsFileName);
 FILE *dest,*ori;

 if (edTestForFile(origFile))
   {// We must use the values in this file
    if (CompareFileNames(origFile,destFile))
      {// We will overwrite the original
       char *bkpName=newStr(origFile);
       ReplaceExtension(bkpName,".bkp",".txt");
       rename(origFile,bkpName);
       ori=fopen(bkpName,"rt");
       delete[] bkpName;
      }
    else
      {// Is OK we are creating another
       ori=fopen(origFile,"rt");
      }
    if (!ori)
       return;
    dest=fopen(destFile,"wt");
    if (!dest)
      {
       fclose(ori);
       return;
      }
    // Put the edited values at the start of the file
    WriteWords(name,col,dest);
    // Copy all but this definition
    char line[maxUserWordLen];
    int  state=stateLookingName;
   
    while (fgets(line,maxUserWordLen,ori))
      {
       ReplaceCRby0(line);
       switch (state)
         {
          case stateLookingName:
               if (line[0]=='.' && strcasecmp(name,line+1)==0)
                  state=stateCollecting;
               else
                  fprintf(dest,"%s\n",line);
               break;
          case stateCollecting:
               if (line[0]=='.' && strcasecmp(name,line+1)!=0)
                 {
                  state=stateLookingName;
                  fprintf(dest,"%s\n",line);
                 }
               break;
         }
      }
    fclose(ori);
   }
 else
   {// Just create a new one
    dest=fopen(destFile,"wt");
    WriteWords(name,col,dest);
   }
 fclose(dest);
}

/**[txh]********************************************************************

  Description:
  Asks for a new user word.

  Return:
  A newly allocated word or 0 if the user aborted or didn't enter anything.

***************************************************************************/

static
char *EnterNewWord()
{
 TSViewCol *col=new TSViewCol(new TDialog(TRect(1,1,1,1),_("New user word")));

 TSHzLabel *lineLabel=new TSHzLabel(_("Word:"),new TSInputLine(80,20));

 col->insert(2,2,lineLabel);
 EasyInsertOKCancel(col,3);

 TDialog *d=col->doIt();
 delete col;

 d->options|=ofCentered;
 d->helpCtx=localCtxHelp;
 char word[80];
 word[0]=0;

 if (execDialog(d,word)==cmOK && *word)
    return newStr(word);

 return 0;
}

// Simple class for a stringable string, I need it to use TDialogAID
class TStrStringable : public TStringCollection, public TStringable
{
public:
 TStrStringable(ccIndex aLimit, ccIndex aDelta) :
   TStringCollection(aLimit,aDelta),
   TStringable() { };

 virtual void getText(char *dest, unsigned item, int maxLen);
 virtual unsigned GetCount(void) { return getCount(); };
 void insert(char *str) { TStringCollection::insert((void *)str); };
 char *at(ccIndex pos) { return (char *)TStringCollection::at(pos); };
};

void TStrStringable::getText(char *dest, unsigned item, int maxLen)
{
 char *s=at(item);
 strncpy(dest,s,maxLen);
 dest[maxLen]=EOS;
}

static TStrStringable *copy;
static int listChanged;

static
int DeleteWord(int wich)
{
 copy->atRemove(wich);
 listChanged++;
 return 1;
}

static
int AddNewWord(void)
{
 char *s=EnterNewWord();
 if (s)
   {
    listChanged++;
    copy->insert(s);
    return 1;
   }
 return 0;
}

static
void EditList(char *name)
{
 int id=SHLNumberOf(name);

 strSHL *s=&TCEditor::SHLArray[id];
 // If the user words aren't loaded do it now
 if (!(s->Flags1 & FG1_UserWordsTried))
    LoadUserWords(s,id);


 TDialogAID *d=CreateAddInsDelDialog(35,3,_("List of words"),12,30,aidOKEnabled);
 TStringableListBoxRec box;
 d->helpCtx=localCtxHelp;

 // Make a copy to edit
 int i;
 TStringCollection *col=s->UserWords;
 copy=new TStrStringable(col ? col->getCount() : 4,6);
 if (col)
    for (i=0; i<col->getCount(); i++)
        copy->insert(newStr((char *)col->at(i)));

 box.items=copy;
 box.selection=0;
 d->DelAction=DeleteWord;
 d->AddAction=AddNewWord;

 if (copy->getCount()==0)
   {
    //TView::disableCommand(cmOKApply);
    TView::disableCommand(cmDeleteKey);
   }
 listChanged=0;
 int ret=execDialog(d,&box);
 if (ret==cmOK && listChanged)
   {// Destroy the previous words
    destroy(col);
    delete[] s->SearchUserWords.firstLetters;
    // Create a new one
    col=new TStringCollection(copy->getCount(),6);
    for (i=0; i<copy->getCount(); i++)
        col->insert(newStr(copy->at(i)));
    s->UserWords=col;
    SETSECreateTables(s->SearchUserWords,s->Flags1 & FG1_CaseSensitive,col);
    // Force a cache reload
    TCEditor::InvalidateSHLCache();
    UpdateFile(name,col);
   }
 destroy(copy);
}

class TDiaUW : public TDialog
{
public:
 TDiaUW(void);
 virtual void handleEvent(TEvent& event);
 TSortedListBox *list;
};

TDiaUW::TDiaUW(void) :
        TDialog(TRect(3,2,1,1),_("User words")),
        TWindowInit(&TDiaUW::initFrame)
{
}

void TDiaUW::handleEvent(TEvent& event)
{
 if (event.what==evCommand && event.message.command==cmYes)
   {
    EditList((char *)(TCEditor::SHLGenList->at(list->focused)));
    clearEvent(event);
   }
 TDialog::handleEvent(event);
}


/**[txh]********************************************************************

  Description:
  Entry point for the dialogs stuff of the user's words. Call it passing the
name of the destination file where the definitions will be stored.@p
  You should specify the help context to be used for the dialogs.

***************************************************************************/

void TCEditUserWords(char *destinationFile, unsigned ctxHelp)
{
 if (!TCEditor::SHLGenList)
    return;

 destFile=newStr(destinationFile);
 localCtxHelp=ctxHelp;
 TDiaUW *duw=new TDiaUW();
 TSViewCol *col=new TSViewCol(duw);

 #define VeSHLW 30
 TSSortedListBox *ListaH=new TSSortedListBox(VeSHLW,12,tsslbVertical);
 duw->list=(TSortedListBox *)ListaH->view;

 TSHzGroup *buts=new TSHzGroup(new TSButton(_("~E~dit"),cmYes,bfDefault),
                               new TSButton(_("O~K~"),cmOK));

 col->insert(2,1,ListaH);
 col->insert(xTSCenter,yTSDown,buts);
 col->doIt();
 delete col;

 duw->helpCtx=localCtxHelp;
 struct
 {
  TCollection *c;
  ccIndex f;
 } box;
 box.c=TCEditor::SHLGenList;
 box.f=0;
 execDialog(duw,&box);

 delete[] destFile;
 destFile=0;
}
