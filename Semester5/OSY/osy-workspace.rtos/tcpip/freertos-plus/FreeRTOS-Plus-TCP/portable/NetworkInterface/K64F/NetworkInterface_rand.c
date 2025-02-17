/*
 * NetworkInterface_rand.c
 *
 */

#include <stdlib.h>

#include "FreeRTOS.h"

UBaseType_t uxRand() { return rand(); }


uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
													uint16_t usSourcePort,
													uint32_t ulDestinationAddress,
													uint16_t usDestinationPort )
{
	return uxRand();
}
