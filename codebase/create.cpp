#include <algorithm>
#include <ctime>
#include <iostream>
#include "create.h"
#include <fstream>
#include <ostream>
#include <string>
#include <chrono>

#include "json.hpp"
using json = nlohmann::json;

time_t get_data(){
    auto tempo_agora = std::chrono::system_clock::now();

    std::time_t tempo_t = std::chrono::system_clock::to_time_t(tempo_agora);

    return tempo_t;
}

std::string reformatar_data(const std::string&data) {
    if(data.length()<10) return data;
    return data.substr(6, 4) + "/" + data.substr(3, 2) + "/" + data.substr(0, 2);
}

void Create::createnew(const std::string& nome_tarefa, 
                    const std::string& data, 
                    const std::string& hora, 
                    const std::string& descricao){

    std::time_t now_time = get_data();
    struct tm *local_tempo = std::localtime(&now_time);
    std::stringstream ss;
    ss<<std::put_time(local_tempo, "%d-%m-%Y");
    std::string data_hj = ss.str();

    std::string arquivo_json = "tarefas.json";
    json dados;
    
    std::ifstream i(arquivo_json);
    if (i.is_open()) {
        try {
            i >> dados;
            i.close();
        } catch (const std::exception& e) {
            std::cerr << "Aviso: Nao foi possivel ler o JSON existente. Criando nova estrutura." << std::endl;
            dados = json::object();
        }
    }
    
    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        dados["tarefas"] = json::array();
    }

    json nova_tarefa = {
        {"nome", nome_tarefa},
        {"descricao", descricao},
        {"data", data},
        {"hora", hora},
        {"criado_em", data_hj},
        {"concluido", false}
    };

    dados["tarefas"].push_back(nova_tarefa);
    
    json& tarefas = dados["tarefas"];
    std::sort(tarefas.begin(), tarefas.end(), [](const json& a, const json& b) {
            // Reformatar a data para comparacao cronologica
            std::string data_a = reformatar_data(a.value("data", "9999/99/99"));
            std::string data_b = reformatar_data(b.value("data", "9999/99/99"));

            if (data_a != data_b) {
                // Se as datas sao diferentes, ordena pela data (mais cedo = menor)
                return data_a < data_b;
            } else {
                // Se as datas sao iguais, ordena pela hora (mais cedo = menor)
                std::string hora_a = a.value("hora", "99:99");
                std::string hora_b = b.value("hora", "99:99");
                return hora_a < hora_b;
            }
        });

    std::ofstream o(arquivo_json);
    o << dados.dump(2) << std::endl;
    o.close();

    std::cout << "✅ Tarefa '" << nome_tarefa << "' criada e salva em " << arquivo_json << std::endl;
}

void Create::list_all(){
    std::string arquivo_json = "tarefas.json";
    std::ifstream i(arquivo_json);
    json dados;

    if (!i.is_open()) {
        std::cerr << "Arquivo " << arquivo_json << " nao encontrado. Nenhuma tarefa para listar." << std::endl;
        return;
    }

    try {
        i >> dados;
        i.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler/parcear o arquivo JSON: " << e.what() << std::endl;
        return;
    }
    
    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        std::cout << "Nao há um array de 'tarefas' valido no arquivo JSON." << std::endl;
        return;
    }
    
    const json& tarefas = dados["tarefas"];
    
    if (tarefas.empty()) {
        std::cout << "Nao ha tarefas cadastradas." << std::endl;
        return;
    }

    std::cout << "\n📋 Lista de Tarefas (" << tarefas.size() << " total):" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    int indice = 1;
    for (const auto& tarefa : tarefas) {
        std::cout << "[" << indice++ << "] " << tarefa.value("nome", "N/A") << std::endl;
        
        std::cout << "    Descricao: " << tarefa.value("descricao", "N/A") << std::endl;
        std::cout << "    Prazo:     " << tarefa.value("data", "N/A") 
                  << " às " << tarefa.value("hora", "N/A") << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
    }
}

void Create::list(){
    std::string arquivo_json = "tarefas.json";
    std::ifstream i(arquivo_json);
    json dados;

    if (!i.is_open()) {
        std::cerr << "Arquivo " << arquivo_json << " nao encontrado. Nenhuma tarefa para listar." << std::endl;
        return;
    }

    try {
        i >> dados;
        i.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler/parcear o arquivo JSON: " << e.what() << std::endl;
        return;
    }
    
    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        std::cout << "Nao há um array de 'tarefas' valido no arquivo JSON." << std::endl;
        return;
    }
    
    const json& tarefas = dados["tarefas"];
    
    if (tarefas.empty()) {
        std::cout << "Nao ha tarefas cadastradas." << std::endl;
        return;
    }

    std::cout << "\n📋 Lista de Tarefas inconcluidas (" << tarefas.size() << " total):" << std::endl;

    int indice = 1;
    for (const auto& tarefa : tarefas) {
        if(tarefa["concluido"] != true){
        std::cout << "[" << indice++ << "] " << tarefa.value("nome", "N/A") << " ";
        std::cout << "    Prazo:     " << tarefa.value("data", "N/A") 
                  << " às " << tarefa.value("hora", "N/A") << std::endl;
        }
    }
}

