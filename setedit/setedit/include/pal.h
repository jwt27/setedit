/* Copyright (C) 1996-2001 by Salvador E. Tropea (SET),
   see copyrigh file for details */
/* That's the palette of the editor constructed with the Robert's macros */

#define Black 0
#define Blue 1
#define Green 2
#define Cyan 3
#define Red 4
#define Magenta 5
#define Brown 6
#define Lightgray 7
#define Darkgray 8
#define Lightblue 9
#define Lightgreen A
#define Lightcyan B
#define Lightred C
#define Lightmagenta D
#define Yellow E
#define White F

//#define __S(color) #color
//#define _S(bg,fg) __S(\x##bg##fg)
#define _S(bg,fg) 0x##bg##fg ,
#define S(index,foreground,background,name) _S(background,foreground)
#define SS(index,foreground,background,name,group) _S(background,foreground)

/* This is the palette for the system when in color mode */
#define SE_cpColor\
        SS( 1,Blue,Lightgray,Color,Desktop) \
        \
        SS( 2,Black,Lightgray,Normal,Menus) \
        S( 3,Darkgray,Lightgray,Disabled) \
        S( 4,Red,Lightgray,Shortcut) \
        S( 5,Black,Green,Selected) \
        S( 6,Darkgray,Green,Selected disabled) \
        S( 7,Red,Green,Shortcut selected) \
        \
        SS( 8,Lightgray,Blue,Frame disabled,Blue Windows) \
        S( 9,White,Blue,Frame) \
        S( A,Lightgreen,Blue,Frame icons) \
        S( B,Blue,Cyan,Scroll bar page) \
        S( C,Blue,Cyan,Scroll bar icons) \
        S( D,Yellow,Blue,Static text) \
        S( E,Blue,Lightgray,Selected text) \
        \
        S( F,Black,Black,reserved) \
        \
        SS(10,Lightgray,Cyan,Frame disabled,Cyan Windows) \
        S(11,White,Cyan,Frame) \
        S(12,Lightgreen,Cyan,Frame icons) \
        S(13,Cyan,Blue,Scroll bar page) \
        S(14,Cyan,Blue,Scroll bar icons) \
        S(15,Yellow,Cyan,Static text) \
        S(16,Blue,Green,Selected text) \
        \
        S(17,Black,Black,reserved) \
        \
        SS(18,Lightgray,Black,Frame disabled,Gray Windows ) \
        S(19,White,Black,Frame) \
        S(1A,White,Black,Frame icons) \
        S(1B,Lightgray,Black,Scroll bar page) \
        S(1C,White,Black,Scroll bar icons) \
        S(1D,Lightgray,Black,Static text) \
        S(1E,Black,Lightgray,Selected text) \
        \
        S(1F,Black,Black,reserved) \
        \
        SS(20,Black,Lightgray,Frame disabled,Dialogs) \
        S(21,White,Lightgray,Frame) \
        S(22,Lightgreen,Lightgray,Frame icons) \
        S(23,Cyan,Blue,Scroll bar page) \
        S(24,Cyan,Blue,Scroll bar icons) \
        S(25,Black,Lightgray,Static text) \
        S(26,Black,Lightgray,Label normal) \
        S(27,White,Lightgray,Label selected) \
        S(28,Yellow,Lightgray,Label shortcut) \
        S(29,Black,Green,Button normal) \
        S(2A,Lightcyan,Green,Button default) \
        S(2B,White,Green,Button selected) \
        S(2C,Darkgray,Lightgray,Button disabled) \
        S(2D,Yellow,Green,Button shortcut) \
        S(2E,Black,Lightgray,Button shadow) \
        S(2F,Black,Cyan,Cluster normal) \
        S(30,White,Cyan,Cluster selected) \
        S(31,Yellow,Cyan,Cluster shortcut) \
        S(32,White,Blue,Input normal) \
        S(33,White,Green,Input selected) \
        S(34,Lightgreen,Blue,Input arrow) \
        S(35,Black,Green,History button) \
        S(36,Green,Lightgray,History sides) \
        S(37,Blue,Cyan,History bar page) \
        S(38,Blue,Cyan,History bar icons) \
        S(39,Black,Cyan,List normal) \
        S(3A,White,Green,List focused) \
        S(3B,Yellow,Cyan,List selected) \
        S(3C,Blue,Cyan,List divider) \
        S(3D,Cyan,Blue,Information pane) \
        S(3E,Darkgray,Cyan,Cluster disabled) \
        \
        S(3F,Black,Black,reserved) \
        \
        SS(40,Yellow,Blue,normal text,Blue Editor) \
        S(41,Blue,Lightgray,marked text) \
        S(42,Lightgray,Blue,comment) \
        S(43,Lightblue,Blue,reserved word) \
        S(44,Yellow,Blue,identifier) \
        S(45,White,Blue,symbol) \
        S(46,Lightcyan,Blue,string) \
        S(47,Magenta,Blue,integer) \
        S(48,Brown,Blue,float) \
        S(49,White,Cyan,octal) \
        S(4A,Lightcyan,Green,hex) \
        S(4B,Lightmagenta,Blue,character) \
        S(4C,Lightgreen,Blue,preprocessor) \
        S(4D,Black,Red,illegal char) \
        S(4E,Green,Blue,user defined words) \
        S(4F,Black,Cyan,reserved) /* CPU line */\
        S(50,White,Red,reserved) /* Breakpoint */\
        S(51,Lightred,Blue,symbol2) \
        \
        SS(52,Yellow,Blue,normal text,Cyan Editor) \
        S(53,Blue,Lightgray,marked text) \
        S(54,Lightgray,Blue,comment) \
        S(55,Lightblue,Blue,reserved word) \
        S(56,Yellow,Blue,identifier) \
        S(57,White,Blue,symbol) \
        S(58,Lightcyan,Blue,string) \
        S(59,Magenta,Blue,integer) \
        S(5A,Brown,Blue,float) \
        S(5B,White,Cyan,octal) \
        S(5C,Lightcyan,Green,hex) \
        S(5D,Lightmagenta,Blue,character) \
        S(5E,Lightgreen,Blue,preprocessor) \
        S(5F,Black,Red,illegal char) \
        S(60,Green,Blue,user defined words) \
        S(61,Black,Cyan,reserved) /* CPU line */\
        S(62,White,Red,reserved) /* Breakpoint */\
        S(63,Lightred,Blue,symbol2) \
        \
        SS(64,Yellow,Blue,normal text,Gray Editor) \
        S(65,Blue,Lightgray,marked text) \
        S(66,Lightgray,Blue,comment) \
        S(67,Lightblue,Blue,reserved word) \
        S(68,Yellow,Blue,identifier) \
        S(69,White,Blue,symbol) \
        S(6A,Lightcyan,Blue,string) \
        S(6B,Magenta,Blue,integer) \
        S(6C,Brown,Blue,float) \
        S(6D,White,Cyan,octal) \
        S(6E,Lightcyan,Green,hex) \
        S(6F,Lightmagenta,Blue,character) \
        S(70,Lightgreen,Blue,preprocessor) \
        S(71,Black,Red,illegal char) \
        S(72,Green,Blue,user defined words) \
        S(73,Black,Cyan,reserved) /* CPU line */\
        S(74,White,Red,reserved) /* Breakpoint */\
        S(75,Lightred,Blue,symbol2) \
        \
        SS(76,Lightgray,Black,Cross cursor,Specials for editors) \
        S(77,Black,Lightgray,editor statusline) \
        S(78,Lightred,Lightgray,parens matching) \
        S(79,White,Magenta,rectangle block) \
        S(7A,Black,Green,odd tab) \
        S(7B,Black,Red,even tab) \
        S(7C,Black,Red,column markers) \
        \
        S(7D,Black,Black,reserved) \
        S(7E,Black,Black,reserved) \
        S(7F,Black,Black,reserved) \
        \
        SS(80,Lightgray,Cyan,Frame disabled,Info viewer) \
        S(81,White,Cyan,Frame) \
        S(82,Lightgreen,Cyan,Frame icons) \
        S(83,Cyan,Blue,Scroll bar page) \
        S(84,Cyan,Blue,Scroll bar icons) \
        S(85,Black,Cyan,Normal text) \
        S(86,Yellow,Cyan,Keyword) \
        S(87,Yellow,Blue,Selected keyword) \
        S(88,Yellow,Red,Marked keyword) \
        S(89,Yellow,Green,Marked text) /* currently fixed to "\0x70"  */ \
        \
        SS(8A,Lightgray,Cyan,Frame disabled,Man page viewer) \
        S(8B,White,Cyan,Frame) \
        S(8C,Lightgreen,Cyan,Frame icons) \
        S(8D,Cyan,Blue,Scroll bar page) \
        S(8E,Cyan,Blue,Scroll bar icons) \
        S(8F,Yellow,Cyan,Static text) \
        S(90,Blue,Green,Selected text) \
        S(91,Black,Cyan,Bold text) \
        S(92,White,Cyan,Underlined text) \
        S(93,Black,Black,reserved)


