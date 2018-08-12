.sample program for SIC/XE architecture
.

prog    start   $1000    .start of sample program
        base    delta
        +ldb     #delta
        lda     zeta
        lds     #3
        ldt     #300
addlf   lda     alpha,x   loop control   qq qq qq 'qq qq qq  
        add     beta,x
        sta     gamma,x                         
        compr   x,t 

        .
. storage allocation section        
alpha   resw    100  
beta    word    3
gamma   word    2
buffer  resb    3000
delta   resb    20
zeta    word    4
        end     prog    .end of sample program     
