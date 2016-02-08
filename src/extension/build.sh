#!/bin/bash
phpize --clean\
 && phpize\
 && ./configure\
 && make\
 && make test\
 && make install
