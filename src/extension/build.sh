#!/bin/bash
phpize --clean\
 && phpize\
 && ./configure\
 && make\
 && make test\

# && make install \
# && php -r "print_r([Tesseract::Version()]);"\
# && php -r "\$t = new Tesseract('/home/stas/work/kitchee/tessdata');print_r(\$t->recognize());"
# #&& php -r "\$t = new Tesseract('/home/stas/work/kitchee/tessdata');\$t->SetImage('/home/stas/work/kitchee/b889e48f75f716bf71effd88bbecf9ea47a4b0e3.69040.png');\$t->recognize();print_r(\$t->getText());"\
