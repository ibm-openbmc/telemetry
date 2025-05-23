#pragma once

#include "errors.hpp"

#include <sdbusplus/exception.hpp>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>

namespace utils
{

template <size_t N>
struct ConstexprString
{
    constexpr ConstexprString(const char (&data)[N]) : s()
    {
        for (size_t i = 0; i < N; ++i)
        {
            s[i] = data[i];
        }
    }

    constexpr operator std::string_view() const
    {
        return {s.data(), s.size()};
    }

    std::array<char, N> s;
};

[[noreturn]] inline void throwConversionError(std::string_view propertyName)
{
    throw errors::InvalidArgument(propertyName, "Cannot convert.");
}

template <class T>
struct EnumTraits;

template <class T, T first, T last>
inline T toEnum(std::underlying_type_t<T> x)
{
    if (x < static_cast<std::underlying_type_t<T>>(first) ||
        x > static_cast<std::underlying_type_t<T>>(last))
    {
        throwConversionError(EnumTraits<T>::propertyName);
    }
    return static_cast<T>(x);
}

template <class T>
constexpr inline std::underlying_type_t<T> toUnderlying(T value)
{
    return static_cast<std::underlying_type_t<T>>(value);
}

template <class T, size_t N>
constexpr inline T minEnumValue(
    std::array<std::pair<std::string_view, T>, N> data)
{
    auto min = data[0].second;
    for (auto [key, value] : data)
    {
        if (toUnderlying(min) > toUnderlying(value))
        {
            min = value;
        }
    }
    return min;
}

template <class T, size_t N>
constexpr inline T maxEnumValue(
    std::array<std::pair<std::string_view, T>, N> data)
{
    auto max = data[0].second;
    for (auto [key, value] : data)
    {
        if (toUnderlying(max) < toUnderlying(value))
        {
            max = value;
        }
    }
    return max;
}

template <class T, size_t N>
inline T toEnum(const std::array<std::pair<std::string_view, T>, N>& data,
                const std::string& value)
{
    auto it = std::find_if(
        std::begin(data), std::end(data),
        [&value](const auto& item) { return item.first == value; });
    if (it == std::end(data))
    {
        throwConversionError(EnumTraits<T>::propertyName);
    }
    return it->second;
}

template <class T, size_t N>
inline std::string_view enumToString(
    const std::array<std::pair<std::string_view, T>, N>& data, T value)
{
    auto it = std::find_if(
        std::begin(data), std::end(data),
        [value](const auto& item) { return item.second == value; });
    if (it == std::end(data))
    {
        throwConversionError(EnumTraits<T>::propertyName);
    }
    return it->first;
}

} // namespace utils
