section .ptextmetadata					MP3_COPY;
contiguous
{
	section .ptextplaylist3init_metadata	MP3_COPY;
    overlay
    {
		contiguous
		{
    section .ptextmp3metadata				MP3_COPY;
			section .ptextid3v2				MP3_COPY;
		}
		section .ptextwmametadata			MP3_COPY;
		section .ptextwavmetadata			MP3_COPY;
	}
}
section .ptextsmvmetadata               MP3_COPY;
contiguous
{
    overlay
    {
        section .ptextid3v2_lyrics_core			MP3_COPY;
        section .ptextlyrics3_core				MP3_COPY;
        section .ptextlrc_core					MP3_COPY;
        section .ptextlyrics_api				MP3_COPY;
        section .ptextwma_lyrics_core			MP3_COPY;
    }
    section LYRICS_COMMON_P            MP3_COPY;
}
contiguous
{
    section .ptextvbr						MP3_COPY;
    section .pdatavbr                       MP3_COPY;
}

section .ptextattachedpictureframe	    MP3_COPY;
section .ptextaudiblemetadata		    MP3_COPY;
section .ptextaudiblefillmetadata		MP3_COPY;
contiguous {
	section .ptextaametastrings				MP3_COPY;
	section .pconstaametastrings			MP3_COPY;
}
contiguous {
	section .ptextaaactivationrecords		MP3_COPY;
}
section .ptextaudiblesongposition		MP3_COPY;
contiguous {
	section .ptextactivationrecords     MP3_COPY;
	section .ptextchecktargeting		MP3_COPY;
	section .ptextdsaverify				MP3_COPY;
	section .ptextsha					MP3_COPY;
}
section .ptextronsdsa					MP3_COPY;
section .ptextmetafillaudiblefileinfo	MP3_COPY;
