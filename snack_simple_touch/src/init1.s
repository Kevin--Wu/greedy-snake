;=========================================

; NAME: 2440INIT.S

; DESC: C start up codes

;       Configure memory, ISR ,stacks

;      Initialize C-variables

;=========================================
;GET类似于C语言的include，option.inc文件内定义了一些全局变量，
;memcfg.inc文件内定义了关于内存bank的符号和数字常量，
;2440addr.inc文件内定义了用于汇编的s3c2440寄存器变量和地址

       GET option.inc
       GET memcfg.inc
       GET 2440addr.inc

;SDRAM自刷新位，把寄存器REFRESH的第22位处置1

BIT_SELFREFRESH   EQU    (1<<22)

;CPSR中的低5位定义了处理器的七种工作模式，为以后切换模式时使用

;Pre-defined constants

USERMODE    EQU     0x10
FIQMODE      EQU                0x11
IRQMODE      EQU               0x12
SVCMODE     EQU      0x13
ABORTMODE   EQU        0x17
UNDEFMODE   EQU        0x1b
MODEMASK    EQU        0x1f

;CPSR中的I位和F位置1，表示禁止任何中断

NOINT       EQU      0xc0

;定义了7种处理器模式下的栈的起始地址，其中用户模式和系统模式共有一个栈空间

;_STACK_BASEADDRESS在option.inc文件内定义，值为0x33ff8000

;The location of stacks

UserStack     EQU      (_STACK_BASEADDRESS-0x3800) ;0x33ff4800 ~
SVCStack    EQU      (_STACK_BASEADDRESS-0x2800) ;0x33ff5800 ~
UndefStack  EQU      (_STACK_BASEADDRESS-0x2400) ;0x33ff5c00 ~
AbortStack   EQU      (_STACK_BASEADDRESS-0x2000) ;0x33ff6000 ~
IRQStack     EQU      (_STACK_BASEADDRESS-0x1000) ;0x33ff7000 ~
FIQStack     EQU      (_STACK_BASEADDRESS-0x0)       ;0x33ff8000 ~

;ARM处理器的两种工作状态：16位和32位
;编译器有相对应的用16位和32位两种编译方式
;这段的目的是统一目前的处理器工作状态和软件编译方式

;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.

       GBLL    THUMBCODE                   ;声明一个全局逻辑变量

       [ {CONFIG} = 16                                 ;if CONFIG == 16

THUMBCODE SETL  {TRUE}               ;THUMBCODE = TRUE

           CODE32                                        ;指示编译器为ARM指令

             |                                                ;else

THUMBCODE SETL  {FALSE}                    ;THUMBCODE = FALSE

    ]

 

;宏定义，在后面出现MOV_PC_LR时，这个宏会被自动展开
;该宏的作用是跳出子程序，返回被调用处

        MACRO
       MOV_PC_LR

             [ THUMBCODE              ;if THUMBCODE == TRUE
           	bx lr
            	 |                                  ;else 即THUMBCODE == FALSE
          	 mov      pc,lr
             ]

       MEND
;该宏定义的作用是有条件地（当Z=1时）跳出子程序，返回被调用处

             MACRO    
             MOVEQ_PC_LR

             [ THUMBCODE

       		 bxeq lr

             |

           moveq pc,lr

             ]

       MEND

 

;该宏定义是把中断服务程序的首地址装载到pc中

;在后面当遇到HandlerXXX HANDLER HandleXXX时，该宏被展开

;注意：HANDLER前的符号HandlerXXX比其后的符号HandleXXX多了一个r

;HandlerXXX为ARM体系中统一定义的几种异常中断

;HandleXXX为每个ARM处理器各自定义的中断，见该文件最后部分的中断向量表

             MACRO
$HandlerLabel HANDLER $HandleLabel

 

