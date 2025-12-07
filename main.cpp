#include <iostream>
#include <string>
#include "codebase/create.h"

int main(int argc, char* argv[]) {
    Create boxcreate;

    if (argc == 1) {
        std::cout << "Nenhum argumento fornecido. Use -h ou --help para ver as opções." << std::endl;
        return 0; 
    }

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " create <nome> <data> <hora> <descricao>" << std::endl;
        std::cerr << "Use " << argv[0] << " -h para ajuda." << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            std::cout << "Uso: " << argv[0] << " [OPCOES]" << std::endl;
            std::cout << std::endl;
            std::cout << "Opcoes disponiveis:" << std::endl;
            std::cout << "  -h, --help    Exibe esta mensagem de ajuda." << std::endl;
            std::cout << "  -v, --version Exibe a versao do programa." << std::endl;
            std::cout << "  -c, --create {nome} {dd/mm/aaaa} {h:min} {descrição} cria uma tarefa." << std::endl;
            std::cout << "  -l, --list lista as tarefas inconcluidas." << std::endl;
            std::cout << "  -la, --list_all lista todas as tarefas criadas." << std::endl;
            std::cout << "  -rm, --remove {indice} remove a tarefa com indice especificado." << std::endl;
            std::cout << "  -ar, --autoremove remove todas as tarefas conlcuidas e antigas." << std::endl;
            std::cout << "  -cl, --conclude conclui uma tarefa." << std::endl;
            
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << argv[0] << " versao 1.0.0" << std::endl;
            return 0;
        } else if (arg == "-c" || arg == "--create"){
            if (argc != 6) {
                std::cerr << "Erro: Sintaxe incorreta para 'create'." << std::endl;
                std::cerr << "Sintaxe: create <nome_tarefa> <dd/mm/aaaa> <hh:mm> <descricao>" << std::endl;
                return 1;
            }
            std::string nome_tarefa = argv[2];
            std::string data_str    = argv[3];
            std::string hora_str    = argv[4];
            std::string descricao   = argv[5];

            boxcreate.createnew(nome_tarefa, data_str, hora_str, descricao);
            return 1;
        } else if (arg == "-la" || arg == "--list_all"){
            if (argc != 2) {
                std::cerr << "Erro: Sintaxe incorreta. Uso: --list_all <indice_da_tarefa>" << std::endl;
                return 1;
            }
            else {
                boxcreate.list_all();
                return 1;
            }
        } else if (arg == "-l" || arg == "--list"){
            if (argc != 2) {
                std::cerr << "Erro: Sintaxe incorreta. Uso: --list <indice_da_tarefa>" << std::endl;
                return 1;
            }
            else {
                boxcreate.list();
                return 1;
            }
        } else if (arg == "-rm" || arg == "--remove"){
            if (argc != 3) {
                std::cerr << "Erro: Sintaxe incorreta. Uso: --remove <indice_da_tarefa>" << std::endl;
                return 1;
            }
            int indice = 0;
            try {
                indice = std::stoi(argv[2]); 
            } catch (const std::exception& e) {
                std::cerr << "Erro: O indice deve ser um numero inteiro valido." << std::endl;
                return 1;
            }
            boxcreate.exclude(indice);
            return 1;
        } else if (arg == "-ar" || arg == "--autoremove"){
            if(argc != 2){
                std::cerr << "Erro: Sintaxe incorreta. Uso: --autoremove" << std::endl;
                return 1;
            }
            else{
                boxcreate.autoremove();
                return 1;
            }
        } else if (arg == "-cl" || arg == "--conclude"){
            if(argc != 3){
                std::cerr << "Erro: Sintaxe incorreta. Uso: --conclude {task_number}" << std::endl;
                return 1;
            } else {
                int indice = 0;
                try {
                    indice = std::stoi(argv[2]); 
                } catch (const std::exception& e) {
                    std::cerr << "Erro: O indice deve ser um numero inteiro valido." << std::endl;
                    return 1;
                }
                boxcreate.conclude(indice);
                return 1;
            }
        }
        else {
            std::cerr << "Erro: Argumento desconhecido '" << arg << "'" << std::endl;
            std::cerr << "Use -h ou --help para ver as opções." << std::endl;
        }
    }

    return 0;
}