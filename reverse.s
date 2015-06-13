*michael fahey, masc1385
      ORG  $7000
IN:   EQU   0008
OUT:  EQU   0012
COUNT EQU   0016

reverse: link      A6,#0
         movem.l   D1/A0-A1,-(sp) 
         movea.l   IN(A6),A0      *in
         movea.l   OUT(A6),A1     *out
         move.w    COUNT(A6),D1   *count          
         tst.w     D1             *stopping condition,count==0 
         BEQ       return
         subq.w    #1,D1          *count--
         addq.l    #1,A0          *in+1
         move.w    D1,-(sp)       *push count-- on stack
         movem.l   A0-A1,-(sp)    *push in+1/out on stack
         jsr       reverse        *recursive call
         adda.l    #10,sp         *pop the garbage
         ext.l     D1
         adda.l    D1,A1          *out+count
         subq.l    #1,A0          *in-1 
         move.b    (A0),(A1)  
return:  movem.l   (sp)+,D1/A0-A1
         unlk       A6     
         rts
         end


