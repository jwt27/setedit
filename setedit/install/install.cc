/*****************************************************************************

TODO: que parsee el path (strtok) y busque instalaciones anteriores, si
encuentra el install.log que lo levante a memoria y que use esos seteos y
que pregunte si la quiere liquidar => barro la lista, si un archivo no
coincide en tiempo y tama�o renombrarlo a un nombre �nico (.001, .002, etc)
al final avisarle cuantos.
Un switch de uninstall y que pregunte si remueve los que modifico con si, no,
todos y ninguno.

Hints:
From Laurynas:
 2) Selecting type of installation - I'd like (and think other users
 would like) to know, how differ given three types of installation?
If done must be done very careful to avoid confusing people, the differences
are technical.

 4) About autoexec.bat update - IMHO the best way to do it is to 
 give for user limited editing capabilities, something I have saw
 in Norton Utilities 8.0 installer - there is given text of 
 autoexec.bat which user is unable to change, added by installer code
 is highlighted and all you can do is to move it with <up> and <down>.
 (Maybe there was an option to refuse changes, I don't remember).
It could be another option, but is complex because no TV class provides it.

Map of v1.02 (text section):
tvision:   120936
libc:      105900
libstdc++:  56748
this code:  20664
allegro:    19956 (139Kb saved using the modified datfile.c was in 1.0)
Perl regex: 19144
libgcc:     10304
easydiag:    7892
internationalization: 7372
extra widgets (progress bar): 3176

*****************************************************************************/

#include <ceditint.h>

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <dos.h>
#include <locale.h>
#include <allegro.h>
#include <pcre.h>
#include <libwin.h>
#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TKeys
#define Uses_TDialog
#define Uses_MsgBox
#define Uses_TEditWindow

#define Uses_ProgBar

#define Uses_TSInputLine
#define Uses_TSLabel
#define Uses_TSLabelRadio
#define Uses_TSButton
#define Uses_TSHzGroup
#define Uses_TSStaticText
#define Uses_TSNoStaticText
#define Uses_TSCheckBoxes
#define Uses_TSTextScroller

#include <easydia1.h>
#include <settvuti.h>
#include <easydiag.h>

#ifndef SUP_PCRE
#error Can not compile the installer without PCRE support
#endif

//   The following are cosmetic because I ensured that
// no graphic code is linked modifying datafile.c
BEGIN_GFX_DRIVER_LIST
END_GFX_DRIVER_LIST
BEGIN_COLOR_DEPTH_LIST
END_COLOR_DEPTH_LIST
BEGIN_JOYSTICK_DRIVER_LIST
END_JOYSTICK_DRIVER_LIST

//#define DEBUG

// Declared in tvedit3.cc
extern ushort doEditDialogDefault(int dialog, va_list arg);
// Declared in tvedit2.cc
ushort execDialog(TDialog *d, void *data);

#define DoAndDel(a,d) TDialog *d=a->doIt(); delete a; d->options|=ofCentered;
#define CreateCol(a)  TSViewCol *col=new TSViewCol(new TDialog(TRect(1,1,1,1),a))

const int staExit=0x100,staSelType=1,staDestination=2,staMiscOps=3,
          staConfirm=4,staInstall=5;
const int retNext=1,retPrev=-1,retExit=0,retStay=2,retExitUn=3;
const int cmStart=0x2000,cmbPrev=0x2001,cmbNext=0x2002;
const int instNormal=0,instProg=1,instDJGPP=2;

int  TypeInstallation=-1;
char Destination[PATH_MAX]="";
char AddToDesktop=1;
char AddToMenu   =1;
char RedmondMenu =0;
char ExtraScrSave=1;

char *Win95Programs=0;
char *Win95Desktop=0;
char *Win95MenuPrograms=0;
char *OldInstallation=0;

const char *cTypeOfInstall=__("Type of installation");
const char *cDestinationDir=__("Destination directory");
const char *cMiscOps=__("Miscellaneous options");
const char *cAddToDesktop=__("Add a direct access to the desktop");
const char *cAddToMenu=__("Add setedit to the Start\\Programs menu");
const char *cRedmondMenu=__("Configure the editor's menu like in Win. programs (eg. ^C=copy)");
const char *cExtraScrSave=__("Extra screen savers (around 160 Kb)");

// XXX.XXX.XXX
char VersionDots[12];
char Version[12];

int editorIsAlive=0;
int DebugMode=0;
int AutoExecWasOK=0;

typedef struct
{
 unsigned flags;
 char *path;
 char *file;
 char *instName;
 char *manifest;
} FullFile;

const unsigned fgNormal=1,fgProgram=2,fgDJGPP=4,fgPIFBinSet=8,fgExScSv=0x10;
const unsigned fgAll=fgNormal | fgProgram | fgDJGPP;
const unsigned fgPrg=fgProgram | fgDJGPP;
const unsigned fgNoDJ=fgNormal | fgProgram;

