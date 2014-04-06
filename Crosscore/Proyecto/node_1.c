//-----------------------------------------------------------------------------------------------
//
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD       TTTTTTTTTTTTTTTTTTTTTTT  iiii
//     B::::::::::::::::B  D::::::::::::DDD    T:::::::::::::::::::::T i::::i
//     B::::::BBBBBB:::::B D:::::::::::::::DD  T:::::::::::::::::::::T  iiii
//     BB:::::B     B:::::BDDD:::::DDDDD:::::D T:::::TT:::::::TT:::::T
//       B::::B     B:::::B  D:::::D    D:::::DTTTTTT  T:::::T  TTTTTTiiiiiii
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T        i:::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B:::::::::::::BB    D:::::D     D:::::D       T:::::T         i::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D    D:::::D        T:::::T         i::::i
//     BB:::::BBBBBB::::::BDDD:::::DDDDD:::::D       TT:::::::TT      i::::::i
//     B:::::::::::::::::B D:::::::::::::::DD        T:::::::::T      i::::::i
//     B::::::::::::::::B  D::::::::::::DDD          T:::::::::T      i::::::i
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD             TTTTTTTTTTT      iiiiiiii
//
//-----------------------------------------------------------------------------------------------
// ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
//
// BDTI provides expert engineering services, including algorithm design and implementation of
// software for embedded vision applications. For questions regarding this demo or information
// on BDTI’s engineering services, contact BDTI:
//
// info@BDTI.com                      +1 510-451-1800                   http://www.BDTI.com
//
// Berkeley Design Technology, Inc.(BDTI) makes no warranty for the use of this software, which
// has been developed for demonstration purposes only.  This software is provided  "as is".  BDTI
// assumes no responsibility for any errors which may appear in this software, nor does it make
// a commitment to update the information contained herein. BDTI specifically disclaims any implied
// warranties of fitness for a particular purpose.  You are solely responsible for determining
// the suitability of this software for your purposes and you use it at your own risk. In no event
// shall BDTI be held liable for any damages of any kind arising out of your use of or inability to
// use this software.
//-----------------------------------------------------------------------------------------------
/**
 * @file: node_1.c
 *
 * @brief: MCAPI Node 1 DSP implementation
 *
 * Target: BF609-eZKit
 * Compiler: CCES
 * Output format: CCES "*.dxe"
 *
 * @author    Rohan Kangralkar
 *
 * LastChange:
 * $Id: node_1.c 9399 2012-11-09 05:09:43Z gprasad $
 *
 * Copyright (c) 2011 Analog Devices, Inc.
 */

#include "BF609_MCAPI_msg_Core0.h"
#include <stdio.h>
#include <assert.h>
#include "adi_common.h"
#include "adi_sigprocess.h"

/**
 * buffer used by core A to send and receive buffer.
 */
unsigned char buffer[MSG_MAX];
extern void InitTitle(void *ptr);
extern void update_contour_count(void);
extern uint32_t CONTOURCount;
#if defined(BF609_VSK_SHOW_LIGHTING_ON_MONITOR)
extern uint32_t Illumination;
#endif
extern VIDEO_BUF *pGFxBuff;
MESSAGE_INFO *pInfo = (MESSAGE_INFO *)&buffer[0];
/**
 * Commands are received via local command end-point
 */
mcapi_endpoint_t dsp_command_endpoint;
/**
 * Data is received via local command end-point
 */
mcapi_endpoint_t dsp_data_endpoint;
/**
 * Data is sent via local command end-point
 */
mcapi_endpoint_t dsp_data_send_endpoint;\

/**
 * Data is sent to remote receive data end-point
 */
mcapi_endpoint_t remote_data_recv_endpoint;

/**
 * Create endpoints and connection for DSP
 * @return MCAPI_SUCCESS/Error Code
 */