$HandlerLabel           

       sub  sp,sp,#4 ;ATPCS规定数据栈为FD类型

 ;即栈指针指向栈顶元素，数据栈向内存地址减小的方向增长

 ;该语句是使栈地址减小4个字节，以留出空间装载中断服务函数首地址

       stmfd     sp!,{r0} ;由于要利用r0寄存器来传递数据，所以要保存r0数据，使其入栈

       ldr     r0,=$HandleLabel     ;把HandleXXX的地址装到r0

       ldr     r0,[r0]    ;装载中断服务函数的起始地址

       str     r0,[sp,#4]               ;中断函数首地址入栈

       ldmfd   sp!,{r0,pc} ;将事先保存的r0数据和中断函数首地址出栈

                      ;并使系统跳转到相应的中断处理函数

       MEND

 

;导入连接器事先定义好的运行域中三个段变量

;ARM的可执行映像文件由RO、RW、ZI三个段组成

;RO为代码段，RW为已初始化的全局变量，ZI为未初始化的全局变量

       IMPORT  |Image$$RO$$Base|    ;RO段起始地址

       IMPORT  |Image$$RO$$Limit|   ;RO段结束地址加1，等于RW段起始地址

       IMPORT  |Image$$RW$$Base|   ;RW段起始地址

       IMPORT  |Image$$ZI$$Base|      ;ZI段起始地址

       IMPORT  |Image$$ZI$$Limit|     ;ZI段结束地址加1

 

;导入两个关于MMU的函数，用于设置时钟模式为异步模式和快速总线模式

       IMPORT      MMU_SetAsyncBusMode

       IMPORT      MMU_SetFastBusMode  ;

 

;导入Main，它为C语言程序入口函数

       IMPORT  Main        ; The main entry of mon program

;定义代码段，名为Init

       AREA    Init,CODE,READONLY

 

;在入口处（0x0）开始的8个字单元空间内，存放的是ARM异常中断向量表，每个字单元空间都是一条跳转指令，当异常发生时，ARM会自动跳转到相应的中断向量处，并由该处的跳转指令再跳转到相应的执行函数处

       ENTRY        ;程序入口处

       EXPORT     __ENTRY           ;导出__ENTRY，即导出代码段入口地址

__ENTRY                                ;主要用于MMU

ResetEntry

       ;1)The code, which converts to Big-endian, should be in little endian code.

       ;2)The following little endian code will be compiled in Big-Endian mode.

       ;  The code byte order should be changed as the memory bus width.

       ;3)The pseudo instruction,DCD can not be used here because the linker generates error.

;在0x0处的异常中断是复位异常中断，是上电后执行的第一条指令

;变量ENDIAN_CHANGE用于标记是否要从小端模式改变为大端模式，因为编译器初始模式是小端模式，如果要用大端模式，就要事先把该变量设置为TRUE，否则为FLASE

;变量ENTRY_BUS_WIDTH用于设置总线的宽度，因为用16位和8位宽度来表示32位数据时，在大端模式下，数据的含义是不同的

