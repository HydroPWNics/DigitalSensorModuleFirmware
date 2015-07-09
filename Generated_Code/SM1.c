/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SM1.c
**     Project     : DigitalSensorModule-Firmware
**     Processor   : MKL05Z16VFK4
**     Component   : SPIMaster_LDD
**     Version     : Component 01.111, Driver 01.02, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-09, 16:31, # CodeGen: 0
**     Abstract    :
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings    :
**          Component name                                 : SM1
**          Device                                         : SPI0
**          Interrupt service/event                        : Enabled
**            Input interrupt                              : INT_SPI0
**            Input interrupt priority                     : medium priority
**            Output interrupt                             : INT_SPI0
**            Output interrupt priority                    : medium priority
**          Settings                                       : 
**            Input pin                                    : Enabled
**              Pin                                        : PTA6/LLWU_P2/TPM0_CH4/SPI0_MISO
**              Pin signal                                 : 
**            Output pin                                   : Enabled
**              Pin                                        : ADC0_SE7/TSI0_IN5/PTA7/IRQ_7/LLWU_P3/SPI0_MISO/SPI0_MOSI
**              Pin signal                                 : 
**            Clock pin                                    : 
**              Pin                                        : ADC0_SE6/TSI0_IN4/PTB0/IRQ_8/LLWU_P4/EXTRG_IN/SPI0_SCK
**              Pin signal                                 : 
**            Chip select list                             : 1
**              Chip select 0                              : 
**                Pin                                      : PTA5/LLWU_P1/RTC_CLK_IN/TPM0_CH5/SPI0_SS_b
**                Pin signal                               : 
**                Active level                             : Low
**            Attribute set list                           : 1
**              Attribute set 0                            : 
**                Width                                    : 8 bits
**                MSB first                                : yes
**                Clock polarity                           : Low
**                Clock phase                              : Capture on leading edge
**                Parity                                   : None
**                Chip select toggling                     : yes
**                Clock rate index                         : 0
**            Clock rate                                   : 0.190735 �s
**            HW input buffer size                         : 1
**            HW input watermark                           : 1
**            Receiver DMA                                 : Disabled
**            HW output buffer size                        : 1
**            HW output watermark                          : 1
**            Transmitter DMA                              : Disabled
**          Initialization                                 : 
**            Initial chip select                          : 0
**            Initial attribute set                        : 0
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnBlockSent                                : Enabled
**              OnBlockReceived                            : Enabled
**              OnError                                    : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init         - LDD_TDeviceData* SM1_Init(LDD_TUserData *UserDataPtr);
**         SendBlock    - LDD_TError SM1_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         ReceiveBlock - LDD_TError SM1_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SM1.c
** @version 01.02
** @brief
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
*/         
/*!
**  @addtogroup SM1_module SM1 module documentation
**  @{
*/         

/* MODULE SM1. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. */

#include "Events.h"
#include "SM1.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" {
#endif 

#define AVAILABLE_EVENTS_MASK (LDD_SPIMASTER_ON_BLOCK_RECEIVED | LDD_SPIMASTER_ON_BLOCK_SENT)

/* These constants contain pins masks */
#define SM1_AVAILABLE_PIN_MASK (LDD_SPIMASTER_INPUT_PIN | LDD_SPIMASTER_OUTPUT_PIN | LDD_SPIMASTER_CLK_PIN | LDD_SPIMASTER_CS_0_PIN)

typedef struct {
  LDD_SPIMASTER_TError ErrFlag;        /* Error flags */
  uint16_t InpRecvDataNum;             /* The counter of received characters */
  uint8_t *InpDataPtr;                 /* The buffer pointer for received characters */
  uint16_t InpDataNumReq;              /* The counter of characters to receive by ReceiveBlock() */
  uint16_t OutSentDataNum;             /* The counter of sent characters */
  uint8_t *OutDataPtr;                 /* The buffer pointer for data to be transmitted */
  uint16_t OutDataNumReq;              /* The counter of characters to be send by SendBlock() */
  LDD_TUserData *UserData;             /* User device data structure */
} SM1_TDeviceData;                     /* Device data structure type */

typedef SM1_TDeviceData* SM1_TDeviceDataPtr; /* Pointer to the device data structure */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static SM1_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/* {Default RTOS Adapter} Global variable used for passing a parameter into ISR */
static SM1_TDeviceDataPtr INT_SPI0__DEFAULT_RTOS_ISRPARAM;
/* Internal method prototypes */

