/* Copyright (C) 1996-2001 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#if defined( Uses_TCEditor_External ) && !defined( __TCEditor_External__ )
#define __TCEditor_External__

// ************* General definitions
#define UnUsed                  __attribute__ ((unused))

// ************* Redraw commands
const int
  ufUpdate = 0x01,
  ufLine   = 0x02,
  ufView   = 0x04,
  ufStatus = 0x08,
  ufFound  = 0x10,
  ufHLChar = 0x20;

// ************* States of the selection
const int
  smExtend = 0x01,
  smDouble = 0x02,
  smEndSel = 0x04,
  smStartSel=0x08,
  smDontSel=0x80;

// ************* Editor dialogs
const int
  edOutOfMemory     = 0,
  edReadError       = 1,
  edWriteError      = 2,
  edCreateError     = 3,
  edSaveModify      = 4,
  edSaveUntitled    = 5,
  edSaveAs          = 6,
  edFind            = 7,
  edSearchFailed    = 8,
  edReplace         = 9,
  edReplacePrompt   = 10,
  edLineLenght      = 11,
  edGotoLine        = 12,
  edSetLocalOptions = 13,
  edLineOverflow    = 14,
  edSetGlobalOptions= 15,
  edJumpToFunction  = 16,
  edNotFromDisk     = 17,
  edIsReadOnly      = 18,
  edStillReadOnly   = 19,
  edSaveBlock       = 20,
  edReadBlock       = 21,
  edActionWOUndo    = 22,
  edFileExists      = 23,
  edFileNoFile      = 24,
  edCantBkp         = 25,
  edLineTooLong     = 26,
  edRectOutside     = 27,
  edExportHTMLOps   = 28,
  edNoPMacrosAvail  = 29,
  edChoosePMacro    = 30,
  edJumpToPrototype = 31,
  edArbitraryIndent = 32,
  edFileCompMant    = 33;

// ************* Various flags
const int
  efCaseSensitive   = 0x00000001,
  efWholeWordsOnly  = 0x00000002,
  efRegularEx       = 0x00000004,
  efSearchInComm    = 0x00000008,
  efSearchOutComm   = 0x00000010,
  efShowFuncName    = 0x00000020,
  efPromptOnReplace = 0x00000040,
  efReplaceAll      = 0x00000080,

  efOptimizedRegex  = 0x00000200,
  efDoReplace       = 0x00000400,
  efInSelection     = 0x00000800,
  efBackupFiles     = 0x00001000,
  efSaveUNIXasis    = 0x00002000,
  efScrollLock      = 0x00004000,
  efUNIXBkpStyle    = 0x00008000,
  efHiddenBkps      = 0x00010000;

const int
  efBasicRegEx      = 0,
  efExtendedRegEx   = 1,
  efPerlRegEx       = 2;

const int
  efNormalText      = 0,
  efTagsText        = 1;

const int
  efOptimizeRegEx   = 0,
  efNoOptimizeRegEx = 1;

// Maximun values allowed
const int
  setMaxScreenX = 200,
  setMaxScreenY = 100;

// WordUnderCursor
const unsigned
  wucTakeOneLeft = 1,
  wucIncludeColon= 2;

const unsigned
  xhtmlTitle      = 1,
  xhtmlBackground = 2,
  xhtmlMonoFont   = 4,
  xhtmlBoldFont   = 8,
  xhtmlUseColors  = 0x10000;

const int maxArbitraryStrLen=250;


enum shlState { shlNoSyntax=0,shlCSyntax=1,shlPascalSyntax=2,
                shlClipperSyntax=3,shlGenericSyntax=4 };

// ************* Search structure for keywords
struct strSETSE
{
 char *firstLetters;
 int *lenTable;
 int *firstWithLength;
 char **mainTable;
 int *equalCharsInNext;
 int maxLen;
};

// ************* Generic Syntax Highligth structure
class TPMCollection;

typedef struct
{// Default local options
 uint32 setOpts;   // Or mask
 uint32 resetOpts; // And mask
 uint16 tabSize;   // 0 means none
 uint16 indentSize;
 uint16 wrapCol;   // 0 means none
} dflOptions;

#ifndef SUP_PCRE
// Just to avoid errors
typedef void pcre;
#else
#include <pcre.h>
#endif

struct strSHL
{
 char *Name;
 char OpenCom1[4];
 int  lOpenCom1;
 char CloseCom1[4];
 int  lCloseCom1;
 char OpenCom2[4];
 int  lOpenCom2;
 char CloseCom2[4];
 int  lCloseCom2;
 char EOLCom1[4];
 int  lEOLCom1;
 char EOLCom2[4];
 int  lEOLCom2;
 char HexStart[4];
 int  lHexStart;
 char Escape;
 char Preprocessor;
 char Flags1;
 char Flags2;
 char Preprocessor2;
 char UseInternal;
 char *Extensions;
 char *EmacsModes;
 char *ShellScript;
 char *PMacros;
 TPMCollection *PM;
 ushort SymbolT[256];
 TStringCollection *Keywords;
 strSETSE Search;
 TStringCollection *UserWords;
 strSETSE SearchUserWords;
 dflOptions df;
 // RegEx to match a file/full path for this SHL
 pcre *NameMatch;
 pcre *PathMatch;
};

#define FG1_CaseSensitive   0x01
#define FG1_NoNumbers       0x02
#define FG1_EOLCInFirstCol1 0x04
#define FG1_PartialKeyword  0x08
#define FG1_RelaxNumbers    0x10
#define FG1_UserWordsTried  0x20
#define FG1_EOLCInFirstCol2 0x40
#define FG1_EOLCInFirstUse1 0x80
#define FG2_EOLCInFirstUse2 0x01
#define FG2_EscapeAnywhere  0x02

/********************************* UNDO types and constants *****************/
#define MAX_UNDO 32
#define UNDO_CHARS_SIZE 100

