;=========================================

; NAME: 2440INIT.S

; DESC: C start up codes

;       Configure memory, ISR ,stacks

;      Initialize C-variables

;=========================================
;GET������C���Ե�include��option.inc�ļ��ڶ�����һЩȫ�ֱ�����
;memcfg.inc�ļ��ڶ����˹����ڴ�bank�ķ��ź����ֳ�����
;2440addr.inc�ļ��ڶ��������ڻ���s3c2440�Ĵ��������͵�ַ

       GET option.inc
       GET memcfg.inc
       GET 2440addr.inc

;SDRAM��ˢ��λ���ѼĴ���REFRESH�ĵ�22λ����1

BIT_SELFREFRESH   EQU    (1<<22)

;CPSR�еĵ�5λ�����˴����������ֹ���ģʽ��Ϊ�Ժ��л�ģʽʱʹ��

;Pre-defined constants

USERMODE    EQU     0x10
FIQMODE      EQU                0x11
IRQMODE      EQU               0x12
SVCMODE     EQU      0x13
ABORTMODE   EQU        0x17
UNDEFMODE   EQU        0x1b
MODEMASK    EQU        0x1f

;CPSR�е�Iλ��Fλ��1����ʾ��ֹ�κ��ж�

NOINT       EQU      0xc0

;������7�ִ�����ģʽ�µ�ջ����ʼ��ַ�������û�ģʽ��ϵͳģʽ����һ��ջ�ռ�

;_STACK_BASEADDRESS��option.inc�ļ��ڶ��壬ֵΪ0x33ff8000

;The location of stacks

UserStack     EQU      (_STACK_BASEADDRESS-0x3800) ;0x33ff4800 ~
SVCStack    EQU      (_STACK_BASEADDRESS-0x2800) ;0x33ff5800 ~
UndefStack  EQU      (_STACK_BASEADDRESS-0x2400) ;0x33ff5c00 ~
AbortStack   EQU      (_STACK_BASEADDRESS-0x2000) ;0x33ff6000 ~
IRQStack     EQU      (_STACK_BASEADDRESS-0x1000) ;0x33ff7000 ~
FIQStack     EQU      (_STACK_BASEADDRESS-0x0)       ;0x33ff8000 ~

;ARM�����������ֹ���״̬��16λ��32λ
;�����������Ӧ����16λ��32λ���ֱ��뷽ʽ
;��ε�Ŀ����ͳһĿǰ�Ĵ���������״̬��������뷽ʽ

;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.

       GBLL    THUMBCODE                   ;����һ��ȫ���߼�����

       [ {CONFIG} = 16                                 ;if CONFIG == 16

THUMBCODE SETL  {TRUE}               ;THUMBCODE = TRUE

           CODE32                                        ;ָʾ������ΪARMָ��

             |                                                ;else

THUMBCODE SETL  {FALSE}                    ;THUMBCODE = FALSE

    ]

 

;�궨�壬�ں������MOV_PC_LRʱ�������ᱻ�Զ�չ��
;�ú�������������ӳ��򣬷��ر����ô�

        MACRO
       MOV_PC_LR

             [ THUMBCODE              ;if THUMBCODE == TRUE
           	bx lr
            	 |                                  ;else ��THUMBCODE == FALSE
          	 mov      pc,lr
             ]

       MEND
;�ú궨����������������أ���Z=1ʱ�������ӳ��򣬷��ر����ô�

             MACRO    
             MOVEQ_PC_LR

             [ THUMBCODE

       		 bxeq lr

             |

           moveq pc,lr

             ]

       MEND

 

;�ú궨���ǰ��жϷ��������׵�ַװ�ص�pc��

;�ں��浱����HandlerXXX HANDLER HandleXXXʱ���ú걻չ��

;ע�⣺HANDLERǰ�ķ���HandlerXXX�����ķ���HandleXXX����һ��r

;HandlerXXXΪARM��ϵ��ͳһ����ļ����쳣�ж�

;HandleXXXΪÿ��ARM���������Զ�����жϣ������ļ���󲿷ֵ��ж�������

             MACRO
$HandlerLabel HANDLER $HandleLabel

 

