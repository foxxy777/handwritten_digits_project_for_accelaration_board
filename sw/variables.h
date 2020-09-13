/**********annotation**********
 * ↓第一次卷积的参数,MODE1=1:要padding，RELU_EN1=1:要relu
 * ********************/
//Conv1
#define    IN_WIDTH1 28
#define    IN_HEIGHT1 28
#define    IN_CH1 1
#define    KERNEL_WIDTH1 3
#define    KERNEL_HEIGHT1 3
#define    X_STRIDE1 1
#define    Y_STRIDE1 1
#define    RELU_EN1 1 //relu 就是把矩阵卷积得到的每个输出矩阵上的元素，负数变0，正数保留
#define    MODE1 1  //0:不要padding, 1 :要padding，padding长宽边上厚度为核的一半长宽 

#if MODE1 //用MODE1的值来定PADINGG的值
#define X_PADDING1 = ((KERNEL_WIDTH1 - 1) / 2)//这里等于1
#define Y_PADDING1 = ((KERNEL_HEIGHT1 - 1) / 2)
#else
#define X_PADDING1 = 0
#define Y_PADDING1 = 0
#endif 

#define    OUT_WIDTH1  ((IN_WIDTH1 + 2 * X_PADDING1 - KERNEL_WIDTH1) / X_STRIDE1 + 1) // 输出长宽是用输入大小，padding厚度，步长一起决定的//这里等于28
#define    OUT_HEIGHT1  ((IN_HEIGHT1 + 2 * Y_PADDING1 - KERNEL_HEIGHT1) / Y_STRIDE1 + 1)
#define    OUT_CH1  16

/**********annotation**********
 * ↓第一次池化的参数，用取max作为池化处理
 * ********************/
//Pool1
#define    MODE11  2  //mode: 0 : MEAN, 1 : MIN, 2 : MAX
#define    IN_WIDTH11  OUT_WIDTH1
#define    IN_HEIGHT11  OUT_HEIGHT1
#define    IN_CH11  OUT_CH1
#define    KERNEL_WIDTH11  2
#define    KERNEL_HEIGHT11  2
#define    OUT_CH11  IN_CH11//16
#define    OUT_WIDTH11  (IN_WIDTH11 / KERNEL_WIDTH11)//14            //16个28*28输入矩阵，取最大值池化后，输出为16个14*14的矩阵
#define    OUT_HEIGHT11  (IN_HEIGHT11 / KERNEL_HEIGHT11)
/**********annotation**********
 * ↓第二次卷积的参数，MODE2=1:要padding，RELU_EN2=1:要relu
 * ********************/
//Conv2
#define    IN_WIDTH2  OUT_WIDTH11
#define    IN_HEIGHT2  OUT_HEIGHT11
#define    IN_CH2  OUT_CH11//第二次卷积我拿进16个矩阵
#define    KERNEL_WIDTH2  3
#define    KERNEL_HEIGHT2  3
#define    X_STRIDE2  1
#define    Y_STRIDE2  1
#define    RELU_EN2  1
#define    MODE2  1

#if MODE2 
#define X_PADDING2 = ((KERNEL_WIDTH2 - 1) / 2)//这里等于1
#define Y_PADDING2 = ((KERNEL_HEIGHT2 - 1) / 2)
#else
#define X_PADDING2 = 0
#define Y_PADDING2 = 0
#endif 

#define    OUT_WIDTH2  ((IN_WIDTH2 + 2 * X_PADDING2 - KERNEL_WIDTH2) / X_STRIDE2 + 1)//这里还是等于14
#define    OUT_HEIGHT2  ((IN_HEIGHT2 + 2 * Y_PADDING2 - KERNEL_HEIGHT2) / Y_STRIDE2 + 1)
#define    OUT_CH2  32//第二次卷积输出32个矩阵

/**********annotation**********
 * ↓第二次池化的参数 ，用取max作为池化处理
 * ********************/
//Pool2
#define    MODE21  2  //mode: 0 : MEAN, 1 : MIN, 2 : MAX
#define    IN_WIDTH21  OUT_WIDTH2
#define    IN_HEIGHT21  OUT_HEIGHT2
#define    IN_CH21  OUT_CH2//32
#define    KERNEL_WIDTH21  2
#define    KERNEL_HEIGHT21  2
#define    OUT_CH21  IN_CH21//32
#define    OUT_WIDTH21  (IN_WIDTH21 / KERNEL_WIDTH21)//7            //一共16*32=512个14*14输入矩阵，第二次池化之后，得到512个7*7的矩阵
#define    OUT_HEIGHT21  (IN_HEIGHT21 / KERNEL_HEIGHT21)
/**********annotation**********
 * ↓ 第一次全连接的参数，全连接，很自然的，不用padding，要relu
 * ********************/
//Fc1
#define    IN_WIDTH3  OUT_WIDTH21//7
#define    IN_HEIGHT3  OUT_HEIGHT21
#define    IN_CH3  OUT_CH21//32
#define    KERNEL_WIDTH3  7
#define    KERNEL_HEIGHT3  7
#define    X_STRIDE3  1
#define    Y_STRIDE3  1
#define    RELU_EN3  1
#define    MODE3  0

#if MODE3 
#define X_PADDING4 = ((KERNEL_WIDTH4 - 1) / 2)
#define Y_PADDING4 = ((KERNEL_HEIGHT4 - 1) / 2)
#else
#define X_PADDING3 = 0
#define Y_PADDING3 = 0
#endif

#define    OUT_WIDTH3  ((IN_WIDTH3 + 2 * X_PADDING3 - KERNEL_WIDTH3) / X_STRIDE3 + 1)//1
#define    OUT_HEIGHT3  ((IN_HEIGHT3 + 2 * Y_PADDING3 - KERNEL_HEIGHT3) / Y_STRIDE3 + 1)
#define    OUT_CH3  128//

/**********annotation**********
 * ↓ 第二次全连接的参数，全连接，很自然的，不用padding，要relu
 * ********************/
//Fc2
#define     IN_WIDTH4  OUT_WIDTH3//1
#define     IN_HEIGHT4  OUT_HEIGHT3
#define     IN_CH4  OUT_CH3//128
#define     KERNEL_WIDTH4  1
#define     KERNEL_HEIGHT4  1
#define     X_STRIDE4  1
#define     Y_STRIDE4  1
#define     RELU_EN4  1
#define     MODE4  0

#if MODE4 
#define X_PADDING4 = ((KERNEL_WIDTH4 - 1) / 2)
#define Y_PADDING4 = ((KERNEL_HEIGHT4 - 1) / 2)
#else
#define X_PADDING4 = 0
#define Y_PADDING4 = 0
#endif 

#define     OUT_WIDTH4  ((IN_WIDTH4 + 2 * 0 - KERNEL_WIDTH4) / X_STRIDE4 + 1)   //  1
#define     OUT_HEIGHT4  ((IN_HEIGHT4 + 2 * 0 - KERNEL_HEIGHT4) / Y_STRIDE4 + 1)
#define     OUT_CH4  10