;由于要考虑到大端和小端模式，以及总线的宽度，因此该处看似较复杂，其实只是一条跳转指令：当为大端模式时，跳转到ChangeBigEndian函数处，否则跳转到ResetHandler函数处

       ASSERT      :DEF:ENDIAN_CHANGE    ;判断是否定义了ENDIAN_CHANGE

                                ;如果没有定义，则报告该处错误信息

       [ ENDIAN_CHANGE                   ;if ENDIAN_CHANGE ==TRUE

              ASSERT  :DEF:ENTRY_BUS_WIDTH  ;判断是否定义了ENTRY_BUS_WIDTH

                                       ;如果没有定义，则报告该处错误信息

 

              [ ENTRY_BUS_WIDTH=32         ;if ENTRY_BUS_WIDTH ==32

;跳转到ChangeBigEndian（ChangeBigEndian在0x24），因此该条指令的机器码为0xea000007

;所以该语句与在该处（即0x0处）直接放入0xea000007数据（即DCD 0xea000007）作用相同

                     b     ChangeBigEndian      

              ]

 

              [ ENTRY_BUS_WIDTH=16         ;if ENTRY_BUS_WIDTH ==16

;在小端模式下，用16位或8位数据总线宽度表示32位数据，与用32位总线宽度表示32位数据，格式完全一致。但在大端模式下，格式就会发生变化

;在复位时，系统默认的是小端模式，所以就要人为地改变数据格式，使得用16位大端数据表示的32位数据也能被小端模式的系统识别

;该语句的目的也是跳转到ChangeBigEndian，即机器码也应该是0xea000007，但为了让小端模式系统识别，就要把机器码的顺序做一下调整，改为0x0007ea00，那么我们就可以用DCD 0x0007ea00把机器码装载进去了，但由于该处不能使用DCD伪指令，因此我们就要用一条真实的指令来代替DCD 0x0007ea00，即该指令编译后的机器码也为0x0007ea00，而andeq  r14,r7,r0,lsl #20就是一条编译后机器码为0x0007ea00的指令，所以我们在该处写上该条指令

                     andeq    r14,r7,r0,lsl #20   ;DCD 0x0007ea00

              ]

 

              [ ENTRY_BUS_WIDTH=8           ;if ENTRY_BUS_WIDTH ==8

;该语句的分析与上一段代码的分析相似

;streq  r0,[r0,-r10,ror #1]编译后的机器码为0x070000ea

                     streq      r0,[r0,-r10,ror #1] ;DCD 0x070000ea

              ]

              |             ;else 即ENDIAN_CHANGE ==FALSE

              b     ResetHandler             ;跳转到ResetHandler处，复位

       ]

       b     HandlerUndef     ;未定义

       b     HandlerSWI ;软件中断

       b     HandlerPabort     ;指令预取中止

       b     HandlerDabort    ;数据访问中止

       b     .             ;保留，跳转到自身地址处，即进入死循环

       b     HandlerIRQ ;外部中断请求

       b     HandlerFIQ ;快速中断请求

;以上为异常中断向量表

 

;跳转到EnterPWDN，处理电源管理的其他非正常模式，在C语言程序段中被调用

;该处地址为0x20，至于为什么要在该处执行，我认为可能是该处离异常中断向量表最近吧

       b     EnterPWDN       ; Must be @0x20.

 

;由0x0跳转至此，目的是把小端模式改为大端模式，即把CP15中的寄存器C1中的第7位置1

ChangeBigEndian

;@0x24

       [ ENTRY_BUS_WIDTH=32         ;if ENTRY_BUS_WIDTH == 32

       ;执行mrc p15,0,r0,c1,c0,0，得到CP15中的寄存器C1，放入r0中

       ;由于mrc p15,0,r0,c1,c0,0的机器码为0xee110f10

       ;因此DCD      0xee110f10的意思就是mrc p15,0,r0,c1,c0,0。下同

              DCD     0xee110f10  ;0xee110f10 => mrc p15,0,r0,c1,c0,0

       ;执行orr r0,r0,#0x80，置r0中的第7位为1，表示选择大端模式

              DCD     0xe3800080 ;0xe3800080 => orr r0,r0,#0x80;  //Big-endian

       ;执行mcr p15,0,r0,c1,c0,0，把r0写入CP15中的寄存器C1

              DCD     0xee010f10  ;0xee010f10 => mcr p15,0,r0,c1,c0,0

       ]

       [ ENTRY_BUS_WIDTH=16         ;if ENTRY_BUS_WIDTH == 16

       ;由于此时系统还不能识别16位或8位大端模式下表示的32为数据

       ;因此还需人为地进行数据调整，即把0xee110f10变为0x0f10ee11

       ;然后用DCD指令存入该数据。下同

              DCD 0x0f10ee11

              DCD 0x0080e380

              DCD 0x0f10ee01

       ]

       [ ENTRY_BUS_WIDTH=8           ;if ENTRY_BUS_WIDTH == 8

              DCD 0x100f11ee

              DCD 0x800080e3

              DCD 0x100f01ee

       ]

    ;相当于NOP指令

    ;作用是等待系统从小端模式向大端模式转换

    ;此后系统就能够自动识别出不同总线宽度下的大端模式，因此以后就无需再人为调整指令了

       DCD 0xffffffff  ;swinv 0xffffff is similar with NOP and run well in both endian mode.

       DCD 0xffffffff

       DCD 0xffffffff

       DCD 0xffffffff

       DCD 0xffffffff

       b ResetHandler       ;跳转到ResetHandler

    

;当系统进入异常中断后，由存放在0x0~0x1C处的中断向量地址中的跳转指令，跳转到此处相应的位置，
;并由事先定义好的宏定义再次跳转到相应的中断服务程序中

HandlerFIQ HANDLER HandleFIQ

HandlerIRQ HANDLER HandleIRQ

HandlerUndef     HANDLER HandleUndef

HandlerSWI HANDLER HandleSWI

HandlerDabort    HANDLER HandleDabort

HandlerPabort     HANDLER HandlePabort

 

;下面这段代码是用于处理非向量中断，即由软件程序来判断到底发生了哪种中断，然后跳转到相应地中断服务程序中

;具体地说就是，当发生中断时，会置INTOFFSET寄存器相应的位为1，然后通过查表（见该程序末端部分的中断向量表），找到相对应的中断入口地址

;观察中断向量表，会发现它与INTOFFSET寄存器中的中断源正好相对应，即向量表的顺序与INTOFFSET寄存器中的中断源的
;由小到大的顺序一致，因此我们可以用基址加变址的方式很容易找到相对应的中断入口地址。其中基址为向量表的首个中断源地址，
;变址为INTOFFSET寄存器的值乘以4（因为系统是用4个字节单元来存放一个中断向量）

IsrIRQ
       sub  sp,sp,#4        ;在栈中留出4个字节空间，以便保存中断入口地址
       stmfd     sp!,{r8-r9}    ;由于要用到r8和r9，因此保存这两个寄存器内的值

       ldr   r9,=INTOFFSET       ;把INTOFFSET寄存器地址装入r9内
       ldr   r9,[r9]           ;读取INTOFFSET寄存器内容
       ldr   r8,=HandleEINT0      ;得到中断向量表的基址
       add r8,r8,r9,lsl #2              ;用基址加变址的方式得到中断向量表的地址
       ldr   r8,[r8]                  ;得到中断服务程序入口地址
       str   r8,[sp,#8]                    ;使中断服务程序入口地址入栈
       ldmfd    sp!,{r8-r9,pc}      ;使r8，r9和入口地址出栈，并跳到中断服务程序中

;定义一个数据缓冲池，供ldr伪指令使用

       LTORG

 

;=======

; ENTRY

;=======

;系统上电或复位后，由0x0处的跳转指令，跳转到该处开始真正执行系统的初始化工作

ResetHandler

    ;在系统初始化过程中，不需要看门狗，因此关闭看门狗功能

       ldr   r0,=WTCON       ;watch dog disable

       ldr   r1,=0x0

       str   r1,[r0]

 

    ;同样，此时也不应该响应任何中断，因此屏蔽所有中断，以及子中断

       ldr   r0,=INTMSK

       ldr   r1,=0xffffffff  ;all interrupt disable

       str   r1,[r0]

 

       ldr   r0,=INTSUBMSK

       ldr   r1,=0x7fff           ;all sub interrupt disable

       str   r1,[r0]

 

;由于启动文件是无法仿真的，因此为了判断该文件中语句的正确与否，往往在需要观察的地方加上一段点亮LED的程序，
;这样就可以知道程序是否已经执行到此处

;下面方括号内的程序就是点亮LED的小程序

        [ {TRUE}
	;rGPFDAT = (rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);
	; Led_Display
 	ldr	r0,=GPBCON
 	ldr	r1,=0x00555555
 	str	r1,[r0]
 	ldr	r0,=GPBDAT
 	ldr	r1,=0x07fe
 	str	r1,[r0]
 	]

 
;下列程序是用于设置系统时钟频率

;设置PLL的锁定时间常数，以得到一定时间的延时

       ;To reduce PLL lock time, adjust the LOCKTIME register.

       ldr   r0,=LOCKTIME

       ldr   r1,=0xffffff

       str   r1,[r0]

 

       [ PLL_ON_START

              ; Added for confirm clock divide. for 2440.

              ; Setting value Fclk:Hclk:Pclk

       ;设置系统的三个时钟频率FCLK、HCLK、PCLK

              ldr   r0,=CLKDIVN

              ldr   r1,=CLKDIV_VAL         ; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.

              str   r1,[r0]

 

       ;program has not been copied, so use these directly

              [ CLKDIV_VAL>1               ;if FCLK：HCLK≠1：1

           ;设置时钟模式为异步模式

                     mrc p15,0,r0,c1,c0,0

                     orr r0,r0,#0xc0000000;R1_nF:OR:R1_iA

                     mcr p15,0,r0,c1,c0,0

                     |                                  ;else

           ;设置时钟模式为快速总线模式

                     mrc p15,0,r0,c1,c0,0

                     bic r0,r0,#0xc0000000;R1_iA:OR:R1_nF

                     mcr p15,0,r0,c1,c0,0

              ]

 

       ;配置UPLL

       ;按照手册中的计算公式，确定MDIV、PDIV和SDIV

       ;得到当系统输入时钟频率为12MHz的情况下，UCLK输出频率为48MHz

              ;Configure UPLL

              ldr   r0,=UPLLCON

              ldr   r1,=((U_MDIV<<12)+(U_PDIV<<4)+U_SDIV)                          ;Fin = 12.0MHz, UCLK = 48MHz

              str   r1,[r0]

       ;等待至少7个时钟周期，以保证系统的正确配置

              nop ; Caution: After UPLL setting, at least 7-clocks delay must be inserted for setting hardware be completed.

              nop

              nop

              nop

              nop

              nop

              nop

       ;配置MPLL，同UPLL

              ;Configure MPLL

              ldr   r0,=MPLLCON

              ldr   r1,=((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV)                         
              
              ;Fin = 12.0MHz, FCLK = 400MHz

              str   r1,[r0]

       ]

 

    ;从SLEEP模式下被唤醒，类似于RESET引脚被触发，因此它也要从0x0处开始执行

    ;在此处要判断是否是由SLEEP模式唤醒引起的复位

       ;Check if the boot is caused by the wake-up from SLEEP mode.

       ldr   r1,=GSTATUS2

       ldr   r0,[r1]

       tst   r0,#0x2         ;检查GSTATUS2寄存器的第1位

       ;In case of the wake-up from SLEEP mode, go to SLEEP_WAKEUP handler.

       bne WAKEUP_SLEEP           ;是被唤醒的，则跳转

 

;设置一个被唤醒复位后的起始点地址标号，可以把它保存到GSTATUS3中

;导出该地址标号，以便在C语言程序中使用

       EXPORT StartPointAfterSleepWakeUp

StartPointAfterSleepWakeUp

 

;设置内存控制寄存器

;关于内存控制寄存器一共有以BWSCON为开始的连续放置的13个寄存器，我们要一次性批量完成这13个寄存器的配置

;因此开辟一段以SMRDATA为地址起始点的13个字单元空间，按顺序放入要写入的13个寄存器内容

       ;Set memory control registers

      ;ldr  r0,=SMRDATA

      adrl r0, SMRDATA   ;得到SMRDATA空间的首地址

       ldr   r1,=BWSCON           ;得到BWSCON的地址

       add r2, r0, #52            ;得到SMRDATA空间的末地址

 

;完成13个字数据的复制

0

       ldr   r3, [r0], #4

       str   r3, [r1], #4

       cmp       r2, r0

       bne %B0

 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;       When EINT0 is pressed,  Clear SDRAM 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; check if EIN0 button is pressed

;检查EIN0按钮是否被按下

       ldr   r0,=GPFCON

       ldr   r1,=0x0

       str   r1,[r0]           ;GPFCON=0，F口为输入

       ldr   r0,=GPFUP

       ldr   r1,=0xff

       str   r1,[r0]           ;GPFUP=0xff，上拉功能无效

 

       ldr   r1,=GPFDAT

       ldr   r0,[r1]           ;读取F口数据

       bic  r0,r0,#(0x1e<<1)        ;仅保留第1位数据，其他清0

       tst   r0,#0x1                ;判断第1位

       bne %F1                     ;不为0表示按钮没有被按下，则向前跳转，不执行清空SDRAM

    

    

 

; Clear SDRAM Start

;清空SDRAM

       ldr   r0,=GPFCON

       ldr   r1,=0x55aa

       str   r1,[r0]           ;GPF7~4为输出，GPF3~0为中断

;      ldr   r0,=GPFUP

;      ldr   r1,=0xff 

;      str   r1,[r0]          ;上拉功能无效

       ldr   r0,=GPFDAT

       ldr   r1,=0x0

       str   r1,[r0]           ;GPFDAT = 0

 

       mov r1,#0

       mov r2,#0

       mov r3,#0

       mov r4,#0

       mov r5,#0

       mov r6,#0

       mov r7,#0

       mov r8,#0

    

       ldr   r9,=0x4000000           ;64MB RAM

       ldr   r0,=0x30000000         ;RAM首地址

;清空64MB的RAM

0     

       stmia      r0!,{r1-r8}

       subs       r9,r9,#32 

       bne %B0

;Clear SDRAM End

 

1

;初始化各种处理器模式下的堆栈

       ;Initialize stacks

       bl    InitStacks                   ;跳转到InitStacks

 

;===========================================================

    

;下面的代码为把ROM中的数据复制到RAM中

       ldr   r0, =BWSCON

       ldr   r0, [r0]

       ands       r0, r0, #6              ;读取OM[1:0]引脚状态

 ;为0表示从NAND Flash启动，不为0则从NOR Flash启动

       bne copy_proc_beg    ;跳转，不用读取NAND Flash

       adr  r0, ResetEntry            ;OM[1:0] == 0，从NAND Flash启动

       cmp       r0, #0                          ;if use Multi-ice, 

       bne copy_proc_beg           ;do not read nand flash for boot

       ;nop

;===========================================================

nand_boot_beg

       [ {TRUE}
;bl RdNF2SDRAM           ;复制NAND Flash到SDRAM

       ]
       ldr   pc, =copy_proc_beg

;===========================================================

copy_proc_beg

       adr  r0, ResetEntry

       ldr   r2, BaseOfROM

       cmp       r0, r2             ;比较程序入口地址与连接器定义的RO基地址

       ldreq      r0, TopOfROM   ;如果相等，把RO尾地址读取到r0中

       beq InitRam        ;如果相等，则跳转

       ldr r3, TopOfROM                  ;否则，把RO尾地址读取到r3中

;下列循环体为在程序入口地址与连接器定义的RO基地址不相等的情况下，把程序复制到RAM中

0     

       ldmia     r0!, {r4-r7}

       stmia      r2!, {r4-r7}

       cmp       r2, r3

       bcc  %B0

;修正非字对齐的情况

       sub  r2, r2, r3

       sub  r0, r0, r2                            

       

InitRam 

       ldr   r2, BaseOfBSS

       ldr   r3, BaseOfZero   

;下面循环体为复制已初始化的全局变量

0

       cmp       r2, r3

       ldrcc      r1, [r0], #4

       strcc       r1, [r2], #4

       bcc  %B0      

 

;下面循环体是为未初始化的全局变量赋值为0

       mov       r0,   #0

       ldr   r3,   EndOfBSS

1     

       cmp       r2,   r3

       strcc       r0, [r2], #4

       bcc  %B1

    

       ldr   pc, =%F2             ;goto compiler address

2

    

;      [ CLKDIV_VAL>1               ;if FCLK：HCLK≠1：1

;      bl    MMU_SetAsyncBusMode      ;设置时钟模式为异步模式

;      |

;      bl MMU_SetFastBusMode     ;设置时钟模式为快速总线模式

;      ]

    

 

;===========================================================

;普通中断处理

;当普通中断发生时，执行的是IsrIRQ

     ; Setup IRQ handler

       ldr   r0,=HandleIRQ   ;This routine is needed

       ldr   r1,=IsrIRQ   ;if there is not 'subs pc,lr,#4' at 0x18, 0x1c

       str   r1,[r0]

 

 

;完成最基本的初始化任务，跳转到由C语言撰写的Main()函数内继续执行其他程序

;这里不能写main，因为写了main，系统会自动为我们完成一些初始化工作，而这些工作在这段程序中是由我们显式地人为完成的。

    [ :LNOT:THUMBCODE

             bl    Main      ;Do not use main() because ......

             b     .

    ]

 

    [ THUMBCODE      ;for start-up code for Thumb mode

             orr   lr,pc,#1

             bx   lr

             CODE16

             bl    Main      ;Do not use main() because ......

             b     .

              CODE32

    ]

 

;初始化堆栈函数

;function initializing stacks

InitStacks

       ;Do not use DRAM,such as stmfd,ldmfd......

       ;Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'

;改变CPSR中M控制位，切换到相应的处理器模式下

;为各自模式下的SP赋值

       mrs r0,cpsr

       bic  r0,r0,#MODEMASK

       orr   r1,r0,#UNDEFMODE|NOINT

       msr cpsr_cxsf,r1         ;UndefMode

       ldr   sp,=UndefStack         ; UndefStack=0x33FF_5C00

 

       orr   r1,r0,#ABORTMODE|NOINT

       msr cpsr_cxsf,r1         ;AbortMode

       ldr   sp,=AbortStack          ; AbortStack=0x33FF_6000

 

       orr   r1,r0,#IRQMODE|NOINT

       msr cpsr_cxsf,r1         ;IRQMode

       ldr   sp,=IRQStack            ; IRQStack=0x33FF_7000

 

       orr   r1,r0,#FIQMODE|NOINT

       msr cpsr_cxsf,r1         ;FIQMode

       ldr   sp,=FIQStack             ; FIQStack=0x33FF_8000

 

       bic  r0,r0,#MODEMASK|NOINT

       orr   r1,r0,#SVCMODE

       msr cpsr_cxsf,r1         ;SVCMode

       ldr   sp,=SVCStack           ; SVCStack=0x33FF_5800

 

;系统模式和用户模式共用一个栈空间，因此不用再重复设置用户模式堆栈

;系统复位后进入的是SVC模式，而且各种模式下的lr不同，因此要想从该函数内返回，要首先切换到SVC模式，再使用lr，这样可以正确返回了

       mov       pc,lr

       ;The LR register will not be valid if the current mode is not SVC mode.

    

;定义一个数据缓冲池

       LTORG

 

;连续13个内存控制寄存器的定义空间

SMRDATA DATA

; Memory configuration should be optimized for best performance

; The following parameter is not optimized.

; Memory access cycle parameter strategy

; 1) The memory settings is  safe parameters even at HCLK=75Mhz.

; 2) SDRAM refresh period is for HCLK<=75Mhz.

 

       DCD (0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))

       DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))   ;GCS0

       DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))   ;GCS1

       DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))   ;GCS2

       DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))   ;GCS3

       DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))   ;GCS4

       DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))   ;GCS5

       DCD ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))    ;GCS6

       DCD ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))    ;GCS7

       DCD ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Tsrc<<18)+(Tchr<<16)+REFCNT)

 

       DCD 0x32       ;SCLK power saving mode, BANKSIZE 128M/128M

 

       DCD 0x30       ;MRSR6 CL=3clk

       DCD 0x30       ;MRSR7 CL=3clk

    