FullFile files[]={
{fgAll   ,"bin/","setedit.exe",0,0},
{fgAll   ,"bin/","e.bat",0,0},
{fgAll | fgPIFBinSet,"bin/","setedit.pif",0,0},
{fgNoDJ  ,"bin/","cwsdpmi.exe",0,0},
{fgNoDJ  ,"bin/","cwsdpmi.doc",0,0},
{fgNoDJ  ,"bin/","emu387.dxe",0,0},
{fgAll   ,"share/setedit/","cpmacros.pmc",0,0},
{fgAll   ,"share/setedit/","clippmac.pmc",0,0},
{fgAll   ,"share/setedit/","perlmac.pmc",0,0},
{fgAll   ,"share/setedit/","pmacros.pmc",0,0},
{fgAll   ,"share/setedit/","htmlmac.pmc",0,0},
{fgAll   ,"share/setedit/","syntaxhl.shl",0,0},
{fgAll   ,"share/setedit/","errors.cle",0,0},
{fgPrg   ,"share/setedit/","editor.tip",0,0},
{fgNormal,"share/setedit/","editor.tip","for-prg.tip",0},
{fgNormal,"share/setedit/","simple.tip","editor.tip",0},
{fgAll   ,"share/setedit/","boxround.sft",0,0},
{fgAll   ,"share/setedit/","cntdown.sft",0,0},
{fgAll   ,"share/setedit/","ocr.sft",0,0},
{fgAll   ,"share/setedit/","rombios.sft",0,0},
{fgAll   ,"share/setedit/","thin.sft",0,0},
{fgAll   ,"share/setedit/","txhgen-i.htm",0,0},
{fgAll   ,"share/setedit/","txhgen-i.txi",0,0},
{fgAll   ,"share/setedit/","txhgen-i.txt",0,0},
{fgAll   ,"share/setedit/","html.frt",0,0},
{fgAll   ,"share/setedit/","multi.frt",0,0},
{fgAll   ,"share/setedit/","tex.frt",0,0},
{fgAll   ,"share/setedit/","redmond.smn",0,0},
{fgPrg   ,"share/setedit/","menubind.smn",0,0},
{fgPrg   ,"share/setedit/","simple.smn",0,0},
{fgNormal,"share/setedit/","simple.smn","menubind.smn",0},
{fgNormal,"share/setedit/","menubind.smn","for-prg.smn",0},
{fgAll   ,"share/setedit/","macros.slp",0,0},
{fgAll   ,"share/setedit/","examples.slp",0,0},
{fgPrg   ,"share/setedit/","simple.dst",0,0},
{fgNormal,"bin/","simple.dst","tcedit.dst",0},
{fgAll   ,"info/","setedit.inf",0,0},
{fgAll   ,"info/","sdg.inf",0,0},
{fgAll   ,"info/","infview.inf",0,0},
{fgDJGPP ,"contrib/setedit.bin/","setedit.txt",0,0},
{fgDJGPP ,"contrib/setedit.bin/","sdg.txt",0,0},
{fgDJGPP ,"contrib/setedit.bin/","infview.txt",0,0},
{fgDJGPP ,"contrib/setedit.bin/","readme.1st",0,0},
{fgDJGPP ,"contrib/setedit.bin/","kextend.zip",0,0},
{fgDJGPP ,"contrib/setedit.bin/","example.zip",0,0},
{fgDJGPP ,"contrib/setedit.bin/","calltpc7.zip",0,0},
{fgDJGPP ,"contrib/setedit.bin/","change.log",0,0},
{fgDJGPP ,"contrib/setedit.bin/","copyrigh",0,0},
{fgDJGPP ,"contrib/setedit.bin/","copying.dj",0,0},
{fgDJGPP ,"contrib/setedit.bin/","copying.gpl",0,0},
{fgDJGPP ,"contrib/setedit.bin/","copying.lgp",0,0},
{fgDJGPP ,"contrib/setedit.bin/","copying.rh",0,0},
{fgDJGPP ,"contrib/setedit.bin/","readme.faq",0,0},
{fgNoDJ  ,"texts/","setedit.txt",0,0},
{fgNoDJ  ,"texts/","sdg.txt",0,0},
{fgNoDJ  ,"texts/","infview.txt",0,0},
{fgNoDJ  ,"texts/","readme.1st",0,0},
{fgNoDJ  ,"texts/","kextend.zip",0,0},
{fgNoDJ  ,"texts/","example.zip",0,0},
{fgNoDJ  ,"texts/","calltpc7.zip",0,0},
{fgNoDJ  ,"texts/","change.log",0,0},
{fgNoDJ  ,"texts/","copyrigh",0,0},
{fgNoDJ  ,"texts/","copying.dj",0,0},
{fgNoDJ  ,"texts/","copying.gpl",0,0},
{fgNoDJ  ,"texts/","copying.lgp",0,0},
{fgNoDJ  ,"texts/","copying.rh",0,0},
{fgNoDJ  ,"texts/","readme.faq",0,0},
{fgAll   ,"share/setedit/","version.txt",0,0},
{fgExScSv,"share/setedit/","ps1.exe",0,0},
{fgExScSv,"share/setedit/","ps2.exe",0,0},
{fgExScSv,"share/setedit/","ps3.exe",0,0},
{fgExScSv,"share/setedit/","ps4.exe",0,0},
{fgExScSv,"share/setedit/","extrscsv.txt",0,0},
{fgDJGPP ,"info/","setedit.1",0,0},
{fgAll   ,"share/locale/es/LC_MESSAGES/","essetedit.mo","setedit.mo",0},
{fgAll   ,"share/locale/de/LC_MESSAGES/","desetedit.mo","setedit.mo",0}
//{fgAll   ,"manifest/","edi0426b.ver",0,0},
//{fgAll   ,"manifest/","edi0426b.mft",0,0}
};

const int numFiles=sizeof(files)/sizeof(FullFile);

int CopyFile(const char *dest, void *ori, long size)
{
 char b[PATH_MAX],bu[PATH_MAX+80],*d=b;
 const char *s=dest;
 struct stat st;
 FILE *de;

 do
   {
    for (;*s && *s!='/'; s++,d++) *d=*s;
    if (*s=='/')
      {
       *d=0;
       if (stat(b,&st))
         { // No existe
          if (mkdir(b,S_IWUSR))
            {
             sprintf(bu,"%s %s",_("Can't create the directory:"),b);
             messageBox(bu,mfError | mfOKButton);
             return 0;
            }
         }
       else
         if (!S_ISDIR(st.st_mode))
           {
            sprintf(bu,"%s %s",_("File name in path:"),b);
            messageBox(bu,mfError | mfOKButton);
            return 0;
           }                      
       *d='/';
       d++;
       s++;
      }
   }
 while (*s);
 de=fopen(dest,"wb");
 if (!de)
    return 0;
 fwrite(ori,size,1,de);
 fclose(de);
 return 1;
}

const char *prefix="test/";

