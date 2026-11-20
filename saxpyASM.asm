; Almero, Antonio Alvaro
; Marquez, John Thomas
; LBYARCH, S30G

section .text
global saxpyASM

saxpyASM:
    
    ; void saxpyASM(int n, float a, float X[], float Y[], float Z[])
    ; Parameters:
    ; rax        <- n
    ; xmm1       <- a
    ; r8         <- X 
    ; r9         <- Y
    ; (rsp + 40) <- Z

    mov rbx, [rsp + 40]     ; transfering Z to rbx
    mov rcx, 0
    

    loop_start:
        cmp rax, rcx
        je loop_end

        movss xmm0, xmm1
        mulss xmm0, [r8 + rcx * 4]
        addss xmm0, [r9 + rcx * 4]
        movss [rbx + rcx * 4], xmm0
        inc rcx
        jmp loop_start

     loop_end: ret