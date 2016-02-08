PHP_ARG_ENABLE(tesseract, Enable tesseract support)

if test "$PHP_TESSERACT" = "yes"; then
    PHP_REQUIRE_CXX()


    PHP_ADD_LIBRARY(stdc++, 1, TESSERACT_SHARED_LIBADD)

    ###############################################################################################################

    SEARCH_PATH="/usr/local /usr"
    SEARCH_FOR="/include/tesseract/baseapi.h"

    #if test -r $PHP_LIBXYZ/$SEARCH_FOR; then # path given as parameter
    #    LIBXYZ_DIR=$PHP_LIBXYZ
    #  else # search default path list
        AC_MSG_CHECKING([for Tesseract files in default path])
        for i in $SEARCH_PATH ; do
          if test -r $i/$SEARCH_FOR; then
            TESSERACT_DIR=$i
            AC_MSG_RESULT(found in $i)
          fi
        done
     # fi

    if test -z "$TESSERACT_DIR"; then
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([Please reinstall the Tesseract distribution])
    fi



    PHP_ADD_INCLUDE($TESSERACT_DIR/include)

    LIBNAME=tesseract

     PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TESSERACT_DIR/lib, TESSERACT_SHARED_LIBADD)


    PHP_SUBST(TESSERACT_SHARED_LIBADD)


    ###############################################################################################################

    AC_DEFINE(HAVE_TESSERACT, 1, [You have Tesseract-OCR extension])
    PHP_NEW_EXTENSION(tesseract, php_tesseract.cpp tesseract.cpp, $ext_shared)


    EXT_TESSERACT_HEADERS="baseapi.h"

    ifdef([PHP_INSTALL_HEADERS], [
       PHP_INSTALL_HEADERS(ext/tesseract, $EXT_TESSERACT_HEADERS)
     ])
fi
