/**
 * @file core.hpp
 * @author Okano Tomoyuki (tomoyuki.okano@tsuneishi.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-29 Created by Okano Tomoyuki.
 * @date 2023-10-02 Updated by Okano Tomoyuki.
 * @n 1.Add copy function
 * @n 2.Add show function,
 * @n 3.Add DEBUG_PRINT MACRO
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _UTILITY_CORE_HPP_
#define _UTILITY_CORE_HPP_

#include <numeric>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <typeinfo>

#ifndef _DEBUG_MODE_
    #define _DEBUG_MODE_ 1 
    #define DEBUG_PRINT(var) if(_DEBUG_MODE_){do{std::cout << #var << "[" << typeid(var).name() << "]:\n";Utility::show(var);}while(0);}
#endif

namespace Utility
{

double radians(const double& value) noexcept
{
    return value * M_PI / 180.;
}

double degrees(const double& value) noexcept
{
    return value * 180. / M_PI;
}

double meter_per_seconds(const double& value) noexcept
{
    return value * 1852. / 3600.;
}

double knots(const double& value) noexcept
{
    return value * 3600. / 1852.;
}

template<typename T>
T limit(const T& value, const T& limit) noexcept
{
    return std::max(std::min(value, limit), -limit);
}

template<typename T>
T limit(const T& value, const T& lower_limit, const T& upper_limit) noexcept
{
    return std::max(std::min(value, upper_limit), lower_limit);
}

double adjust_pi(const double& value) noexcept
{
    double result = value;
    while(result>M_PI)
        result -= 2*M_PI;
    while(result<-M_PI)
        result += 2*M_PI;
    return result;
}

double adjust_180(const double& value) noexcept
{
    double result = value;
    while(result>180.)
        result -= 360.;
    while(result<-180.)
        result += 360.;
    return result;
}

template<typename T>
std::string concat(const std::vector<T>& origin, const char& separator=',') noexcept
{
    std::string result;
    for (const auto& str : origin)
        result += std::to_string(str) + separator;
    result.pop_back(); // erase separator character placed in end.
    return result;
}

std::vector<std::string> split(const std::string& origin, const char& separator=',') noexcept
{
    std::vector<std::string> result;
    std::string element;
    std::istringstream iss(origin);
    while(std::getline(iss, element, separator))
    {
        result.push_back(element);
    }

    return result;
}

std::vector<int> range(const int& end) noexcept
{
    std::vector<int> result(end, 0);
    std::iota(result.begin(), result.end(), 0);
    return result;
}

std::vector<int> range(const int& start, const int& end, const int& interval=1) noexcept
{
    std::vector<int> result((std::ceil(static_cast<float>(end-start)/interval)), 0);
    std::iota(result.begin(), result.end(), 0);
    for (auto& i : result)
    {
        i = start + interval * i;
    }
    return result;
}

template<size_t ROW, size_t COL, typename T>
bool copy(T (&source)[ROW][COL], std::vector<std::vector<T>>& target, const size_t& row_size=ROW, const size_t& column_size=COL) noexcept
{
    if(!(row_size>0 && column_size>0 && row_size<=ROW && column_size<=COL))
        return false;
    
    target.resize(row_size);
    for (auto& row : target)
        row.resize(column_size);

    for (const auto& i : range(row_size))
        for (const auto& j : range(column_size))
            target[i][j] = source[i][j];

    return true;
}

template<size_t ROW, size_t COL, typename T>
bool copy(std::vector<std::vector<T>>& source, T (&target)[ROW][COL], const size_t& row_size=ROW, const size_t& column_size=COL) noexcept
{
    if(!(row_size>0 && column_size>0 && row_size<=ROW && column_size<=COL))
        return false;
    
    target.resize(row_size);
    for (auto& row : target)
        row.resize(column_size);

    for (const auto& i : range(row_size))
        for (const auto& j : range(column_size))
            target[i][j] = source[i][j];

    return true;
}

template<typename T> 
void show(T e) noexcept
{
    std::cout << e << std::endl;
}

template<typename T> 
void show(const std::vector<T>& v) noexcept
{
    std::cout << "{" << concat(v) << "}" << std::endl;
}

template<typename T> 
void show(const std::vector<std::vector<T>>& vv) noexcept
{ 
    std::string s; 
    for(const auto& v : vv)
        s += " {" + concat(v) + "},\n";
    s.pop_back();
    s.pop_back();
    std::cout << "{\n" << s << "\n}" << std::endl;
}

} // Utility

#endif // _UTILITY_CORE_HPP_
