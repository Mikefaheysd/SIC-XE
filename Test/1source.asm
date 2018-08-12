.sample program for SIC/XE architecture
.

prog    start   $1000    .start of sample program

        lds     #3
        ldt     #300
addlf   lda     alpha,x   .loop control     
        add     beta,x
        sta     gamma,x                         
        compr   x,t 

        .
. storage allocation section        
alpha   resw    100  
beta    word    3
gamma   word    2
        end     prog    .end of sample program     