class Installer : public TApplication
{
public:
 Installer();
 //static TMenuBar *initMenuBar(TRect r);
 static TStatusLine *initStatusLine(TRect r);
 void handleEvent( TEvent& event );

private:
 void Start();
};

Installer::Installer() :
           TProgInit(&Installer::initStatusLine, &Installer::initMenuBar,
                     &Installer::initDeskTop)
{
}

/*TMenuBar *Installer::initMenuBar(TRect r)
{
 r.b.y = r.a.y + 1;

 TMenuItem& mI =*new TMenuItem( "~T~est", cmTest, kbNoKey, hcNoContext );
 return new TMenuBar( r, new TMenu( mI ) );
}*/

TStatusLine *Installer::initStatusLine(TRect r)
{
 r.a.y=r.b.y-1;

 TStatusLine *sL=new TStatusLine(r,
   *new TStatusDef(0x1000,0x1001) +
    *new TStatusItem(_("~Ctrl-F4~ Finish edition, or click in the button found in the top left corner"),kbCtrlF4,cmClose) +
   *new TStatusDef(0,0xFFFF) +
    *new TStatusItem(0,kbF10,cmMenu) +
    *new TStatusItem(_("~ESC~ Aborts installation"),kbEsc,cmCancel));

 return sL;
}

void Installer::handleEvent(TEvent &event)
{
 TApplication::handleEvent(event);

 if (event.what == evCommand)
   {
    switch (event.message.command)
      {
       case cmStart:
            Start();
            break;
      }
   }
}

/*ushort execDialog( TDialog *d, void *data )
{
 TView *p=TProgram::application->validView( d );
 if (p==0)
    return cmCancel;
 else
  {
   if (data!=0)
      p->setData(data);
   ushort result=TProgram::deskTop->execView(p);
   if (result!=cmCancel && data!=0)
      p->getData( data );
   TObject::destroy( p );
   return result;
  }
}*/


int ConfirmExit()
{
 return messageBox(_("Are you sure you want to abort the installation?"),
                   mfYesButton | mfNoButton | mfWarning)==cmYes;
}

char *cSelectType=__("Select the type");
const char *cNormal=__("Normal");
const char *cForProgrammers=__("For programmers");
const char *cForDJGPP=__("For djgpp programmers");

int SelectTypeInstall(char *djdir)
{
 static int AlreadyAsked=0;

 CreateCol(_(cTypeOfInstall));

 TSLabel *l=TSLabelRadio(cSelectType,cNormal,cForProgrammers,cForDJGPP,0);
 col->insert(xTSCenter,1,l);
 EasyInsertOKCancel(col);
 DoAndDel(col,d);

 int ops=TypeInstallation;
 if (ops==-1)
    ops=djdir ? instDJGPP : instNormal;

 if (execDialog(d,&ops)!=cmOK)
    return retExit;
 TypeInstallation=ops;
 if (TypeInstallation==instDJGPP && !djdir)
   { // Avoid asking more than once
    if (AlreadyAsked)
       return retNext;
    AlreadyAsked=1;
    return messageBox(_("Looks like djgpp isn't installed. DJGPP environment variable isn't defined or is wrong. Exit to fix it first?"),
                      mfError | mfYesButton | mfNoButton)==cmYes ?
                      retExitUn : retNext;
   }
 return retNext;
}

void InsertPrevNextCancel(TSViewCol *col)
{
 TSHzGroup *but12=new TSHzGroup(new TSButton(_("< ~P~rev"),cmYes),
                  new TSHzGroup(new TSButton(_("> ~N~ext"),cmOK,bfDefault),
                                new TSButton(_("Cancel"),cmCancel)));
 col->insert(xTSCenter,yTSDown,but12);
}

void InsertYesNoCancel(TSViewCol *col)
{
 TSHzGroup *but12=new TSHzGroup(new TSButton(_("~Y~es"),cmYes,bfDefault),
                  new TSHzGroup(new TSButton(_("~N~o"),cmNo),
                                new TSButton(_("Cancel"),cmCancel)));
 col->insert(xTSCenter,yTSDown,but12);
}

int Translate(int val)
{
 switch (val)
   {
    case cmOK:
         return retNext;
    case cmYes:
         return retPrev;
   }
 return retExit;
}

int TestCreation()
{
 char eof=26;
 char dest[PATH_MAX];

 strcpy(dest,Destination);
 strcat(dest,"/delete.me");
 int ret=CopyFile(dest,&eof,1);
 if (ret)
    unlink(dest);
 return ret;
}

int AskDestination(char *djdir)
{
 static int AlreadyAsked=0;

 CreateCol(_(cDestinationDir));
 TSLabel *label=new TSLabel(_("Where do you want to install the editor?"),
                            new TSInputLine(PATH_MAX,50));
 col->insert(xTSCenter,2,label);
 InsertPrevNextCancel(col);
 DoAndDel(col,d);

 if (!*Destination)
   {
    if (OldInstallation)
       strcpy(Destination,OldInstallation);
    else if (djdir && TypeInstallation==instDJGPP)
       strcpy(Destination,djdir);
    else
      {
       if (Win95Programs)
         {
          strcpy(Destination,Win95Programs);
          strcat(Destination,"\\setedit");
         }
       else
          strcpy(Destination,"c:/setedit");
      }
   }
 int ret=Translate(execDialog(d,Destination));
 if (ret==retNext)
   {
    fexpand(Destination);
    if (Destination[1]==':' && !driveValid(Destination[0]))
      {
       messageBox(_("Invalid drive"),mfError | mfOKButton);
       ret=retStay;
      }
    else
      if (!TestCreation())
        {
         messageBox(_("Can't create files there"),mfError | mfOKButton);
         ret=retStay;
        }
    if (!AlreadyAsked && OldInstallation &&
        strcasecmp(OldInstallation,Destination)!=0)
      {
       int r=messageBox(_("You already installed (or tried) the editor, but in a different place. Do you want to continue?"),
                        mfWarning | mfYesButton | mfNoButton);
       if (r==cmYes)
          AlreadyAsked=1;
       else
          ret=retStay;
      }
   }
 return ret;
}


