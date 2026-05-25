#ifndef CSSWORK_H
#define CSSWORK_H

#define PROJECT_PATH "/Users/alekseypodoplelov/Documents/milandr_server/"

#include <cstdint>
#include <__stddef_size_t.h>
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

#endif
