/**
 * @file core.hpp
 * @author Okano Tomoyuki (tomoyuki.okano@tsuneishi.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-29
 * 
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

double adjust_180(const double& value) noexcept
{
    double result = value;
    while(result>180.)
        result -= 360.;
    while(result<-180.)
        result += 360.;
    return result;
}

std::string concat(const std::vector<std::string>& origin, const char& separator=',') noexcept
{
    std::string result;
    for (const auto& str : origin)
    {
        result += str + separator;
    }
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

} // Utility

#endif // _UTILITY_CORE_HPP_
