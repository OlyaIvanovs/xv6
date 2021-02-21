#include "string.h"
#include "base.h"

void memset(void *va, u8 pattern, int len)
{
    // TODO!
}

int memcmp(const u8 *va, const char *string, int len)
{
    for (int i = 0; i < len; ++i)
    {
        int diff = va[i] - string[i];
        if (diff != 0)
        {
            return diff;
        }
    }
}

// Used for checking checksum MP_FPStruct
u8 sum_bytes(u8 *va, int count)
{
    u8 result = 0;
    for (int i = 0; i < count; ++i)
    {
        result += va[i];
    }
    return result;
}