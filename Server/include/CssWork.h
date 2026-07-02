
#pragma once

#include <cstdint>
#include <vector>

enum class ModeCalcCrc : uint8_t
{
    Sav = 0x0,
    Malcev
};

class CssWork
{
public:
    static int CalcCrc(std::vector<uint8_t> &bytes, ModeCalcCrc mode);
    static std::vector<uint8_t> hexToBytes(const std::string& hex);
    static void testCrc(std::string strHexBytes);
    CssWork() = default;
    ~CssWork() = default;
};
