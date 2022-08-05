#include "../include/main.h"

int main() {
    Fit *fit = new Fit("./files/2022-01-25-18-21-39.fit");
    fit->ReadFile();
    fit->PrintHeader();
    fit->DecodeFile();
    return 0;
}