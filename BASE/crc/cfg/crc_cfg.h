#ifndef _CRC_CFG_H_
#define _CRC_CFG_H_

#include "BASE/types.h"

/** Hardware based CRC8 calculation */
#define CRC_8_HARDWARE  1u /* not supported on MPC5602B */
/** Table based CRC8 calculation */
#define CRC_8_TABLE     2u
/** Runtime based CRC8 calculation */
#define CRC_8_RUNTIME   3u

/** Hardware based CRC16 calculation */
#define CRC_16_HARDWARE  1u /* not supported on MPC5602B */
/** Table based CRC16 calculation */
#define CRC_16_TABLE     2u
/** Runtime based CRC16 calculation */
#define CRC_16_RUNTIME   3u

/** Hardware based CRC32 calculation */
#define CRC_32_HARDWARE  1u /* not supported on MPC5602B */
/** Table based CRC16 calculation */
#define CRC_32_TABLE     2u
/** Runtime based CRC16 calculation */
#define CRC_32_RUNTIME   3u

/* Switch to select one of the available calculation method */
#define CRC_8_MODE      CRC_8_RUNTIME
#define CRC_16_MODE     CRC_16_RUNTIME
#define CRC_32_MODE     CRC_32_RUNTIME

/** Initial value for the 8-bit CRC calculation */
#define CRC_INITIAL_VALUE8     0x00u

/** Initial value for the 16-bit CRC calculation */
#define CRC_INITIAL_VALUE16    0xFFFFu

/** Initial value for the 32-bit CRC calculation */
#define CRC_INITIAL_VALUE32    0xFFFFFFFFu

#endif /* ==== End of file _CRC_CFG_H_ =================================== */
