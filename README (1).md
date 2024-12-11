
# Validador de Notas Fiscais Eletrônicas

### Descrição
Este projeto é um validador de Notas Fiscais Eletrônicas (NFe) que utiliza **Qt Framework** e **libxml2** para:
- Validar arquivos XML contra **DTD** e **XSD**.
- Extrair informações de Notas Fiscais e exibi-las em uma interface gráfica amigável.
- Exibir informações como número total de NFs, produtos, valores e impostos.

---

## Pré-requisitos

### Ferramentas Necessárias
1. **Qt Framework 5 ou superior**:
   - Qt Creator IDE para compilar e executar o projeto.
2. **libxml2**:
   - Biblioteca para manipulação de XML e validação de DTD/XSD.
3. **CMake** (opcional):
   - Para configurar e compilar o projeto, se preferir usar `CMake`.

### Instalação das Dependências

#### No Ubuntu/Debian:
```bash
sudo apt update
sudo apt install qt5-default qtcreator libxml2 libxml2-dev build-essential
```

#### No Windows:
1. Baixe e instale o **Qt Framework** no [site oficial do Qt](https://www.qt.io/).
2. Baixe o instalador de **libxml2** no [site oficial do Gnome](http://xmlsoft.org/).

#### No MacOS:
```bash
brew install qt libxml2
```

---

## Configuração do Ambiente

### Configuração do Qt Creator

1. Abra o Qt Creator.
2. Clique em `File > Open File or Project` e selecione o arquivo `.pro` do projeto.
3. Vá em `Tools > Options > Kits` e configure o kit do Qt para garantir que a versão correta do Qt e o compilador estejam definidos.
4. Certifique-se de que o caminho para as bibliotecas `libxml2` está configurado no sistema:

   **No Linux/MacOS**:
   - Adicione o seguinte ao arquivo `.pro`:
     ```plaintext
     INCLUDEPATH += /usr/include/libxml2
     LIBS += -lxml2
     ```

   **No Windows**:
   - Adicione o caminho para o diretório de include e as bibliotecas estáticas na configuração do projeto.

### Configuração Manual do Caminho do DTD/XSD

1. Certifique-se de que o arquivo DTD/XSD esteja no mesmo diretório do XML ou ajuste o caminho absoluto no código para corresponder.

   Por exemplo:
   ```xml
   <!DOCTYPE Nota SYSTEM "caminho/para/nota.dtd">
   ```

---

## Estrutura do Projeto

```plaintext
validadornf/
├── src/
│   ├── main.cpp                # Arquivo principal do projeto
│   ├── dashboard.h             # Declarações da classe Dashboard
│   ├── dashboard.cpp           # Implementação das funções de validação e interface
├── resources/
│   ├── nota.dtd                # DTD para validação
│   ├── notas_fiscais.xsd       # XSD para validação
│   ├── exemplo.xml             # XML de exemplo
├── validadornf.pro             # Arquivo de configuração do Qt Creator
├── README.md                   # Documentação do projeto
```

---

## Executando o Projeto

1. **Compilação**:
   - No **Qt Creator**, clique em `Build > Build Project`.

2. **Execução**:
   - Clique em `Run` para iniciar a aplicação.

3. **Validação de XML**:
   - Faça upload de arquivos XML clicando no botão `Upload XML`.
   - Escolha entre validação por **DTD** ou **XSD**.
   - Selecione o arquivo de esquema correspondente ao tipo escolhido.
   - Os dados das notas fiscais aparecerão na interface gráfica.

---

## Testando com Arquivos de Exemplo

### Exemplo de Arquivo XML (`exemplo.xml`)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE Nota SYSTEM "nota.dtd">
<Nota>
    <ID>12345</ID>
    <Data>2024-12-06</Data>
    <ValorTotal>1500.75</ValorTotal>
    <Impostos>300.15</Impostos>
    <Produtos>
        <Produto>
            <Nome>Produto A</Nome>
            <Preco>500.25</Preco>
        </Produto>
        <Produto>
            <Nome>Produto B</Nome>
            <Preco>1000.50</Preco>
        </Produto>
    </Produtos>
</Nota>
```

### Exemplo de Arquivo DTD (`nota.dtd`)

```dtd
<!ELEMENT Nota (ID, Data, ValorTotal, Impostos, Produtos)>
<!ELEMENT ID (#PCDATA)>
<!ELEMENT Data (#PCDATA)>
<!ELEMENT ValorTotal (#PCDATA)>
<!ELEMENT Impostos (#PCDATA)>
<!ELEMENT Produtos (Produto+)>
<!ELEMENT Produto (Nome, Preco)>
<!ELEMENT Nome (#PCDATA)>
<!ELEMENT Preco (#PCDATA)>
```

### Exemplo de Arquivo XSD (`notas_fiscais.xsd`)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema" elementFormDefault="qualified">
    <xsd:element name="Nota">
        <xsd:complexType>
            <xsd:sequence>
                <xsd:element name="ID" type="xsd:string"/>
                <xsd:element name="Data" type="xsd:date"/>
                <xsd:element name="ValorTotal" type="xsd:decimal"/>
                <xsd:element name="Impostos" type="xsd:decimal"/>
                <xsd:element name="Produtos">
                    <xsd:complexType>
                        <xsd:sequence>
                            <xsd:element name="Produto" maxOccurs="unbounded">
                                <xsd:complexType>
                                    <xsd:sequence>
                                        <xsd:element name="Nome" type="xsd:string"/>
                                        <xsd:element name="Preco" type="xsd:decimal"/>
                                    </xsd:sequence>
                                </xsd:complexType>
                            </xsd:element>
                        </xsd:sequence>
                    </xsd:complexType>
                </xsd:element>
            </xsd:sequence>
        </xsd:complexType>
    </xsd:element>
</xsd:schema>
```

---

## Erros Comuns e Soluções

1. **Erro ao carregar DTD/XSD**:
   - Verifique se o caminho do arquivo está correto.
   - Use caminhos absolutos no XML ou forneça o caminho correto ao programa.

2. **Qt não encontra `libxml2`**:
   - Confirme se a biblioteca está instalada corretamente e o caminho de include está configurado.

3. **Interface não exibe dados**:
   - Certifique-se de que o XML é válido e os elementos têm os nomes corretos.

---

