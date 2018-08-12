*----------------------------------------------------------------------
* Programmer: Michael Fahey
* Class Account: masc1385
* Assignment or Title: Program 3
* Filename: prog3.s
* Date completed:11/15/2012  
*----------------------------------------------------------------------
* Problem statement: calculate minimum of can given the cost of end material and side material and desired volume
* Input: cost of end and side materials, desired volume
* Output: can cost, diameter, height
* Error conditions tested:n/a 
* Included files: prog3.s
* Method and/or pseudocode: n/a 
* References: n/a
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

********** CONSTANTS **********
TWO:           EQU    $40000000        
PI:            EQU    $40490FDA
ONE_THIRD:     EQU    $3EAAAAAb
START_R:       EQU    $3C23D70A
*******************************

start:  initIO                  * Initialize (required for I/O)
        initF
        setEVT
        lineout  title  
        lineout  skipln 
        lineout  p1
        floatin  buffer
        cvtaf    buffer,D5   * END cost
        lineout  p2
        floatin  buffer
        cvtaf    buffer,D6   * SIDE cost
        lineout  p3
        floatin  buffer
        cvtaf    buffer,D7   * VOLUME
        lineout  skipln
        lineout  calc
        lineout  skipln

**********************************************************************
** Calculus Answer
** Formula for the radius of the optimum can:
** radius = (((volume*side_cost)/(2*PI*end_cost))^(1/3)      

** numerator, volume*side_cost:
        move.l      D7,D1       * VOLUME
        fmul        D6,D1       * VOLUME*SIDE_COST 
         
** denominator, 2*PI*end_cost        
        move.l      D5,D2       * END_COST
        fmul        #TWO,D2     * END_COST * 2.0
        fmul        #PI,D2      * END_COST * 2.0 * PI

** now take result to 1/3 power
        fdiv        D2,D1        * numerator/denominator
        move.l      #ONE_THIRD,D0              
        fpow        D1,D0 *(numerator/denominator) ^ (1/3)
        
** Calulus answer done, now calculate diameter, height, cost
        move.l      D0,D1       * D1 has radius
        fmul        #TWO,D0     * D0 has diameter        
        cvtfa       diameter,#2
        
** calculate height = (volume / PI*r^2)
        move.l      D1,D2       * radius
        fmul        D2,D2       * radius^2
        fmul        #PI,D2      * radius^2*PI
        move.l      D7,D3       * copy of volume
        fdiv        D2,D3       * vol / PI*radius^2  HEIGHT --> D3
        move.l      D3,D0      
        cvtfa       height,#2
        
** calculate cost = SIDE_COST*SIDE_SURFACE + 2*END_COST*END_SURFACE
        *** side cost:
        move.l      #PI,D2
        fmul        #TWO,D2     * 2*PI
        fmul        D1,D2       * 2*PI*radius
        fmul        D3,D2       * 2*PI*radius*height  = side surface area
        fmul        D6,D2       * side surface area * SIDE_COST
        
        *** end cost:
        move.l      #PI,D0
        fmul        #TWO,D0     * 2*PI
        fmul        D1,D0       * 2*PI*radius
        fmul        D1,D0       * 2*PI*radius*radius
        fmul        D5,D0       * 2*PI*radius*radius*END_COST
        fadd        D2,D0
        cvtfa       cost,#2
        
** DONE, print the  calculus answer
        
        lineout     ans1
        lineout     ans2
        lineout     ans3
        lineout     skipln
        
**********************************************************************
*** ADD YOUR CODE HERE FOR THE BRUTE FORCE ANSWER
*** Register usage:
*** D5 ->  END_COST
*** D6 ->  SIDE_COST
*** D7 ->  VOLUME
         move.l     #START_R,D1 *put .01 into D1
         move.l     D1,D2 
         fmul       D2,D2       * radius^2
         fmul       #PI,D2      * radius^2*PI
         move.l     D7,D3       * copy of volume
         fdiv       D2,D3       * vol / PI*radius^2  HEIGHT --> D3
         move.l     #PI,D2
         fmul       #TWO,D2     * 2*PI
         fmul       D1,D2       * 2*PI*radius
         fmul       D3,D2       * 2*PI*radius*height  = side surface area
         fmul       D6,D2       * side surface area * SIDE_COST
         move.l     #PI,D4
         fmul       #TWO,D4     * 2*PI
         fmul       D1,D4       * 2*PI*radius
         fmul       D1,D4       * 2*PI*radius*radius
         fmul       D5,D4       * 2*PI*radius*radius*END_COST
         fadd       D2,D4       *old cost in D4 
DO:      fadd       #START_R,D1 *increase radius by .01  
         move.l     D1,D2       * radius
         fmul       D2,D2       * radius^2
         fmul       #PI,D2      * radius^2*PI
         move.l     D7,D3       * copy of volume
         fdiv       D2,D3       * vol / PI*radius^2  HEIGHT --> D3 
         move.l     #PI,D2
         fmul       #TWO,D2     * 2*PI
         fmul       D1,D2       * 2*PI*radius
         fmul       D3,D2       * 2*PI*radius*height  = side surface area
         fmul       D6,D2       * side surface area * SIDE_COST
         move.l     #PI,D0
         fmul       #TWO,D0     * 2*PI
         fmul       D1,D0       * 2*PI*radius
         fmul       D1,D0       * 2*PI*radius*radius
         fmul       D5,D0       * 2*PI*radius*radius*END_COST
         fadd       D0,D2
         fcmp       D2,D4       *compare new cost to old cost
         ble        DONE
         move.l     D2,D4       *replace old cost with new cost
         bra        DO          *reloop    
DONE:    fsub       #START_R,D1 *subtract .01 for optimum radius 
         fmul       #TWO,D1     *mult radius by 2 for diameter 
         move.l     D4,D0 
         cvtfa      cost,#2  
         move.l     D1,D0
         cvtfa      diameter,#2
         move.l     D3,D0
         cvtfa      height,#2    
         lineout    brute       *print brute force answer
         lineout    skipln
         lineout    ans1
         lineout    ans2
         lineout    ans3                    
        
        break
        

buffer:   ds.b    80      
p1:       dc.b    'Please enter the cost of the end material in dollars/cm^2:',0 
p2:       dc.b    'Please enter the cost of the side material in dollars/cm^2:',0 
p3:       dc.b    'Please enter the volume of the can in milliliters:',0 
ans1:     dc.b    'Can cost: '
cost:     ds.b    40
ans2:     dc.b    'Diameter: '
diameter: ds.b    40
ans3:     dc.b    'Height: '
height:   ds.b    40 
title:    dc.b    'Program #3, masc1385, Michael Fahey',0
brute:    dc.b    'Brute Force Answer: ',0
calc:     dc.b    'Calculus Answer: ',0  
skipln:   ds.b    0 
          end
