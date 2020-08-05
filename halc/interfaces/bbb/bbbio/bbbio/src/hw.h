#ifndef BBBIO_HW_H
#define BBBIO_HW_H

#define HWREG(x)       (*((volatile unsigned int* )(x)))
#define HWREGH(x)      (*((volatile unsigned short* )(x)))
#define HWADD(x, y)    (((void*)x) + y)

#endif //BBBIO_HW_H