mcapi_status_t dsp_initialize(void) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /**
   * Create end-point for data and command transfer
   */
  dsp_command_endpoint = mcapi_endpoint_create(PORT_COMMAND, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  dsp_data_endpoint = mcapi_endpoint_create(PORT_DATA, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  dsp_data_send_endpoint = mcapi_endpoint_create(PORT_DATA_RECV, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  /**
   * Get the remote endpoints for communication
   */
  remote_data_recv_endpoint = mcapi_endpoint_get(DOMAIN, NODE_CORE_0,
      PORT_DATA_RECV, MCAPI_TIMEOUT_INFINITE, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_get failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif

  dsp_data_endpoint = dsp_data_send_endpoint;
  dsp_command_endpoint = dsp_data_send_endpoint;

  return status;
}

/**
 * closes the ports and shutsdown the DSP
 * @return MCAPI_SUCCESS/ MCAPI error code
 */
mcapi_status_t dsp_shutdown(void) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Delete the end-points */
  mcapi_endpoint_delete(dsp_command_endpoint, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  mcapi_endpoint_delete(dsp_data_endpoint, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  mcapi_endpoint_delete(dsp_data_send_endpoint, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

/**
 * dsp receive dsp cmd
 * @return MCAPI_SUCCESS/ error code
 */
mcapi_status_t receive_dsp_cmd(mcapi_uint32_t *cmd) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  size_t rcv_size = 0;

  /** Sanity Check */
  assert(NULL != cmd);

  /** receive the command */
  mcapi_msg_recv(dsp_command_endpoint, (void *) cmd, sizeof(mcapi_uint32_t),
      &rcv_size, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

/**
 * dsp receive dsp cmd
 * @return MCAPI_SUCCESS/ error code
 */
mcapi_status_t receive_dsp_data(void *data, mcapi_uint32_t *size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  size_t rx = 0;
  int count = 0;

  /** Sanity Check */
  assert(NULL != data);
  assert(NULL != size);

  *size = 0;
  do {
    mcapi_msg_recv(dsp_data_endpoint, data, MCAPI_MAX_MSG_SIZE, &rx, &status);
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
    *size += rx;
    data = (unsigned char *) data + MCAPI_MAX_MSG_SIZE;

  } while (MCAPI_MAX_MSG_SIZE == rx);

  return status;
}

/**
 * Sending data from DSP to GPP
 * @param data data to be sent
 * @param size size of the data to be sent
 * @return MCAPI_SUCCESS/error code
 */
mcapi_status_t send_dsp_data(void *data, mcapi_uint32_t size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  int count = 0;
  int tx = 0;

  /** Sanity Check */
  assert(NULL != data);

  count = size;
  while (count >= MCAPI_MAX_MSG_SIZE) {

    count -= MCAPI_MAX_MSG_SIZE;
    tx = MCAPI_MAX_MSG_SIZE;

    mcapi_msg_send(dsp_data_send_endpoint, remote_data_recv_endpoint, data, tx,
        0, &status);
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
    data = (unsigned char *) data + MCAPI_MAX_MSG_SIZE;

  }

  mcapi_msg_send(dsp_data_send_endpoint, remote_data_recv_endpoint, data, count,
      0, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

void process_data(void) {

  int count = 0;

  for (count = 0; count < sizeof(buffer); count++) {
    buffer[count] += 0x5;
  }
}

/**
 * DSP command loop
 * @return MCAPI_SUCCESS/ error code
 */
mcapi_status_t dsp_command_loop(void) {
FILE *fp1;
uint32_t FrameNumber = 0;

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  mcapi_uint32_t command = DSP_READY,nCurrContourCount=0;
  mcapi_uint32_t rx_size = 0;
  MT9M114_VIDEO_BUF *pVideoBuff;
  uint32_t *pTemp = (uint32_t *)&buffer[0];

  while (command != DSP_TERMINATE) {
    status = receive_dsp_cmd(&command);
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d receive_dsp_cmd failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
    switch (command) {
    case DSP_DATA:
#ifdef DEBUG_INFO
      printf("[CORE B]: Received command (DSP_DATA)\n");
#endif
      status = receive_dsp_data(buffer, &rx_size);
#ifdef DEBUG_INFO
      if (MCAPI_SUCCESS != status) {
        fprintf(stdout, "[%s] %d status: %d receive_dsp_data failure\n",
            __FILE__, __LINE__, status);
        exit(1);
      }
#endif
#ifdef DEBUG_INFO
      printf("[CORE B]: Received data\n");
#endif
      break;

    case DSP_CODE:
#ifdef DEBUG_INFO
      printf("[CORE B]: Received command (DSP_CODE)\n");
#endif
      break;

    case DSP_INIT:
      /* Tell DSP to Execute – Assume code writes to result_buffer */
#ifdef DEBUG_INFO
      printf("[CORE B]: Received command (DSP_INIT)\n");
#endif
      InitTitle((void*)(*pTemp));
#ifdef DEBUG_INFO
      printf("[CORE B]: Processing complete\n");
      printf("[CORE B]: Sending data\n");
#endif
      status = send_dsp_data(buffer, rx_size);
#ifdef DEBUG_INFO
      if (MCAPI_SUCCESS != status) {
        fprintf(stdout, "[%s] %d status: %d send_dsp_data failure\n", __FILE__,
            __LINE__, status);
        exit(1);
      }
#endif
#ifdef DEBUG_INFO
      printf("[CORE B]: Sending data complete\n");
#endif
      break;

    case DSP_GFXCONTOUR:
      /* Tell CORE B to find contour & update dot count */
#ifdef DEBUG_INFO
      printf("[CORE B]: Received command (DSP_GFXCONTOUR)\n");
#endif
      pGFxBuff =(VIDEO_BUF *) pInfo->pBuf2;
      if(pInfo->pBuf1 != NULL)
      {
        if((nCurrContourCount = ADI_FindContours(pInfo->pBuf1, pInfo->nWidth,pInfo->nHeight))!= 0XFFFFFFFF)
        {
        	CONTOURCount =nCurrContourCount;
#if 0
// EMG start
   if( (FrameNumber++ > 20) && (CONTOURCount != 72) )
   {
	   // Moved here for dropbox indication
       fp1=fopen( "FindContours.dat","wb");
       printf( "Capturing data - \n" );
       fwrite( pInfo->pBuf1, 1, (720*480), fp1);
       fclose(fp1);
       printf( "Data Captured\n" );
       while(1);
   }
// EMGend
#endif
        }

      }
#if defined(BF609_VSK_SHOW_LIGHTING_ON_MONITOR)
      Illumination = pInfo->nIllumination;
#endif	  
      update_contour_count();
      /* Mark the buffer valid for submitting to display.*/
  	  pVideoBuff =  (MT9M114_VIDEO_BUF *)pInfo->pBuf2;
	  pVideoBuff->eStatus = CONTOUR_COUNT_BUFFER_READY;
      pInfo->nCompletionFlag = 1; /* to indicate that core B has completed processing */
#ifdef DEBUG_INFO
      printf("[CORE B]: Processing complete\n");
      printf("[CORE B]: Sending data\n");
#endif
      status = send_dsp_data(buffer, rx_size);
#ifdef DEBUG_INFO
      if (MCAPI_SUCCESS != status) {
        fprintf(stdout, "[%s] %d status: %d send_dsp_data failure\n", __FILE__,
            __LINE__, status);
        exit(1);
      }
#endif
#ifdef DEBUG_INFO
      printf("[CORE B]: Sending data complete\n");
#endif
      break;

    case DSP_TERMINATE:
#ifdef DEBUG_INFO
      printf("[CORE B]: Received command (DSP_TERMINATE)\n");
#endif
      status = dsp_shutdown();
#ifdef DEBUG_INFO
      if (MCAPI_SUCCESS != status) {
        fprintf(stdout, "[%s] %d status: %d dsp_shutdown failure\n", __FILE__,
            __LINE__, status);
        exit(1);
      }
#endif
      break;
    default:
      break;
    }
  }

  return status;
}

/**
 * Start point for node 1
 * @return PASS/FAIL
 */
int main_node1(const mcapi_info_t * const mcapi_info) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Sanity Check */
  assert( NULL != mcapi_info);

  status = dsp_initialize();
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d dsp_initialize failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
  status = dsp_command_loop();
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d dsp_command_loop failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return PASS;
}
