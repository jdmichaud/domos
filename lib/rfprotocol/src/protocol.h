#ifndef __RFPROTOCOL_H__
#define __RFPROTOCOL_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitop.h"

#define RFP_VERSION 1

#define MAGIC_NUMBER      178

#define WRONG_MAGIC_ERROR 1
#define PARITY_ERROR      2

/*
 * Reminder
 *
 *                  DE AD BE EF    DE AD BE EF
 * address 0 DE <---^  |  |  |      |  |  |  ^---> EF address 0
 * address 1 AD <------   |  |      |  |   ------> BE address 1
 * address 2 BE <---------   |      |   ---------> AD address 2
 * address 3 EF <------------        ------------> DE address 3
 *
 *            Big-Endian                    Little-Endian
 */

/*! \brief Packet structure use as the first element sent in a message
 *
 *  The protocol consist of a header packet, represented by the packet_t
 *  structure and a buffer containing the content of the message.
 *  The header packet contains:
 *  nb of bits  name               comment
 *  ----------------------------------------------------------------------------
 *  8           magic number       Defined in this file, its value is always
 *                                 178. The 433Mhz band is pretty busy so we
 *                                 will be looking at this value to identify
 *                                 one of our messages.
 *  1           parity bit         Used to detect integrity errors in the
 *                                 packet structure.
 *                                 1: the number of 1 in the fields stype, sid,
 *                                    battery replacement, mlength and message
 *                                    is odd,
 *                                 0: otherwise.
 *                                 The parity bit does not include the byte of
 *                                 message after the first one (after the one
 *                                 stored in the header packet)
 *  8           sensor type        8 bits integer representing the type of
 *                                 sensor. See sensor_type.h
 *  8           sensor id          8 bits integer representing the identifier of
 *                                 the sensor.
 *  1           battery            0: Battery level is OK
 *              replacement        1: Battery needs to be replaced
 *              indicator
 *  30          length of the      The total length of the message following the
 *              message            header packet
 *  8           first octet of     The first octet of the message.
 *              the message
 * 0           9          17         25                                         56
 * .... ....  .... ....  .... ....  .... ....  .... ....  .... ....  .... ....  .... ....
 * \^^^ ^^^/  ^\^^ ^^^^  /\^^ ^^^^  /|\^ ^^^^  ^^^^ ^^^^  ^^^^ ^^^^  ^^^^ ^^^/  \^^^ ^^^/
 *     |      |    |          |      |                  |                         \_ message
 *     |      |    |          |      |                  \____________________ message length
 *     |      |    |          |      \________________________ battery replacement indicator
 *     |      |    |          \___________________________________________________ sensor id
 *     |      |    \____________________________________________________________ sensor type
 *     |      \__________________________________________________________________ parity bit
 *     \_______________________________________________________________________ magic number
 */
#define MAGIC_NUMBER_SIZE   8
#define PARITY_SIZE         1
#define SENSOR_TYPE_SIZE    8
#define SENSOR_ID_SIZE      8
#define BATT_REPL_SIZE      1
#define MESSAGE_LENGTH_SIZE 30
#define MESSAGE_SIZE        8

#define MAGIC_NUMBER_OFFSET   0
#define PARITY_OFFSET         MAGIC_NUMBER_SIZE
#define SENSOR_TYPE_OFFSET    PARITY_OFFSET + PARITY_SIZE
#define SENSOR_ID_OFFSET      SENSOR_TYPE_OFFSET + SENSOR_TYPE_SIZE
#define BATT_REPL_OFFSET      SENSOR_ID_OFFSET + SENSOR_ID_SIZE
#define MESSAGE_LENGTH_OFFSET BATT_REPL_OFFSET + BATT_REPL_SIZE
#define MESSAGE_OFFSET        MESSAGE_LENGTH_OFFSET + MESSAGE_LENGTH_SIZE

typedef uint64_t packet_t;

typedef struct {
  uint8_t stype; // Type of the device. See sensor_types.h
  uint8_t sid; // ID of the device
  bool battery_indicator; // 0: battery OK, 1: Battery to be replaced
  uint32_t mlength; // Length of the message
  uint8_t message; // Message
} packet_s;

#ifdef __cplusplus
extern "C"{
#endif


/*! \brief Hamming weigh computation on 64 bits.
 *
 *  Compute the number of 1s in the provided 64 bits parameter.
 */
uint8_t hammingw(uint64_t x);

/*! \brief From various information, create a header packet.
 *
 *  Takes the packet information and the message and provide a header packet and
 *  a pointer of the rest of the message.
 */
void create_packet(uint8_t sensor_type, uint8_t sensor_id,
                   bool battery_indicator, uint32_t message_length,
                   uint8_t *message, packet_t *packet, uint8_t **messagerest);

/*! \brief Convert 64 bit to packet structure.
 *
 *  Convert a raw 64 bits integer to a packet_t structure, checking the magic
 *  number and the parity bit.
 *  Returns:
 *  If the magic number is wrong, returns WRONG_MAGIC_ERROR
 *  If the parity is incorrect, returns PARITY_ERROR
 */
int read_packet(packet_t packet, packet_s *spacket);

/*! \brief Convert a buffer to a header packet and a buffer.
 *
 *  Take a buffer and produce a packet and the rest of the message in
 *  messagerest
 *  Returns:
 *  If the magic number is wrong, returns WRONG_MAGIC_ERROR
 *  If the parity is incorrect, returns PARITY_ERROR
 */
int read_message(uint8_t *buffer, packet_s *packet, uint8_t **messagerest);

#ifdef __cplusplus
}
#endif

#endif // __RFPROTOCOL_H__

