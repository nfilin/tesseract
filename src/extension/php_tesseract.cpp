#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_tesseract.h"

#include "tesseract.h"



struct tesseract_object {
    zend_object std;
    Tesseract *tesseract;
};

zend_object_handlers tesseract_object_handlers;

//zend_error_handling tesseract_original_error_handling;

zend_class_entry *tesseract_ce;

PHP_METHOD(Tesseract, Version);

void tesseract_free_storage(void *object TSRMLS_DC)
{
    tesseract_object *obj = (tesseract_object *)object;
    delete obj->tesseract;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value tesseract_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    tesseract_object *obj = (tesseract_object *)emalloc(sizeof(tesseract_object));
    memset(obj, 0, sizeof(tesseract_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    //zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

    retval.handle = zend_objects_store_put(obj, NULL,
        tesseract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &tesseract_object_handlers;

    return retval;
}


const zend_function_entry tesseract_methods[] = {
    PHP_ME(Tesseract, Version, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(tesseract)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Tesseract", tesseract_methods);
    tesseract_ce = zend_register_internal_class(&ce TSRMLS_CC);
        tesseract_ce->create_object = tesseract_create_handler;
        memcpy(&tesseract_object_handlers,
            zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        tesseract_object_handlers.clone_obj = NULL;
    return SUCCESS;
}

zend_module_entry tesseract_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_TESSERACT_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(tesseract),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_TESSERACT_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(tesseract)

PHP_METHOD(Tesseract, Version)
{
    const char* v = Tesseract::Version();
    int i = strlen(v);
    RETURN_STRING(v, i);
}