int GetRegistryKey(char *name, FILE *f, char *buffer)
{
 int l=strlen(name);
 char *s,*d;

 while (!feof(f))
   {
    fgets(buffer,PATH_MAX+40,f);
    if (strncmp(buffer+1,name,l)==0)
      {
       s=d=buffer;
       s+=l+4;
       for (; *s && *s!='"'; s++)
          {
           if (*s=='\\')
              s++;
           *(d++)=*s;
          }
       *d=0;
       return 1;
      }
   }
 return 0;
}


/**[txh]********************************************************************

  Description:
  For some internationalization problem Wincrap uses one code page for a
file name accessed by DOS and other for itself. So regedit reports the name
in the W code page (ISO 8859 1 in my case), but if I must create a file
I must provide the name in the DOS code page (CP 850 in my case). The result
is that I can't trust in any value outside the 0-127 range, so I use glob
to find the real value. It happends with the spanish word "Men�".

***************************************************************************/

char *SolveFuckName(char *b)
{
 unsigned char *s=(unsigned char *)strdup(b),*ori;
 int changed=0;
 glob_t dirs;
 char *ret=(char *)s;

 for (ori=s; *s; s++)
    if (*s>127)
      {
       *s='?';
       changed++;
      }
 if (changed)
   {
    if (glob((char *)ori,0,0,&dirs)==0)
       ret=strdup(dirs.gl_pathv[0]);
    else
       ret=0;
    globfree(&dirs);
    free(ori);
   }

 return ret;
}

#define DEBUG_WIN
#ifdef DEBUG_WIN
#define dbprint(a...) fprintf(stderr,a)
#else
#define dbprint(a...)
#endif

int GetWindowsInformation()
{
 int ret=0;
 char command[PATH_MAX];

 CreateCol(_("Searching Windows information"));

 TSStaticText *l=new TSStaticText(_("Windows 9x or similar detected, wait while\nfinding information about your system"));
 TSNoStaticText *m=new TSNoStaticText(_("Looking in registry       "));
 m->Flags=wSpan;
 col->insert(2,1,l);
 col->insert(2,yTSUnder,m,0,l);
 DoAndDel(col,d);
 TNoStaticText *message=(TNoStaticText *)m->view;

 TProgram::deskTop->insert(d);
 long hKey,bufType=REG_SZ,bufLen=sizeof(command);

 if (!w95_reg_openkey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",&hKey))
   {
    message->setText(_("Searching programs dir"));
    if (!w95_reg_queryvalueex(hKey,"ProgramFilesDir",0,&bufType,command,&bufLen))
      {
       Win95Programs=SolveFuckName(command);
       w95_reg_closekey(hKey);
       message->setText(_("Looking in registry       "));
       if (!w95_reg_openkey(HKEY_USERS,".Default\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",&hKey))
         {
          message->setText(_("Searching desktop and menu"));
          bufType=REG_SZ; bufLen=sizeof(command);
          if (!w95_reg_queryvalueex(hKey,"Desktop",0,&bufType,command,&bufLen))
            {
             Win95Desktop=SolveFuckName(command);
             bufType=REG_SZ; bufLen=sizeof(command);
             if (!w95_reg_queryvalueex(hKey,"Programs",0,&bufType,command,&bufLen))
               {
                Win95MenuPrograms=SolveFuckName(command);
                ret=1;
               }
            }
         }
       w95_reg_closekey(hKey);
      }
    else
       w95_reg_closekey(hKey);
   }
 TProgram::deskTop->remove(d);

 return ret;
}

void AddItem(TSItem **ops, char *value)
{
 if (*ops==0)
   { // First item
    *ops=new TSItem(value,0);
   }
 else
   {
    TSItem *p=*ops;
    while (p->next)
      {
       p=p->next;
      }
    p->next=new TSItem(value,0);
   }
}

int AskMiscOps()
{
 int options=1,desktop=0,menu=0,menuRedmond=0,opsMask=0,menuESS;
 TSItem *ops=0;

 if (Win95Desktop)
   {
    AddItem(&ops,_(cAddToDesktop));
    desktop=options;
    options<<=1;
    opsMask|=AddToDesktop ? desktop : 0;
   }
 if (Win95MenuPrograms)
   {
    AddItem(&ops,_(cAddToMenu));
    menu=options;
    options<<=1;
    opsMask|=AddToMenu ? menu : 0;
   }
 if (TypeInstallation==instProg || TypeInstallation==instDJGPP)
   {
    AddItem(&ops,_(cRedmondMenu));
    menuRedmond=options;
    options<<=1;
    opsMask|=RedmondMenu ? menuRedmond : 0;
   }
 // Extra screen savers (external ones)
 AddItem(&ops,_(cExtraScrSave));
 menuESS=options;
 options<<=1;
 opsMask|=ExtraScrSave ? menuESS : 0;
 // None applies
 if (options==1)
    return retNext;

 CreateCol(_(cMiscOps));
 TSCheckBoxes *c=new TSCheckBoxes(ops);
 col->insert(xTSCenter,1,c);
 InsertPrevNextCancel(col);
 DoAndDel(col,d);

 int ret=Translate(execDialog(d,&opsMask));
 if (ret==retNext)
   {
    if (Win95Desktop)
       AddToDesktop=opsMask & desktop ? 1 : 0;
    else
       AddToDesktop=0;
    if (Win95MenuPrograms)
       AddToMenu=opsMask & menu ? 1 : 0;
    else
       AddToMenu=0;
    if (TypeInstallation==instProg || TypeInstallation==instDJGPP)
       RedmondMenu=opsMask & menuRedmond ? 1 : 0;
    else
       RedmondMenu=0; // Just in case that's a second pass with different options
    ExtraScrSave=opsMask & menuESS ? 1 : 0;
   }

 return ret;
}

const int LinesInList=12;

int ConfirmValues()
{
 TNSCollection *strs=new TNSCollection(8,4);
 int lines=0;

 strs->insert(_(cTypeOfInstall));
 switch (TypeInstallation)
   {
    case instNormal:
         strs->insert(_("  Normal"));
         break;
    case instProg:
         strs->insert(_("  For programmers"));
         break;
    case instDJGPP:
         strs->insert(_("  For djgpp programmers"));
         break;
   }
 strs->insert("");
 lines+=3;

 strs->insert(_(cDestinationDir));
 char *dp=(char *)alloca(strlen(Destination)+1+2);
 strcpy(dp,"  ");
 strcat(dp,Destination);
 strs->insert(dp);
 strs->insert("");
 lines+=2;

 strs->insert(_(cMiscOps));
 lines++;
 #define I(ops) if(ops) { char *a=_(c##ops); char *s=(char *)alloca(strlen(a)+3); \
                          strcpy(s,"  "); strcat(s,a); strs->insert(s); lines++; }
 I(AddToDesktop)
 I(AddToMenu)
 I(RedmondMenu)
 I(ExtraScrSave)

 CreateCol(_("Confirm options"));
 TSTextScroller *s=new TSTextScroller(70,LinesInList,strs,1,lines>LinesInList);
 TSStaticText *q=new TSStaticText(_("Are these options correct?"));
 col->insert(xTSCenter,1,s);
 col->insert(xTSCenter,yTSUnder,q,0,s);
 InsertYesNoCancel(col);
 DoAndDel(col,d);

 int ret=execDialog(d,0);
 if (ret==cmYes)
    return retNext;
 if (ret==cmNo)
    return retPrev;
 return retExit;
}

