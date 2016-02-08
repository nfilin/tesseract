#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_tesseract.h"

struct tesseract_object {
    zend_object std;
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
};

zend_object_handlers tesseract_object_handlers;

//zend_error_handling tesseract_original_error_handling;

zend_class_entry *tesseract_ce;
// #############################   Method declaration ###########################################
PHP_METHOD(Tesseract, __construct);
PHP_METHOD(Tesseract, Version);
PHP_METHOD(Tesseract, SetImage);
PHP_METHOD(Tesseract, GetUTF8Text);
PHP_METHOD(Tesseract, getText);
PHP_METHOD(Tesseract, getHOCR);
PHP_METHOD(Tesseract, GetHOCRText);
PHP_METHOD(Tesseract, clear);
PHP_METHOD(Tesseract, recognize);
// ##############################################################################################

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

// #############################   Method list ###########################################

const zend_function_entry tesseract_methods[] = {
    PHP_ME(Tesseract,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Tesseract, Version, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(Tesseract, SetImage, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, GetUTF8Text, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, getText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, getHOCR, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, GetHOCRText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, clear, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Tesseract, recognize, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// #######################################################################################

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


// #############################   Method implementation ###########################################

PHP_METHOD(Tesseract, __construct)
{
    tesseract::TessBaseAPI *tesseract = NULL;
    //Tesseract *tesseract = NULL;
    zval *object = getThis();


                const char* datapath = NULL;
                const char* language = "eng";
                tesseract::OcrEngineMode mode = tesseract::OEM_DEFAULT;

                char **configs =  NULL;
                int configs_size = 0;
                const GenericVector<STRING> *vars_vec = NULL;
                const GenericVector<STRING> *vars_values =NULL;
                bool set_only_non_debug_params = false;
    char *_datapath;
    int _datapath_len;
    char *datapath_;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &_datapath, &_datapath_len) == FAILURE) {
        RETURN_NULL();
    }

    if(_datapath_len > 0){
        datapath_ = (char *)malloc((_datapath_len+1)*sizeof(char));
        strncpy(datapath_, _datapath, _datapath_len);
        datapath_[_datapath_len]='\0';
    } else {
        datapath_ = NULL;
    }
    tesseract = new tesseract::TessBaseAPI();
    tesseract->Init(datapath_, language, mode, configs, configs_size, vars_vec, vars_values, set_only_non_debug_params);
    //tesseract = new Tesseract(datapath_, language, mode, configs, configs_size, vars_vec, vars_values, set_only_non_debug_params);
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->tesseract = tesseract;

}


PHP_METHOD(Tesseract, Version)
{
    const char* v = tesseract::TessBaseAPI::Version();
    //const char* v = Tesseract::Version();
    int i = strlen(v);
    RETURN_STRING(v, i);
}

PHP_METHOD(Tesseract, SetImage)
{
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
    zval *object = getThis();
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(
            object TSRMLS_CC);
    tesseract = obj->tesseract;

    if(tesseract == NULL)
        zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Tesseract object not initialized", 0 TSRMLS_CC);
    zval *image;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &image) == FAILURE) {
            RETURN_NULL();
    }

    if(image->type == IS_STRING){
            char *path;
            int len = image->value.str.len;
            path = (char*)malloc((len+1)*sizeof(char));
            strncpy(path, image->value.str.val, len);
            path[len] = '\0';
            //printf("STRING: [%s]\n", path);
            Pix *img = pixRead(path);
            tesseract->SetImage(img);
   /* } else if(image->type == IS_OBJECT){
            printf("OBJECT\n");
    } else if(image->type == IS_RESOURCE){
            printf("STRING\n");
    } else if(image->type == IS_ARRAY){
            printf("ARRAY\n");*/
    } else {
            zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Incorrect image type", 0 TSRMLS_CC);
    }

    *return_value = *object;
    zval_copy_ctor(return_value);

}

PHP_METHOD(Tesseract, getText){
    CALL_METHOD(Tesseract, GetUTF8Text, return_value, getThis());
}

PHP_METHOD(Tesseract, GetUTF8Text){
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(
            getThis() TSRMLS_CC);
    tesseract = obj->tesseract;

    if(!tesseract->GetInputImage()){
        zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Image should be specified first", 0 TSRMLS_CC);
        return;
    }

    char *text;
    text = tesseract->GetUTF8Text();
    if(text != NULL){
        RETURN_STRING(text, strlen(text));
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(Tesseract, GetHOCRText){
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(
            getThis() TSRMLS_CC);
    tesseract = obj->tesseract;
    long page_num = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &page_num) == FAILURE) {
            RETURN_NULL();
    }

    if(!tesseract->GetInputImage()){
        zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Image should be specified first", 0 TSRMLS_CC);
       return;
    }

    char *text;
    text = tesseract->GetHOCRText((int)page_num);
    if(text != NULL){
        RETURN_STRING(text, strlen(text));
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(Tesseract, getHOCR){
     CALL_METHOD(Tesseract, GetHOCRText, return_value, getThis());
}



PHP_METHOD(Tesseract, clear){
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
    zval *object = getThis();
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(
            object TSRMLS_CC);
    tesseract = obj->tesseract;
    tesseract->Clear();

    *return_value = *object;
    zval_copy_ctor(return_value);
}

PHP_METHOD(Tesseract, recognize){
    tesseract::TessBaseAPI *tesseract;
    //Tesseract *tesseract;
    zval *object = getThis();
    tesseract_object *obj = (tesseract_object *)zend_object_store_get_object(
            object TSRMLS_CC);
    tesseract = obj->tesseract;

    if(!tesseract->GetInputImage()){
        zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Image should be specified first", 0 TSRMLS_CC);
        return;
    }

    if(tesseract->Recognize(NULL) != 0){
        zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Recognition failed", 0 TSRMLS_CC);
        return;
    }

    *return_value = *object;
    zval_copy_ctor(return_value);
}