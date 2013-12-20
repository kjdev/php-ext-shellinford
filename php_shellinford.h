#ifndef PHP_SHELLINFORD_H
#define PHP_SHELLINFORD_H

#ifdef __cplusplus
extern "C" {
#endif

#define SHELLINFORD_EXT_VERSION "0.0.1"

#define SHELLINFORD_NS "Shellinford"
#define SHELLINFORD_ZEND_METHOD(classname, name) \
ZEND_METHOD(Shellinford_##classname, name)
#define SHELLINFORD_ZEND_ME(classname, name, arg_info, flags) \
ZEND_ME(Shellinford_##classname, name, arg_info, flags)
#define SHELLINFORD_ZEND_MALIAS(classname, name, alias, arg_info, flags) \
ZEND_MALIAS(Shellinford_##classname, name, alias, arg_info, flags)
#define SHELLINFORD_LONG_CONSTANT(name, val) \
REGISTER_NS_LONG_CONSTANT(SHELLINFORD_NS, name, val, CONST_CS|CONST_PERSISTENT)
#define SHELLINFORD_STRING_CONSTANT(name, val) \
REGISTER_NS_STRING_CONSTANT(SHELLINFORD_NS, name, val, CONST_CS|CONST_PERSISTENT)

extern zend_module_entry shellinford_module_entry;
#define phpext_shellinford_ptr &shellinford_module_entry

#ifdef PHP_WIN32
#    define PHP_SHELLINFORD_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_SHELLINFORD_API __attribute__ ((visibility("default")))
#else
#    define PHP_SHELLINFORD_API
#endif

#ifdef ZTS
#    include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(shellinford)
ZEND_END_MODULE_GLOBALS(shellinford)

#ifdef ZTS
#    define SHELLINFORD_G(v) TSRMG(shellinford_globals_id, zend_shellinford_globals *, v)
#else
#    define SHELLINFORD_G(v) (shellinford_globals.v)
#endif

#ifdef __cplusplus
}
#endif

#endif  /* PHP_SHELLINFORD_H */
