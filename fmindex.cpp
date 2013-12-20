#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#    include "config.h"
#endif

#include "php.h"
#include "php_ini.h"

#ifdef __cplusplus
}
#endif

#include "php_shellinford.h"
#include "fmindex.h"
#include "exception.h"

ZEND_EXTERN_MODULE_GLOBALS(shellinford)

zend_class_entry *php_shellinford_fmindex_ce;
static zend_object_handlers php_shellinford_fmindex_handlers;

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_clear, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_push_back, 0, 0, 1)
    ZEND_ARG_INFO(0, doc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_build, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_docsize, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_search, 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_get_document, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_write, 0, 0, 1)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_shellinford_fmindex_read, 0, 0, 1)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

SHELLINFORD_ZEND_METHOD(FMindex, __construct)
{
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters_none() == FAILURE) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    intern->index = new shellinford::fm_index();

    if (!intern->index) {
        SHELLINFORD_EXCEPTION(0, "Shellinford\\FMindex object has not been"
                              " correctly initialized by its constructor");
        RETURN_FALSE;
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, clear)
{
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        intern->index->clear();
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, push_back)
{
    char *doc;
    int doc_len;
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                              &doc, &doc_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (!doc || doc_len <= 0) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        intern->index->push_back(doc);
        RETVAL_TRUE;
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_FALSE;
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, build)
{
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters_none() == FAILURE) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        intern->index->build();
        RETVAL_TRUE;
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_FALSE;
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, docsize)
{
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters_none() == FAILURE) {
        RETURN_LONG(0);
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        RETVAL_LONG(intern->index->docsize());
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_LONG(0);
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, search)
{
    char *key;
    int key_len;
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                              &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (!key || key_len <= 0) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    array_init(return_value);

    try {
        std::map<uint64_t, uint64_t> dids;
        intern->index->search(key, dids);
        std::map<uint64_t, uint64_t>::const_iterator i = dids.begin();
        std::map<uint64_t, uint64_t>::const_iterator e = dids.end();
        while (i != e) {
            std::string doc = intern->index->get_document(i->first);
            add_index_stringl(return_value, i->first,
                              doc.c_str(), doc.length(), 1);
            i++;
        }
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, get_document)
{
    long id;
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
                              &id) == FAILURE) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        std::string doc = intern->index->get_document(id);
        RETVAL_STRINGL(doc.c_str(), doc.length(), 1);
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_FALSE;
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, write)
{
    char *filename;
    int filename_len;
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                              &filename, &filename_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (!filename || filename_len <= 0) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        intern->index->write(filename);
        RETVAL_TRUE;
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_FALSE;
    }
}

SHELLINFORD_ZEND_METHOD(FMindex, read)
{
    char *filename;
    int filename_len;
    php_shellinford_fmindex_t *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                              &filename, &filename_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (!filename || filename_len <= 0) {
        RETURN_FALSE;
    }

    SHELLINFORD_FMINDEX_OBJ(intern, getThis());

    try {
        intern->index->read(filename);
        RETVAL_TRUE;
    } catch (const char *err) {
        SHELLINFORD_ERR(E_WARNING, err);
        RETVAL_FALSE;
    }
}

static zend_function_entry php_shellinford_fmindex_methods[] = {
    SHELLINFORD_ZEND_ME(FMindex, __construct,
                        arginfo_shellinford_fmindex___construct,
                        ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    SHELLINFORD_ZEND_ME(FMindex, clear,
                        arginfo_shellinford_fmindex_clear, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, push_back,
                        arginfo_shellinford_fmindex_push_back, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_MALIAS(FMindex, pushBack, push_back,
                            arginfo_shellinford_fmindex_push_back,
                            ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, build,
                        arginfo_shellinford_fmindex_build, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, docsize,
                        arginfo_shellinford_fmindex_docsize, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, search,
                        arginfo_shellinford_fmindex_search, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, get_document,
                        arginfo_shellinford_fmindex_get_document,
                        ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_MALIAS(FMindex, getDocument, get_document,
                            arginfo_shellinford_fmindex_get_document,
                            ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, write,
                        arginfo_shellinford_fmindex_write, ZEND_ACC_PUBLIC)
    SHELLINFORD_ZEND_ME(FMindex, read,
                        arginfo_shellinford_fmindex_read, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

static void
php_shellinford_fmindex_free_storage(void *object TSRMLS_DC)
{
    php_shellinford_fmindex_t *intern = (php_shellinford_fmindex_t *)object;

    if (!intern) {
        return;
    }

    if (intern->index) {
        delete intern->index;
    }

    zend_object_std_dtor(&intern->std TSRMLS_CC);
    efree(object);
}

static zend_object_value
php_shellinford_fmindex_new_ex(zend_class_entry *ce,
                               php_shellinford_fmindex_t **ptr TSRMLS_DC)
{
    php_shellinford_fmindex_t *intern;
    zend_object_value retval;

    intern = (php_shellinford_fmindex_t *)emalloc(
        sizeof(php_shellinford_fmindex_t));
    memset(intern, 0, sizeof(php_shellinford_fmindex_t));
    if (ptr) {
        *ptr = intern;
    }

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    retval.handle = zend_objects_store_put(
        intern, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)php_shellinford_fmindex_free_storage,
        NULL TSRMLS_CC);
    retval.handlers = &php_shellinford_fmindex_handlers;

    intern->index = NULL;

    return retval;
}

static zend_object_value
php_shellinford_fmindex_new(zend_class_entry *ce TSRMLS_DC)
{
    return php_shellinford_fmindex_new_ex(ce, NULL TSRMLS_CC);
}

PHP_SHELLINFORD_API int
php_shellinford_fmindex_class_register(TSRMLS_D)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, ZEND_NS_NAME(SHELLINFORD_NS, "FMindex"),
                     php_shellinford_fmindex_methods);

    ce.create_object = php_shellinford_fmindex_new;

    php_shellinford_fmindex_ce = zend_register_internal_class(&ce TSRMLS_CC);
    if (php_shellinford_fmindex_ce == NULL) {
        return FAILURE;
    }

    memcpy(&php_shellinford_fmindex_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    php_shellinford_fmindex_handlers.clone_obj = NULL;

    return SUCCESS;
}