void Create::exclude(int indice_tarefa){
    std::string arquivo_json = "tarefas.json";
    std::ifstream i(arquivo_json);
    json dados;
    
    if (!i.is_open()) {
        std::cerr << "Erro: Arquivo " << arquivo_json << " nao encontrado." << std::endl;
        return;
    }

    try {
        i >> dados;
        i.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler/parcear o arquivo JSON: " << e.what() << std::endl;
        return;
    }

    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        std::cerr << "Nao ha um array de 'tarefas' valido no arquivo JSON." << std::endl;
        return;
    }
    
    json& tarefas = dados["tarefas"];

    int indice_base_0 = indice_tarefa - 1; 

    if (indice_base_0 < 0 || indice_base_0 >= tarefas.size()) {
        std::cerr << "Erro: Indice de tarefa invalido. Use um numero entre 1 e " << tarefas.size() << "." << std::endl;
        return;
    }

    tarefas.erase(tarefas.begin() + indice_base_0);
    
    std::ofstream o(arquivo_json);
    o << dados.dump(2) << std::endl;
    o.close();

    std::cout << "🗑️ Tarefa #" << indice_tarefa << " excluida com sucesso de " << arquivo_json << std::endl;
}

void  Create::autoremove(){
    std::time_t now_time = get_data();
    struct tm *local_tempo = std::localtime(&now_time);
    std::stringstream ss;
    ss<<std::put_time(local_tempo, "%d-%m-%Y");

    std::string arquivo_json = "tarefas.json";
    std::ifstream i(arquivo_json);
    json dados;
    
    if (!i.is_open()) {
        std::cerr << "Erro: Arquivo " << arquivo_json << " nao encontrado." << std::endl;
        return;
    }

    try {
        i >> dados;
        i.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler/parcear o arquivo JSON: " << e.what() << std::endl;
        return;
    }

    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        std::cerr << "Nao ha um array de 'tarefas' valido no arquivo JSON." << std::endl;
        return;
    }
    
    json& tarefas = dados["tarefas"];

    for (int i = tarefas.size() - 1; i >= 0; --i) {
        auto tarefa = tarefas[i];
        std::string data_completa = tarefa.value("data", "00/00/0000");
        std::string ano, mes, dia, anosys, messys, diasys;
        anosys = ss.str().substr(6, 4);
        messys = ss.str().substr(3, 2);
        diasys = ss.str().substr(0, 2);
        ano = data_completa.substr(6, 4);
        mes = data_completa.substr(3, 2);
        dia = data_completa.substr(0, 2);

        if(std::stoi(ano) < std::stoi(anosys)){
            tarefas.erase(tarefas.begin() + i);
            
            std::ofstream o(arquivo_json);
            o << dados.dump(2) << std::endl;
            o.close();
            std::cout << "🗑️ Tarefa #" << i+1 << " excluida com sucesso de " << arquivo_json << std::endl;

        } else if (std::stoi(mes) < std::stoi(messys)){
            tarefas.erase(tarefas.begin() + i);
    
            std::ofstream o(arquivo_json);
            o << dados.dump(2) << std::endl;
            o.close();
            std::cout << "🗑️ Tarefa #" << i+1 << " excluida com sucesso de " << arquivo_json << std::endl;

        } else if (std::stoi(dia) < std::stoi(diasys)){
            tarefas.erase(tarefas.begin() + i);
            
            std::ofstream o(arquivo_json);
            o << dados.dump(2) << std::endl;
            o.close();
            std::cout << "🗑️ Tarefa #" << i+1 << " excluida com sucesso de " << arquivo_json << std::endl;

        } else if (tarefa["concluido"] == true){

            tarefas.erase(tarefas.begin() + i);
            
            std::ofstream o(arquivo_json);
            o << dados.dump(2) << std::endl;
            o.close();
            std::cout << "🗑️ Tarefa #" << i+1 << " excluida com sucesso de " << arquivo_json << std::endl;

        }
    }
    return;
}

void Create::conclude(int indice_tarefa){
    std::string arquivo_json = "tarefas.json";
    std::ifstream i(arquivo_json);
    json dados;

    if (!i.is_open()) {
        std::cerr << "Arquivo " << arquivo_json << " nao encontrado. Nenhuma tarefa para listar." << std::endl;
        return;
    }

    try {
        i >> dados;
        i.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler/parcear o arquivo JSON: " << e.what() << std::endl;
        return;
    }
    
    if (dados.find("tarefas") == dados.end() || !dados["tarefas"].is_array()) {
        std::cout << "Nao há um array de 'tarefas' valido no arquivo JSON." << std::endl;
        return;
    }
    
    json& tarefas = dados["tarefas"];

    json& tarefa = tarefas[indice_tarefa-1];

    if (tarefa.contains("concluido")) {
        // Altera o valor
        tarefa["concluido"] = true;
        
        // 4. Salvar o JSON modificado
        std::ofstream o(arquivo_json);
        o << dados.dump(2) << std::endl;
        o.close();
        std::cout << "✏️ Tarefa #" << indice_tarefa << " concluida com sucesso." << std::endl;
    }
}