void ReplaceBy(char *ori, char *New, char *bkp, char *b1, char *b2)
{
 // Reflect it in the list
 sprintf(b1,"share/setedit/%s",bkp);
 sprintf(b1,"share/setedit/%s",New);
 int i;
 for (i=0; i<numFiles; i++)
    {
     if (strcmp(files[i].manifest,New)==0)
       {
        free(files[i].manifest);
        files[i].manifest=strdup(bkp);
        break;
       }
    }

 sprintf(b1,"%s/share/setedit/%s",Destination,bkp);
 sprintf(b2,"%s/share/setedit/%s",Destination,ori);
 rename(b2,b1);
 sprintf(b1,"%s/share/setedit/%s",Destination,New);
 rename(b1,b2);
}

// Estoy aca, hay que agregar el cwsdpmi a la distribucion y solo instalarlo
// cuando sea necesario.
#ifdef DEBUG
#define DAT_FILENAME "../install/install.dat"
#else
#define DAT_FILENAME "#"
#endif

/**[txh]********************************************************************

  Description:
  It sets the working directory of the pif. If anything goes wrong I give
up inmediatly. As the path is limited to 64 bytes (ASCIIZ) I use the SFN
name instead of the LFN.

***************************************************************************/

void ProcessPifFile(char *code, char *buffer, const char *dest,
                    const char *path)
{
 char curd[PATH_MAX];

 strcpy(buffer,dest);
 strcat(buffer,path);
 if (!getcwd(curd,PATH_MAX))
    return;
 if (chdir(buffer))
    return;
 putenv("LFN=n");
 int ret=!getcwd(buffer,PATH_MAX);
 putenv("LFN=y");
 if (ret || chdir(curd) || strlen(buffer)>=64)
    return;
 strcpy(code+0x65,buffer);
}

