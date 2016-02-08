#include "tesseract.h"


const char* Tesseract::Version() {
    return tesseract::TessBaseAPI::Version();
}