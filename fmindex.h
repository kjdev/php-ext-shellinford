#ifndef PHP_SHELLINFORD_FMINDEX_H
#define PHP_SHELLINFORD_FMINDEX_H

#include "shellinford_fm_index.h"

typedef struct {
    zend_object std;
    shellinford::fm_index *index;
} php_shellinford_fmindex_t;

#ifdef __cplusplus
extern "C" {
#endif

#include "php_shellinford.h"

extern PHP_SHELLINFORD_API zend_class_entry *php_shellinford_fmindex_ce;

PHP_SHELLINFORD_API int php_shellinford_fmindex_class_register(TSRMLS_D);

#define SHELLINFORD_FMINDEX_OBJ(_self, _obj) \
_self = (php_shellinford_fmindex_t *)zend_object_store_get_object(_obj TSRMLS_CC)

#ifdef __cplusplus
}
#endif

#endif  /* PHP_SHELLINFORD_FMINDEX_H */
