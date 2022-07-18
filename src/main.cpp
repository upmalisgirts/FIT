#include "../include/main.h"

int main() {
    Fit *fit = new Fit("testname.fit");
    fit->ReadFile();
    return 0;
}