// File: menus.dsc   MP3_COPY is attr=i (run-time loaded) for player.abs build
section .ptextmainmenu                      MP3_COPY;
section .ptextdisplay                       MP3_COPY;
section .ptextvoicemenu                     MP3_COPY;
section .ptextfmtunermenu                   MP3_COPY;
contiguous
{
    section .ptextmusicmenu                     MP3_COPY;
    section .ptextmusicmenu_extra           MP3_COPY;
}
section .ptextplaymusicmenu                 MP3_COPY;
section .ptextnewmusicmenu                  MP3_COPY;
section .ptextbrowsemenu                    MP3_COPY;
section .ptextbrowsemenu_extra              MP3_COPY;
section .ptextrecorderstatemachine          MP3_COPY;
section .ptextdisplaylists                  MP3_COPY;
section .ptextcontrastmenu                  MP3_COPY;
section .ptexttimedatemenu                  MP3_COPY;
section .ptextsettimemenu                   MP3_COPY;
section .ptextsetdatemenu                   MP3_COPY;
section .ptexteqmenu                        MP3_COPY;
section .ptextsettingsmenu                  MP3_COPY;
section .ptextpwrsettingsmenu               MP3_COPY;
section .ptextplaymodemenu                  MP3_COPY;
section .ptextdeletemenu                    MP3_COPY;
section .ptextaboutmenu                     MP3_COPY;
section .ptextbacklightmenu                 MP3_COPY;
section .ptexttestmenu                      MP3_COPY;
section .ptextsdmd                          MP3_COPY;
section .ptextjpegdisplaymenu				MP3_COPY;
section .ptextmusiclib_build_function_sec   MP3_COPY;
section .ptextmusiclib_build_function_flush MP3_COPY;
contiguous 
{   overlay 
    {   section .ptextplayerstatemachine    MP3_COPY;
        section .ptextspectrogram           MP3_COPY;
    }
    section .ptextplayerlib                 MP3_COPY;
}
section .ptextplayerlib_extra               MP3_COPY;
section .ptextsysrecord                     MP3_COPY;
section .ptextvoice1                        MP3_COPY;
section .ptextmp3filename                   MP3_COPY;
contiguous 
{   section .ptextrecordsettingsmenu        MP3_COPY;
    section .pdatarecordsettingsmenu        MP3_COPY;
}
contiguous
{   overlay {
        contiguous {
            overlay {
                overlay {
                    section .ptextmotionvideomenu           MP3_COPY;
                    section .ptextmotionvideomenuinitstate  MP3_COPY;
                }
                contiguous {
                    overlay {
                        section .ptextjpegmanualmenu    MP3_COPY;
                        section .ptextjpegslideshowmenu	MP3_COPY;
                        section .ptextjpegthumbnailmenu MP3_COPY;
                    }
                    section .ptextjpegdisplaycommon     MP3_COPY;
                }
            }
            section .ptextjpegfileutil                  MP3_COPY;
        }
        contiguous {
            section .ptextjpegfileutilextra             MP3_COPY;
        }
        overlay
        {   overlay
            {   contiguous
                {   section .ptextplaylist1         MP3_COPY;
                    section .ptextlongfilenames     MP3_COPY;
                }                                            
                section .ptextplaylist2init         MP3_COPY;
                section .ptextplaylist3init         MP3_COPY;
            }
            contiguous
            {   section .ptextplaylist2             MP3_COPY;
                section .ptextshuffletrackorder     MP3_COPY;
            }                                                
            section .ptextplaylist3                 MP3_COPY;
            section .ptextplaylist2traverse         MP3_COPY;
        }
    }                                                    
    section .ptextfindfirst                     MP3_COPY;
    section .ptextfindnext                      MP3_COPY;
}
contiguous
{   section .ptextbatterychargetask         MP3_COPY;
    section .pdatabatterychargetask         MP3_COPY;
    section .ptextliion                     MP3_COPY;
}
section .ptextwowmenu       MP3_COPY;
contiguous{
	overlay{
		section .ptextwowctrl       MP3_COPY;
		section .ptextwow16k        MP3_COPY;
		section .ptextwow32k        MP3_COPY;
		section .ptextwow8k         MP3_COPY;
		section .ptextwow11k        MP3_COPY;
		section .ptextwow22k        MP3_COPY;
		section .ptextwow24k        MP3_COPY;
		section .ptextwow44k        MP3_COPY;
		section .ptextwow48k        MP3_COPY;
	}
	section .ptextsetfiltercoefptrs		MP3_COPY;
}
// These 3 sections are used to restore the resource.bin file when
// it gets corrupted.  When called during bootup the restoredrivefromback up code reboots
// after it complets so it does not matter where this section
// get placed. When called realtime (between songs, during encode, during fmtuner) it is called using
// syscallfunction so its all good.
contiguous
{
    section .ptextnandsystemdrivewritesector            MP3_COPY;
    section .ptextrestoredrivefrombackup                MP3_COPY;
    section .ptextpartialrestoredrivefrombackup         MP3_COPY;
	section .ptextnandsystemdriveerase                  MP3_COPY;
    label eRestoreSysDrive_P;
}
