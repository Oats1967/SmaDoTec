#ifndef _CRC_H_
#define _CRC_H_ /* PRQA S 602 */

#ifdef __cplusplus
extern "C" {
#endif

#include "BASE/crc/cfg/crc_cfg.h"

#define CRC_CRC32_PRESET_VALUE                         0xFFFFFFFFu
#define CRC_CRC16_PRESET_VALUE                         0xFFFFu
#define CRC_CRC16_ISO15693_PRESET_VALUE                0xFFFuF
#define CRC_CRC16_CCITT_PRESET_VALUE                   0xFFFFu

/* CRC_CRC32 -> CRC32, Initial Value 0xFFFFFFFF, reverse databytes, reverse result, Final XOR 0xFFFFFFFF */
#define CRC_CRC32_REVERSEDPOLYNOMIAL                   0x04C11DB7u /* = 0xEDB88320 = x^32 + x^26 + x^23 + x^22 + x^16 + x^12 +  x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + X^0 */
#define CRC_CRC32_CHECK_VALUE                          0xCBF43926u /* for "123456789" */

/* CRC_CRC16_ISO15693 -> CRC-CCITT, Initial Value 0xFFFF, reverse databytes, reverse result, Final XOR 0xFFFF */
#define CRC_CRC16_ISO15693_REVERSEDPOLYNOMIAL          0x8408u /* = 0x1021 = x^16 + x^12 + x^5 + 1 */
#define CRC_CRC16_ISO15693_CHECK_VALUE                 0x906Eu /* for "123456789" */

/* CRC_CRC16_CCITT -> CRC-CCITT, Initial Value 0xFFFF, Final XOR 0x0000 */
#define CRC_CRC16_CCITT_REVERSEDPOLYNOMIAL             0x8408u /* = 0x1021 = x^16 + x^12 + x^5 + 1 */
#define CRC_CRC16_CCITT_CHECK_VALUE                    0x29B1u /* for "123456789" */

/* CRC_CRC16-> CRC-CRCITT, Initial Value 0xFFFF, reverse databytes, reverse result, Final XOR 0x0000 */
#define CRC_CRC16_REVERSEDPOLYNOMIAL                   0x8408u /* = 0x1021 = x^16 + x^12 + x^5 + 1 */
#define CRC_CRC16_CHECK_VALUE                          0x6F91u /* for "123456789" */

extern uint8_t crc_calculateCRC8(const uint8_t* crc_DataPtr, uint32_t crc_Length, uint8_t crc_StartValue8);
extern uint16_t crc_calculateCRC16(const uint8_t* crc_DataPtr, uint32_t crc_Length, uint16_t crc_StartValue16);
extern uint32_t crc_calculateCRC32(const uint8_t* crc_DataPtr, uint32_t crc_Length, uint32_t crc_StartValue32);

#ifdef __cplusplus
}
#endif

#endif /* _CRC_H_ */
