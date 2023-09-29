#ifndef _UTILITY_SHARED_MEMORY_HPP_
#define _UTILITY_SHARED_MEMORY_HPP_

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <chrono>

#ifdef __unix__
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#else
#include <stdio.h>
#include <windows.h>
#endif

namespace Utility
{

template <typename T>
class SharedMemory final
{

#ifdef __unix__
    using MapHandle = int;
#else
    using MapHandle = HANDLE;
#endif

private:
    MapHandle map_handle_;
    char* data_;
    int buffer_size_;

public:
    explicit SharedMemory(const char* file_path)
    {
        buffer_size_ = static_cast<int>(sizeof(T));
#ifdef __unix__
        int page_size = sysconf(_SC_PAGE_SIZE);
        int total_size = ((buffer_size_ + 1) / page_size + 1) * page_size;
        map_handle_ = open(file_path, O_RDWR, 0660);
        if (map_handle_ == -1)
        {
            map_handle_ = open(file_path, O_RDWR | O_CREAT, 0660);
            if (map_handle_ == -1)
                throw std::runtime_error("File open failed.");
            char* buffer = new char[total_size];
            write(map_handle_, buffer, total_size);
            delete buffer;
            close(map_handle_);
            map_handle_ = open(file_path, O_RDWR, 0660);
            if (map_handle_ == -1)
                throw std::runtime_error("File open failed.");
        }

        lseek(map_handle_, 0, SEEK_SET);
        data_ = reinterpret_cast<char*>(mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, map_handle_, 0));
        if (data_ == MAP_FAILED)
            throw std::runtime_error("File map failed.");
#else
        wchar_t fname[260];
        int i = 0;
        while (*file_path != '\0')
        {
            mbtowc(&fname[i++], file_path, MB_CUR_MAX);
            file_path++;
        }
        fname[i] = '\0';
        MapHandle handle = CreateFileW(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (handle == INVALID_HANDLE_VALUE)
        {
            handle = CreateFileW(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
            if (handle == INVALID_HANDLE_VALUE)
                throw std::runtime_error("file open failed");
        }
        int total_size = buffer_size_ + 1;
        map_handle_ = CreateFileMappingW(handle, 0, PAGE_READWRITE, 0, total_size, NULL);
        if (map_handle_ == 0)
            throw std::runtime_error("file open failed");
        data_ = reinterpret_cast<char*>(MapViewOfFile(map_handle_, FILE_MAP_ALL_ACCESS, 0, 0, total_size));
        if (data_ == 0)
            throw std::runtime_error("file mapping failed");
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
#endif
    }

    ~SharedMemory()
    {
#ifdef __unix__
        close(map_handle_);
#else
        UnmapViewOfFile(data_);
        if (map_handle_ != INVALID_HANDLE_VALUE)
        {
            CloseHandle(map_handle_);
            map_handle_ = INVALID_HANDLE_VALUE;
        }
        data_ = nullptr;
#endif
    }

    void flush()
    {
        std::memset(data_+buffer_size_, 0, 1);
    }

    bool try_read(T* buffer, const double& timeout=1.) const
    {
        std::chrono::nanoseconds nano_sec(static_cast<int>(1e+6*timeout));
        auto start = std::chrono::system_clock::now();
        while(true)
        {
            if (data_[buffer_size_] == 0)
                break;

            auto waist = std::chrono::system_clock::now() - start;
            if(waist > nano_sec)
                return false;
        }

        data_[buffer_size_] = 1; // lock
        std::memcpy(buffer, data_, buffer_size_);
        data_[buffer_size_] = 0; // unlock
        return true;
    }

    bool try_write(const T* buffer, const double& timeout=1.) const
    { 
        std::chrono::nanoseconds nano_sec(static_cast<int>(1e+6*timeout));
        auto start = std::chrono::system_clock::now();
        while(true)
        {
            if (data_[buffer_size_] == 0)
                break;

            auto waist = std::chrono::system_clock::now() - start;
            if(waist > nano_sec)
                return false;
        }

        data_[buffer_size_] = 1; // lock
        std::memcpy(data_, buffer, buffer_size_);
        data_[buffer_size_] = 0; // unlock
        return true;
    }
};

} // Utility

#endif // _UTILITY_SHARED_MEMORY_HPP_
