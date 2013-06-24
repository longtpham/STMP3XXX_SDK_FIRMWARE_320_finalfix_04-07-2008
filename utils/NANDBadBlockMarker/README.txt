=======================================================================================
NANDBadBlockMarker Utility: This utility works in the BoxView debugger.
=======================================================================================
Function:
This routine will mark blocks bads by writing to the redundant area of the block.
The blocks to be marked bad are specified by the user in the BoxView Debugger.

=======================================================================================

To BUILD:
 in the Make directory, compile with:   nandbadblockmarker ALL DEBUG LIBS

=======================================================================================

To RUN: 
 in the Boxview debugger, load the file:  nandbadblockmarker.abs
 After it loads, in the WATCH window add the variable g_NandBadBlocks.
 This variable is a struct that looks like this:

typedef struct {
      int    iNumBadBlocks;         // number of bad blocks to set
      WORD   wBadBlockAddr[MAX_BAD_BLOCKS];         // block address to mark bad
} BAD_BLOCK_MAKER_TABLE;

and there are MAX_CHIPS (for now, 4) of these inside it.

Open it up by clicking the "+" sign.
To add bad blocks to Nand 0, open the "[0]" variable.
First, change the iNumBadBlocks to the number of bad blocks you want to add.
Next, begin adding the block numbers (in ascending order) to the wBadBlockAddr locations.

If you want to add blocks to Nand 1, etc. -- repeat the above steps for "[1]", etc.

After adding all the bad blocks, press F5 to run the program.
It should run until it hits "// SUCCESS!!!!! DONE!!!!!" in the code.

At this point, the bad blocks have been marked.

=======================================================================================
