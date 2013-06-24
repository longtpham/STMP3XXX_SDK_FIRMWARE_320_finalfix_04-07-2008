/**************************************************************************
**
**  FILE        :   local_copy1.dsc 
**
**  VERSION     :   06/20/05
**
**  DESCRIPTION :   This descriptor file describes all of the sections that 
**                  are local to the appliation that will fit within one 
**                  0xA000 word sized copy section.
**
**      NOTE    :   If this cluster runs out of space, create a new file 
**                  (local_copyX.dsc) and update local_copies.dsc to reflect
**                  the new file.
**
**************************************************************************/

copy .ptextaboutmenu;
copy .ptextbacklightmenu;
copy .ptextcontrastmenu;
copy .ptextdeletemenu;
copy .ptextdisplay;
copy .ptextdisplaylists;
copy .ptexteqmenu;
copy .ptextfmtunermenu;
copy .ptextmainmenu;
copy .ptextmusicmenu;
copy .ptextmusicmenu_extra;
copy .ptextsaveusertime;
copy .ptextsetdatemenu;
copy .ptextsettimemenu;
copy .ptextsettingsmenu;
copy .ptexttimedatemenu;
copy .ptextplaymodemenu;
copy .ptextpwrsettingsmenu;
copy .ptextspectrogram;
copy .ptexttestmenu;
copy .ptexttimedatemenu;
copy .ptextvoicemenu;
copy .ptextwowmenu;