$HandlerLabel           

       sub  sp,sp,#4 ;ATPCS�涨����ջΪFD����

 ;��ջָ��ָ��ջ��Ԫ�أ�����ջ���ڴ��ַ��С�ķ�������

 ;�������ʹջ��ַ��С4���ֽڣ��������ռ�װ���жϷ������׵�ַ

       stmfd     sp!,{r0} ;����Ҫ����r0�Ĵ������������ݣ�����Ҫ����r0���ݣ�ʹ����ջ

       ldr     r0,=$HandleLabel     ;��HandleXXX�ĵ�ַװ��r0

       ldr     r0,[r0]    ;װ���жϷ���������ʼ��ַ

       str     r0,[sp,#4]               ;�жϺ����׵�ַ��ջ

       ldmfd   sp!,{r0,pc} ;�����ȱ����r0���ݺ��жϺ����׵�ַ��ջ

                      ;��ʹϵͳ��ת����Ӧ���жϴ�����

       MEND

 

;�������������ȶ���õ��������������α���

;ARM�Ŀ�ִ��ӳ���ļ���RO��RW��ZI���������

;ROΪ����Σ�RWΪ�ѳ�ʼ����ȫ�ֱ�����ZIΪδ��ʼ����ȫ�ֱ���

       IMPORT  |Image$$RO$$Base|    ;RO����ʼ��ַ

       IMPORT  |Image$$RO$$Limit|   ;RO�ν�����ַ��1������RW����ʼ��ַ

       IMPORT  |Image$$RW$$Base|   ;RW����ʼ��ַ

       IMPORT  |Image$$ZI$$Base|      ;ZI����ʼ��ַ

       IMPORT  |Image$$ZI$$Limit|     ;ZI�ν�����ַ��1

 

;������������MMU�ĺ�������������ʱ��ģʽΪ�첽ģʽ�Ϳ�������ģʽ

       IMPORT      MMU_SetAsyncBusMode

       IMPORT      MMU_SetFastBusMode  ;

 

;����Main����ΪC���Գ�����ں���

       IMPORT  Main        ; The main entry of mon program

;�������Σ���ΪInit

       AREA    Init,CODE,READONLY

 

;����ڴ���0x0����ʼ��8���ֵ�Ԫ�ռ��ڣ���ŵ���ARM�쳣�ж�������ÿ���ֵ�Ԫ�ռ䶼��һ����תָ����쳣����ʱ��ARM���Զ���ת����Ӧ���ж������������ɸô�����תָ������ת����Ӧ��ִ�к�����

       ENTRY        ;������ڴ�

       EXPORT     __ENTRY           ;����__ENTRY���������������ڵ�ַ

__ENTRY                                ;��Ҫ����MMU

ResetEntry

       ;1)The code, which converts to Big-endian, should be in little endian code.

       ;2)The following little endian code will be compiled in Big-Endian mode.

       ;  The code byte order should be changed as the memory bus width.

       ;3)The pseudo instruction,DCD can not be used here because the linker generates error.

;��0x0�����쳣�ж��Ǹ�λ�쳣�жϣ����ϵ��ִ�еĵ�һ��ָ��

;����ENDIAN_CHANGE���ڱ���Ƿ�Ҫ��С��ģʽ�ı�Ϊ���ģʽ����Ϊ��������ʼģʽ��С��ģʽ�����Ҫ�ô��ģʽ����Ҫ���ȰѸñ�������ΪTRUE������ΪFLASE

;����ENTRY_BUS_WIDTH�����������ߵĿ�ȣ���Ϊ��16λ��8λ�������ʾ32λ����ʱ���ڴ��ģʽ�£����ݵĺ����ǲ�ͬ��

