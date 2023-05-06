#pragma once

#include <iostream>
#include <string>
#include <chrono>

class ProfilerScope
{
public:
    ProfilerScope(const std::string& name = "Scope") : m_Name(name), m_TimeStart(std::chrono::high_resolution_clock::now()) 
    {

    }
    ~ProfilerScope()
    {
        auto timeEnd = std::chrono::high_resolution_clock::now();
        std::cout << m_Name << " running time: " << std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - m_TimeStart).count() << " milliseconds.\n";
    }
private:
    std::string m_Name;
    std::chrono::high_resolution_clock::time_point m_TimeStart;
};