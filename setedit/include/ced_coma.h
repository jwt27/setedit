/* Copyright (C) 1996-2005 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#if defined( Uses_TCEditor_Commands ) && !defined( __TCEditor_Commands__ )
#define __TCEditor_Commands__

const int
/* Robert's TV library supports enable/disable commands for commands up to
   1023. A safe value for RHIDE is from 600. */
  cmbBaseNumber        = 600,
  cmbCtrlK             = 254,
  cmbCtrlQ             = 255,
  cmbToLower           =   1,
  cmbSelLength         =   2,
  cmbFirstLineInScreen =   3,
  cmbLastLineInScreen  =   4,
  cmbIndentBlkOne      =   5,
  cmbUnIndentBlkOne    =   6,
  cmbIndentBlk         =   7,
  cmbUnIndentBlk       =   8,
  cmbSearchStart       =   9,
  cmbSearchEnd         =  10,
  cmbExpandCode        =  11,
  cmbDelPrevWord       =  12,
  cmbEndSelect         =  13,
  cmbSmartUnIndent     =  14,
  cmbSmartIndent       =  15,
  cmbReplaceSelect     =  16,
  cmbGotoEditorLine    =  17,
  cmbGoBeginBlock      =  18,
  cmbGoEndBlock        =  19,
  cmbMarkLine          =  20,
  cmbMarkWord          =  21,
  cmbMoveBlock         =  22,
  cmbScrollDown        =  23,
  cmbScrollUp          =  24,
  cmbPutMark0          =  25,
  cmbPutMark1          =  26,
  cmbPutMark2          =  27,
  cmbPutMark3          =  28,
  cmbPutMark4          =  29,
  cmbPutMark5          =  30,
  cmbPutMark6          =  31,
  cmbPutMark7          =  32,
  cmbPutMark8          =  33,
  cmbPutMark9          =  34,
  cmbGotoMark0         =  35,
  cmbGotoMark1         =  36,
  cmbGotoMark2         =  37,
  cmbGotoMark3         =  38,
  cmbGotoMark4         =  39,
  cmbGotoMark5         =  40,
  cmbGotoMark6         =  41,
  cmbGotoMark7         =  42,
  cmbGotoMark8         =  43,
  cmbGotoMark9         =  44,
  cmbInsertText        =  45,
  cmbReadBlock         =  46,
  cmbWriteBlock        =  47,
  cmbNewLine           =  48,
  cmbBackSpace         =  49,
  cmbCharLeft          =  50,
  cmbCharRight         =  51,
  cmbWordLeft          =  52,
  cmbWordRight         =  53,
  cmbLineStart         =  54,
  cmbLineEnd           =  55,
  cmbLineUp            =  56,
  cmbLineDown          =  57,
  cmbPageUp            =  58,
  cmbPageDown          =  59,
  cmbTextStart         =  60,
  cmbTextEnd           =  61,
  cmbDelChar           =  62,
  cmbDelWord           =  63,
  cmbDelStart          =  64,
  cmbDelEnd            =  65,
  cmbDelLine           =  66,
  cmbInsMode           =  67,
  cmbStartSelect       =  68,
  cmbHideSelect        =  69,
  cmbIndentMode        =  70,
  cmbUpdateTitle       =  71,
  cmbCut               =  72,
  cmbCopy              =  73,
  cmbPaste             =  74,
  cmbUndo              =  75,
  cmbSearchAgain       =  76,
  cmbClear             =  77,
  cmbReplace           =  78,
  cmbFind              =  79,
  cmbSearchOpPar       =  80,
  cmbSearchClPar       =  81,
  cmbSearchOpCor       =  82,
  cmbSearchClCor       =  83,
  cmbToUpper           =  84,
  cmbRedo              =  85,
  cmbSetLocalOptions   =  86,
  cmbExpandAllTabs     =  87,
  cmbCompactBuffer     =  88,
  cmbRecordMacro       =  89,
  cmbStopMacro         =  90,
  cmbPlayMacro         =  91,
  cmbSetGlobalOptions  =  92,
  cmbSaveAsConvertEOL  =  93,
  cmbCopyBlock         =  94,
  cmbSelRectStart      =  95,
  cmbSelRectEnd        =  96,
  cmbSelRectCopy       =  97,
  cmbSelRectPaste      =  98,
  cmbSelRectDel        =  99,
  cmbSelRectCut        = 100,
  cmbSelRectMove       = 101,
  cmbSelRectHide       = 102,
  cmbLastPosCur        = 103,
  cmbToggleMoveOnPaste = 104,
  cmbJumpToFunction    = 105,
  cmbProfileEditor     = 106,
  cmbColorsChanged     = 107,
  cmbLoadFileUnderCur  = 108,
  cmbSearchComplement  = 109,
  cmbCopyClipWin       = 110, // Old name, deprecated
  cmbCopyOSClip        = 110,
  cmbPasteClipWin      = 111, // Old name, deprecated
  cmbPasteOSClip       = 111,
  cmbIntelIndentMode   = 112,
  cmbDelCharClear      = 113,
  cmbSave              = 114,
  cmbSaveAs            = 115,
  cmbChooseMacro       = 116,
  cmbRepeatMacro       = 117,
  cmbGenCodeForMacro   = 118,
  cmbSelectOn          = 119,
  cmbSelectOff         = 120,
  cmbSaveSameTime      = 121,
  cmbWrapOn            = 122,
  cmbWrapOff           = 123,
  cmbWrapToggle        = 124,
  cmbUpdateCodePage    = 125,
  cmbMouseToggle       = 126,
  cmbSelRectToUpper    = 127,
  cmbSelRectToLower    = 128,
  cmbPasteEmacsMode    = 129,
  cmbGoEndOfWord       = 130,
  cmbForceMatchPairHL  = 131,
  cmbChoosePMacrosList = 132,
  cmbPushCursorPos     = 133,
  cmbPopCursorPos      = 134,
  cmbToggleCharCase    = 135,
  cmbJumpToPrototype   = 136,
  cmbCutClipWin        = 137,
  cmbCutOSClip         = 137,
  cmbQuotedPrintDecode = 138,
  cmbArbitraryIndent   = 139,
  cmbLastColInScreen   = 140,
  cmbLineOrScrEnd      = 141,
  cmbWhichFunctionIs   = 142,
  cmbCommentIndent     = 143,
  cmbCommentUnIndent   = 144,
  cmbInvertCase        = 145,
  cmbAltCase           = 146,
  cmbInsertKeyName     = 147,
  cmbRunSel_sLisp      = 148,
  cmbRunEnter_sLisp    = 149,
  cmbCopyClipFile      = 150,
  cmbPasteClipFile     = 151,
  cmbEditorGotFocus    = 152,
  cmbJumpLastCursorPos = 153,
  cmbInsertNewLine     = 154,
  cmbSaveAsNoConvertEOL= 155,
  cmbExpandAllTabsNi   = 156,
  cmbCompactBufferNi   = 157,
  cmbSearchSelForward  = 158,
  cmbSearchSelBackward = 159,
  cmbSmartTab          = 160,
  
  cmbLastCommand       = 161;