;����Ҫ���ǵ���˺�С��ģʽ���Լ����ߵĿ�ȣ���˸ô����ƽϸ��ӣ���ʵֻ��һ����תָ���Ϊ���ģʽʱ����ת��ChangeBigEndian��������������ת��ResetHandler������

       ASSERT      :DEF:ENDIAN_CHANGE    ;�ж��Ƿ�����ENDIAN_CHANGE

                                ;���û�ж��壬�򱨸�ô�������Ϣ

       [ ENDIAN_CHANGE                   ;if ENDIAN_CHANGE ==TRUE

              ASSERT  :DEF:ENTRY_BUS_WIDTH  ;�ж��Ƿ�����ENTRY_BUS_WIDTH

                                       ;���û�ж��壬�򱨸�ô�������Ϣ

 

              [ ENTRY_BUS_WIDTH=32         ;if ENTRY_BUS_WIDTH ==32

;��ת��ChangeBigEndian��ChangeBigEndian��0x24������˸���ָ��Ļ�����Ϊ0xea000007

;���Ը�������ڸô�����0x0����ֱ�ӷ���0xea000007���ݣ���DCD 0xea000007��������ͬ

                     b     ChangeBigEndian      

              ]

 

              [ ENTRY_BUS_WIDTH=16         ;if ENTRY_BUS_WIDTH ==16

;��С��ģʽ�£���16λ��8λ�������߿�ȱ�ʾ32λ���ݣ�����32λ���߿�ȱ�ʾ32λ���ݣ���ʽ��ȫһ�¡����ڴ��ģʽ�£���ʽ�ͻᷢ���仯

;�ڸ�λʱ��ϵͳĬ�ϵ���С��ģʽ�����Ծ�Ҫ��Ϊ�ظı����ݸ�ʽ��ʹ����16λ������ݱ�ʾ��32λ����Ҳ�ܱ�С��ģʽ��ϵͳʶ��

;������Ŀ��Ҳ����ת��ChangeBigEndian����������ҲӦ����0xea000007����Ϊ����С��ģʽϵͳʶ�𣬾�Ҫ�ѻ������˳����һ�µ�������Ϊ0x0007ea00����ô���ǾͿ�����DCD 0x0007ea00�ѻ�����װ�ؽ�ȥ�ˣ������ڸô�����ʹ��DCDαָ�������Ǿ�Ҫ��һ����ʵ��ָ��������DCD 0x0007ea00������ָ������Ļ�����ҲΪ0x0007ea00����andeq  r14,r7,r0,lsl #20����һ������������Ϊ0x0007ea00��ָ����������ڸô�д�ϸ���ָ��

                     andeq    r14,r7,r0,lsl #20   ;DCD 0x0007ea00

              ]

 

              [ ENTRY_BUS_WIDTH=8           ;if ENTRY_BUS_WIDTH ==8

;�����ķ�������һ�δ���ķ�������

;streq  r0,[r0,-r10,ror #1]�����Ļ�����Ϊ0x070000ea

                     streq      r0,[r0,-r10,ror #1] ;DCD 0x070000ea

              ]

              |             ;else ��ENDIAN_CHANGE ==FALSE

              b     ResetHandler             ;��ת��ResetHandler������λ

       ]

       b     HandlerUndef     ;δ����

       b     HandlerSWI ;����ж�

       b     HandlerPabort     ;ָ��Ԥȡ��ֹ

       b     HandlerDabort    ;���ݷ�����ֹ

       b     .             ;��������ת�������ַ������������ѭ��

       b     HandlerIRQ ;�ⲿ�ж�����

       b     HandlerFIQ ;�����ж�����

;����Ϊ�쳣�ж�������

 

;��ת��EnterPWDN�������Դ���������������ģʽ����C���Գ�����б�����

;�ô���ַΪ0x20������ΪʲôҪ�ڸô�ִ�У�����Ϊ�����Ǹô����쳣�ж������������

       b     EnterPWDN       ; Must be @0x20.

 

;��0x0��ת���ˣ�Ŀ���ǰ�С��ģʽ��Ϊ���ģʽ������CP15�еļĴ���C1�еĵ�7λ��1

ChangeBigEndian

;@0x24

       [ ENTRY_BUS_WIDTH=32         ;if ENTRY_BUS_WIDTH == 32

       ;ִ��mrc p15,0,r0,c1,c0,0���õ�CP15�еļĴ���C1������r0��

       ;����mrc p15,0,r0,c1,c0,0�Ļ�����Ϊ0xee110f10

       ;���DCD      0xee110f10����˼����mrc p15,0,r0,c1,c0,0����ͬ

              DCD     0xee110f10  ;0xee110f10 => mrc p15,0,r0,c1,c0,0

       ;ִ��orr r0,r0,#0x80����r0�еĵ�7λΪ1����ʾѡ����ģʽ

              DCD     0xe3800080 ;0xe3800080 => orr r0,r0,#0x80;  //Big-endian

       ;ִ��mcr p15,0,r0,c1,c0,0����r0д��CP15�еļĴ���C1

              DCD     0xee010f10  ;0xee010f10 => mcr p15,0,r0,c1,c0,0

       ]

       [ ENTRY_BUS_WIDTH=16         ;if ENTRY_BUS_WIDTH == 16

       ;���ڴ�ʱϵͳ������ʶ��16λ��8λ���ģʽ�±�ʾ��32Ϊ����

       ;��˻�����Ϊ�ؽ������ݵ���������0xee110f10��Ϊ0x0f10ee11

       ;Ȼ����DCDָ���������ݡ���ͬ

              DCD 0x0f10ee11

              DCD 0x0080e380

              DCD 0x0f10ee01

       ]

       [ ENTRY_BUS_WIDTH=8           ;if ENTRY_BUS_WIDTH == 8

              DCD 0x100f11ee

              DCD 0x800080e3

              DCD 0x100f01ee

       ]

    ;�൱��NOPָ��

    ;�����ǵȴ�ϵͳ��С��ģʽ����ģʽת��

    ;�˺�ϵͳ���ܹ��Զ�ʶ�����ͬ���߿���µĴ��ģʽ������Ժ����������Ϊ����ָ����

       DCD 0xffffffff  ;swinv 0xffffff is similar with NOP and run well in both endian mode.

       DCD 0xffffffff

       DCD 0xffffffff

       DCD 0xffffffff

       DCD 0xffffffff

       b ResetHandler       ;��ת��ResetHandler

    

;��ϵͳ�����쳣�жϺ��ɴ����0x0~0x1C�����ж�������ַ�е���תָ���ת���˴���Ӧ��λ�ã�
;�������ȶ���õĺ궨���ٴ���ת����Ӧ���жϷ��������

HandlerFIQ HANDLER HandleFIQ

HandlerIRQ HANDLER HandleIRQ

HandlerUndef     HANDLER HandleUndef

HandlerSWI HANDLER HandleSWI

HandlerDabort    HANDLER HandleDabort

HandlerPabort     HANDLER HandlePabort

 

;������δ��������ڴ���������жϣ���������������жϵ��׷����������жϣ�Ȼ����ת����Ӧ���жϷ��������

;�����˵���ǣ��������ж�ʱ������INTOFFSET�Ĵ�����Ӧ��λΪ1��Ȼ��ͨ��������ó���ĩ�˲��ֵ��ж����������ҵ����Ӧ���ж���ڵ�ַ

;�۲��ж��������ᷢ������INTOFFSET�Ĵ����е��ж�Դ�������Ӧ�����������˳����INTOFFSET�Ĵ����е��ж�Դ��
;��С�����˳��һ�£�������ǿ����û�ַ�ӱ�ַ�ķ�ʽ�������ҵ����Ӧ���ж���ڵ�ַ�����л�ַΪ��������׸��ж�Դ��ַ��
;��ַΪINTOFFSET�Ĵ�����ֵ����4����Ϊϵͳ����4���ֽڵ�Ԫ�����һ���ж�������

IsrIRQ
       sub  sp,sp,#4        ;��ջ������4���ֽڿռ䣬�Ա㱣���ж���ڵ�ַ
       stmfd     sp!,{r8-r9}    ;����Ҫ�õ�r8��r9����˱����������Ĵ����ڵ�ֵ

       ldr   r9,=INTOFFSET       ;��INTOFFSET�Ĵ�����ַװ��r9��
       ldr   r9,[r9]           ;��ȡINTOFFSET�Ĵ�������
       ldr   r8,=HandleEINT0      ;�õ��ж�������Ļ�ַ
       add r8,r8,r9,lsl #2              ;�û�ַ�ӱ�ַ�ķ�ʽ�õ��ж�������ĵ�ַ
       ldr   r8,[r8]                  ;�õ��жϷ��������ڵ�ַ
       str   r8,[sp,#8]                    ;ʹ�жϷ��������ڵ�ַ��ջ
       ldmfd    sp!,{r8-r9,pc}      ;ʹr8��r9����ڵ�ַ��ջ���������жϷ��������

;����һ�����ݻ���أ���ldrαָ��ʹ��

       LTORG

 

;=======

; ENTRY

;=======

;ϵͳ�ϵ��λ����0x0������תָ���ת���ô���ʼ����ִ��ϵͳ�ĳ�ʼ������

ResetHandler

    ;��ϵͳ��ʼ�������У�����Ҫ���Ź�����˹رտ��Ź�����

       ldr   r0,=WTCON       ;watch dog disable

       ldr   r1,=0x0

       str   r1,[r0]

 

    ;ͬ������ʱҲ��Ӧ����Ӧ�κ��жϣ�������������жϣ��Լ����ж�

       ldr   r0,=INTMSK

       ldr   r1,=0xffffffff  ;all interrupt disable

       str   r1,[r0]

 

       ldr   r0,=INTSUBMSK

       ldr   r1,=0x7fff           ;all sub interrupt disable

       str   r1,[r0]

 

;���������ļ����޷�����ģ����Ϊ���жϸ��ļ���������ȷ�����������Ҫ�۲�ĵط�����һ�ε���LED�ĳ���
;�����Ϳ���֪�������Ƿ��Ѿ�ִ�е��˴�

;���淽�����ڵĳ�����ǵ���LED��С����

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

 
;���г�������������ϵͳʱ��Ƶ��

;����PLL������ʱ�䳣�����Եõ�һ��ʱ�����ʱ

       ;To reduce PLL lock time, adjust the LOCKTIME register.

       ldr   r0,=LOCKTIME

       ldr   r1,=0xffffff

       str   r1,[r0]

 

       [ PLL_ON_START

              ; Added for confirm clock divide. for 2440.

              ; Setting value Fclk:Hclk:Pclk

       ;����ϵͳ������ʱ��Ƶ��FCLK��HCLK��PCLK

              ldr   r0,=CLKDIVN

              ldr   r1,=CLKDIV_VAL         ; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.

              str   r1,[r0]

 

       ;program has not been copied, so use these directly

              [ CLKDIV_VAL>1               ;if FCLK��HCLK��1��1

           ;����ʱ��ģʽΪ�첽ģʽ

                     mrc p15,0,r0,c1,c0,0

                     orr r0,r0,#0xc0000000;R1_nF:OR:R1_iA

                     mcr p15,0,r0,c1,c0,0

                     |                                  ;else

           ;����ʱ��ģʽΪ��������ģʽ

                     mrc p15,0,r0,c1,c0,0

                     bic r0,r0,#0xc0000000;R1_iA:OR:R1_nF

                     mcr p15,0,r0,c1,c0,0

              ]

 

       ;����UPLL

       ;�����ֲ��еļ��㹫ʽ��ȷ��MDIV��PDIV��SDIV

       ;�õ���ϵͳ����ʱ��Ƶ��Ϊ12MHz������£�UCLK���Ƶ��Ϊ48MHz

              ;Configure UPLL

              ldr   r0,=UPLLCON

              ldr   r1,=((U_MDIV<<12)+(U_PDIV<<4)+U_SDIV)                          ;Fin = 12.0MHz, UCLK = 48MHz

              str   r1,[r0]

       ;�ȴ�����7��ʱ�����ڣ��Ա�֤ϵͳ����ȷ����

              nop ; Caution: After UPLL setting, at least 7-clocks delay must be inserted for setting hardware be completed.

              nop

              nop

              nop

              nop

              nop

              nop

       ;����MPLL��ͬUPLL

              ;Configure MPLL

              ldr   r0,=MPLLCON

              ldr   r1,=((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV)                         
              
              ;Fin = 12.0MHz, FCLK = 400MHz

              str   r1,[r0]

       ]

 

    ;��SLEEPģʽ�±����ѣ�������RESET���ű������������ҲҪ��0x0����ʼִ��

    ;�ڴ˴�Ҫ�ж��Ƿ�����SLEEPģʽ��������ĸ�λ

       ;Check if the boot is caused by the wake-up from SLEEP mode.

       ldr   r1,=GSTATUS2

       ldr   r0,[r1]

       tst   r0,#0x2         ;���GSTATUS2�Ĵ����ĵ�1λ

       ;In case of the wake-up from SLEEP mode, go to SLEEP_WAKEUP handler.

       bne WAKEUP_SLEEP           ;�Ǳ����ѵģ�����ת

 

;����һ�������Ѹ�λ�����ʼ���ַ��ţ����԰������浽GSTATUS3��

;�����õ�ַ��ţ��Ա���C���Գ�����ʹ��

       EXPORT StartPointAfterSleepWakeUp

StartPointAfterSleepWakeUp

 

;�����ڴ���ƼĴ���

;�����ڴ���ƼĴ���һ������BWSCONΪ��ʼ���������õ�13���Ĵ���������Ҫһ�������������13���Ĵ���������

;��˿���һ����SMRDATAΪ��ַ��ʼ���13���ֵ�Ԫ�ռ䣬��˳�����Ҫд���13���Ĵ�������

       ;Set memory control registers

      ;ldr  r0,=SMRDATA

      adrl r0, SMRDATA   ;�õ�SMRDATA�ռ���׵�ַ

       ldr   r1,=BWSCON           ;�õ�BWSCON�ĵ�ַ

       add r2, r0, #52            ;�õ�SMRDATA�ռ��ĩ��ַ

 

;���13�������ݵĸ���

0

       ldr   r3, [r0], #4

       str   r3, [r1], #4

       cmp       r2, r0

       bne %B0

 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;       When EINT0 is pressed,  Clear SDRAM 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; check if EIN0 button is pressed

;���EIN0��ť�Ƿ񱻰���

       ldr   r0,=GPFCON

       ldr   r1,=0x0

       str   r1,[r0]           ;GPFCON=0��F��Ϊ����

       ldr   r0,=GPFUP

       ldr   r1,=0xff

       str   r1,[r0]           ;GPFUP=0xff������������Ч

 

       ldr   r1,=GPFDAT

       ldr   r0,[r1]           ;��ȡF������

       bic  r0,r0,#(0x1e<<1)        ;��������1λ���ݣ�������0

       tst   r0,#0x1                ;�жϵ�1λ

       bne %F1                     ;��Ϊ0��ʾ��ťû�б����£�����ǰ��ת����ִ�����SDRAM

    

    

 

; Clear SDRAM Start

;���SDRAM

       ldr   r0,=GPFCON

       ldr   r1,=0x55aa

       str   r1,[r0]           ;GPF7~4Ϊ�����GPF3~0Ϊ�ж�

;      ldr   r0,=GPFUP

;      ldr   r1,=0xff 

;      str   r1,[r0]          ;����������Ч

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

       ldr   r0,=0x30000000         ;RAM�׵�ַ

;���64MB��RAM

0     

       stmia      r0!,{r1-r8}

       subs       r9,r9,#32 

       bne %B0

;Clear SDRAM End

 

1

;��ʼ�����ִ�����ģʽ�µĶ�ջ

       ;Initialize stacks

       bl    InitStacks                   ;��ת��InitStacks

 

;===========================================================

    

;����Ĵ���Ϊ��ROM�е����ݸ��Ƶ�RAM��

       ldr   r0, =BWSCON

       ldr   r0, [r0]

       ands       r0, r0, #6              ;��ȡOM[1:0]����״̬

 ;Ϊ0��ʾ��NAND Flash��������Ϊ0���NOR Flash����

       bne copy_proc_beg    ;��ת�����ö�ȡNAND Flash

       adr  r0, ResetEntry            ;OM[1:0] == 0����NAND Flash����

       cmp       r0, #0                          ;if use Multi-ice, 

       bne copy_proc_beg           ;do not read nand flash for boot

       ;nop

;===========================================================

nand_boot_beg

       [ {TRUE}
;bl RdNF2SDRAM           ;����NAND Flash��SDRAM

       ]
       ldr   pc, =copy_proc_beg

;===========================================================

copy_proc_beg

       adr  r0, ResetEntry

       ldr   r2, BaseOfROM

       cmp       r0, r2             ;�Ƚϳ�����ڵ�ַ�������������RO����ַ

       ldreq      r0, TopOfROM   ;�����ȣ���ROβ��ַ��ȡ��r0��

       beq InitRam        ;�����ȣ�����ת

       ldr r3, TopOfROM                  ;���򣬰�ROβ��ַ��ȡ��r3��

;����ѭ����Ϊ�ڳ�����ڵ�ַ�������������RO����ַ����ȵ�����£��ѳ����Ƶ�RAM��

0     

       ldmia     r0!, {r4-r7}

       stmia      r2!, {r4-r7}

       cmp       r2, r3

       bcc  %B0

;�������ֶ�������

       sub  r2, r2, r3

       sub  r0, r0, r2                            

       

InitRam 

       ldr   r2, BaseOfBSS

       ldr   r3, BaseOfZero   

;����ѭ����Ϊ�����ѳ�ʼ����ȫ�ֱ���

0

       cmp       r2, r3

       ldrcc      r1, [r0], #4

       strcc       r1, [r2], #4

       bcc  %B0      

 

;����ѭ������Ϊδ��ʼ����ȫ�ֱ�����ֵΪ0

       mov       r0,   #0

       ldr   r3,   EndOfBSS

1     

       cmp       r2,   r3

       strcc       r0, [r2], #4

       bcc  %B1

    

       ldr   pc, =%F2             ;goto compiler address

2

    

;      [ CLKDIV_VAL>1               ;if FCLK��HCLK��1��1

;      bl    MMU_SetAsyncBusMode      ;����ʱ��ģʽΪ�첽ģʽ

;      |

;      bl MMU_SetFastBusMode     ;����ʱ��ģʽΪ��������ģʽ

;      ]

    

 

;===========================================================

;��ͨ�жϴ���

;����ͨ�жϷ���ʱ��ִ�е���IsrIRQ

     ; Setup IRQ handler

       ldr   r0,=HandleIRQ   ;This routine is needed

       ldr   r1,=IsrIRQ   ;if there is not 'subs pc,lr,#4' at 0x18, 0x1c

       str   r1,[r0]

 

 

;���������ĳ�ʼ��������ת����C����׫д��Main()�����ڼ���ִ����������

;���ﲻ��дmain����Ϊд��main��ϵͳ���Զ�Ϊ�������һЩ��ʼ������������Щ��������γ���������������ʽ����Ϊ��ɵġ�

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

 

;��ʼ����ջ����

;function initializing stacks

InitStacks

       ;Do not use DRAM,such as stmfd,ldmfd......

       ;Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'

;�ı�CPSR��M����λ���л�����Ӧ�Ĵ�����ģʽ��

;Ϊ����ģʽ�µ�SP��ֵ

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

 

;ϵͳģʽ���û�ģʽ����һ��ջ�ռ䣬��˲������ظ������û�ģʽ��ջ

;ϵͳ��λ��������SVCģʽ�����Ҹ���ģʽ�µ�lr��ͬ�����Ҫ��Ӹú����ڷ��أ�Ҫ�����л���SVCģʽ����ʹ��lr������������ȷ������

       mov       pc,lr

       ;The LR register will not be valid if the current mode is not SVC mode.

    

;����һ�����ݻ����

       LTORG

 

;����13���ڴ���ƼĴ����Ķ���ռ�

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

    

;��������

BaseOfROM       DCD     |Image$$RO$$Base|

TopOfROM DCD     |Image$$RO$$Limit|

BaseOfBSS  DCD     |Image$$RW$$Base|

BaseOfZero DCD     |Image$$ZI$$Base|

EndOfBSS   DCD     |Image$$ZI$$Limit|

 

;����ʹ�����ֶ���

       ALIGN

    

;Function for entering power down mode

; 1. SDRAM should be in self-refresh mode.

; 2. All interrupt should be maksked for SDRAM/DRAM self-refresh.

; 3. LCD controller should be disabled for SDRAM/DRAM self-refresh.

; 4. The I-cache may have to be turned on.

; 5. The location of the following code may have not to be changed.

 

;����ģʽ����

;��C�����ж���Ϊ��#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)

;void EnterPWDN(int clkcon);

EnterPWDN

       mov r2,r0             ;r0Ϊ�ú����������clkcon

       tst r0,#0x8           ;�ж�clkcon�еĵ�3λ���Ƿ�Ҫ�л���SLEEPģʽ

       bne ENTER_SLEEP ;�л���SLEEPģʽ

 

ENTER_STOP          ;IDLEģʽ

    ;����SDRAMΪ��ˢ�·�ʽ

       ldr r0,=REFRESH

       ldr r3,[r0]             ;r3=rREFRESH

       mov r1, r3

       orr r1, r1, #BIT_SELFREFRESH

       str r1, [r0]            ;Enable SDRAM self-refresh

 

    ;�ȴ�һ��ʱ��

       mov r1,#16                 ;wait until self-refresh is issued. may not be needed.

0     subs r1,r1,#1

       bne %B0

 

       ldr r0,=CLKCON             

       str r2,[r0]                    ;�õ�2λ������IDLEģʽ

 

    ;�ȴ�һ��ʱ��

       mov r1,#32

0     subs r1,r1,#1 ;1) wait until the STOP mode is in effect.

       bne %B0             ;2) Or wait here until the CPU&Peripherals will be turned-off

                     ;   Entering SLEEP mode, only the reset by wake-up is available.

 

    ;��IDLEģʽ�±����ѣ�ϵͳ�Ӹô�����ִ��

 

    ;ȡ��SDRAM��ˢ�·�ʽ

       ldr r0,=REFRESH ;exit from SDRAM self refresh mode.

       str r3,[r0]

 

       MOV_PC_LR           ;���أ������Ϊһ���궨��

 

ENTER_SLEEP        ;SLEEPģʽ

       ;NOTE.

       ;1) rGSTATUS3 should have the return address after wake-up from SLEEP mode.

 

    ;����SDRAMΪ��ˢ�·�ʽ

       ldr r0,=REFRESH

       ldr r1,[r0]             ;r1=rREFRESH

       orr r1, r1, #BIT_SELFREFRESH

       str r1, [r0]            ;Enable SDRAM self-refresh

 

    ;�ȴ�һ��ʱ��

       mov r1,#16                 ;Wait until self-refresh is issued,which may not be needed.

0     subs r1,r1,#1

       bne %B0

 

    ;�ڽ���SLEEPģʽ֮ǰ�����ñ�Ҫ��ʱ�Ӻ�OFFREFRESH

       ldr   r1,=MISCCR

       ldr   r0,[r1]

       orr   r0,r0,#(7<<17)  ;Set SCLK0=0, SCLK1=0, SCKE=0.

       str   r0,[r1]

 

       ldr r0,=CLKCON             

       str r2,[r0]                    ;�õ�3λ������SLEEPģʽ

 

       b .                 ;CPU will die here.

 

 

;��SLEEPģʽ�±����Ѻ���

WAKEUP_SLEEP

       ;Release SCLKn after wake-up from the SLEEP mode.

    ;����ʱ�Ӻ�OFFREFRESH

       ldr   r1,=MISCCR

       ldr   r0,[r1]

       bic  r0,r0,#(7<<17)  ;SCLK0:0->SCLK, SCLK1:0->SCLK, SCKE:0->=SCKE.

       str   r0,[r1]

 

       ;Set memory control registers

    ;�����ڴ���ƼĴ���

      ldr   r0,=SMRDATA  ;be careful! 

       ldr   r1,=BWSCON    ;BWSCON Address

       add r2, r0, #52     ;End address of SMRDATA

0

       ldr   r3, [r0], #4

       str   r3, [r1], #4

       cmp       r2, r0

       bne %B0

 

    ;�ȴ�һ��ʱ��

       mov r1,#256

0     subs r1,r1,#1 ;1) wait until the SelfRefresh is released.

       bne %B0

 

    ;GSTATUS3�������Ҫ��SLEEPģʽ���Ѻ��ִ�е�ַ

       ldr r1,=GSTATUS3 ;GSTATUS3 has the start address just after SLEEP wake-up

       ldr r0,[r1]

 

       mov pc,r0            ;��ת��GSTATUS3��ŵĵ�ַ��

    

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

 

;��0x33FF_FF00�������ж�������

;^��MAP��ͬ��ʣ�#��FIELD��ͬ���
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

       END                    ;�����β
