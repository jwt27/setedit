/*----------------------------------------------------------*/
/*                                                          */
/*   Turbo Vision 1.0                                       */
/*   Copyright (c) 1991 by Borland International            */
/*                                                          */
/*   Turbo Vision TVEDIT header file                        */
/*----------------------------------------------------------*/

class TMenuBar;
class TStatusLine;
class TEditWindow;
class TDialog;

const int
  //cmCalculator = 104,
  //cmDosShell   = 105,
  cmInfView    = 106,
  cmGrep       = 107,
  cmSyntax     = 108,
  cmSyntaxOps  = 109,
  cmSyntaxList = 110,
  cmAbout      = 111;

class fpstream;

class TEditorMiApp : public TApplication
{

public:

    TEditorMiApp();

    virtual void handleEvent( TEvent& event );
    static TMenuBar *initMenuBar( TRect );
    static TStatusLine *initStatusLine( TRect );
    virtual void outOfMemory();
    virtual TPalette& getPalette() const;
    void saveDesktop(const char *fName);
    void storeDesktop(fpstream &s);
    void retrieveDesktop(const char *name);
    void loadDesktop(fpstream &s);
    
private:

    void dosShell();
    void tile();
    void cascade();
    void grepWindow();
};

extern TEditWindow *clipWindow;
extern void SyntaxSearch(void);

ushort execDialog( TDialog *d, void *data );
TDialog *createFindDialog();
TDialog *createReplaceDialog();
ushort doEditDialog( int dialog, ... );

