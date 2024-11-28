/*
 * NetworkInterface.c
 *
 */

#include <stdlib.h>

#include <fsl_enet.h>
#include <fsl_phy.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"

#define driver_DEBUG_PRINTF 0

#if ( driver_DEBUG_PRINTF != 0 )
#if ( ipconfigHAS_DEBUG_PRINTF == 1 )
#include <fsl_debug_console.h>
#endif
#endif

#define ENET_Base 		ENET
#define ENET_PHYAdr 	0x00U
#define CORE_CLK_FREQ 	CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ENET_RXBD_NUM 	(4)
#define ENET_TXBD_NUM 	(4)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)

AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
          ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
          ENET_BUFF_ALIGNMENT);

enet_handle_t g_enet_handle;
TaskHandle_t g_xRxTaskHandle = NULL;
volatile TaskHandle_t g_xTxTaskHandle = NULL;

static void vRecvTask( void * );

static void ethernet_callback( ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param );

BaseType_t xNetworkInterfaceInitialise( void )
{
#if ( driver_DEBUG_PRINTF != 0 )
	FreeRTOS_printf( ( "xNetworkInterfaceInitialise...\r\n" ) );
#endif

	enet_config_t config;
	status_t status;
	BaseType_t xstatus;
	bool link = false;

	/* prepare the buffer configuration. */
	enet_buffer_config_t buffConfig[] = {{
	        ENET_RXBD_NUM,
	        ENET_TXBD_NUM,
	        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
	        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
	        &g_rxBuffDescrip[0],
	        &g_txBuffDescrip[0],
	        &g_rxDataBuff[0][0],
	        &g_txDataBuff[0][0],
	    }};

	ENET_GetDefaultConfig( &config );

	uint32_t init_retry_count = 5;
	status = PHY_Init(ENET_Base, ENET_PHYAdr, CORE_CLK_FREQ);
	while ( ( status != kStatus_Success ) && ( init_retry_count-- ) )
	{
#if ( driver_DEBUG_PRINTF != 0 )
		FreeRTOS_debug_printf( ( "\r\nPHY Auto-negotiation failed. Please check the cable connection and link partner setting. Retry count-down %d.\r\n", init_retry_count ) );
#endif
		status = PHY_Init(ENET_Base, ENET_PHYAdr, CORE_CLK_FREQ);
	}
	if ( status != kStatus_Success ) return pdFAIL;

	status = PHY_GetLinkStatus(ENET_Base, ENET_PHYAdr, &link);
	if ( status != kStatus_Success ) return pdFAIL;

	if ( link )
	{
		phy_speed_t speed;
		phy_duplex_t duplex;
		/* Get the actual PHY link speed. */
		PHY_GetLinkSpeedDuplex(ENET_Base, ENET_PHYAdr, &speed, &duplex);
		/* Change the MII speed and duplex for actual link status. */
		config.miiSpeed  = (enet_mii_speed_t)speed;
		config.miiDuplex = (enet_mii_duplex_t)duplex;
	}

	xstatus = xTaskCreate( vRecvTask, "RecvTask", configMINIMAL_STACK_SIZE + 128, NULL, configMAX_PRIORITIES - 1, &g_xRxTaskHandle );
	if ( xstatus != pdPASS ) return pdFAIL;

	NVIC_SetPriority( ENET_Receive_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1 );
	NVIC_SetPriority( ENET_Transmit_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1 );

	config.interrupt |= kENET_RxFrameInterrupt | kENET_TxFrameInterrupt | kENET_TxBufferInterrupt;

	ENET_Init(ENET_Base, &g_enet_handle, &config, &buffConfig[0], ipLOCAL_MAC_ADDRESS, CORE_CLK_FREQ );

	ENET_SetCallback(&g_enet_handle, ethernet_callback, NULL );

	ENET_ActiveRead(ENET_Base);

	return pdPASS;
}

BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxNetworkBuffer, BaseType_t xReleaseAfterSend )
{
    status_t result;
    uint32_t count = 4;

	do
	{
		result = ENET_SendFrame(ENET_Base, &g_enet_handle, pxNetworkBuffer->pucEthernetBuffer,  pxNetworkBuffer->xDataLength );

		if ( result == kStatus_ENET_TxFrameBusy )
		{
			g_xTxTaskHandle = xTaskGetCurrentTaskHandle();

			ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS( 1000 ) );
		}

	} while ( ( result == kStatus_ENET_TxFrameBusy ) && ( count-- ) );

	g_xTxTaskHandle = NULL;
