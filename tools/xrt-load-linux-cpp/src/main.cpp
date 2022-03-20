
#include <xrt.h>
#include <xrt/xrt_device.h>
#include <experimental/xrt_xclbin.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <fstream>

std::vector<std::uint8_t> read_file(const char* path) {
    std::fstream fs(path);
    if(!fs) {
        std::stringstream ss;
        ss << "Faile to open file - " << path;
        throw std::runtime_error(ss.str());
    }

    fs.seekg(0, std::ios_base::end);
    auto size = fs.tellg();
    fs.seekg(0, std::ios_base::beg);
    std::vector<std::uint8_t> content;
    content.resize(size);
    fs.read(reinterpret_cast<char*>(content.data()), size);
    return std::move(content);
}

static constexpr const std::size_t OFFSET_BOOTROM = 0x80000000;
static constexpr const std::size_t OFFSET_DEVICE_TREE = 0x83000000;
static constexpr const std::size_t OFFSET_SBI = 0x80010000;
static constexpr const std::size_t OFFSET_KERNEL = 0x81000000;
static constexpr const std::size_t OFFSET_ROOTFS = 0x88000000;

static void transfer_data(xrt::device& device, std::vector<std::uint8_t>& data, std::size_t offset, const char* name)
{
    std::cout << "Transferring " << name << " (size: " << data.size() << ")..." << std::endl;
    auto result = xclUnmgdPwrite(device, 0, data.data(), data.size(), offset);
    if( result != 0 ) {
        std::stringstream ss;
        ss << "Failed to transfer " << name << " - " << result;
        throw std::runtime_error(ss.str());
    }
    std::cout << "\tVeryfing " << name << "...";

    std::vector<std::uint8_t> verify;
    verify.resize(data.size());
    result = xclUnmgdPread(device, 0, verify.data(), verify.size(), offset);
    if( result != 0 ) {
        std::stringstream ss;
        ss << "Failed to read back " << name << " - " << result;
        throw std::runtime_error(ss.str());
    }
    for(std::size_t i = 0; i < verify.size(); i++) {
        if(verify[i] != data[i]) {
            std::stringstream ss;
            ss << "Verify error at " << i;
            throw std::runtime_error(ss.str());
        }
    }
    std::cout << "Ok" << std::endl;
}
static int app_main(int argc, char* argv[])
{
    if( argc < 6 ) {
        std::cerr << "Usage: " << argv[0] << " BOOTROM DEVICETREE SBI KERNEL ROOTFS" << std::endl;
        return 1;
    }
    auto bootrom_path = argv[1];
    auto devicetree_path = argv[2];
    auto sbi_path = argv[3];
    auto kernel_path = argv[4];
    auto rootfs_path = argv[5];

    std::cout << "Bootrom    : " << bootrom_path << std::endl;
    std::cout << "Device Tree: " << devicetree_path << std::endl;
    std::cout << "SBI        : " << sbi_path << std::endl;
    std::cout << "Kernel     : " << kernel_path << std::endl;
    std::cout << "RootFS     : " << rootfs_path << std::endl;

    auto bootrom = read_file(bootrom_path);
    auto device_tree = read_file(devicetree_path);
    auto sbi = read_file(sbi_path);
    auto kernel = read_file(kernel_path);
    auto rootfs = read_file(rootfs_path);

    xrt::device device(0);
    std::cout << device.get_info<xrt::info::device::bdf>() << std::endl;
    
    transfer_data(device, bootrom,     OFFSET_BOOTROM, "Bootrom");
    transfer_data(device, device_tree, OFFSET_DEVICE_TREE, "Device Tree");
    transfer_data(device, sbi,         OFFSET_SBI, "SBI");
    transfer_data(device, kernel,      OFFSET_KERNEL, "Kernel");
    transfer_data(device, rootfs,      OFFSET_ROOTFS, "RootFS");

    return 0;
}

int main(int argc, char* argv[])
{
    try {
        return app_main(argc, argv);
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}