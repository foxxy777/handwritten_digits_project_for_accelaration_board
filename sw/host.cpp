#include "figure_function.h"

int main(int argc, char* argv[]) {
	cout<<"step1:setting address of xclbin"<<endl<<endl;
    ///**********annotation**********
    // * ↓ 通过argv把xo文件的xclbin传进来
    // * ********************/
    //if (argc != 2) {
    //    std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
    //    return EXIT_FAILURE;
    //}
    /**********annotation**********
    * ↓ 通过argv把xo文件的xclbin传进来
    * ********************/
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];//传入卷积kernel的地址
    std::string binaryFile_2 = argv[2];//传入池化kernel的地址
    cout<<argv[0]<<endl;//地址    
    cout<<argv[1]<<endl;//第一个xclbin
    cout<<argv[2]<<endl;//第二个xclbin
    cl_int err;

    cout<<"step2:read in binary files"<<endl<<endl;
    /**********annotation**********
     * ↓分配内存并读图像
     * ********************/
    //sizeoffile
    size_t img_total_size = sizeoffile("./1.bmp");//1862
    size_t img_size = img_total_size - BMP_OFFSET;//1862-1078=784=28*28
    cout<<"img_size"<<img_size<<endl;
    //分配784个byte内存
    std::vector<char,aligned_allocator<char>> din_img(img_size);//以4096整数倍为边界，分配img_size(1862)*8bit 那么多的内存,格式:vector<数组每个元素的格式，分配器(用于指定内存分配方法)>
    cout<<"allocation of memory finished"<<endl;
    //readbmpfile读图像数据,放入vector img_in中
    //uint8_t* ptr_buffer = readbmpfile("/home/xy/vitis_hls_prjs/figure_project/figure_project/Emulation-HW/src/1.bmp");
    uint8_t* ptr_buffer = readbmpfile("./1.bmp");
    cout<<"readbmpfile finished"<<endl;
    //灰度值取反
    reverse_gray_figure(ptr_buffer,img_size);
    //赋值进host上的memory:vector
    cout<<"reverse_gray_figure finished"<<endl;

    for(size_t i = 0 ; i < img_size ; i++){
        din_img[i] = ptr_buffer[i];
    }
    cout<<"setting din_img finished"<<endl<<endl;

    delete[] ptr_buffer;

    /**********annotation**********
     * ↓  分配内存并读第一套卷积核
     * ********************/
    size_t size_w_conv1_byte = sizeoffile("./W_conv1.bin");
    size_t size_w_conv1_float = size_w_conv1_byte/4;
    std::vector<float,aligned_allocator<float>> din_w_conv1(size_w_conv1_float);
    cout<<"allocation of memory finished"<<endl;

    float* ptr_buffer_w_conv1 = readfilterfile_to_float("./W_conv1.bin",IN_HEIGHT1,IN_WIDTH1,IN_CH1,OUT_CH1);
    //把数据从我们的读取函数取出来，放到vector里面
    cout<<"readfilterfile_to_float (W_conv1.bin) finished"<<endl;
    for(size_t i = 0 ; i < size_w_conv1_float ; i++){
        din_w_conv1[i] = ptr_buffer_w_conv1[i];
    }
    cout<<"setting din_w_conv1 finished"<<endl;

    delete[] ptr_buffer_w_conv1;

    /**********annotation**********
     * ↓ 分配内存并读第一套bias
     * ********************/
    size_t size_b_conv1_byte = sizeoffile("./b_conv1.bin");
    size_t size_b_conv1_float = size_b_conv1_byte/4;
    std::vector<float,aligned_allocator<float>> din_b_conv1(size_b_conv1_float);

    float* ptr_buffer_b_conv1 = readbiasfile_to_float("./b_conv1.bin",OUT_CH1);
    cout<<"readbiasfile_to_float (b_conv1.bin) finished"<<endl;
    for(size_t i = 0 ; i < size_b_conv1_float ; i++){
        din_b_conv1[i] = ptr_buffer_b_conv1[i];
    }
    cout<<"setting din_b_conv1 finished"<<endl;

    delete[] ptr_buffer_b_conv1;

    /**********annotation**********
    * ↓  分配内存并读第二套卷积核
    * ********************/
    size_t size_w_conv2_byte = sizeoffile("./W_conv2.bin");
    size_t size_w_conv2_float = size_w_conv2_byte / 4;
    std::vector<float, aligned_allocator<float>> din_w_conv2(size_w_conv2_float);
    cout << "allocation of memory finished" << endl;

    float* ptr_buffer_w_conv2 = readfilterfile_to_float("./W_conv2.bin", IN_HEIGHT2, IN_WIDTH2, IN_CH2, OUT_CH2);
    //把数据从我们的读取函数取出来，放到vector里面
    cout << "readfilterfile_to_float (W_conv2.bin) finished" << endl;
    for (size_t i = 0; i < size_w_conv2_float; i++) {
        din_w_conv1[2] = ptr_buffer_w_conv2[i];
    }
    cout << "setting din_w_conv1 finished" << endl;

    delete[] ptr_buffer_w_conv2;

    /**********annotation**********
 * ↓ 分配内存并读第二套bias
 * ********************/
    size_t size_b_conv2_byte = sizeoffile("./b_conv2.bin");
    size_t size_b_conv2_float = size_b_conv2_byte / 4;
    std::vector<float, aligned_allocator<float>> din_b_conv2(size_b_conv2_float);

    float* ptr_buffer_b_conv2 = readbiasfile_to_float("./b_conv2.bin", OUT_CH2);
    cout << "readbiasfile_to_float (b_conv1.bin) finished" << endl;
    for (size_t i = 0; i < size_b_conv2_float; i++) {
        din_b_conv2[i] = ptr_buffer_b_conv2[i];
    }
    cout << "setting din_b_conv2 finished" << endl;

    delete[] ptr_buffer_b_conv2;
    /**********annotation**********
     * ↓ 分配内存并读第一套fc(全连接)核
     * ********************/
    size_t size_w_fc1_byte = sizeoffile("./W_fc1.bin");
    size_t size_w_fc1_float = size_w_fc1_byte / 4;
    std::vector<float, aligned_allocator<float>> din_w_fc1(size_w_fc1_float);
    cout << "allocation of memory finished" << endl;

    float* ptr_buffer_w_fc1 = readfilterfile_to_float("./W_fc1.bin", IN_HEIGHT3, IN_WIDTH3, IN_CH3, OUT_CH3);
    //把数据从我们的读取函数取出来，放到vector里面
    cout << "readfilterfile_to_float (W_fc1.bin) finished" << endl;
    for (size_t i = 0; i < size_w_fc1_float; i++) {
        din_w_fc1[i] = ptr_buffer_w_fc1[i];
    }
    cout << "setting din_w_fc1 finished" << endl;

    delete[] ptr_buffer_w_fc1;

    /**********annotation**********
     * ↓ 分配内存并读第一套fc(全连接)偏置
     * ********************/
    size_t size_b_fc1_byte = sizeoffile("./b_fc1.bin");
    size_t size_b_fc1_float = size_b_fc1_byte / 4;
    std::vector<float, aligned_allocator<float>> din_b_fc1(size_b_fc1_float);

    float* ptr_buffer_b_fc1 = readbiasfile_to_float("./b_fc1.bin", OUT_CH2);
    cout << "readbiasfile_to_float (b_conv1.bin) finished" << endl;
    for (size_t i = 0; i < size_b_fc1_float; i++) {
        din_b_fc1[i] = ptr_buffer_b_fc1[i];
    }
    cout << "setting din_b_fc1 finished" << endl;

    delete[] ptr_buffer_b_fc1;

    /**********annotation**********
 * ↓ 分配内存并读第二套fc(全连接)核
 * ********************/
    size_t size_w_fc2_byte = sizeoffile("./W_fc2.bin");
    size_t size_w_fc2_float = size_w_fc2_byte / 4;
    std::vector<float, aligned_allocator<float>> din_w_fc2(size_w_fc2_float);
    cout << "allocation of memory finished" << endl;

    float* ptr_buffer_w_fc2 = readfilterfile_to_float("./W_fc2.bin", IN_HEIGHT4, IN_WIDTH4, IN_CH4, OUT_CH4);
    //把数据从我们的读取函数取出来，放到vector里面
    cout << "readfilterfile_to_float (W_fc2.bin) finished" << endl;
    for (size_t i = 0; i < size_w_fc2_float; i++) {
        din_w_fc2[i] = ptr_buffer_w_fc2[i];
    }
    cout << "setting din_w_fc2 finished" << endl;

    delete[] ptr_buffer_w_fc2;

    /**********annotation**********
     * ↓ 分配内存并读第二套fc(全连接)偏置
     * ********************/
    size_t size_b_fc2_byte = sizeoffile("./b_fc2.bin");
    size_t size_b_fc2_float = size_b_fc2_byte / 4;
    std::vector<float, aligned_allocator<float>> din_b_fc2(size_b_fc2_float);

    float* ptr_buffer_b_fc2 = readbiasfile_to_float("./b_fc2.bin", OUT_CH2);
    cout << "readbiasfile_to_float (b_conv1.bin) finished" << endl;
    for (size_t i = 0; i < size_b_fc2_float; i++) {
        din_b_fc2[i] = ptr_buffer_b_fc2[i];
    }
    cout << "setting din_b_fc2 finished" << endl;

    delete[] ptr_buffer_b_fc2;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
    /**********annotation**********
     * ↓分配输出第一次卷积后，dout_featureout1的内存，并且初始化为0
     * ********************/
    size_t size_featureout1 = OUT_CH1* IN_WIDTH1* IN_HEIGHT1;//16*28*28
        std::vector<float,aligned_allocator<float>> dout_featureout1(size_featureout1);
    for(size_t i = 0 ; i < size_featureout1 ; i++){
        dout_featureout1[i] = 0;
    } 
    cout<<"allocation and setting of dou_featureout1"<<endl<<endl;

    /**********annotation**********
     * ↓分配输出第一次池化doutfeatureout11的内存，并且初始化为0
     * ********************/
    size_t size_featureout11 = OUT_CH11* OUT_WIDTH11* OUT_HEIGHT11;//16*14*14
    std::vector<float, aligned_allocator<float>> dout_featureout11(size_featureout11);
    for (size_t i = 0; i < size_featureout11; i++) {
        dout_featureout11[i] = 0;
    }
    cout << "allocation and setting of dou_featureout11" << endl << endl;

    /**********annotation**********
     * ↓分配输出第二次卷积后，dout_featureout2的内存，并且初始化为0
     * ********************/
    size_t size_featureout2 = OUT_CH2 * IN_WIDTH2 * IN_HEIGHT2;//32*14*14
    std::vector<float, aligned_allocator<float>> dout_featureout2(size_featureout2);
    for (size_t i = 0; i < size_featureout2; i++) {
        dout_featureout2[i] = 0;
    }
    cout << "allocation and setting of dou_featureout2" << endl << endl;

    /**********annotation**********
     * ↓分配输出第二次池化doutfeatureout21的内存，并且初始化为0
     * ********************/
    size_t size_featureout21 = OUT_CH21 * OUT_WIDTH21 * OUT_HEIGHT21;//32*7*7
    std::vector<float, aligned_allocator<float>> dout_featureout21(size_featureout21);
    for (size_t i = 0; i < size_featureout21; i++) {
        dout_featureout21[i] = 0;
    }
    cout << "allocation and setting of dou_featureout21" << endl << endl;

    /**********annotation**********
    * ↓分配输出第一次全连接dout_featureout3的内存，并且初始化为0
    * ********************/
    size_t size_featureout3 = OUT_CH3 * OUT_WIDTH3 * OUT_HEIGHT3;//32*1*1
    std::vector<float, aligned_allocator<float>> dout_featureout3(size_featureout3);
    for (size_t i = 0; i < size_featureout3; i++) {
        dout_featureout3[i] = 0;
    }
    cout << "allocation and setting of dou_featureout3" << endl << endl;
  
    /**********annotation**********
* ↓分配输出第二次全连接dout_featureout4的内存，并且初始化为0
* ********************/
    size_t size_featureout4 = OUT_CH4 * OUT_WIDTH4 * OUT_HEIGHT4;//10*1*1
    std::vector<float, aligned_allocator<float>> dout_featureout4(size_featureout4);
    for (size_t i = 0; i < size_featureout4; i++) {
        dout_featureout4[i] = 0;
    }
    cout << "allocation and setting of dou_featureout4" << endl << endl;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

    cout << "step3:OPENCL configuration" << endl << endl;
    /**********annotation**********
     * ↓OPENCL的操作:
     * one device
     * one context
     * one queue
     * two binary files
     * two programs
     * two kernels
     * ********************/
    //器件
    std::vector<cl::Device> devices = get_devices("Xilinx");
    devices.resize(1);
    cl::Device device = devices[0];

    // ------------------------------------------------------------------------------------
    // Step 1: Create Context//据描述:context是runtime用来管理指令队列和kernel的东西。好比，runtime是军官，context是花名册，kernel是具体干活的小兵崽2333
    // ------------------------------------------------------------------------------------
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    cout<<"create context"<<endl;

    // ------------------------------------------------------------------------------------
    // Step 1: Create Command Queue//生产指令列表
    // ------------------------------------------------------------------------------------
    OCL_CHECK(err, cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    cout<<"create commandqueue"<<endl;
    // ------------------------------------------------------------------
    // Step 1: Load Binary File from disk//读取二进制xo文件
    // ------------------------------------------------------------------	
    unsigned fileBufSize;//fileBufSize在下面的read_binary_file函数里面被赋了文件大小值
    char* fileBuf = read_binary_file(binaryFile, fileBufSize);//这里用到了最开始的argv，所以把上面的语句放这应该会更好点
    cl::Program::Binaries bins{{fileBuf, fileBufSize}};
    cout<<"read conv xclbin"<<endl;

    unsigned fileBufSize_2;//fileBufSize在下面的read_binary_file函数里面被赋了文件大小值
    char* fileBuf_2 = read_binary_file(binaryFile_2, fileBufSize_2);//这里用到了最开始的argv，所以把上面的语句放这应该会更好点
    cl::Program::Binaries bins_2{ {fileBuf_2, fileBufSize_2} };
    cout << "read pool xclbin" << endl;

    // -------------------------------------------------------------
    // Step 1: Create the program object from the binary and program the FPGA device with it//相当于烧bitstream
    // -------------------------------------------------------------	
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
    cout<<"program xclbin"<<endl;

    OCL_CHECK(err, cl::Program program_2(context, devices, bins_2, NULL, &err));
    cout << "program xclbin" << endl;
    // -------------------------------------------------------------
    // Step 1: Create Kernels//相当于给烧了bitstream的PL部分配上PS的寄存器
    // -------------------------------------------------------------
    OCL_CHECK(err, cl::Kernel krnl_Conv(program,"Conv", &err));//kernel1
    cout<<"create kernel:Conv finished"<<endl<<endl;

    OCL_CHECK(err, cl::Kernel krnl_Pool(program_2, "Pool", &err));//kernel2
    cout << "create kernel:Pool finished" << endl << endl;
    //这里用的名字应该是hls选的那个TOP函数的函数名

    cout<<"step4:setting parameters for kernel"<<endl;
    // ================================================================
    // Step 2: Setup Buffers and run Kernels

    // Allocate Global Memory for source_in1 //在device上面分配内存,分配的时候，这里应该.data()只是返回指针，所以这里没有真的在做搬运，而只是标了真正在queue里面要搬运时候的地址
    // .......................................................	
    // 自己做个规范，host的内存，用下划线，global的内存，用驼峰命名法
    // 图像din_img就是第一个conv的feature_in[];
    /*
    OCL_CHECK(err, cl::Buffer dinImage   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,img_size, din_img.data(), &err));

    OCL_CHECK(err, cl::Buffer dinWeightConv1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_w_conv1_float, din_w_conv1.data(), &err));

    OCL_CHECK(err, cl::Buffer dinBiasConv1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_b_conv1_float, din_b_conv1.data(), &err));

    OCL_CHECK(err, cl::Buffer doutFeatureOut1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout, dout_featureout.data(), &err));
	*/
///////////////////////////////// 读img 第一次卷积核与偏置 输出doutFeatureOut1 输出doutFeatureOut11
    OCL_CHECK(err, cl::Buffer dinImage   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,img_size, din_img.data(), &err));

    OCL_CHECK(err, cl::Buffer dinWeightConv1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_w_conv1_byte, din_w_conv1.data(), &err));

    OCL_CHECK(err, cl::Buffer dinBiasConv1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_b_conv1_byte, din_b_conv1.data(), &err));

    OCL_CHECK(err, cl::Buffer doutFeatureOut1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout1*4, dout_featureout1.data(), &err));
    
    OCL_CHECK(err, cl::Buffer doutFeatureOut11   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout11*4, dout_featureout11.data(), &err));
