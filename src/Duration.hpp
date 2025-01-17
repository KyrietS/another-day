#pragma once
#include <chrono>

using Duration =
    std::common_type<std::chrono::seconds, std::common_type<std::chrono::minutes, std::chrono::hours>::type>::type;
