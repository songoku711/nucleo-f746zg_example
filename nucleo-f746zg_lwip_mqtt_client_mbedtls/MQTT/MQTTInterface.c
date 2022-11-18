/*
 * MQTTInterface.c
 *
 *  Created on: Nov 12, 2022
 *      Author: cuhuy
 */

#include "MQTTInterface.h"
#include "stm32f7xx_hal.h"

#include MBEDTLS_CONFIG_FILE
#include "mbedtls/platform.h"

#include <string.h>
#include "lwip.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include <string.h>

#define SERVER_PORT "8883"
#define SERVER_NAME "192.168.10.12"

#define DEBUG_LEVEL 1

const char mbedtls_root_certificate[] =
    "-----BEGIN CERTIFICATE-----\r\n"
    "MIIDpTCCAo2gAwIBAgIUXSrnkf/1Sb4jNRsMtNjpoA8OPb0wDQYJKoZIhvcNAQEL\r\n"
    "BQAwYjELMAkGA1UEBhMCVk4xDjAMBgNVBAgMBUhhbm9pMQ4wDAYDVQQHDAVIYW5v\r\n"
    "aTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMRAwDgYDVQQDDAdM\r\n"
    "b25nQ0gyMB4XDTIyMTExNzE0NTAzN1oXDTI1MDgxMzE0NTAzN1owYjELMAkGA1UE\r\n"
    "BhMCVk4xDjAMBgNVBAgMBUhhbm9pMQ4wDAYDVQQHDAVIYW5vaTEhMB8GA1UECgwY\r\n"
    "SW50ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMRAwDgYDVQQDDAdMb25nQ0gyMIIBIjAN\r\n"
    "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA36Ag0z+H+hVl8gm9nTeHpAeyXpnQ\r\n"
    "rvaOUxfTNTgSHD09DT3aDN6NqsOLTwOz0tuVhSEl11YrNwixGCdVCNOm4OBGjm5v\r\n"
    "f0EXMyVxjCELDzRsW/RX/J99vmr0n7qRFYZ7vbMu7gGZrIO1IJSDljAB7E/NJXOn\r\n"
    "u8Gib2DtfYmS++TCJFCRxnO/4g2m+2h1gxzT1VcoWZ7+hiY8eF101SyXm4Ye8N/1\r\n"
    "t32fhhaavVGilZw6ZCCCNQqsRiXA1GWLw90olbZ3wTq7nhJS9SG5YqaUCOt86WgJ\r\n"
    "9qkwtKhAYSoMHJH4Vj8CVrQdHQ2HD7QxvVO6rseGBj1sV7N/LzJXY2Dq5wIDAQAB\r\n"
    "o1MwUTAdBgNVHQ4EFgQUjQ7m1ScrLBkRlP55hvQfUFFTszQwHwYDVR0jBBgwFoAU\r\n"
    "jQ7m1ScrLBkRlP55hvQfUFFTszQwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0B\r\n"
    "AQsFAAOCAQEAosPzysqQxs9RvY9HEShSxNpv+dUMKvGBiD59BcO5zvbywYmVSMQ/\r\n"
    "/gWtakx9h7IOvYc3FVnGH7YkQpgl6IQMeR7C+YtehsXMJqiidN2pvxnZMT5ZfYWf\r\n"
    "TgFpYSIDLawuH6p9E9KjjQFaa43e63dCl49m1GONPD4Go2bc7I6vhpcgQYeB4s1d\r\n"
    "/G89mc/bc+5CcS+xy+FPvwM8f9vsdhLNVRFy2mczNhjwWkZQltcY91nsY98tShno\r\n"
    "Xd/bQJL6+L2en9q7Z7jTbv7FaS6jIbLJC6l1Elij5382GSDhpExb78+ZdL3BASlp\r\n"
    "gm7zS1WkbvsUcEORzZXvMUGqqG0K8Ye6pA==\r\n"
    "-----END CERTIFICATE-----\r\n";

const size_t mbedtls_root_certificate_len = sizeof(mbedtls_root_certificate);

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#define MEMORY_HEAP_SIZE      65536
uint8_t alloc_buf[MEMORY_HEAP_SIZE];
#endif

mbedtls_net_context server_fd;
const char *pers = "mbedtls";

mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_x509_crt cacert;

static void my_debug(void *ctx, int level, const char *file, int line, const char *str) {
  ((void) level);
  mbedtls_fprintf((FILE*) ctx, "%s:%04d: %s", file, line, str);
  fflush((FILE*) ctx);
}

