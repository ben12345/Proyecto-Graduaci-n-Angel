/**
 * @file: node_0.c
 *
 * @brief: MCAPI example implementation (5.4.3)
 * The scenario contains the execution of a DSP algorithm,
 * such as a FIR filter, on a multi-core processor system
 * containing a general-purpose processor (GPP) and DSP.
 * The high-level steps are:
 * 1. Initialize communication channels between processors.
 * 2. GPP sends data for processing to DSP.
 * 3. GPP sends code to execute to DSP
 * 4. GPP signals for the DSP to begin execution.
 *
 * Target: BF609-eZKit
 * Compiler: CCES
 * Output format: CCES "*.dxe"
 *
 * Copyright(c) 2011 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 */

#include "BF609_MCAPI_msg_Core0.h"

#include <stdio.h>
#include <assert.h>
#include "adi_common.h"

/**
 * buffer used by core A to send and receive buffer.
 */
unsigned char buffer[MSG_MAX];

/**
 * Commands are sent via local command end-point
 */
mcapi_endpoint_t command_endpoint;
/**
 * Data is sent via local data end-point
 */
mcapi_endpoint_t data_endpoint;
/**
 * Data is received via local data recv end-point
 */
mcapi_endpoint_t data_recv_endpoint;

/**
 * Remote endpoint on DSP to receive command
 */
mcapi_endpoint_t remote_command_endpoint;
/**
 * Remote endpoint on DSP to receive data
 */
mcapi_endpoint_t remote_data_endpoint;
/**
 * Remote endpoint on DSP to send command
 */
mcapi_endpoint_t remote_data_recv_endpoint;

/**
 * Function declarations
 */
mcapi_status_t initialize_comms(void);

/**
 * Creates local end-points on GPP and gets remote DSP end-points *
 * @return MCAPI_SUCCESS/error
 */
mcapi_status_t initialize_comms(void) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /**
   * Create local end-points for communication
   */
#ifdef DEBUG_INFO
  printf("[CORE A]: Initializing communication\n");
