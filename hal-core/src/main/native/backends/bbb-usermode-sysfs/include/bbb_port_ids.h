#pragma once

#define P8 (0x1 << 6)
#define P9 (0x1 << 7)
#define USR (0x3 << 6)
#define GPIO_ID(header, number) (header | number)
#define P9_41A GPIO_ID(P9, 60)
#define P9_42A GPIO_ID(P9, 61)
#define P9_41B GPIO_ID(P9, 62)
#define P9_42B GPIO_ID(P9, 63)
