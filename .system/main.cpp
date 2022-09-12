#include "exam.hpp"

// ==> seção de ajuda
void exam::exam_help()
{
    std::cout << "Comandos:" << std::endl;
    std::cout << LIME << "    help:" << RESET << " exibir esta ajuda" << std::endl;
    std::cout << LIME << "    status:" << RESET << " exibir informações sobre o exame" << std::endl;
    std::cout << LIME << "    finish:" << RESET << " sair do exame" << std::endl;
    std::cout << LIME << "    grademe:" << RESET << " classifique seu exercício" << std::endl;
    std::cout << LIME << "    repo_git:" << RESET << " visite o repositório do github" << std::endl;
    std::cout << "Veja o repositório do github para encontrar mais comandos 'cheat'" << std::endl;
    if (vip)
    {
        std::cout << BOLD << LIME << "MENU VIP:" << RESET << std::endl;
        std::cout << LIME << "    force_success:" << RESET << " forçar um ex ao sucesso" << std::endl;
        std::cout << LIME << "    remove_grade_time:" << RESET << " remover o limite de tempo da nota entre duas notas" << std::endl;
        std::cout << LIME << "    gradenow:" << RESET << " exercício de grau instantâneo" << std::endl;
    }
}

// include for readline
#include <readline/readline.h>
#include <readline/history.h>

// ==> Shell prompt
void exam::exam_prompt(void)
{
    std::string input;
    int is_linux = 0;
    #ifdef __linux__
        is_linux = 1;
    #endif
    // std::cout << YELLOW << "examshell" << RESET << "> ";

    // if (!std::getline(std::cin, input))
    //     sigd();
    //use of readline
    while(1)
    {
    char *line = readline("\e[93mexamshell\e[0m> ");
    if (line == NULL)
        sigd();
    input = line;
    // input = buf;
    if (input == "finish" || input == "exit" || input == "quit")
    {
        if (std::ifstream(".system/exam_token/actuel_token.txt"))
            remove(".system/exam_token/actuel_token.txt");
        exit(0);

    }
    else if (input == "grademe")
    {
        add_history(line);
        grademe();
    }
    else if (input == "status")
    {
        add_history(line);
        if(vip)
            infovip();
        else
            info();
    }
    else if (input == "force_success")
    {
        add_history(line);
        std::string tmp = "bash .system/data_sender.sh \"cheatcode:force_success\"";
        system(tmp.c_str());
        success_ex();
    }
    else if (input == "help")
    {
        add_history(line);
        exam_help();
    }
    else if (input == "repo_git")
    {
        add_history(line);
        std::string tmp = "bash .system/data_sender.sh \"cheatcode:repo_git\"";
        system(tmp.c_str());
        std::cout << "Abrindo repositório git..." << std::endl;
        if (is_linux)
            std::system("xdg-open http://git.grademe.fr");
        else
            std::system("open http://git.grademe.fr");
    }
    else if (input == "remove_grade_time")
    {
        add_history(line);
        std::string tmp = "bash .system/data_sender.sh \"cheatcode:remove_grade_time\"";
        system(tmp.c_str());
        std::cout << "Time between grading is now removed for this exam" << std::endl;
        waiting_time = false;
    }
    else if (input == "gradenow" && vip)
    {
        add_history(line);
        grade_request(1);
    }
    else if (input == "")
        std::cout << REMOVE_LINE;
    else if (input != "")
        std::cout << "           **Unknown command**     type " << LIME << "help" << RESET << " for more help" << std::endl;
    // info();
    }
}

// ==> Definir uma boa pasta e copiar assuntos, etc...
bool exam::prepare_current_ex(void)
{
    if (level == level_max)
    {
        std::cout << "Você atingiu o nível máximo deste exame." << std::endl;
        return (false);
    }
    if (!file_exists(get_path()))
    {
        std::cout << "Erro: Não é possível carregar o exercício, não é possível encontrar um caminho válido" << std::endl;
        return (false);
    }

// limpa todos os arquivos antigos
    clean_all();

// cria diretório para o exercício atual
    system("mkdir rendu && mkdir subjects && mkdir .system/grading");

// copia todos os arquivos no atual get_path() + attachement/* para o diretório de assuntos
    std::string cmd_system_call = "cp -r " + get_path() + "/attachement/*" + " subjects/";
    system(cmd_system_call.c_str());

// copia todos os arquivos no get_path() atual sem a pasta de anexos para o diretório .system/grading/
    cmd_system_call = "cp " + get_path() + "* .system/grading/ >/dev/null 2>&1";
    system(cmd_system_call.c_str());

    return (false);
}
// jo je taime

// ==> Starting a new exercice/exam
bool exam::start_new_exam(void)
{
    std::string enter;
    if (!backup)
    {
        if (level == 0)
            connexion();
        list_ex_lvl = list_dir();
        exercice ex = *randomize_exercice(list_ex_lvl);
        current_ex = new exercice(ex);
        prepare_current_ex();
        store_data();
    }
    if(connexion_need)
    {
        std::cout << "Você está conectado " << LIME << username << RESET << "!" << std::endl;
        std::cout << "Você pode sair a qualquer momento. Se este programa disser que você ganhou pontos,\nentão eles serão contados aconteça o que acontecer.\n"
                  << std::endl;
        std::cout << BOLD << WHITE << "Você está prestes a iniciar o projeto" << LIME << BOLD << "ExamRank0" << exam_number << BOLD << WHITE << ", in " << MAGENTA << "REAL" << BOLD << WHITE << " modo, no nível " << YELLOW << level << BOLD << WHITE << "." << RESET << std::endl;
        std::cout << WHITE << BOLD << "Você teria " << LIME << BOLD << (time_max / 60) << "hrs " << BOLD << WHITE << "para concluir este projeto." << RESET << std::endl << "Pressione uma tecla para iniciar o exame 🏁" << std::endl;
        if (!std::getline(std::cin, enter))
            sigd();
        connexion_need = false;
    }
    if(vip)
        infovip();
    else
        info();
    exam_prompt();
    return (true);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigc);
    signal(SIGQUIT, sigc);
    signal(SIGTERM, sigc);


    if (file_exists("a.out"))
        remove("a.out");

    exam exm;

    if (file_exists(".system/exam_token/actuel_token.txt"))
        exm.restore_data();
    else
        exm.ask_param();
    

    exm.start_new_exam();

    
    return (0);
}