;运行域定义

BaseOfROM       DCD     |Image$$RO$$Base|

TopOfROM DCD     |Image$$RO$$Limit|

BaseOfBSS  DCD     |Image$$RW$$Base|

BaseOfZero DCD     |Image$$ZI$$Base|

EndOfBSS   DCD     |Image$$ZI$$Limit|

 

;重新使数据字对齐

       ALIGN

    

;Function for entering power down mode

; 1. SDRAM should be in self-refresh mode.

; 2. All interrupt should be maksked for SDRAM/DRAM self-refresh.

; 3. LCD controller should be disabled for SDRAM/DRAM self-refresh.

; 4. The I-cache may have to be turned on.

; 5. The location of the following code may have not to be changed.

 

;掉电模式函数

;在C语言中定义为：#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)

;void EnterPWDN(int clkcon);

EnterPWDN

       mov r2,r0             ;r0为该函数输入参数clkcon

       tst r0,#0x8           ;判断clkcon中的第3位，是否要切换到SLEEP模式

       bne ENTER_SLEEP ;切换到SLEEP模式

 

ENTER_STOP          ;IDLE模式

    ;设置SDRAM为自刷新方式

       ldr r0,=REFRESH

       ldr r3,[r0]             ;r3=rREFRESH

       mov r1, r3

       orr r1, r1, #BIT_SELFREFRESH

       str r1, [r0]            ;Enable SDRAM self-refresh

 

    ;等待一段时间

       mov r1,#16                 ;wait until self-refresh is issued. may not be needed.