enum UndoState { undoNoUndo, undoInMov, undoPutChar, undoDelChar, undoInsert,
                 undoDelete, undoPreDelete, undoPreInsert, undoDestroyLine,
                 undoDelCharDel, undoCutInMov, undoIndBlockSp,undoIndBlockTab,
                 undoIndBlock, undoPre1IndBlock, undoPre2IndBlock, undoUnIndBlock,
                 undoToUpper, undoToLower, undoOvrPutChar, undoPreInsertSp,
                 undoRectPaste, undoRectDel, undoRectCopy, undoRectStart,
                 undoRectEnd, undoRectHide, undoDeleteBuf, undoPostCopyInfo,
                 undoPreCopyInfo, undoRecodeBlock, undoInvertCase, undoAltCase };

enum UndoDestroy { destBack };

struct BufPlusLen
{
 unsigned len;
 char s[0];
};

struct RecodeInfo
{
 int sourID;
 int destID;
 unsigned ops;
};

struct UndoCell
{
 int X,Y;
 int Xf,Yf;
 char *s;
 union
   {
    char *s;
    int  l;
    unsigned OffSet;
    BufPlusLen *BufL;
    RecodeInfo *Recode;
   } s2;
 UndoState Type;
 int Length;
 uint32 selStart,selEnd;
 uint32 selStartf,selEndf;
 char Flags;
 char Count;
};

struct stUndoInsert
{
 char *s;
 BufPlusLen *Eated;
 int   l;
};

const int
      undoSelHided=1,
      undoModified=2,
      undoSelHidedF=4,
      undoModifiedF=8,
      undoOverWriteF=16,
      undoOverWrite=32;
/************************* End of UNDO types and constants *****************/

// ***************** Various Structures
struct GlobalOptionsRect
{
 uint32 t1;
 char tab[3];
 char ind[3];
 char wcol[4];
};
const uint32
 goAutoIndent       =0x00001,
 goUseTabs          =0x00002,
 goPersistentBlocks =0x00004,
 goIntelIndent      =0x00008,
 goCrossCursorInCol =0x00010,
 goCrossCursorInRow =0x00020,
 goShowMatchPair    =0x00040,
 goShowMatchPairFly =0x00080,
 goShowMatchPairNow =0x00100,
 goNoMoveToEndPaste =0x00200,
 goTransparentSel   =0x00400,
 goOptimalFill      =0x00800,
 goWrapLine         =0x01000,
 goScrollLock       =0x02000,
 goSeeTabs          =0x04000,
 goNoInsideTabs     =0x08000,
 goTabIndents       =0x10000,
 goUseIndentSize    =0x20000,
 goDontPurgeSpaces  =0x40000,
 goBackSpUnindents  =0x80000;

const uint32
 loOverwrite        =0x00001,
 loAutoIndent       =0x00002,
 loUseTabs          =0x00004,
 loPersistentBlocks =0x00008,
 loIntelIndent      =0x00010,
 loCrossCursorInCol =0x00020,
 loCrossCursorInRow =0x00040,
 loShowMatchPair    =0x00080,
 loShowMatchPairFly =0x00100,
 loShowMatchPairNow =0x00200,
 loTransparentSel   =0x00400,
 loOptimalFill      =0x00800,
 loWrapLine         =0x01000,
 loSeeTabs          =0x02000,
 loNoInsideTabs     =0x04000,
 loTabIndents       =0x08000,
 loUseIndentSize    =0x10000,
 loDontPurgeSpaces  =0x20000,
 loBackSpUnindents  =0x40000;


typedef unsigned (*TEditorDialog)( int, ... );
extern unsigned defEditorDialog( int dialog, ... );
extern int InitTCEditor(char *s,Boolean force);
extern char *LoadKeyForTCEditorError(void);
extern int LoadKeysForTCEditor(char *file);
extern int PipeTCEditor(unsigned);
// From loadusew.cc:
extern void LoadUserWords(strSHL *s, int id);
extern void TCEditUserWords(char *destinationFile, unsigned contextHelp);
extern const char *GetNameOfUserWordsFile();
// From loaddefl.cc:
extern void LoadDefaultOpts(strSHL *s, int cant);
extern void TCEditDefaultOpts(char *destinationFile, unsigned ctxHelp);
extern const char *GetNameOfDefaultOptsFile();

class TDialog;
// It must be located in the application to customize anything
ushort execDialog(TDialog *d,void *data);

// Default dialog functions from doedidia.cc
typedef struct
{
 uint32 t2;
 TCollection *items;
 ccIndex selection;
} ShlDiagBox;
TDialog *createSetLocalOptions(ShlDiagBox *shlBox);
TDialog *createGotoLineDialog();
TDialog *createSetGlobalOptions();
TDialog *createYesNoAllCancel(TPoint *size, TPoint *cursor);
TDialog *createFindDialog(void *regexBox);
TDialog *createReplaceDialog(void *regexBox);
TDialog *createHTMLExportOps();
TDialog *createPMChoose();
TDialog *createArbitraryIndent(int len);
unsigned LimitedFileNameDialog(unsigned flags, const char *format, const char *file);
void     ShowSavePoint(const char *file);

#endif

#if defined(Uses_EditorId) && !defined(EditorId_Defined)
#define EditorId_Defined
// Identifies a file stored on disk
struct stEditorId
{
 dev_t dev;
 ino_t inode;
};

extern int  FillEditorId(stEditorId *id, const char *name=0, struct stat *st=0);
extern int  CompareEditorId(stEditorId *id1, stEditorId *id2);
extern int  IsEmptyEditorId(stEditorId *id);
#endif