/*
** ===================================================================
**     Method      :  SM1_Init (component SPIMaster_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc.
**         If the "Enable in init. code" is set to "yes" value then the
**         device is also enabled(see the description of the Enable()
**         method). In this case the Enable() method is not necessary
**         and needn't to be generated. 
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* SM1_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  SM1_TDeviceDataPtr DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* Interrupt vector(s) allocation */
  /* {Default RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_SPI0__DEFAULT_RTOS_ISRPARAM = DeviceDataPrv;
  DeviceDataPrv->ErrFlag = 0x00U;      /* Clear error flags */
  /* Clear the receive counters and pointer */
  DeviceDataPrv->InpRecvDataNum = 0x00U; /* Clear the counter of received characters */
  DeviceDataPrv->InpDataNumReq = 0x00U; /* Clear the counter of characters to receive by ReceiveBlock() */
  DeviceDataPrv->InpDataPtr = NULL;    /* Clear the buffer pointer for received characters */
  /* Clear the transmit counters and pointer */
  DeviceDataPrv->OutSentDataNum = 0x00U; /* Clear the counter of sent characters */
  DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
  DeviceDataPrv->OutDataPtr = NULL;    /* Clear the buffer pointer for data to be transmitted */
  /* SIM_SCGC4: SPI0=1 */
  SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;
  /* Interrupt vector(s) priority setting */
  /* NVIC_IPR2: PRI_10=0x80 */
  NVIC_IPR2 = (uint32_t)((NVIC_IPR2 & (uint32_t)~(uint32_t)(
               NVIC_IP_PRI_10(0x7F)
              )) | (uint32_t)(
               NVIC_IP_PRI_10(0x80)
              ));
  /* NVIC_ISER: SETENA|=0x0400 */
  NVIC_ISER |= NVIC_ISER_SETENA(0x0400);
  /* PORTA_PCR6: ISF=0,MUX=3 */
  PORTA_PCR6 = (uint32_t)((PORTA_PCR6 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x04)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x03)
               ));
  /* PORTA_PCR7: ISF=0,MUX=3 */
  PORTA_PCR7 = (uint32_t)((PORTA_PCR7 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x04)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x03)
               ));
  /* PORTB_PCR0: ISF=0,MUX=3 */
  PORTB_PCR0 = (uint32_t)((PORTB_PCR0 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x04)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x03)
               ));
  /* PORTA_PCR5: ISF=0,MUX=3 */
  PORTA_PCR5 = (uint32_t)((PORTA_PCR5 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x04)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x03)
               ));
  /* SPI0_C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=0,CPHA=0,SSOE=1,LSBFE=0 */
  SPI0_C1 = (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK); /* Set configuration register */
  /* SPI0_C2: SPMIE=0,??=0,TXDMAE=0,MODFEN=1,BIDIROE=0,RXDMAE=0,SPISWAI=0,SPC0=0 */
  SPI0_C2 = SPI_C2_MODFEN_MASK;        /* Set configuration register */
  /* SPI0_BR: ??=0,SPPR=0,SPR=1 */
  SPI0_BR = (SPI_BR_SPPR(0x00) | SPI_BR_SPR(0x01)); /* Set baud rate register */
  /* SPI0_C1: SPE=1 */
  SPI0_C1 |= SPI_C1_SPE_MASK;          /* Enable SPI module */
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_SM1_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv); /* Return pointer to the data data structure */
}

/*
** ===================================================================
**     Method      :  SM1_ReceiveBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         This method specifies the number of data to receive. The
**         method returns ERR_BUSY until the specified number of
**         characters is received. The method [CancelBlockReception]
**         can be used to cancel a running receive operation. If a
**         receive operation is not in progress (the method was not
**         called or a previous operation has already finished) all
**         received characters will be lost without any notification.
**         To prevent the loss of data call the method immediately
**         after the last receive operation has finished (e.g. from the
**         [OnBlockReceived] event). This method finishes immediately
**         after calling it - it doesn't wait the end of data reception.
**         Use event [OnBlockReceived] to check the end of data
**         reception or method GetReceivedDataNum to check the state of
**         receiving.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         BufferPtr       - Pointer to A buffer where
**                           received characters will be stored.
**     @param
**         Size            - Size of the block
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous receive request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SM1_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  if (((SM1_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq != 0x00U) { /* Is the previous receive operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->InpDataPtr = (uint8_t*)BufferPtr; /* Store a pointer to the input data. */
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq = Size; /* Store a number of characters to be received. */
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->InpRecvDataNum = 0x00U; /* Set number of received characters to zero. */
  if ((SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) != 0U) {
    (void)SPI_PDD_ReadData8bit(SPI0_BASE_PTR); /* Dummy read of the data register */
  }
  SPI_PDD_EnableInterruptMask(SPI0_BASE_PTR, SPI_PDD_RX_BUFFER_FULL_OR_FAULT); /* Enable Rx buffer full interrupt */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_SendBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         Sends a block of characters. The method returns ERR_BUSY