0     subs r1,r1,#1

       bne %B0

 

       ldr r0,=CLKCON             

       str r2,[r0]                    ;置第2位，进入IDLE模式

 

    ;等待一段时间

       mov r1,#32

0     subs r1,r1,#1 ;1) wait until the STOP mode is in effect.

       bne %B0             ;2) Or wait here until the CPU&Peripherals will be turned-off

                     ;   Entering SLEEP mode, only the reset by wake-up is available.

 

    ;从IDLE模式下被唤醒，系统从该处继续执行

 

    ;取消SDRAM自刷新方式

       ldr r0,=REFRESH ;exit from SDRAM self refresh mode.

       str r3,[r0]

 

       MOV_PC_LR           ;返回，该语句为一个宏定义

 

ENTER_SLEEP        ;SLEEP模式

       ;NOTE.

       ;1) rGSTATUS3 should have the return address after wake-up from SLEEP mode.

 

    ;设置SDRAM为自刷新方式

       ldr r0,=REFRESH

       ldr r1,[r0]             ;r1=rREFRESH

       orr r1, r1, #BIT_SELFREFRESH

       str r1, [r0]            ;Enable SDRAM self-refresh

 

    ;等待一段时间

       mov r1,#16                 ;Wait until self-refresh is issued,which may not be needed.

