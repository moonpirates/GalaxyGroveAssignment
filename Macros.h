// Macros.h - (c) 2024 Jordi Scharp <www.moonpirates.com>
#pragma once

#define TEXT_COLOR(colorCode) "\033["#colorCode"m"
#define TEXT_RESET() "\033[0m"
#define LOG_ERROR(text) std::cout << TEXT_COLOR(31) << text << TEXT_RESET();