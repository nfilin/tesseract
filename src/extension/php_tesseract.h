#ifndef PHP_TESSERACT_H
#define PHP_TESSERACT_H
 
#define PHP_TESSERACT_EXTNAME  "tesseract"
#define PHP_TESSERACT_EXTVER   "0.1"
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 
 
extern "C" {
#include "php.h"
#include "zend_exceptions.h"

#ifdef ZTS
#include "TSRM.h"
#endif

}

extern zend_error_handling tesseract_original_error_handling;
extern zend_module_entry tesseract_module_entry;
#define phpext_tesseract_ptr &tesseract_module_entry;

#ifdef PHP_WIN32
#	define PHP_TESSERACT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TESSERACT_API __attribute__ ((visibility("default")))
#else
#	define PHP_TESSERACT_API
#endif


#define PHP_TESSERACT_ERROR_HANDLING() do { \
	zend_replace_error_handling(EH_THROW, tesseract_ce_exception, &tesseract_original_error_handling TSRMLS_CC); \
} while(0)

#define PHP_TESSERACT_RESTORE_ERRORS() do { \
	zend_restore_error_handling(&tesseract_original_error_handling TSRMLS_CC); \
} while(0)

//extern zend_object_handlers tesseract_std_object_handlers;
extern zend_object_handlers tesseract_object_handlers;
extern zend_class_entry *tesseract_ce_exception;

#define CALL_METHOD_BASE(classname, name) zim_##classname##_##name

#define CALL_METHOD(classname, name, retval, thisptr)                  \
  CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


#endif /* PHP_TESSERACT_H */