0     subs r1,r1,#1

       bne %B0

 

    ;在进入SLEEP模式之前，配置必要的时钟和OFFREFRESH

       ldr   r1,=MISCCR

       ldr   r0,[r1]

       orr   r0,r0,#(7<<17)  ;Set SCLK0=0, SCLK1=0, SCKE=0.

       str   r0,[r1]

 

       ldr r0,=CLKCON             

       str r2,[r0]                    ;置第3位，进入SLEEP模式

 

       b .                 ;CPU will die here.

 

 

;从SLEEP模式下被唤醒函数

WAKEUP_SLEEP

       ;Release SCLKn after wake-up from the SLEEP mode.

    ;设置时钟和OFFREFRESH

       ldr   r1,=MISCCR

       ldr   r0,[r1]

       bic  r0,r0,#(7<<17)  ;SCLK0:0->SCLK, SCLK1:0->SCLK, SCKE:0->=SCKE.

       str   r0,[r1]

 

       ;Set memory control registers

    ;配置内存控制寄存器

      ldr   r0,=SMRDATA  ;be careful! 

       ldr   r1,=BWSCON    ;BWSCON Address

       add r2, r0, #52     ;End address of SMRDATA

0

       ldr   r3, [r0], #4

       str   r3, [r1], #4

       cmp       r2, r0

       bne %B0

 

    ;等待一段时间

       mov r1,#256

