// gui.cpp
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <iostream>
#include "gui.h"
#include "xml_processor.h" // Inclua para poder usar a função de processamento XML

// Função que será chamada quando o botão for clicado
void onButtonClick(Fl_Widget* widget, void* data) {
    const char* filename = "notas_fiscais.xml"; // Nome do arquivo XML a ser processado
    processXML(filename); // Chama a função de processamento XML
}

void createMainWindow() {
    Fl_Window* window = new Fl_Window(800, 600, "Analisador de Notas Fiscais");

    Fl_Button* button = new Fl_Button(350, 250, 100, 40, "Processar XML");
    button->callback(onButtonClick); // Define a callback para o botão

    window->end();
    window->show();
}
