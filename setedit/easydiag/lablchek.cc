/* Copyright (C) 1996,1997,1998,1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#define Uses_TSLabelCheck
#define Uses_TSCheckBoxes
#include <easydia1.h>
#include <tv.h>
#include <easydiag.h>
#include <stdarg.h>

TSLabel *TSLabelCheck(const char *name, ...)
{
 const char *s;
 va_list arg;
 va_start(arg,name);

 TSItem *first=0,*last=0,*aux;
 while ((s=va_arg(arg,const char *))!=0)
   {
    aux=new TSItem(_(s),0);
    if (!first)
       first=aux;
    if (last)
       last->next=aux;
    last=aux;
   }
 va_end(arg);
 return new TSLabel(_(name),new TSCheckBoxes(first));
}

TSLabel *TSLabelCheck(int columns, const char *name, ...)
{
 const char *s;
 va_list arg;
 va_start(arg,name);

 TSItem *first=0,*last=0,*aux;
 while ((s=va_arg(arg,const char *))!=0)
   {
    aux=new TSItem(_(s),0);
    if (!first)
       first=aux;
    if (last)
       last->next=aux;
    last=aux;
   }
 va_end(arg);
 return new TSLabel(_(name),new TSCheckBoxes(first,-1,columns));
}
