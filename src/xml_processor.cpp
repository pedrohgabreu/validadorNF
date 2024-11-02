// xml_processor.cpp
#include <iostream>
#include <pugixml.hpp>
#include "xml_processor.h"

void processXML(const char* filename) {
    pugi::xml_document doc;

    // Carregar o arquivo XML
    pugi::xml_parse_result result = doc.load_file(filename);
    
    if (!result) {
        std::cerr << "Erro ao carregar o arquivo XML: " << result.description() << std::endl;
        return;
    }

    // Acessar o nó raiz
    pugi::xml_node root = doc.child("root"); // substitua "root" pelo nome do seu nó raiz

    // Iterar sobre os nós filhos
    for (pugi::xml_node nf = root.child("NotaFiscal"); nf; nf = nf.next_sibling("NotaFiscal")) {
        const char* numero = nf.child("Numero").text().as_string();
        const char* data = nf.child("Data").text().as_string();
        const char* valor = nf.child("Valor").text().as_string();

        // Exibir os dados
        std::cout << "Nota Fiscal: " << numero << ", Data: " << data << ", Valor: " << valor << std::endl;
    }
}
