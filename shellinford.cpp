#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#    include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"

#ifdef __cplusplus
}
#endif

#include "php_shellinford.h"
#include "fmindex.h"

ZEND_DECLARE_MODULE_GLOBALS(shellinford)

ZEND_INI_BEGIN()
ZEND_INI_END()

static void
shellinford_init_globals(zend_shellinford_globals *shellinford_globals)
{}

ZEND_MINIT_FUNCTION(shellinford)
{
    ZEND_INIT_MODULE_GLOBALS(shellinford, shellinford_init_globals, NULL);
    REGISTER_INI_ENTRIES();

    php_shellinford_fmindex_class_register(TSRMLS_C);

    return SUCCESS;
}

ZEND_MSHUTDOWN_FUNCTION(shellinford)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

ZEND_MINFO_FUNCTION(shellinford)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "Shellinford support", "enabled");
    php_info_print_table_row(2, "Extension Version", SHELLINFORD_EXT_VERSION);
    php_info_print_table_end();
}

zend_module_entry shellinford_module_entry = {
    STANDARD_MODULE_HEADER,
    "shellinford",
    NULL,
    ZEND_MINIT(shellinford),
    ZEND_MSHUTDOWN(shellinford),
    NULL,
    NULL,
    ZEND_MINFO(shellinford),
    SHELLINFORD_EXT_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_SHELLINFORD
ZEND_GET_MODULE(shellinford)
#endif
