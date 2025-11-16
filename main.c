#include "include/app.h"
#include "include/ui.h"

int main(){
    AppData appData;
    initAppData(&appData);
    mainMenu(&appData);
    freeAppData(&appData);
    return 0;
}