int DoInstall()
{
 int i,actions=2;
 unsigned mask=0;
 DATAFILE *p;
 char fullName[PATH_MAX],auxName[PATH_MAX],*mft;

 if (AddToDesktop) actions++;
 if (AddToMenu) actions++;
 if (RedmondMenu) actions++;
 switch (TypeInstallation)
   {
    case instNormal:
         mask=fgNormal;
         break;
    case instProg:
         mask=fgProgram;
         break;
    case instDJGPP:
         mask=fgDJGPP;
         break;
   }
 if (ExtraScrSave) mask|=fgExScSv;
 // Count how many belongs to this distribution
 for (i=0; i<numFiles; i++)
     if (files[i].flags & mask)
        actions++;

 // Put some message to left space for the file names
 ProgBar_Init(_("Installing"),actions,_("Installing files for the editor"));
 for (i=0; i<numFiles; i++)
    {
     if (!(files[i].flags & mask))
        continue;
     ProgBar_UpDate(i);
     ProgBar_SetComments(files[i].file);
     p=load_datafile_object(DAT_FILENAME,files[i].file);
     if (!p)
       {
        messageBox(mfError | mfOKButton,_("Unable to uncompress %s, most probably this file is damaged"),files[i].file);
        return 0;
       }
     strcpy(fullName,Destination);
     strcat(fullName,"/");
     mft=fullName+strlen(fullName);
     strcat(fullName,files[i].path);
     if (files[i].instName)
        strcat(fullName,files[i].instName);
     else
        strcat(fullName,files[i].file);
     CopyFile(fullName,p->dat,p->size);
     files[i].manifest=strdup(mft);
     if (strcmp(files[i].file,"version.txt")==0)
       {
        char *s=(char *)p->dat;
        int i;
        for (i=0; *s!='\r' && *s!='\n'; s++,i++) VersionDots[i]=*s;
        VersionDots[i]=0;
        for (s++; isspace(*s); s++);
        for (i=0; *s!='\r' && *s!='\n'; s++,i++) Version[i]=*s;
        Version[i]=0;
       }
     unload_datafile_object(p);
    }

 // It changes some names must be done before the manifest
 if (RedmondMenu)
   {
    ProgBar_UpDate(i++);
    ProgBar_SetComments(_("Replacing default menu"));
    ReplaceBy("menubind.smn","redmond.smn","default.smn",fullName,auxName);
   }

 // ** Extra files:
 // Version for djgpp, to avoid hardcoding the version in the installer
 sprintf(auxName,"edi%sb.ver",Version);
 ProgBar_UpDate(i++);
 ProgBar_SetComments(auxName);
 sprintf(fullName,"%s/manifest/edi%sb.ver",Destination,Version);
 p=load_datafile_object(DAT_FILENAME,auxName);
 if (!p)
   {
    messageBox(_("Unable to uncompress files, most probably this file is damaged"),mfError | mfOKButton);
    return 0;
   }
 CopyFile(fullName,p->dat,p->size);
 unload_datafile_object(p);
 // Manifest, It is installation dependant
 sprintf(fullName,"%s/manifest/edi%sb.mft",Destination,Version);
 ProgBar_UpDate(i++);
 ProgBar_SetComments("manifest");
 FILE *f=fopen(fullName,"wt");
 if (f)
   {
    for (i=0; i<numFiles; i++)
       {
        if (files[i].manifest)
           fprintf(f,"%s\n",files[i].manifest);
       }
    fclose(f);
   }
 // Create a report of the installed stuff
 sprintf(fullName,"%s/share/setedit/install.log",Destination);
 f=fopen(fullName,"wt");
 if (f)
   {
    struct stat st;
    fprintf(f,"[Install]\nBase=%s\nVersion1=%s\nVersion2=%s\n",Destination,VersionDots,Version);
    for (i=0; i<numFiles; i++)
       {
        if (files[i].manifest)
          {
           sprintf(fullName,"%s/%s",Destination,files[i].manifest);
           if (stat(fullName,&st)==0)
              fprintf(f,"File=%s,%d,%d\n",files[i].manifest,st.st_mtime,st.st_size);
          }
       }
    fprintf(f,"AddToDesktop=%d\n",AddToDesktop);
    fprintf(f,"AddToMenu=%d\n",AddToMenu);
    fprintf(f,"RedmondMenu=%d\n",RedmondMenu);
    fprintf(f,"TypeInstallation=%d\n",TypeInstallation);
    fclose(f);
   }

 // Now do the extra actions:
 if (AddToDesktop)
   {
    ProgBar_UpDate(i++);
    ProgBar_SetComments(_("Adding icon to desktop"));
    p=load_datafile_object(DAT_FILENAME,"setedit.pif");
    if (p)
      {
       ProcessPifFile((char *)p->dat,auxName,Destination,"/bin");
       strcpy(fullName,Win95Desktop);
       strcat(fullName,"/setedit.pif");
       fexpand(fullName);
       CopyFile(fullName,p->dat,p->size);
       unload_datafile_object(p);
      }
   }
 if (AddToMenu)
   {
    ProgBar_UpDate(i++);
    ProgBar_SetComments(_("Adding icon to menu"));
    p=load_datafile_object(DAT_FILENAME,"setedit.pif");
    if (p)
      {
       ProcessPifFile((char *)p->dat,auxName,Destination,"/bin");
       strcpy(fullName,Win95MenuPrograms);
       strcat(fullName,"/SET Edit/setedit.pif");
       fexpand(fullName);
       CopyFile(fullName,p->dat,p->size);
       unload_datafile_object(p);
      }
   }
 #if 0 // Now is a trick in the flags of the array
 if (TypeInstallation==instNormal)
   {
    ProgBar_UpDate(i++);
    ProgBar_SetComments(_("Simplifying configuration"));
    ReplaceBy("menubind.smn","simple.smn","default.smn",fullName,auxName);
    ReplaceBy("editor.tip","simple.tip","default.tip",fullName,auxName);
    ReplaceBy("tcedit.dst","simple.dst","default.dst",fullName,auxName);
   }
 #endif
 ProgBar_DeInit();
 return 1;
}

void SearchOldSET_FILES()
{
 char *set_files=getenv("SET_FILES");
 char b[PATH_MAX];

 if (set_files)
   {
    strcpy(b,set_files);
    strlwr(b);
    char *start=strstr(b,"share/setedit");
    if (!start)
      {
       messageBox(_("You defined SET_FILES environment variable in a way that isn't compatible with the installer, be careful"),mfWarning | mfOKButton);
      }
    else
      {
       strcpy(b,set_files);
       *start=0;
       fexpand(b);
       OldInstallation=strdup(b);
      }
   }
}

pcre *CompiledPCRE;
int PCREMaxMatchs;
int *PCREMatchs;

void CreateRegex(const char *match)
{
 const char *error;
 int   errorOffset;
 CompiledPCRE=pcre_compile(match,PCRE_MULTILINE | PCRE_CASELESS,&error,&errorOffset,0);
 PCREMaxMatchs=(pcre_info(CompiledPCRE,0,0)+1)*3;
 PCREMatchs=new int[PCREMaxMatchs];
}

int DoRegexSearch(char *search, int len)
{
 return pcre_exec(CompiledPCRE,0,search,len,PCRE206 0,PCREMatchs,PCREMaxMatchs);
}

void ReleaseRegex()
{
 delete []PCREMatchs;
 delete CompiledPCRE;
}

char *SearchPATH(char *text, long len)
{
 char *lastMatch=0;
 int hits;
 char *search=text;

 CreateRegex("^ *(SET\\s+)*PATH((\\s*=)|(\\s))");
 do
   {
    hits=DoRegexSearch(search,len);
    if (hits>0)
      {
       lastMatch=text+PCREMatchs[0];
       search+=PCREMatchs[1];
      }
   }
 while (hits>0);
 ReleaseRegex();

 return lastMatch;
}

void RemovePATH(char *buffer, long &len)
{ // Looks complex but is a silly regex
 CreateRegex("^Rem \\*\\* By SET Edit Installer \\*\\*(.*)\n *(SET\\s+)*PATH\\s*=(.*)\n");
 if (DoRegexSearch(buffer,len)>0)
   {
    int start=PCREMatchs[0];
    int end=PCREMatchs[1];
    int l=end-start;
    memmove(buffer+start,buffer+start+l,len-end+1);
    len-=l;
   }
 ReleaseRegex();
}

