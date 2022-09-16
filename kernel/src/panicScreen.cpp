#include "panicScreen.h"
#include "BasicRenderer.h"

void Panic(const char* panicMessage){
    mainRendering->ClearColour = 0x00ff0000;
    mainRendering->remove();

    mainRendering->CursorPosition = {0, 0};

    mainRendering->Colour = 0;

    mainRendering->Print("Kernel Panic");

    mainRendering->nextLine();
    mainRendering->nextLine();

    mainRendering->Print(panicMessage);
}