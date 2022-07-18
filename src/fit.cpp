#include "../include/fit.h"

Fit::Fit(const char *fname){
    filename = fname;
}

void Fit::ReadFile() {
    cout << filename << endl;
}