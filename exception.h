#ifndef PHP_SHELLINFORD_EXCEPTION_H
#define PHP_SHELLINFORD_EXCEPTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zend_exceptions.h"

#include "php_shellinford.h"

extern PHP_SHELLINFORD_API zend_class_entry *php_shellinford_except_ce;

PHP_SHELLINFORD_API int php_shellinford_except_class_register(TSRMLS_D);

#define SHELLINFORD_ERR(_flag, ...) \
php_error_docref(NULL TSRMLS_CC, _flag, __VA_ARGS__)

#define SHELLINFORD_EXCEPTION(_code, ...) \
zend_throw_exception_ex(php_shellinford_except_ce, _code TSRMLS_CC, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  /* PHP_SHELLINFORD_EXCEPTION_H */