#if ( driver_DEBUG_PRINTF != 0 )
	FreeRTOS_debug_printf( ( "MAC send %d\r\n", pxNetworkBuffer->xDataLength ) );
#endif

	if( xReleaseAfterSend != pdFALSE )
	{
		/* It is assumed SendData() copies the data out of the FreeRTOS+TCP Ethernet
		buffer.  The Ethernet buffer is therefore no longer needed, and must be
		freed for re-use. */
		vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
	}

	if ( result == kStatus_Success )
	{
		iptraceNETWORK_INTERFACE_TRANSMIT();
		return pdPASS;
	}

	return pdFAIL;
}

static void vRecvTask( void *pvParameters )
{
	NetworkBufferDescriptor_t *pxBufferDescriptor;
	uint32_t length;
	status_t result;
	IPStackEvent_t xRxEvent;
#if ( driver_DEBUG_PRINTF != 0 )
	FreeRTOS_printf( ( "vRecvTask started...\r\n" ) );
#endif

    for( ;; )
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
#if ( driver_DEBUG_PRINTF != 0 )
        FreeRTOS_debug_printf( ( "vRecvTask notified...\r\n" ) );
#endif
        for( ;; )
        {

			result = ENET_GetRxFrameSize( &g_enet_handle, &length );
#if ( driver_DEBUG_PRINTF != 0 )
			FreeRTOS_debug_printf( ( "Recv frame %d %d\r\n", result, length ) );
#endif

			if( result == kStatus_Success )
			{
				pxBufferDescriptor = pxGetNetworkBufferWithDescriptor( length, portMAX_DELAY );

				if( pxBufferDescriptor != NULL )
				{
					result = ENET_ReadFrame( ENET_Base, &g_enet_handle, pxBufferDescriptor->pucEthernetBuffer, length);
#if ( driver_DEBUG_PRINTF != 0 )
					FreeRTOS_debug_printf( ( "Recv frame from MAC\r\n", result, length ) );
#endif
					pxBufferDescriptor->xDataLength = length;
					if( eConsiderFrameForProcessing( pxBufferDescriptor->pucEthernetBuffer )
																		  == eProcessBuffer )
					{
#if ( driver_DEBUG_PRINTF != 0 )
						FreeRTOS_debug_printf( ( "Recv process...\r\n" ) );
#endif
						xRxEvent.eEventType = eNetworkRxEvent;
						xRxEvent.pvData = ( void * ) pxBufferDescriptor;

						if( xSendEventStructToIPTask( &xRxEvent, 0 ) == pdFALSE )
						{
							vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );

							iptraceETHERNET_RX_EVENT_LOST();
						}
						else
						{
							iptraceNETWORK_INTERFACE_RECEIVE();
						}
					}
					else
					{
#if ( driver_DEBUG_PRINTF != 0 )
						FreeRTOS_debug_printf( ( "Recv NOprocess...\r\n" ) );
#endif
						vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );
					}
				}
				else
				{
#if ( driver_DEBUG_PRINTF != 0 )
					FreeRTOS_debug_printf( ( "Recv No buffer...\r\n" ) );
#endif
					iptraceETHERNET_RX_EVENT_LOST();
				}
			}
			else if ( result == kStatus_ENET_RxFrameError )
			{
#if ( driver_DEBUG_PRINTF != 0 )
				FreeRTOS_debug_printf( ( "Recv Frame Error...\r\n" ) );
#endif
				ENET_ReadFrame( ENET_Base, &g_enet_handle, NULL, 0 );
			}
			else if ( result == kStatus_ENET_RxFrameEmpty )
			{
#if ( driver_DEBUG_PRINTF != 0 )
				FreeRTOS_debug_printf( ( "Recv Frame Empty...\r\n" ) );
#endif
				break;
			}
        } // for ()
    } // for ()
}

static void ethernet_callback( ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param )
{
	portBASE_TYPE taskToWake = pdFALSE;

    switch (event)
    {
        case kENET_RxEvent:
        {
#if ( driver_DEBUG_PRINTF != 0 )
        	FreeRTOS_debug_printf( ( "RxEvent\r\n" ) );
#endif
        	vTaskNotifyGiveFromISR( g_xRxTaskHandle, &taskToWake );

			portYIELD_FROM_ISR( taskToWake );

            break;
        }
        case kENET_TxEvent:
        {
            if ( g_xTxTaskHandle ) vTaskNotifyGiveFromISR( g_xTxTaskHandle, &taskToWake );

            portYIELD_FROM_ISR( taskToWake );

            break;
        }

        default:
            break;
    }
}