static
ushort doSpecialEditDialog(int dialog, ...)
{ // Here I change the save stuff
 va_list arg;

 switch (dialog)
   {
    case edSaveModify:
         return messageBox(_("You modified the autoexec.bat, Do you want to save the changes?")
                           ,mfInformation | mfYesNoCancel);
   }

 va_start(arg, dialog);
 ushort ret=doEditDialogDefault(dialog,arg);
 va_end(arg);
 return ret;
}

// Just to create a fake title
class TEditWindow2 : public TEditWindow
{
public:
 TEditWindow2::TEditWindow2( const TRect& bounds,
                             const char *fileName
                           ) :
     TWindowInit( &TEditWindow2::initFrame ),
     TEditWindow(bounds,fileName,1) {};
 const char *getTitle(short);
 void close();
};

const char *TEditWindow2::getTitle( short )
{
 return "c:\\autoexec.bat";
}

void TEditWindow2::close()
{
 TWindow::close();
 editorIsAlive=0;
}

int EditAutoexec(const char *name, int modified, char *buf, unsigned l)
{
 CreateCol(_("Instructions for edition"));
 TSStaticText *t1=new TSStaticText(_("Now I'll give you the oportunity to edit the autoexec.bat. To finish click in the small rectable found in the top-left corner or press Ctrl+F4."),70);
 col->insert(2,2,t1);
 TSStaticText *last;
 if (modified)
   {
    last=new TSStaticText(_("I added the needed command, it is marked with a comment. Please don't remove the comment is used to know where is the text added by the installer."),70);
    col->insert(2,yTSUnder,last,0,t1);
   }
 else
   {
    last=new TSStaticText(_("The text I suggest to add is currently in the clipboard."),70);
    col->insert(2,yTSUnder,last,0,t1);
   }
 t1=new TSStaticText(_("You can move lines using:\nShift + arrows keys: Select text\nCtrl+Insert: Copy to the clipboard\nCtrl+Delete: Delete a block\nShift+Insert: Paste from the clipboard\nShift+Delete: Copy to clipboard and then delete the block\nCtrl+U: Undo (only one action)"),70);
 col->insert(2,yTSUnder,t1,0,last);

 col->insert(xTSCenter,yTSDown,new TSButton(_("~O~k"),cmOK,bfDefault));
 DoAndDel(col,d);
 execDialog(d,0);

 TEditWindow2 *ed=new TEditWindow2(TRect(1,1,76,21),name);
 ed->helpCtx=0x1000;

 // Create a clipboard or the user won't be able to copy/paste
 TView *p=TProgram::application->validView(new TEditWindow(TRect(1,1,76,21),0,2));
 if (p)
   {
    p->hide();
    TProgram::deskTop->insert(p);
    TEditor::clipboard=((TEditWindow *)p)->editor;
    TEditor::clipboard->canUndo=False;
    TEditor::clipboard->insertText(buf,l,True);
   }
 // Function to handle the editor's dialogs
 TEditor::editorDialog=doSpecialEditDialog;
 TProgram::deskTop->insert(ed);

 // Make the application work until the editor is killed
 editorIsAlive=1;
 do
   {
    TEvent e;
    TProgram::application->getEvent(e);
    TProgram::application->handleEvent(e);
   }
 while (editorIsAlive);

 return 0;
}

char *ReadFile(FILE *f, long &l)
{
 fseek(f,0,SEEK_END);
 long len=ftell(f);
 rewind(f);
 char *s=(char *)malloc(len+l+1);
 fread(s,len,1,f);
 s[len]=0;
 fclose(f);
 l=len;
 return s;
}

int AlreadyInPath()
{
 char *path=getenv("PATH");
 if (path)
   {
    strupr(path);
    for (char *s=path; *s; s++) if (*s=='/') *s='\\';
    char *dest=strdup(Destination);
    strupr(dest);
    int ret=strstr(path,dest)!=0;
    free(dest);
    return ret;
   }
 return 0;
}

const char *byHand   =__("Do it yourself editing autoexec.bat later");
const char *byHandNow=__("Do it yourself but now");
const char *doItAndSh=__("Let me do it and show you the result");
const char *doItAndNS=__("Let me do it and don't show you the result");
char *cSelectHow=__("Select how to do it");
const char  pathComm[]="PATH=%PATH%;";
const char  pathBIN[] ="\\bin";
const char *Autoexec   ="c:/autoexec.bat";
const char *AutoexecBKP="c:/autoexec.bak";
const char  RemWarning[]="Rem ** By SET Edit Installer ** don't edit\r\n";
const char *cErrorCreate=__("Error creating autoexec.bat");