#endif
  command_endpoint = mcapi_endpoint_create(PORT_COMMAND, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif

  data_endpoint = mcapi_endpoint_create(PORT_DATA, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  data_recv_endpoint = mcapi_endpoint_create(PORT_DATA_RECV, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_create failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  /**
   * Get the remote end-points for communication
   */
  remote_command_endpoint = mcapi_endpoint_get(DOMAIN, NODE_CORE_1,
      PORT_COMMAND, MCAPI_TIMEOUT_INFINITE, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_get failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  remote_data_endpoint = mcapi_endpoint_get(DOMAIN, NODE_CORE_1, PORT_DATA,
      MCAPI_TIMEOUT_INFINITE, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_get failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  remote_data_recv_endpoint = mcapi_endpoint_get(DOMAIN, NODE_CORE_1,
      PORT_DATA_RECV, MCAPI_TIMEOUT_INFINITE, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_get failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif

  data_endpoint  = data_recv_endpoint;
  command_endpoint = data_recv_endpoint ;
  remote_data_endpoint = remote_data_recv_endpoint;
  remote_command_endpoint = remote_data_recv_endpoint;

  return status;
}

/**
 * Sending data from GPP to DSP
 * @param data data to be sent
 * @param size size of the data to be sent
 * @return MCAPI_SUCCESS/error code
 */
mcapi_status_t send_data(void *data, mcapi_uint32_t size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  int count = 0;
  int tx = 0;

  /** Sanity Check */
  assert(NULL != data);

  count = size;
  while (count >= MCAPI_MAX_MSG_SIZE) {

    count -= MCAPI_MAX_MSG_SIZE;
    tx = MCAPI_MAX_MSG_SIZE;

    mcapi_msg_send(data_endpoint, remote_data_endpoint, data, tx, 0, &status);
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
    data = (unsigned char *) data + MCAPI_MAX_MSG_SIZE;

  }

  mcapi_msg_send(data_endpoint, remote_data_endpoint, data, count, 0, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

/**
 * Sends commands over the command channel
 * @param cmd command
 * @return MCAPI_SUCCESS/error code
 */
mcapi_status_t send_dsp_cmd(mcapi_uint32_t cmd) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Send DSP command */
  mcapi_msg_send(command_endpoint, remote_command_endpoint, (void *) &cmd,
      sizeof(mcapi_uint32_t), 0, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

/**
 * Read the data back
 * @param dst buffer of received data
 * @param size size of the received data
 * @return MCAPI_SUCCESS/error
 */
mcapi_status_t read_data(void *dst, mcapi_uint32_t *size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  size_t rx = 0;
  int count = 0;

  /** sanity check */
  assert(NULL != dst);
  assert(NULL != size);

  *size = 0;
  do {
    mcapi_msg_recv(data_recv_endpoint, dst, MCAPI_MAX_MSG_SIZE, &rx, &status);
#ifdef DEBUG_INFO
    if (MCAPI_SUCCESS != status) {
      fprintf(stdout, "[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__,
          __LINE__, status);
      exit(1);
    }
#endif
    *size += rx;
    dst = (unsigned char *) dst + MCAPI_MAX_MSG_SIZE;

  } while (MCAPI_MAX_MSG_SIZE == rx);

  return status;
}

/**
 * Close all the ports and shutdown
 * @return MCAPI_SUCCESS/error code
 */
mcapi_status_t shutdown_comms(void) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Delete the end-points */
  mcapi_endpoint_delete(command_endpoint, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  mcapi_endpoint_delete(data_endpoint, &status);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  mcapi_endpoint_delete(data_recv_endpoint, &status);
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
 * Example implementation for init graphics buffer
 * @param data data to be sent for processing
 * @param data_size size of the data
 * @return
 */
mcapi_status_t perform_init_function(void *data, int data_size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  mcapi_uint32_t size = 0;
  uint32_t *ptrchk = data;

  /** Send command and data for processing to the DSP*/
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending command to CORE B: (DSP_DATA)\n");
#endif
  status = send_dsp_cmd(DSP_DATA);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_dsp_cmd failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending data to CORE B\n");
#endif
  status = send_data(data, data_size);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_data failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  /**
   * Send command to init the graphics buffers
   */
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending command to CORE B: (DSP_INIT)\n");
#endif
  status = send_dsp_cmd(DSP_INIT);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_dsp_cmd failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
#ifdef DEBUG_INFO
  printf("[CORE A]: Receive processed data back from CORE B\n");
#endif
  status = read_data(ptrchk+1, &size);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d read_data failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}

/**
 * Example implementation for contour detection and updating dot count
 * @param data data to be sent for processing
 * @param data_size size of the data
 * @return
 */
mcapi_status_t perform_process_function(void *data, int data_size) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;
  mcapi_uint32_t size = 0;
  MESSAGE_INFO *pInfo = data;

  /** Send command and data for processing to the DSP*/
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending command to CORE B: (DSP_DATA)\n");
#endif
  status = send_dsp_cmd(DSP_DATA);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_dsp_cmd failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending data to CORE B\n");
#endif
  status = send_data(data, data_size);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_data failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  /**
   * Send command to update dot count
   */
#ifdef DEBUG_INFO
  printf("[CORE A]: Sending command to CORE B: (DSP_GFXCONTOUR)\n");
#endif
  status = send_dsp_cmd(DSP_GFXCONTOUR);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d send_dsp_cmd failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
#ifdef DEBUG_INFO
  printf("[CORE A]: Receive processed data back from CORE B\n");
#endif
 // status = read_data(pInfo, &size);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d read_data failure\n", __FILE__,
        __LINE__, status);
    exit(1);
  }
#endif
  return status;
}


mcapi_status_t get_data_from_coreb(void )
{
	 mcapi_status_t status = MCAPI_ERR_GENERAL;
	  MESSAGE_INFO *pInfo =( MESSAGE_INFO *)buffer;
	 uint32_t size;
	 status = read_data(pInfo, &size);
	 return(status);
}

/** Initialize the buffer */
int data_init(unsigned char *buff, int size) {

  int count = 0;

  assert( NULL != buff);

  for (count = 0; count < size; count++) {
    buff[count] = 0xA5;
  }

  return PASS;
}

mcapi_status_t data_cmp(unsigned char *buff, int size) {

  int count = 0;

  assert( NULL != buff);

  for (count = 0; count < size; count++) {
    if (0xAA != buff[count]) {
      return MCAPI_ERR_GENERAL;
    }
  }

  return MCAPI_SUCCESS;
}

/**
 * Start point for node 1 init
 * @return PASS/FAIL
 */
int main_node0_init(const mcapi_info_t * const mcapi_info) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Sanity Check */
  assert( NULL != mcapi_info);

  status = perform_init_function(buffer, 4);
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d perform_init_function failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  return PASS;
}

/**
 * Start point for node 1 graphics & contour
 * @return PASS/FAIL
 */
int main_node0_process(const mcapi_info_t * const mcapi_info) {

  mcapi_status_t status = MCAPI_ERR_GENERAL;

  /** Sanity Check */
  assert( NULL != mcapi_info);

  status = perform_process_function(buffer, sizeof(MESSAGE_INFO));
#ifdef DEBUG_INFO
  if (MCAPI_SUCCESS != status) {
    fprintf(stdout, "[%s] %d status: %d perform_process_function failure\n",
        __FILE__, __LINE__, status);
    exit(1);
  }
#endif
  return PASS;
}