0     subs r1,r1,#1 ;1) wait until the SelfRefresh is released.

       bne %B0

 

    ;GSTATUS3存放着想要从SLEEP模式唤醒后的执行地址

       ldr r1,=GSTATUS3 ;GSTATUS3 has the start address just after SLEEP wake-up

       ldr r0,[r1]

 

       mov pc,r0            ;跳转到GSTATUS3存放的地址处

    

;=====================================================================

; Clock division test

; Assemble code, because VSYNC time is very short

;=====================================================================

       EXPORT CLKDIV124

       EXPORT CLKDIV144

    

CLKDIV124

    

       ldr     r0, = CLKDIVN

       ldr     r1, = 0x3             ; 0x3 = 1:2:4

       str     r1, [r0]

;      wait until clock is stable

       nop

       nop

       nop

       nop

       nop

 

       ldr     r0, = REFRESH

       ldr     r1, [r0]

       bic         r1, r1, #0xff

       bic         r1, r1, #(0x7<<8)

       orr          r1, r1, #0x470      ; REFCNT135

       str     r1, [r0]

       nop

       nop

       nop

       nop

       nop

       mov     pc, lr

 

CLKDIV144

       ldr     r0, = CLKDIVN

       ldr     r1, = 0x4             ; 0x4 = 1:4:4

       str     r1, [r0]