int DefinePath()
{
 strcat(Destination,pathBIN);
 for (char *s=Destination; *s; s++) if (*s=='/') *s='\\';
 if (AlreadyInPath())
   {
    AutoExecWasOK=1;
    return 1;
   }

 unsigned options=0;
 int goOn=1,lenPATH;
 char b[strlen(Destination)+sizeof(pathBIN)+sizeof(pathComm)+sizeof(RemWarning)+3];
 do
   {
    CreateCol(_("Configuring the system"));
   
    TSStaticText *x1=new TSStaticText(_("The editor was succesfully installed, now you must add the directory\nwhere the editor was installed to your PATH, like this:"));
    strcpy(b,pathComm);
    strcat(b,Destination);
    TSStaticText *x2=new TSStaticText(b,70);
    strcat(b,"\r\n");
    lenPATH=strlen(b);
    memmove(b+sizeof(RemWarning)-1,b,lenPATH+1);
    memcpy(b,RemWarning,sizeof(RemWarning)-1);
    lenPATH+=sizeof(RemWarning)-1;
   
    TSLabel *la=TSLabelRadio(cSelectHow,byHand,byHandNow,doItAndSh,doItAndNS,0);
    col->insert(2,2,x1);
    col->insert(2,yTSUnder,x2,0,x1);
    col->insert(2,yTSUnder,la,0,x2);
    EasyInsertOKCancel(col);
    DoAndDel(col,d);

    if (execDialog(d,&options)==cmOK)
       goOn=0;
    else
       if (ConfirmExit())
          return 0;
   }
 while (goOn);

 if (options)
   {
    FILE *f=fopen(Autoexec,"rb");
    if (!f)
      {
       messageBox(_("Sorry, I can't find autoexec.bat"),mfError | mfOKButton);
       options=0;
      }
    else
      { // Ok, get the file
       long l=lenPATH;
       char *s=ReadFile(f,l);

       // The user wants to let me try, poor boy ;-)
       if (options>1)
         {
          // Make a backup of the autoexec.bat
          if (!CopyFile(AutoexecBKP,s,l))
            {
             free(s);
             messageBox(_("Can't backup the autoexec.bat, sorry do it by hand"),mfError|mfOKButton);
             return 0;
            }
          // Remove any previous value we put there
          RemovePATH(s,l);
          // Insert the line
          char *line=SearchPATH(s,l);
          int offset;
          if (!line)
            {
             offset=0; // At the beginning if not found
             /* I don't think that's really needed
             memmove(b+sizeof(RemWarning)+4,
                     b+sizeof(RemWarning)-1+sizeof(pathComm)-1,
                     lenPATH-sizeof(pathComm)+1);
             lenPATH-=7;*/
            }
          else
            {
             // Look for EOL
             for (; *line && *line!='\n'; line++);
             offset=line-s;
             if (*line)
                offset++;
            }
          // Insert the line
          memmove(s+offset+lenPATH,s+offset,l+1-offset);
          memcpy(s+offset,b,lenPATH);
          l+=lenPATH;
         }
       // Ok, the user trusted as (why? ;-)
       if (options==3)
         {
          if (!CopyFile(Autoexec,s,l))
            {
             messageBox(_(cErrorCreate),mfError|mfOKButton);
             return 0;
            }
          return 1;
         }
       // He want's to edit
       char *tmp=tmpnam(0);
       if (!CopyFile(tmp,s,l))
         {
          messageBox(_("Can't create temporal copy of autoexec.bat"),mfError|mfOKButton);
          return 0;
         }
       EditAutoexec(tmp,options>1,b,lenPATH);
       free(s);
       f=fopen(tmp,"rb");
       if (f)
         {
          l=0;
          s=ReadFile(f,l);
          unlink(tmp);
          if (CopyFile(Autoexec,s,l))
            {
             messageBox(_("A copy of the original autoexec.bat was stored in autoexec.bak"),mfInformation|mfOKButton);
             return 1;
            }
         }
       unlink(tmp);
       // Something went wrong, try to fix it
       rename(AutoexecBKP,Autoexec);
       messageBox(_(cErrorCreate),mfError|mfOKButton);
      }
   }

 return 0;
}

/**[txh]********************************************************************

  Description:
  Extra test, just in case because Allegro 3.11 detection is weak. Doesn't
file in the NT server I tested, but a user reported strange things with a
Workstation.

***************************************************************************/

static
int IsWindowsNT()
{
 char *s=getenv("OS");
 return s && strcasecmp(s,"Windows_NT")==0;
}

#define MoveNext(prev,next) if (ret==retNext) state=next; \
                            else if (ret==retPrev) state=prev;

void Installer::Start()
{
 char *djdir=getenv("DJDIR");
 int state=staSelType,ret=0;

 SearchOldSET_FILES();

 //TEST
 #if 0
 strcpy(Destination,"g:/dj");
 DefinePath();
 return;
 #endif
 insert(new TStaticText(TRect(0,0,80,1),_("\x3SET Edit installer v1.10, copyright (c) 1999 by Salvador E. Tropea")));

 if (os_type==OSTYPE_WIN95 && !IsWindowsNT())
   {
    if (!GetWindowsInformation())
       messageBox(_("Failed, disabling all or part of the Win9x specific stuff"),mfError | mfOKButton);
   }
 // State machine
 while (state!=staExit && state!=staInstall)
   {
    switch (state)
      {
       case staSelType:
            // Type of installation: djgpp, programmer or simple
            //ret=SelectTypeInstall(0); // Test
            ret=SelectTypeInstall(djdir);
            if (ret==retNext)
               state=staDestination;
            break;
       case staDestination:
            // Ask the destination:
            //ret=AskDestination(0);// Test
            ret=AskDestination(djdir);
            MoveNext(staSelType,staMiscOps);
            break;
       case staMiscOps:
            ret=AskMiscOps();
            MoveNext(staDestination,staConfirm);
            break;
       case staConfirm:
            ret=ConfirmValues();
            MoveNext(staSelType,staInstall);
            break;
      }
    if (ret==retExitUn || (ret==retExit && ConfirmExit()))
       state=staExit;
   }
 if (state==staInstall && DoInstall())
   { // Ok, all is installed
    if (!DefinePath())
      {
       messageBox(_("Don't forget to update your autoexec.bat later!"),mfInformation | mfOKButton);
      }
    messageBox(mfInformation | mfOKButton,_("Editor installed succesfully%s. Run it using e.bat"),
               AutoExecWasOK ? "" : _(", after rebooting your system you'll be able to use the editor"));
   }

 message(TProgram::application,evCommand,cmQuit,0);
}

int main(int argc, char **argv)
{
 allegro_init();

 if (argc==2 && strncmp(argv[1],"-d",2)==0)
   {
    DebugMode=atoi(argv[1]+2);
   }

 putenv("LFN=y");

 //------ International support
 #ifndef NO_INTL_SUP
 char localedir[PATH_MAX];
 setlocale(LC_ALL, "");
 strcpy(localedir,".");
 BINDTEXTDOMAIN("install",localedir);
 TEXTDOMAIN("install");
 #endif
 //------ end of int. support

 TEvent init;
 init.what=evCommand;
 init.message.command=cmStart;

 Installer installer;
 installer.putEvent(init);
 installer.run();

 return 0;
}