const int
  cmcCtrlK             = cmbCtrlK+cmbBaseNumber,
  cmcCtrlQ             = cmbCtrlQ+cmbBaseNumber,
  cmcToUpper           = cmbToUpper+cmbBaseNumber,
  cmcToLower           = cmbToLower+cmbBaseNumber,
  cmcSelLength         = cmbSelLength+cmbBaseNumber,
  cmcFirstLineInScreen = cmbFirstLineInScreen+cmbBaseNumber,
  cmcLastLineInScreen  = cmbLastLineInScreen+cmbBaseNumber,
  cmcIndentBlkOne      = cmbIndentBlkOne+cmbBaseNumber,
  cmcUnIndentBlkOne    = cmbUnIndentBlkOne+cmbBaseNumber,
  cmcIndentBlk         = cmbIndentBlk+cmbBaseNumber,
  cmcUnIndentBlk       = cmbUnIndentBlk+cmbBaseNumber,
  cmcSearchStart       = cmbSearchStart+cmbBaseNumber,
  cmcSearchEnd         = cmbSearchEnd+cmbBaseNumber,
  cmcExpandCode        = cmbExpandCode+cmbBaseNumber,
  cmcDelPrevWord       = cmbDelPrevWord+cmbBaseNumber,
  cmcEndSelect         = cmbEndSelect+cmbBaseNumber,
  cmcSmartUnIndent     = cmbSmartUnIndent+cmbBaseNumber,
  cmcSmartIndent       = cmbSmartIndent+cmbBaseNumber,
  cmcReplaceSelect     = cmbReplaceSelect+cmbBaseNumber,
  cmcGotoEditorLine    = cmbGotoEditorLine+cmbBaseNumber,
  cmcGoBeginBlock      = cmbGoBeginBlock+cmbBaseNumber,
  cmcGoEndBlock        = cmbGoEndBlock+cmbBaseNumber,
  cmcMarkLine          = cmbMarkLine+cmbBaseNumber,
  cmcMarkWord          = cmbMarkWord+cmbBaseNumber,
  cmcMoveBlock         = cmbMoveBlock+cmbBaseNumber,
  cmcScrollDown        = cmbScrollDown+cmbBaseNumber,
  cmcScrollUp          = cmbScrollUp+cmbBaseNumber,
  cmcPutMark0          = cmbPutMark0+cmbBaseNumber,
  cmcPutMark1          = cmbPutMark1+cmbBaseNumber,
  cmcPutMark2          = cmbPutMark2+cmbBaseNumber,
  cmcPutMark3          = cmbPutMark3+cmbBaseNumber,
  cmcPutMark4          = cmbPutMark4+cmbBaseNumber,
  cmcPutMark5          = cmbPutMark5+cmbBaseNumber,
  cmcPutMark6          = cmbPutMark6+cmbBaseNumber,
  cmcPutMark7          = cmbPutMark7+cmbBaseNumber,
  cmcPutMark8          = cmbPutMark8+cmbBaseNumber,
  cmcPutMark9          = cmbPutMark9+cmbBaseNumber,
  cmcGotoMark0         = cmbGotoMark0+cmbBaseNumber,
  cmcGotoMark1         = cmbGotoMark1+cmbBaseNumber,
  cmcGotoMark2         = cmbGotoMark2+cmbBaseNumber,
  cmcGotoMark3         = cmbGotoMark3+cmbBaseNumber,
  cmcGotoMark4         = cmbGotoMark4+cmbBaseNumber,
  cmcGotoMark5         = cmbGotoMark5+cmbBaseNumber,
  cmcGotoMark6         = cmbGotoMark6+cmbBaseNumber,
  cmcGotoMark7         = cmbGotoMark7+cmbBaseNumber,
  cmcGotoMark8         = cmbGotoMark8+cmbBaseNumber,
  cmcGotoMark9         = cmbGotoMark9+cmbBaseNumber,
  cmcInsertText        = cmbInsertText+cmbBaseNumber,
  cmcReadBlock         = cmbReadBlock+cmbBaseNumber,
  cmcWriteBlock        = cmbWriteBlock+cmbBaseNumber,
  cmcNewLine           = cmbNewLine+cmbBaseNumber,
  cmcBackSpace         = cmbBackSpace+cmbBaseNumber,
  cmcCharLeft          = cmbCharLeft+cmbBaseNumber,
  cmcCharRight         = cmbCharRight+cmbBaseNumber,
  cmcWordLeft          = cmbWordLeft+cmbBaseNumber,
  cmcWordRight         = cmbWordRight+cmbBaseNumber,
  cmcLineStart         = cmbLineStart+cmbBaseNumber,
  cmcLineEnd           = cmbLineEnd+cmbBaseNumber,
  cmcLineUp            = cmbLineUp+cmbBaseNumber,
  cmcLineDown          = cmbLineDown+cmbBaseNumber,
  cmcPageUp            = cmbPageUp+cmbBaseNumber,
  cmcPageDown          = cmbPageDown+cmbBaseNumber,
  cmcTextStart         = cmbTextStart+cmbBaseNumber,
  cmcTextEnd           = cmbTextEnd+cmbBaseNumber,
  cmcDelChar           = cmbDelChar+cmbBaseNumber,
  cmcDelWord           = cmbDelWord+cmbBaseNumber,
  cmcDelStart          = cmbDelStart+cmbBaseNumber,
  cmcDelEnd            = cmbDelEnd+cmbBaseNumber,
  cmcDelLine           = cmbDelLine+cmbBaseNumber,
  cmcInsMode           = cmbInsMode+cmbBaseNumber,
  cmcStartSelect       = cmbStartSelect+cmbBaseNumber,
  cmcHideSelect        = cmbHideSelect+cmbBaseNumber,
  cmcIndentMode        = cmbIndentMode+cmbBaseNumber,
  cmcUpdateTitle       = cmbUpdateTitle+cmbBaseNumber,
  cmcCut               = cmbCut+cmbBaseNumber,
  cmcCopy              = cmbCopy+cmbBaseNumber,
  cmcPaste             = cmbPaste+cmbBaseNumber,
  cmcUndo              = cmbUndo+cmbBaseNumber,
  cmcSearchAgain       = cmbSearchAgain+cmbBaseNumber,
  cmcClear             = cmbClear+cmbBaseNumber,
  cmcReplace           = cmbReplace+cmbBaseNumber,
  cmcFind              = cmbFind+cmbBaseNumber,
  cmcSearchOpPar       = cmbSearchOpPar+cmbBaseNumber,
  cmcSearchClPar       = cmbSearchClPar+cmbBaseNumber,
  cmcSearchOpCor       = cmbSearchOpCor+cmbBaseNumber,
  cmcSearchClCor       = cmbSearchClCor+cmbBaseNumber,
  cmcRedo              = cmbRedo+cmbBaseNumber,
  cmcSetLocalOptions   = cmbSetLocalOptions+cmbBaseNumber,
  cmcExpandAllTabs     = cmbExpandAllTabs+cmbBaseNumber,
  cmcCompactBuffer     = cmbCompactBuffer+cmbBaseNumber,
  cmcRecordMacro       = cmbRecordMacro+cmbBaseNumber,
  cmcStopMacro         = cmbStopMacro+cmbBaseNumber,
  cmcPlayMacro         = cmbPlayMacro+cmbBaseNumber,
  cmcSetGlobalOptions  = cmbSetGlobalOptions+cmbBaseNumber,
  cmcSaveAsConvertEOL  = cmbSaveAsConvertEOL+cmbBaseNumber,
  cmcCopyBlock         = cmbCopyBlock+cmbBaseNumber,
  cmcSelRectStart      = cmbSelRectStart+cmbBaseNumber,
  cmcSelRectEnd        = cmbSelRectEnd+cmbBaseNumber,
  cmcSelRectCopy       = cmbSelRectCopy+cmbBaseNumber,
  cmcSelRectPaste      = cmbSelRectPaste+cmbBaseNumber,
  cmcSelRectDel        = cmbSelRectDel+cmbBaseNumber,
  cmcSelRectCut        = cmbSelRectCut+cmbBaseNumber,
  cmcSelRectMove       = cmbSelRectMove+cmbBaseNumber,
  cmcSelRectHide       = cmbSelRectHide+cmbBaseNumber,
  cmcLastPosCur        = cmbLastPosCur+cmbBaseNumber,
  cmcToggleMoveOnPaste = cmbToggleMoveOnPaste+cmbBaseNumber,
  cmcJumpToFunction    = cmbJumpToFunction+cmbBaseNumber,
  cmcProfileEditor     = cmbProfileEditor+cmbBaseNumber,
  cmcColorsChanged     = cmbColorsChanged+cmbBaseNumber,
  cmcLoadFileUnderCur  = cmbLoadFileUnderCur+cmbBaseNumber,
  cmcSearchComplement  = cmbSearchComplement+cmbBaseNumber,
  cmcCopyClipWin       = cmbCopyClipWin+cmbBaseNumber,
  cmcPasteClipWin      = cmbPasteClipWin+cmbBaseNumber,
  cmcIntelIndentMode   = cmbIntelIndentMode+cmbBaseNumber,
  cmcDelCharClear      = cmbDelCharClear+cmbBaseNumber,
  cmcSave              = cmbSave+cmbBaseNumber,
  cmcSaveAs            = cmbSaveAs+cmbBaseNumber,
  cmcChooseMacro       = cmbChooseMacro+cmbBaseNumber,
  cmcRepeatMacro       = cmbRepeatMacro+cmbBaseNumber,
  cmcGenCodeForMacro   = cmbGenCodeForMacro+cmbBaseNumber,
  cmcSelectOn          = cmbSelectOn+cmbBaseNumber,
  cmcSelectOff         = cmbSelectOff+cmbBaseNumber,
  cmcSaveSameTime      = cmbSaveSameTime+cmbBaseNumber,
  cmcWrapOn            = cmbWrapOn+cmbBaseNumber,
  cmcWrapOff           = cmbWrapOff+cmbBaseNumber,
  cmcWrapToggle        = cmbWrapToggle+cmbBaseNumber,
  // No longer used:
  //cmcUpdateCodePage    = cmbUpdateCodePage+cmbBaseNumber,
  cmcMouseToggle       = cmbMouseToggle+cmbBaseNumber,
  cmcSelRectToUpper    = cmbSelRectToUpper+cmbBaseNumber,
  cmcSelRectToLower    = cmbSelRectToLower+cmbBaseNumber,
  cmcPasteEmacsMode    = cmbPasteEmacsMode+cmbBaseNumber,
  cmcGoEndOfWord       = cmbGoEndOfWord+cmbBaseNumber,
  cmcForceMatchPairHL  = cmbForceMatchPairHL+cmbBaseNumber,
  cmcChoosePMacrosList = cmbChoosePMacrosList+cmbBaseNumber,
  cmcPushCursorPos     = cmbPushCursorPos+cmbBaseNumber,
  cmcPopCursorPos      = cmbPopCursorPos+cmbBaseNumber,
  cmcToggleCharCase    = cmbToggleCharCase+cmbBaseNumber,
  cmcJumpToPrototype   = cmbJumpToPrototype+cmbBaseNumber,
  cmcCutClipWin        = cmbCutClipWin+cmbBaseNumber,
  cmcQuotedPrintDecode = cmbQuotedPrintDecode+cmbBaseNumber,
  cmcArbitraryIndent   = cmbArbitraryIndent+cmbBaseNumber,
  cmcLastColInScreen   = cmbLastColInScreen+cmbBaseNumber,
  cmcLineOrScrEnd      = cmbLineOrScrEnd+cmbBaseNumber,
  cmcWhichFunctionIs   = cmbWhichFunctionIs+cmbBaseNumber,
  cmcCommentIndent     = cmbCommentIndent+cmbBaseNumber,
  cmcCommentUnIndent   = cmbCommentUnIndent+cmbBaseNumber,
  cmcInvertCase        = cmbInvertCase+cmbBaseNumber,
  cmcAltCase           = cmbAltCase+cmbBaseNumber,
  cmcInsertKeyName     = cmbInsertKeyName+cmbBaseNumber,
  cmcRunSel_sLisp      = cmbRunSel_sLisp+cmbBaseNumber,
  cmcRunEnter_sLisp    = cmbRunEnter_sLisp+cmbBaseNumber,
  cmcCopyClipFile      = cmbCopyClipFile+cmbBaseNumber,
  cmcPasteClipFile     = cmbPasteClipFile+cmbBaseNumber,
  cmcEditorGotFocus    = cmbEditorGotFocus+cmbBaseNumber,
  cmcJumpLastCursorPos = cmbJumpLastCursorPos+cmbBaseNumber,
  cmcInsertNewLine     = cmbInsertNewLine+cmbBaseNumber,
  cmcSaveAsNoConvertEOL= cmbSaveAsNoConvertEOL+cmbBaseNumber,
  cmcExpandAllTabsNi   = cmbExpandAllTabsNi+cmbBaseNumber,
  cmcCompactBufferNi   = cmbCompactBufferNi+cmbBaseNumber,
  cmcSearchSelForward  = cmbSearchSelForward+cmbBaseNumber,
  cmcSearchSelBackward = cmbSearchSelBackward+cmbBaseNumber,
  cmcSmartTab          = cmbSmartTab+cmbBaseNumber;

const int cmcFirstMacro=0x2800,
          cmcLastMacro =0x2FFF;

extern int SearchEdCommand(char *name);
extern int SearchEditCommand(char *name);
extern int SearchEditFlag(char *name, unsigned &val);
extern int SearchSimpleCommand(char *name);
extern int SearchHelpCtxCommand(char *name);
extern char *TranslateEdCommand(unsigned cmc_Com);
class  TNoCaseNoOwnerStringCollection;
extern TNoCaseNoOwnerStringCollection *CreateEdCommandCol(void);
#endif
