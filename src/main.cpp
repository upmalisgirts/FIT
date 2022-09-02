#include "../include/main.h"

int main() {
    Fit *fit = new Fit("./files/2022-05-07-08-57-46.fit");
    fit->ReadFile();
    fit->PrintHeader();
    fit->DecodeFile();
    return 0;
}