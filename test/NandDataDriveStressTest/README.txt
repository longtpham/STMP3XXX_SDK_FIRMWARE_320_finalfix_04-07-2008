=======================================================================================
NANDDataDriveStressTest: This test works in the BoxView debugger or independently on an eval board.

=======================================================================================
Function:
This routine will write to a NAND or multi-NAND in a continuous loop, stressing the NAND.
It can be run for days and days. Eventually, bad blocks should begin appearing from the stress test.

The test will first perform a series of write sectors and check each write by reading back the sector
and comparing. First it writes an integer, then it writes the negation of that integer.

Next, the test will perform a series of multi-writes on the NANDs. The number of sectors written
in each multi-sector is generated randomly, as is the loop counter. These written sectors are also 
checked with a read sector to be sure the write has occurred.

=======================================================================================

To BUILD:
 in the Make directory, compile with:   nddStressTest ALL NANDx D3410 LIBS PLIBS

=======================================================================================

To RUN: 
For BoxView Debugger, load nddstresstest.abs and then run it.
For eval board, rename nddStressTest.sb to bootmanager.sb, copy to your SigmaTel player directory, and 
perform a firmware update to the player. Then, let it run.

=======================================================================================
DISPLAY:

The display will look like this:

001:10:15             <--- how long the test has been running (i.e. 1 hour, 10 minutes, 15 seconds)
NumBadBlocks = 2      <--- the current total of bad blocks across all NANDs
   2756290            <--- total sectors written 
0:0002  000:00:00     <--- NAND0:Block2 was bad at time 0 (at beginning of test it was bad)
1:0020  085:12:11     <--- NAND1:Block20 went bad at time 85 hours, 12 minutes, and 11 seconds

NOTE: The display can hold 3 bad blocks, after 3, it will begin displaying 3, then after 3 seconds it will update
to the next set of bad blocks and rotate through like that.

NOTE2: TO STOP THE TEST ON EVAL BOARD (without debugger): Press the STOP button and the top line should display
"OK to unplug". Then, disconnect it. This will ensure that a write is not in progress.