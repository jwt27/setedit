/* Copyright (C) 2001 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#ifndef LOADSHL_H_INCLUDED
#define LOADSHL_H_INCLUDED

typedef struct
{
 int PCREMaxMatchs;
 int *PCREMatchs;
} PCREData;

void  PCREInitCompiler(PCREData &p);
void  PCREStopCompiler(PCREData &p);
pcre *PCRECompileRegEx(char *text, PCREData &p);
int   PCREDoSearch(char *search, int len, pcre *CompiledPCRE, PCREData &p);
#define PCREDataDestroy(p) PCREInitCompiler(p)

struct strSHL;
class  TStringCollection;
extern int   LoadSyntaxHighLightFile(char *name, strSHL *&hl, TStringCollection *list,
                                     int &cant);
extern int   LoadSyntaxHighLightKeywords(strSHL &hl);
extern void  UnLoadSyntaxHighLightFile(strSHL *&hl, TStringCollection *list,int &Cant);
extern char *SHLNameOf(unsigned number);
extern int   SHLNumberOf(char *name);
class TCEditor;
extern char *SHLConstructEmacsModeComment(TCEditor &e);
extern int   SHLSelect(TCEditor &e, char *buffer, int lenBuf);
extern void  SHLTransferDefaultsNewFile(TCEditor &e);

#endif // LOADSHL_H_INCLUDED