int net_init(Network *n) {
  int ret;

  //initialize mbedTLS realted variables
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
  mbedtls_memory_buffer_alloc_init(alloc_buf, sizeof(alloc_buf));
#endif

#if defined(MBEDTLS_DEBUG_C)
  mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

  //mbedtls_net_init(&server_fd); //MX_LWIP_Init() is called already
  mbedtls_ssl_init(&ssl);
  mbedtls_ssl_config_init(&conf);
  mbedtls_x509_crt_init(&cacert);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  mbedtls_entropy_init(&entropy);
  if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char*) pers,
      strlen(pers))) != 0) {
    return -1;
  }

  //register functions
  n->mqttread = net_read; //receive function
  n->mqttwrite = net_write; //send function
  n->disconnect = net_disconnect; //disconnection function

  return 0;
}

int net_connect(Network *n, char *ip, int port) {
  int ret;

  // SSL/TLS connection process. refer to ssl client1 example
  ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char*) mbedtls_root_certificate,
      mbedtls_root_certificate_len);
  if (ret < 0) {
    printf("mbedtls_x509_crt_parse failed.\n");
    return -1;
  }

  ret = mbedtls_net_connect(&server_fd, SERVER_NAME, SERVER_PORT,
  MBEDTLS_NET_PROTO_TCP);
  if (ret < 0) {
    printf("mbedtls_net_connect failed.\n");
    return -1;
  }

  ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
  MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
  if (ret < 0) {
    printf("mbedtls_ssl_config_defaults failed.\n");
    return -1;
  }

  mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
  mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
  mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
  mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

  ret = mbedtls_ssl_setup(&ssl, &conf);
  if (ret < 0) {
    printf("mbedtls_ssl_setup failed.\n");
    return -1;
  }

  ret = mbedtls_ssl_set_hostname(&ssl, SERVER_NAME);
  if (ret < 0) {
    printf("mbedtls_ssl_set_hostname failed.\n");
    return -1;
  }

  mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv,
  NULL);

  while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      printf("mbedtls_ssl_handshake failed.\n");
      return -1;
    }
  }

  ret = mbedtls_ssl_get_verify_result(&ssl);
  if (ret < 0) {
    printf("mbedtls_ssl_get_verify_result failed.\n");
    return -1;
  }

  return 0;
}

int net_read(Network *n, unsigned char *buffer, int len, int timeout_ms) {
  int ret;
  int received = 0;
  int error = 0;
  int complete = 0;

  //set timeout
  if (timeout_ms != 0) {
    mbedtls_ssl_conf_read_timeout(&conf, timeout_ms);
  }

  //read until received length is bigger than variable len
  do {
    ret = mbedtls_ssl_read(&ssl, buffer, len);
    if (ret > 0) {
      received += ret;
    } else if (ret != MBEDTLS_ERR_SSL_WANT_READ) {
      error = 1;
    }
    if (received >= len) {
      complete = 1;
    }
  } while (!error && !complete);

  return received;
}

int net_write(Network *n, unsigned char *buffer, int len, int timeout_ms) {
  int ret;
  int written;

  //check all bytes are written
  for (written = 0; written < len; written += ret) {
    while ((ret = mbedtls_ssl_write(&ssl, buffer + written, len - written)) <= 0) {
      if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
        return ret;
      }
    }
  }

  return written;
}

void net_disconnect(Network *n) {
  int ret;

  do {
    ret = mbedtls_ssl_close_notify(&ssl);
  } while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);

  mbedtls_ssl_session_reset(&ssl);
  mbedtls_net_free(&server_fd);
}

void net_clear() {
  mbedtls_net_free(&server_fd);
  mbedtls_x509_crt_free(&cacert);
  mbedtls_ssl_free(&ssl);
  mbedtls_ssl_config_free(&conf);
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
  mbedtls_memory_buffer_alloc_free();
#endif
}

uint32_t MilliTimer;

//Timer functions
char TimerIsExpired(Timer *timer) {
  long left = timer->end_time - MilliTimer;
  return (left < 0);
}

void TimerCountdownMS(Timer *timer, unsigned int timeout) {
  timer->end_time = MilliTimer + timeout;
}

void TimerCountdown(Timer *timer, unsigned int timeout) {
  timer->end_time = MilliTimer + (timeout * 1000);
}

int TimerLeftMS(Timer *timer) {
  long left = timer->end_time - MilliTimer;
  return (left < 0) ? 0 : left;
}

void TimerInit(Timer *timer) {
  timer->end_time = 0;
}