#define Normal 07
#define Light 0F
#define Inverse 70
#define Underline 01
#define Empty 00

//#define __M(color) #color
//#define _M(col) __M(\x##col)
#define _M(color) 0x##color ,
#define M(index,color,name) _M(color)

/* This is the palette for the system when in monochrome mode */
#define SE_cpMonochrome\
        M( 1,Normal,Color) /* Desktop */\
        \
        M( 2,Normal,normal text) /* Used by menus and statusline */\
        M( 3,Normal,disabled text) \
        M( 4,Underline,shortcut) \
        M( 5,Inverse,normal selected) \
        M( 6,Underline,disabled selected) \
        M( 7,Inverse,shortcut selected) \
        \
        M( 8,Normal,passive frame) /* Used by blue windows */\
        M( 9,Light,active frame) \
        M( A,Normal,frame icons) \
        M( B,Normal,scrollbar) \
        M( C,Normal,scrollbar icons) \
        M( D,Normal,normal text) \
        M( E,Inverse,selected text) \
        \
        M( F,Empty,reserved) \
        \
        M(10,Normal,passive frame) /* Used by cyan windows */\
        M(11,Light,active frame) \
        M(12,Normal,frame icons) \
        M(13,Normal,scrollbar) \
        M(14,Normal,scrollbar icons) \
        M(15,Normal,normal text) \
        M(16,Inverse,selected text) \
        \
        M(17,Empty,reserved) \
        \
        M(18,Normal,passive frame) /* Used by gray windows */\
        M(19,Light,active frame) \
        M(1A,Normal,frame icons) \
        M(1B,Normal,scrollbar) \
        M(1C,Normal,scrollbar icons) \
        M(1D,Normal,normal text) \
        M(1E,Inverse,selected text) \
        \
        M(1F,Empty,reserved) \
        \
        M(20,Normal,passive frame) /* Used by dialogs */\
        M(21,Light,active frame) \
        M(22,Normal,frame icons) \
        M(23,Normal,scrollbar) \
        M(24,Normal,scrollbar icons) \
        M(25,Normal,static text) \
        M(26,Normal,label normal) \
        M(27,Inverse,label selected) \
        M(28,Underline,label shortcut) \
        M(29,Inverse,button normal) \
        M(2A,Underline,button default) \
        M(2B,Inverse,button focused) \
        M(2C,Normal,button disabled) \
        M(2D,Normal,button shortcut) \
        M(2E,Normal,button shadow) \
        M(2F,Inverse,cluster normal) \
        M(30,Normal,cluster selected) \
        M(31,Light,cluster shortcut) \
        M(32,Underline,inputline normal) \
        M(33,Inverse,inputline selected) \
        M(34,Normal,inputline arrows) \
        M(35,Normal,history arrow) \
        M(36,Normal,history side) \
        M(37,Normal,history window scrollbar) \
        M(38,Normal,history window scrollbar icons) \
        M(39,Inverse,list normal) \
        M(3A,Normal,list focused) \
        M(3B,Underline,list selected) \
        M(3C,Normal,list divider) \
        M(3D,Normal,infopane) \
        \
        M(3E,Empty,reserved) \
        M(3F,Empty,reserved) \
        \
        M(40,Normal,normal text) /* Used by blue editors */\
        M(41,Inverse,marked text) \
        M(42,Light,comment) \
        M(43,Light,reserved word) \
        M(44,Normal,identifier) \
        M(45,Light,symbol) \
        M(46,Light,string) \
        M(47,Light,integer decimal) \
        M(48,Light,float) \
        M(49,Light,octal) \
        M(4A,Light,hex) \
        M(4B,Light,character) \
        M(4C,Light,preprocessor) \
        M(4D,Inverse,illegal character) \
        M(4E,Light,user reserved word) \
        M(4F,Underline,CPU line) \
        M(50,Inverse,breakpoint) \
        M(51,Light,symbol2) \
        \
        M(52,Normal,normal text) /* Used by cyan editors */\
        M(53,Inverse,marked text) \
        M(54,Light,comment) \
        M(55,Light,reserved word) \
        M(56,Normal,identifier) \
        M(57,Light,symbol) \
        M(58,Light,string) \
        M(59,Light,integer decimal) \
        M(5A,Light,float) \
        M(5B,Light,octal) \
        M(5C,Light,hex) \
        M(5D,Light,character) \
        M(5E,Light,preprocessor) \
        M(5F,Inverse,illegal character) \
        M(60,Light,user reserved word) \
        M(61,Underline,CPU line) \
        M(62,Inverse,breakpoint) \
        M(63,Light,symbol2) \
        \
        M(64,Normal,normal text) /* Used by gray editors */\
        M(65,Inverse,marked text) \
        M(66,Light,comment) \
        M(67,Light,reserved word) \
        M(68,Normal,identifier) \
        M(69,Light,symbol) \
        M(6A,Light,string) \
        M(6B,Light,integer decimal) \
        M(6C,Light,float) \
        M(6D,Light,octal) \
        M(6E,Light,hex) \
        M(6F,Light,character) \
        M(70,Light,preprocessor) \
        M(71,Inverse,illegal character) \
        M(72,Light,user reserved word) \
        M(73,Underline,CPU line) \
        M(74,Inverse,breakpoint) \
        M(75,Light,symbol2) \
        \
        M(76,Inverse,cross cursor) \
        M(77,Inverse,editor statusline) \
        M(78,Inverse,matching paranthesis) \
        M(79,Inverse,rectangular blocks) \
        M(7A,Inverse,odd tab) \
        M(7B,Underline,even tab) \
        M(7C,Inverse,column markers) \
        \
        M(7D,Empty,reserved) \
        M(7E,Empty,reserved) \
        M(7F,Empty,reserved) \
        \
        M(80,Normal,passive frame) /* Used by the help viewer */\
        M(81,Light,active frame) \
        M(82,Normal,Cyan) /* frame icons */\
        M(83,Normal,scollbar) \
        M(84,Normal,scrollbar icons) \
        M(85,Normal,normal text) \
        M(86,Underline,keyword) \
        M(87,Inverse,selected keyword) \
        M(88,Inverse,keyword in marked area) \
        M(89,Inverse,marked text) /* Currently fixed to "\0x70" */\
        \
        M(8A,Normal,passive frame) /* Man page viewer */\
        M(8B,Light,active frame) \
        M(8C,Normal,frame icons) \
        M(8D,Normal,scrollbar) \
        M(8E,Normal,scrollbar icons) \
        M(8F,Normal,normal text) \
        M(90,Inverse,selected text) \
        M(91,Light,bold text) \
        M(92,Underline,underlined text) \
        \
        M(93,Empty,reserved)


#define SE_cpBlackWhite SE_cpColor

// SAA changed it but I don't think that's really needed
// I keep the code just in case
//#ifdef TVComp_BCPP
//#define _S(bg,fg) 0x##bg##fg ,
//extern char SE___cpColor[];
// #define SE_cpColor SE___cpColor
//#define SE_cpColor SE__cpColor GCC
// editmain: Pull it as an array
// char SE___cpColor[] = { SE__cpColor  0 };

