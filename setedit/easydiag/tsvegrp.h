/* Copyright (C) 1996,1997,1998,1999,2000 by Salvador E. Tropea (SET),
   see copyrigh file for details */
#if defined(Uses_TSVeGroup) && !defined(__TSVeGroup_Defined__)
#define __TSVeGroup_Defined__

class TSVeGroup : public TSView
{
public:
 TSVeGroup(TSView *este, TSView *ant,int sep=1);
 virtual ~TSVeGroup();
 virtual void insert(TDialog *);
 virtual void setWidth(int aW);
 void makeSameW() { setWidth(w); };
 int sepa;
 TSView *Este,*Ant;
};

TSVeGroup *MakeVeGroup(TSView *este, TSView *ant, ...);
TSVeGroup *MakeVeGroup(int sepa, TSView *este, TSView *ant, ...);
#endif
