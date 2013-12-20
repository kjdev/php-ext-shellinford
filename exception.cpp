#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#    include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_main.h"

#ifdef __cplusplus
}
#endif

#include "php_shellinford.h"
#include "exception.h"

ZEND_EXTERN_MODULE_GLOBALS(shellinford)

zend_class_entry *php_shellinford_except_ce;

PHP_SHELLINFORD_API int
php_shellinford_except_class_register(TSRMLS_D)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, ZEND_NS_NAME(SHELLINFORD_NS, "Exception"), NULL);

    php_shellinford_except_ce = zend_register_internal_class_ex(
        &ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
    if (php_shellinford_except_ce == NULL) {
        return FAILURE;
    }

    return SUCCESS;
}
