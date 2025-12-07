#ifndef CREATE_H
#define CREATE_H

#include <string>

class Create{
    private:

    public:
    void createnew(const std::string& nome_tarefa, 
                    const std::string& data, 
                    const std::string& hora, 
                    const std::string& descricao); //

    void modify();
    void conclude(int indice_tarefa);

    void exclude(int indice_tarefa); //
    void autoremove(); //

    void list_all(); //
    void list(); //

};

#endif