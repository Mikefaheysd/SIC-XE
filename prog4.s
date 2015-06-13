*----------------------------------------------------------------------
* Programmer: Michael Fahey
* Class Account: masc1385
* Assignment or Title: prog4
* Filename: prog4.s
* Date completed:  12/4/2012
*----------------------------------------------------------------------
* Problem statement:reverse user inputted string using recursion
* Input: user inputs a string
* Output: user inputted string in reverse
* Error conditions tested: n/a
* Included files: reverse.s
* Method and/or pseudocode:Proffessor Riggens algorithm
* References:n/a
*----------------------------------------------------------------------
*
        ORG     $0
        DC.L    $3000           * Stack pointer value after a reset
        DC.L    start           * Program counter value after a reset
        ORG     $3000           * Start at location 3000 Hex
*
*----------------------------------------------------------------------
*
#minclude /home/ma/cs237/bsvc/iomacs.s
#minclude /home/ma/cs237/bsvc/evtmacs.s
*
*----------------------------------------------------------------------
*
* Register use
*    D0:original count
*    D1:count--
*    A0:IN(user input)
*    A1:OUT(reversed string)
*
*----------------------------------------------------------------------
*
reverse: EQU   $7000
start:  initIO                  * Initialize (required for I/O)
	setEVT			* Error handling routines
*	initF			* For floating point macros only	

				* Your code goes HERE
          lineout   title
          lineout   prompt
          linein    buffer    *user inputs string
          move.w    D0,D1     *move count to D1
          move.w    D1,-(sp)  *move count to stack
          pea       out       *move out to stack
          pea       buffer    *move in to stack
          jsr       reverse   *jump to sub-reverse
          adda.l    #10,SP
          lea       out,A1    *load reversed string into A1
          adda.l    D0,A1     *add string size to A1
          clr.b     (A1)      *null terminate
          lineout   skipln
          lineout   answer    
          lineout   out       *prints reversed string
                              


        break                   * Terminate execution
*
*----------------------------------------------------------------------
*       Storage declarations

				* Your storage declarations go 
				* HERE
skipln:   dc.b      0
title:    dc.b      'Program #4,Michael Fahey, masc1385 ',0
prompt:   dc.b      'Enter a string:',0
buffer:   ds.b      82
answer:   dc.b      'Here is the string backwards:',0
out:      ds.b      82
    
        end
