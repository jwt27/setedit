/* Copyright (C) 1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#define Uses_stdio
#define Uses_ctype
#define Uses_stdlib
#define Uses_unistd
#define Uses_string
#define Uses_sys_stat
#define Uses_AllocLocal
#define Uses_TScroller
#define Uses_TNSCollection
#define Uses_TWindow
#define Uses_TRect
#define Uses_TPalette
#define Uses_TEvent
#define Uses_TFrame

#include <tv.h>

#define Uses_TEnhancedText
#define Uses_TManPageView
#define Uses_TManWindow

#include <manview.h>

#include <dyncat.h>
#include <diaghelp.h>
#include <rhutils.h>

static const char *ExtraOps;

// Maximun width of a line in a man page, 256 should be more than enough
int TEnhancedText::maxWidth=255;

static char *CreateTempManPage(char *str);
static char *CreateCommandLine(const char *file, const char *sections);

TEnhancedText::TEnhancedText(const char *aFileName, const char *aCommandLine) :
  TNSCollection(25,25)
{
 fileName=newStr(aFileName);
 commandLine=newStr(aCommandLine);
 isOK=1;
 rows=cols=0;

 FILE *f=fopen(aFileName,"rt");
 if (!f)
    isOK=0;
 else
   {
    char *s;
    int count;
    AllocLocalStr(buf,maxWidth+1);
    AllocLocalStr(line,maxWidth*2);

    while (fgets(buf,maxWidth,f))
      {// Parse it and create a character/attribute string
       int x=0;
       for (count=0, s=buf; *s!='\n' && *s!=0; s++)
          {
           if (s[0]==8)
             {
              if (count && s[1]) // Be sure we won't use -1, you never know
                {
                 if (s[-1]=='_')
                   {
                    line[(count-1)*2+1]=2;
                    line[(count-1)*2]=s[1];
                   }
                 else
                    // Is that true?
                    line[(count-1)*2+1]=1;
                 s++;
                }
             }
           else
             {
              if (*s!='\t')
                {
                 line[count*2]=*s;
                 line[count*2+1]=0;
                 count++;
                 x++;
                }
              else
                {
                 int size=8-x%8;
                 x+=size;
                 while (size--)
                   {
                    line[count*2]=' ';
                    line[count*2+1]=0;
                    count++;
                   }
                }
             }
          }
       LineOfEText *l=(LineOfEText *)new char[sizeof(LineOfEText)+count*2];
       l->len=count;
       memcpy(l->text,line,count*2);
       insert(l);
       rows++;
       if (count>cols)
          cols=count;
      }
    fclose(f);
   }
}

void TEnhancedText::copyLine(int y, int w, ushort *line, char *colors)
{
 ushort space=' ' |  (colors[0]<<8);
 int i;
 if (y>=getCount())
    for (i=0; i<w; i++) line[i]=space;
 else
   {
    LineOfEText *l=(LineOfEText *)at(y);
    if (l->len)
      {
       int len=min(l->len,w);
       char *d=(char *)line;
       char *s=(char *)l->text;
       for (i=0; i<w; i++)
          {
           d[i*2]=s[i*2];
           d[i*2+1]=colors[s[i*2+1]];
          }
       w-=len;
       line+=len;
      }
    if (w)
       for (i=0; i<w; i++) line[i]=space;
   }
}

TEnhancedText::~TEnhancedText()
{
 delete[] fileName;
 delete[] commandLine;
}

TManPageView::TManPageView(const TRect& bounds, TScrollBar *aHScrollBar,
                           TScrollBar *aVScrollBar)
  : TScroller(bounds,aHScrollBar,aVScrollBar)
{
 text=0;
 options |= ofSelectable;
 growMode = gfGrowHiX | gfGrowHiY;
}

#define cManPage "\x06\x08\x09"

TPalette& TManPageView::getPalette() const
{
 static TPalette palette(cManPage,sizeof(cManPage)-1);
 return palette;
}

void TManPageView::handleEvent( TEvent& event )
{
 TScroller::handleEvent(event);
 if (event.what==evMouseDown && event.mouse.doubleClick && text)
   {
    TPoint mouse=makeLocal(event.mouse.where);
    mouse.x+=delta.x;
    mouse.y+=delta.y;
    if (mouse.y<text->getCount())
      {
       LineOfEText *line=(LineOfEText *)text->at(mouse.y);
       int len=line->len,start=mouse.x;
       if (start<len)
         {
          uchar *s=(uchar *)&line->text;
          int end;
          // Ok, we have a pointer to the character under the cursor
          for (end=start; end<len && !ucisspace(s[end*2]) && s[end*2]!=')'; end++);
          if (s[end*2]==')')
            { // Only if we have something like ....)
             for (;start && !ucisspace(s[start*2]); start--);
             if (ucisspace(s[start*2])) start++;
             // OK, that's "something)", now copy it to a buffer
             char *aux;
             AllocLocalStr(name,end-start+2);
             for (aux=name; start<=end; start++,aux++) *aux=s[start*2];
             *aux=0;
             // Check we have only one () pair
             aux=strchr(name,'(');
             if (aux)
               { // We have "something(...)"
                char *section=aux+1;
                *aux=0;
                for (aux++; *aux && *aux!='(' && *aux!=')'; aux++);
                if (*aux==')' && !aux[1])
                  {
                   *aux=0;
                   // Try it.
                   char *cmd=CreateCommandLine(name,section);
                   char *tmp=CreateTempManPage(cmd);
                  
                   InsertText(new TEnhancedText(tmp,cmd));
                   message(owner,evBroadcast,cmMPUpdateTitle,name);
                   scrollTo(0,0);
                   unlink(tmp);
                   free(cmd);
                   free(tmp);
                   drawView();
                   //printf("Man page: %s secci�n: %s\n",name,section);
                  }
               }
            }
         }
      }
   }
}

void TManPageView::InsertText(TEnhancedText *aText)
{
 if (!aText || !aText->isOK)
    return;
 if (text)
    destroy(text);
 text=aText;
 setLimit(aText->cols,aText->rows);
 //drawView();
}

void TManPageView::draw()
{
 char colors[3];
 colors[0]=getColor(1);
 colors[1]=getColor(2);
 colors[2]=getColor(3);
 
 int w=size.x+delta.x,y;
 AllocLocalUShort(line,w+1);
 if (!text || !text->isOK)
   { // Draw an empty window if something went wrong
    ushort space=' ' |  (colors[0]<<8);
    y=w;
    while (y--)
       line[y]=space;
    for (y=0; y<size.y; y++)
        writeLine(0,y,size.x,1,line);
    return;
   }

 for (y=0; y<size.y; y++)
    {
     text->copyLine(y+delta.y,w,line,colors);
     writeLine(0,y,size.x,1,line+delta.x);
    }
}

TManPageView::~TManPageView()
{
 destroy(text);
}

void TManPageView::write(opstream& os)
{
 TScroller::write(os);
 os.writeString(text->commandLine);
}

void *TManPageView::read(ipstream& is)
{
 TScroller::read(is);
 char *aCommandLine=is.readString();

 char *tmp=CreateTempManPage(aCommandLine);
 InsertText(new TEnhancedText(tmp,aCommandLine));
 unlink(tmp);
 free(tmp);
 delete[] aCommandLine;
 return this;
}

// New color: Like a cyan window plus 2 colors for bold and underline
#define cManWindow "\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93"

TPalette& TManWindow::getPalette() const
{
 static TPalette palette(cManWindow,sizeof(cManWindow)-1);
 return palette;
}


static TRect defaultSizeWindow;
static
TRect &getDefaultSizeWindow()
{
 defaultSizeWindow.a.x=0;
 defaultSizeWindow.a.y=0;
 defaultSizeWindow.b.x=GetDeskTopCols()-4;
 defaultSizeWindow.b.y=GetDeskTopRows()-4;
 return defaultSizeWindow;
}

TManWindow::TManWindow(const char *fileName, const char *name,
                       char *aCommandLine) :
       TWindow(getDefaultSizeWindow(),name,wnNoNumber),
       TWindowInit(&TManWindow::initFrame)
{
 TRect r=getDefaultSizeWindow();
 options|=ofCentered;
 r.grow(-1,-1);

 page=new TManPageView(r,standardScrollBar(sbHorizontal | sbHandleKeyboard),
                       standardScrollBar(sbVertical | sbHandleKeyboard));
 page->InsertText(new TEnhancedText(fileName,aCommandLine));
 unlink(fileName);
 insert(page);
 helpCtx=hcManPage;
}

TManWindow::TManWindow(StreamableInit) :
    TWindow(streamableInit),
    TWindowInit(NULL)
{
 page=0;
}

void TManWindow::handleEvent( TEvent& event )
{
 TWindow::handleEvent(event);
 if (event.what==evBroadcast && event.message.command==cmMPUpdateTitle)
   {
    DeleteArray((char *)title);
    title=newStr((char *)event.message.infoPtr);
    frame->draw();
   }
}

// These members are needed to initialize page propperly
// Note that page is saved *once*, not twice (from group and explicit)
#if 1
void TManWindow::write(opstream &os)
{
 TWindow::write(os);
 os << page;
}

void *TManWindow::read(ipstream &is)
{
 TWindow::read(is);
 is >> page;
 return this;
}
#endif


static
int isEmpty(const char *str)
{
 if (!str) return 1;
 for (; *str; str++)
    {
     if (!isspace((unsigned char)*str));
        return 0;
    }
 return 1;
}

static
char *CreateTempManPage(char *str)
{
 DynStrCatStruct st;

 char *tmp=unique_name("man",0);
 char *tmpErr=open_stderr();
 DynStrCatInit(&st,str);
 DynStrCat(&st," > ");
 DynStrCat(&st,tmp);
 TV_System(st.str);
 free(st.str);
 close_stderr();

 struct stat s;
 if (stat(tmp,&s)!=0 || s.st_size==0)
   {// No stdout info
    unlink(tmp);
    free(tmp);
    return strdup(tmpErr);
   }
 unlink(tmpErr);
 //free(tmpErr); Nope, io.cc makes it
 return tmp;
}

static
char *CreateCommandLine(const char *file, const char *sections)
{
 DynStrCatStruct st;

 DynStrCatInit(&st,"man");
 if (ExtraOps && !isEmpty(ExtraOps))
   {
    DynStrCat(&st," ");
    DynStrCat(&st,(char *)ExtraOps);
   }
 if (sections && !isEmpty(sections))
   {
    #ifdef __DJGPP__
    DynStrCat(&st," -s ");
    #else
    DynStrCat(&st," -S ");
    #endif
    DynStrCat(&st,(char *)sections);
   }
 DynStrCat(&st," ");
 DynStrCat(&st,(char *)file);

 return st.str;
}

TManWindow *CreateManWindow(const char *file, const char *sections,
                            const char *extraOps)
{
 ExtraOps=extraOps;
 char *cmd=CreateCommandLine(file,sections);
 char *tmp=CreateTempManPage(cmd);

 TManWindow *ret=new TManWindow(tmp,file,cmd);
 free(cmd);
 free(tmp);

 return ret;
}
