// BulkWriteClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <chrono>
#include <random>

#include "../BulkWrite/Interfaces.h"
#include "../BulkWrite/IFileStream.h"
#include "../BulkWrite/IFileProcessor.h"

#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace FileAPI;

using Image = std::vector<unsigned char>;

void fillWithRandomValues(Image &image)
{
    //https://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
    static std::uniform_int_distribution<int> distribution(
        std::numeric_limits<uint8_t>::min(),
        std::numeric_limits<uint8_t>::max());
    static std::default_random_engine generator;

    std::generate(image.begin(), image.end(), []() { return distribution(generator); });
}

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S:%e] [%L] [%P:%t] %v");
    const auto logger = spdlog::basic_logger_mt("logger", "logs/log.txt");


    constexpr auto SIZE = 2048 * 2048; // 2048 width and 2048 height
    constexpr auto NOPIXELS = 1;
    constexpr size_t imageSizeInBytes = SIZE * NOPIXELS;

    Image image1(SIZE*NOPIXELS);
    fillWithRandomValues(image1);

    cout << "Enter Number of images :";
    auto noImages = 0; cin >> noImages;

    auto fileProcessor(CreateFileProcessor(*logger));
    fileProcessor->enable();

    const std::string filename("c:\\temp\\Image.data");
    auto fileStream = fileProcessor->open(filename, "w+");
    const auto bytes = noImages*imageSizeInBytes;
    fileStream->setSize(bytes);

    //write loop
    auto i = 0;
    auto start = chrono::high_resolution_clock::now();
    while (i<noImages) 
    {
        auto writeRequest = fileStream->write(&image1[0], imageSizeInBytes, OFFSET_NONE);
        writeRequest.wait();
        auto writeIOStatus = writeRequest.get();
        if (writeIOStatus.status != Status::OK)
            std::cout << "ERROR in Write at " << i << std::endl;
        fileStream->seek(writeIOStatus.transferedBytes, RelativePosition::CURRENT);
        ++i;
    }
    auto end = chrono::high_resolution_clock::now();
    const auto writeMs = chrono::duration_cast<chrono::milliseconds>(end - start);

    //read loop
    fileStream->seek(0); // reset to start of the file.
    i = 0;
    start = chrono::high_resolution_clock::now();
    while (i < noImages)
    {
        Image image2(SIZE*NOPIXELS);
        auto readRequest = fileStream->read(&image2[0], imageSizeInBytes, OFFSET_NONE);
        readRequest.wait();
        auto readIOStatus = readRequest.get();
        if (readIOStatus.status != Status::OK || (image1 != image2))
            std::cout << "ERROR in read at "<< i << std::endl;
        fileStream->seek(readIOStatus.transferedBytes, RelativePosition::CURRENT);
        ++i;
    }
    end = chrono::high_resolution_clock::now();
    const auto readMs = chrono::duration_cast<chrono::milliseconds>(end - start);

    std::cout << "Write took: " << writeMs.count() << " ms for total " << noImages << " images" << std::endl;
    std::cout << "read took: " << readMs.count() << " ms for total " << noImages << " images" << std::endl;
    std::cout << "waiting enter key :" << std::endl;
    std::cin >> noImages;
    fileStream.reset();
    fileProcessor.reset();
    return 0;
}
