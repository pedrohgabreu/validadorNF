// main.cpp
#include <FL/Fl.H>         // Include necessário para usar a classe Fl
#include <FL/Fl_Window.H>  // Include necessário para usar a classe Fl_Window
#include <iostream>
#include "gui.h"

int main(int argc, char* argv[]) {
    Fl::visual(FL_DOUBLE | FL_INDEX); // Configura o modo visual (opcional)
    createMainWindow(); // Cria a janela da interface gráfica
    return Fl::run();    // Inicia o loop da interface gráfica
}