/////////////////////////////////       第二次卷积核与偏置 输出doutFeatureOut2 输出doutFeatureOut21
    OCL_CHECK(err, cl::Buffer dinWeightConv2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_w_conv2_byte, din_w_conv2.data(), &err));

    OCL_CHECK(err, cl::Buffer dinBiasConv2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_b_conv2_byte, din_b_conv2.data(), &err));

    OCL_CHECK(err, cl::Buffer doutFeatureOut2   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout2*4, dout_featureout2.data(), &err));
    
    OCL_CHECK(err, cl::Buffer doutFeatureOut21   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout21*4, dout_featureout21.data(), &err));

/////////////////////////////////       第一次全连接的核与偏置  输出doutFeatureOut3 
    OCL_CHECK(err, cl::Buffer dinWeightFc1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_w_fc1_byte, din_w_fc1.data(), &err));

    OCL_CHECK(err, cl::Buffer dinBiasFc1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_b_fc1_byte, din_b_fc1.data(), &err));

    OCL_CHECK(err, cl::Buffer doutFeatureOut3   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout3*4, dout_featureout3.data(), &err));
    
/////////////////////////////////       第二次全连接的核与偏置  输出doutFeatureOut4
    OCL_CHECK(err, cl::Buffer dinWeightFc2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_w_fc2_byte, din_w_fc2.data(), &err));

    OCL_CHECK(err, cl::Buffer dinBiasFc2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, size_b_fc2_byte, din_b_fc2.data(), &err));

    OCL_CHECK(err, cl::Buffer doutFeatureOut4   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,size_featureout4*4, dout_featureout4.data(), &err));
    

     // ============================================================================
    // 设置第一次卷积和池化的参数
    OCL_CHECK(err, err = krnl_Conv.setArg(0, IN_CH1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(1,IN_HEIGHT1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(2,IN_WIDTH1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(3,OUT_CH1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(4, KERNEL_WIDTH1));//
    OCL_CHECK(err, err = krnl_Conv.setArg(5,KERNEL_HEIGHT1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(6,X_STRIDE1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(7,Y_STRIDE1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(8,MODE1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(9,RELU_EN1 ));//
    OCL_CHECK(err, err = krnl_Conv.setArg(10, dinImage));//
    OCL_CHECK(err, err = krnl_Conv.setArg(11, dinWeightConv1));//
    OCL_CHECK(err, err = krnl_Conv.setArg(12, dinBiasConv1));//
    OCL_CHECK(err, err = krnl_Conv.setArg(13, doutFeatureOut1));//

    cout<<"setting parameters for first krnl_Conv finished"<<endl;

    OCL_CHECK(err, err = krnl_Pool.setArg(0, IN_CH11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(1, IN_HEIGHT11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(2, IN_WIDTH11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(3, OUT_CH11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(4, KERNEL_WIDTH11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(5, KERNEL_HEIGHT11));//
    OCL_CHECK(err, err = krnl_Pool.setArg(6, doutFeatureOut1));//读入上面的结果doutFeatureOut1
    OCL_CHECK(err, err = krnl_Pool.setArg(7, doutFeatureOut11));//输出为doutFeatureOut11

    cout << "setting parameters for first krnl_Pool finished" << endl;



/**********annotation**********
 * 准备执行第一次卷积池化
 * ****************/
    cout<<"push command into queue"<<endl;
    //从stackoverflow上看，在这把kernel压入队列之后，压入的操作就定死了，后面就可以去改argument重新压新操作进队列了
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({dinImage, dinWeightConv1,dinBiasConv1},0/* 0 means from host*/));	//把device上的global内容给到kernel里面，话说这里应该是写错了，0标志这里做的是从global传去给kernel里面

    OCL_CHECK(err, err = q.enqueueTask(krnl_Conv));

    OCL_CHECK(err, err = q.enqueueTask(krnl_Pool));

    // ============================================================================
    // 设置第二次卷积和池化的参数
    OCL_CHECK(err, err = krnl_Conv.setArg(0, IN_CH2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(1, IN_HEIGHT2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(2, IN_WIDTH2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(3, OUT_CH2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(4, KERNEL_WIDTH2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(5, KERNEL_HEIGHT2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(6, X_STRIDE2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(7, Y_STRIDE2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(8, MODE2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(9, RELU_EN2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(10, doutFeatureOut11));//
    OCL_CHECK(err, err = krnl_Conv.setArg(11, dinWeightConv2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(12, dinBiasConv2));//
    OCL_CHECK(err, err = krnl_Conv.setArg(13, doutFeatureOut2));//

    cout << "setting parameters for second krnl_Conv finished" << endl;

    OCL_CHECK(err, err = krnl_Pool.setArg(0, IN_CH21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(1, IN_HEIGHT21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(2, IN_WIDTH21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(3, OUT_CH21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(4, KERNEL_WIDTH21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(5, KERNEL_HEIGHT21));//
    OCL_CHECK(err, err = krnl_Pool.setArg(6, doutFeatureOut2));//读入上面的结果doutFeatureOut1
    OCL_CHECK(err, err = krnl_Pool.setArg(7, doutFeatureOut21));//输出为doutFeatureOut11

    cout << "setting parameters for second krnl_Pool finished" << endl;

    OCL_CHECK(err, err = q.enqueueTask(krnl_Conv));

    OCL_CHECK(err, err = q.enqueueTask(krnl_Pool));

    // ============================================================================
      // 设置第一次全连接的参数
    OCL_CHECK(err, err = krnl_Conv.setArg(0, IN_CH3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(1, IN_HEIGHT3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(2, IN_WIDTH3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(3, OUT_CH3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(4, KERNEL_WIDTH3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(5, KERNEL_HEIGHT3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(6, X_STRIDE3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(7, Y_STRIDE3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(8, MODE3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(9, RELU_EN3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(10, doutFeatureOut21));//
    OCL_CHECK(err, err = krnl_Conv.setArg(11, dinWeightConv3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(12, dinBiasConv3));//
    OCL_CHECK(err, err = krnl_Conv.setArg(13, doutFeatureOut3));//

    OCL_CHECK(err, err = q.enqueueTask(krnl_Conv));

    cout << "setting parameters for first fc finished" << endl;

     // ============================================================================
      // 设置第二次全连接的参数
    OCL_CHECK(err, err = krnl_Conv.setArg(0, IN_CH4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(1, IN_HEIGHT4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(2, IN_WIDTH4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(3, OUT_CH4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(4, KERNEL_WIDTH4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(5, KERNEL_HEIGHT4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(6, X_STRIDE4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(7, Y_STRIDE4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(8, MODE4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(9, RELU_EN4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(10, doutFeatureOut21));//
    OCL_CHECK(err, err = krnl_Conv.setArg(11, dinWeightConv4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(12, dinBiasConv4));//
    OCL_CHECK(err, err = krnl_Conv.setArg(13, doutFeatureOut4));//

    OCL_CHECK(err, err = q.enqueueTask(krnl_Conv));

    cout << "setting parameters for first fc finished" << endl;

    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({doutFeatureOut4},CL_MIGRATE_MEM_OBJECT_HOST));//把算完的doutFeatureOut1拿出来

    
    cout<<"queue starts"<<endl;
    q.finish();
    cout<<"queue finished"<<endl;

    cout << dout_featureout4 << endl;


    delete[] fileBuf;//这是上面读进来的xo文件
    delete[] fileBuf_2;//这是上面读进来的xo文件

    /**********annotation**********
     * 从host上输出结果
     * ****************/
    /* 输出第一次卷积结果
    cout<<"output"<<endl;
    for (int k = 0; k < 16; k++) {
        for (int i = 0; i < 28; i++) {
            for (int j = 0; j < 28; j++) {
                cout<<dout_featureout[k*28*28+i*28+j]<<endl; 

            }
        }
    }
    */

    return 0;
}