;      wait until clock is stable

       nop

       nop

       nop

       nop

       nop

 

       ldr     r0, = REFRESH

       ldr     r1, [r0]

       bic         r1, r1, #0xff

       bic         r1, r1, #(0x7<<8)

       orr          r1, r1, #0x630      ; REFCNT675 - 1520

       str     r1, [r0]

       nop

       nop

       nop

       nop

       nop

       mov     pc, lr

 

 

       ALIGN

 

       AREA RamData, DATA, READWRITE

 

;在0x33FF_FF00处定义中断向量表

;^是MAP的同义词，#是FIELD的同义词
ISR_STARTADDRESS             ; _ISR_STARTADDRESS=0x33FF_FF00

HandleReset      #   4

HandleUndef            #   4

HandleSWI         #   4

HandlePabort         #   4

HandleDabort        #   4

HandleReserved     #   4

HandleIRQ         #   4

HandleFIQ          #   4

 

;Do not use the label 'IntVectorTable',

;The value of IntVectorTable is different with the address you think it may be.

;IntVectorTable

;@0x33FF_FF20

HandleEINT0            #   4

HandleEINT1            #   4

HandleEINT2            #   4

HandleEINT3            #   4

HandleEINT4_7 #   4

HandleEINT8_23      #   4

HandleCAM              #   4           ; Added for 2440.

HandleBATFLT #   4

HandleTICK              #   4

HandleWDT              #   4

HandleTIMER0        #   4

HandleTIMER1        #   4

HandleTIMER2        #   4

HandleTIMER3        #   4

HandleTIMER4        #   4

HandleUART2        #   4

;@0x33FF_FF60

HandleLCD              #   4

HandleDMA0            #   4

HandleDMA1            #   4

HandleDMA2            #   4

HandleDMA3            #   4

HandleMMC              #   4

HandleSPI0        #   4

HandleUART1          #   4

HandleNFCON         #   4           ; Added for 2440.

HandleUSBD            #   4

HandleUSBH            #   4

HandleIIC           #   4

HandleUART0 #   4

HandleSPI1              #   4

HandleRTC               #   4

HandleADC              #   4

;@0x33FF_FFA0

       END                    ;程序结尾