**         when the previous block transmission is not completed. The
**         method [CancelBlockTransmission] can be used to cancel a
**         transmit operation. This method finishes immediately after
**         calling it - it doesn't wait the end of data transmission.
**         Use event [OnBlockSent] to check the end of data
**         transmission or method GetSentDataNum to check the state of
**         sending.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         BufferPtr       - Pointer to the block of data
**                           to send.
**     @param
**         Size            - Number of characters in the buffer.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous transmit request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SM1_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  if (((SM1_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq != 0x00U) { /* Is the previous transmit operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->OutDataPtr = (uint8_t*)BufferPtr; /* Set a pointer to the output data. */
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq = Size; /* Set the counter of characters to be sent. */
  ((SM1_TDeviceDataPtr)DeviceDataPtr)->OutSentDataNum = 0x00U; /* Clear the counter of sent characters. */
  SPI_PDD_EnableInterruptMask(SPI0_BASE_PTR, SPI_PDD_TX_BUFFER_EMPTY); /* Enable Tx buffer empty interrupt */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_Interrupt (component SPIMaster_LDD)
**
**     Description :
**         The ISR function handling the device receive/transmit 
**         interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(SM1_Interrupt)
{
  /* {Default RTOS Adapter} ISR parameter is passed through the global variable */
  SM1_TDeviceDataPtr DeviceDataPrv = INT_SPI0__DEFAULT_RTOS_ISRPARAM;
  uint8_t StatReg = SPI_PDD_ReadStatusReg(SPI0_BASE_PTR); /* Read status register */

  (void)DeviceDataPrv;                 /* Supress unused variable warning if needed */
  if ((StatReg & SPI_PDD_RX_BUFFER_FULL) != 0U) { /* Is any char in HW Rx buffer? */
    if (DeviceDataPrv->InpDataNumReq != 0x00U) { /* Is the receive block operation pending? */
      *(DeviceDataPrv->InpDataPtr++) = SPI_PDD_ReadData8bit(SPI0_BASE_PTR); /* Put a character to the receive buffer and increment pointer to receive buffer */
      DeviceDataPrv->InpRecvDataNum++; /* Increment received char. counter */
      if (DeviceDataPrv->InpRecvDataNum == DeviceDataPrv->InpDataNumReq) { /* Is the requested number of characters received? */
        SPI_PDD_DisableInterruptMask(SPI0_BASE_PTR, SPI_PDD_RX_BUFFER_FULL_OR_FAULT); /* Disable Rx buffer full interrupt */
        DeviceDataPrv->InpDataNumReq = 0x00U; /* If yes then clear number of requested characters to be received. */
        SM1_OnBlockReceived(DeviceDataPrv->UserData);
      }
    }
  }
  if ((StatReg & SPI_PDD_TX_BUFFER_EMPTYG) != 0U) { /* Is HW Tx buffer empty? */
    if (DeviceDataPrv->OutSentDataNum < DeviceDataPrv->OutDataNumReq) { /* Is number of sent characters less than the number of requested incoming characters? */
      SPI_PDD_WriteData8Bit(SPI0_BASE_PTR, (*((uint8_t *)DeviceDataPrv->OutDataPtr++))); /* Put a character with command to the transmit register and increment pointer to the transmitt buffer */
      DeviceDataPrv->OutSentDataNum++; /* Increment the counter of sent characters. */
      if (DeviceDataPrv->OutSentDataNum == DeviceDataPrv->OutDataNumReq) {
        DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
        SM1_OnBlockSent(DeviceDataPrv->UserData);
      }
    } else {
      SPI_PDD_DisableInterruptMask(SPI0_BASE_PTR, SPI_PDD_TX_BUFFER_EMPTY); /* Disable TX interrupt */
    }
  }
}

/*lint -restore Enable MISRA rule (11.4) checking. */
/* END SM1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
