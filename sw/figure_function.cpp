#include "figure_function.h"

char* read_binary_file(const std::string &xclbin_file_name, unsigned &nb)
{
    std::cout << "INFO: Reading " << xclbin_file_name << std::endl;

	if(access(xclbin_file_name.c_str(), R_OK) != 0) {
		printf("ERROR: %s xclbin not available please build\n", xclbin_file_name.c_str());
		exit(EXIT_FAILURE);
	}
    //Loading XCL Bin into char buffer
    std::cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
    std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char *buf = new char [nb];
    bin_file.read(buf, nb);
    return buf;
}

std::vector<cl::Device> get_devices(const std::string& vendor_name) {

    size_t i;
    cl_int err;
    std::vector<cl::Platform> platforms;
    OCL_CHECK(err, err = cl::Platform::get(&platforms));
    cl::Platform platform;
    for (i  = 0 ; i < platforms.size(); i++){
        platform = platforms[i];
        OCL_CHECK(err, std::string platformName = platform.getInfo<CL_PLATFORM_NAME>(&err));
        if (platformName == vendor_name){
            std::cout << "Found Platform" << std::endl;
            std::cout << "Platform Name: " << platformName.c_str() << std::endl;
            break;
        }
    }
    if (i == platforms.size()) {
        std::cout << "Error: Failed to find Xilinx platform" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Getting ACCELERATOR Devices and selecting 1st such device
    std::vector<cl::Device> devices;
    OCL_CHECK(err, err = platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices));
    return devices;
}



float* readfilterfile_to_float(const char* filepath, int hight, int width, int channel_in, int channel_out) {
    FILE* bin_pFile;
    size_t bin_size;
    size_t bin_size_float;
    size_t result;

    //bin_pFile = fopen("\\data\\W_conv1.bin", "rb");
    bin_pFile = fopen(filepath, "rb");
    if (bin_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bin大小(byte个数)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //算bin内容按float来算的个数 
    bin_size_float = bin_size / 4;

    // 开块动态数组
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size是以byte为单位的，要转成float型，就要/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //将bin内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result是正确读到的元素数量,(读去哪，一次读多少byte，共读多少次，用哪个指针去读)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    return ptr_buffer;
}

float* readbiasfile_to_float(const char* filepath, int channel_out) {
    FILE* bin_pFile;
    size_t bin_size;
    size_t bin_size_float;
    size_t result;

    //bin_pFile = fopen("\\data\\W_conv1.bin", "rb");
    bin_pFile = fopen(filepath, "rb");
    if (bin_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bin大小(byte个数)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //算bin内容按float来算的个数
    bin_size_float = bin_size / 4;

    // 开块动态数组
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size是以byte为单位的，要转成float型，就要/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //将bin内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result是正确读到的元素数量,(读去哪，一次读多少byte，共读多少次，用哪个指针去读)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //输出内容
    for (int u = 0; u < channel_out; u++) {//channel out
        cout << setw(20) << (ptr_buffer[u]) << endl;
    }
    return ptr_buffer;


}



uint8_t* readbmpfile(const char* filepath) {
    FILE* img_pFile;
    size_t img_size;
    size_t result;

    img_pFile = fopen(filepath, "rb");
    if (img_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bmp大小
    fseek(img_pFile, 0, SEEK_END);//把指针放去末尾
    img_size = ftell(img_pFile);//读出指针的位置，也就是这个文件的byte数量
    rewind(img_pFile);//将指针放回开头

    // 开块动态数组
    uint8_t* ptr_buffer;
    ptr_buffer = new uint8_t[img_size - BMP_OFFSET];
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //跳1078
    fseek(img_pFile, BMP_OFFSET, SEEK_SET);

    //将bmp从1078位以后的内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(uint8_t), img_size - BMP_OFFSET, img_pFile);//result是正确读到的元素数量
    if (result != img_size - BMP_OFFSET) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    fclose(img_pFile);
    return ptr_buffer;
}


size_t sizeoffile(const char* filepath) {
    FILE* img_pFile;
    size_t img_size;

    img_pFile = fopen(filepath, "rb");
    if (img_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bmp大小
    fseek(img_pFile, 0, SEEK_END);//把指针放去末尾
    img_size = ftell(img_pFile);//读出指针的位置，也就是这个文件的byte数量
    rewind(img_pFile);//将指针放回开头

    fclose(img_pFile);
    return img_size;
}

void reverse_gray_figure(uint8_t* ptr_buffer,size_t size) {
    for (size_t i = 0; i < size; i++) {
    	//cout<<i<<"  "<<(int)ptr_buffer[i]<<endl;
    	uint8_t temp = 255-(int)ptr_buffer[i];
    	ptr_buffer[i] = temp;
        //ptr_buffer[i] = 255 - ptr_buffer[i];
        //cout<<i<<"  "<<(int)ptr_buffer[i]<<endl;
        //cout<<endl;
    }
}

