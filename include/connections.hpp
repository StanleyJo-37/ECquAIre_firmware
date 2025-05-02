#pragma once

#include <cstddef>
#include "mbedtls/md.h"

inline bool get_hmac(mbedtls_md_type_t type = MBEDTLS_MD_SHA256, const unsigned char *key, size_t key_len, const unsigned char *input, size_t input_len, unsigned char *output) {
  const mbedtls_md_info_t *mbedtls_info = mbedtls_md_info_from_type(type);
  mbedtls_md_context_t ctx;

  mbedtls_md_init(&ctx);
  
  if (mbedtls_md_setup(&ctx, mbedtls_info, 1) != 0) {
    mbedtls_md_free(&ctx);
    return false;
  }

  mbedtls_md_hmac_starts(&ctx, key, key_len);
  mbedtls_md_hmac_update(&ctx, input, input_len);
  mbedtls_md_hmac_finish(&ctx, output);
  mbedtls_md_free(&ctx);

  return true;
}