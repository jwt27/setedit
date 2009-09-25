/* Copyright (C) 1996-2009 by Salvador E. Tropea (SET),
   see copyrigh file for details */
char *strncpyZ(char *dest, const char *orig, int size)
{
 if (size)
    size--;
 for (;orig && *orig && size; size--,dest++,orig++)
     *dest=*orig;
 *dest=0;
 return dest;
}

