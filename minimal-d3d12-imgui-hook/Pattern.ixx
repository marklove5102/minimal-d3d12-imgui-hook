module;

#include "pch.h"
#include <sigmatch/sigmatch.hpp>

export module Pattern;

export template<typename T>
T Find(const uint64_t nBaseAddress, const sigmatch::search_context& context, const sigmatch::signature &sig)
{
    auto result = context.search(sig);
    if(result.has_error())
    {
        for(auto& error : result.error_messages())
        {
            LOG_ERROR("{}", error);
        }
    }
    if(result.has_warning())
    {
        for(auto& warning : result.warning_messages())
        {
            LOG_WARN("{}", warning);
        }
    }
    if(result.matches().empty())
    {
        LOG_ERROR("Pattern not found!");
    }
    return reinterpret_cast<T>(result.matches()[0]);
}

export template<typename T>
T FindRelative(const uint64_t nBaseAddress, const sigmatch::search_context& context, const sigmatch::signature &sig, int32_t offset)
{
    auto result = Find<const std::byte*>(nBaseAddress, context, sig);
    int32_t v = *reinterpret_cast<const int32_t*>(&result[offset]);
    auto end = reinterpret_cast<uint64_t>(result + offset + v + sizeof(int32_t));
    return reinterpret_cast<T>(end);
}

export template<typename T>
T FindAbsolute(const uint64_t nBaseAddress, const sigmatch::search_context& context, const sigmatch::signature &sig, int32_t offset)
{
    auto result = Find<const std::byte*>(nBaseAddress, context, sig);
    int32_t v = *reinterpret_cast<const int32_t*>(&result[offset]);
    return reinterpret_cast<T>(nBaseAddress + v);
}