#include "exam.hpp"

void exam::fail_ex()
{
    std::string tmp = "bash .system/data_sender.sh \"fail_ex:" + current_ex->get_name() + " level:" + std::to_string(level) + " assignement:" + std::to_string(current_ex->get_assignement()) + "\"";
    system(tmp.c_str());
    current_ex->up_assignement();
    current_ex->set_time_bef_grade(time(NULL) + current_ex->grade_time() * 60);
    store_data();
}

void exam::success_ex()
{
    // insert current_ex in lvl_ex
    lvl_ex.insert(std::pair<int, exercice>(current_ex->get_lvl(), *current_ex));
    // *current_ex;
    std::cout << std::endl << LIME << ">>>>>>>>>> SUCCESS <<<<<<<<<<" << RESET << std::endl << std::endl; 
    std::string tmp = "bash .system/data_sender.sh \"success_ex: " + current_ex->get_name() + " level:" + std::to_string(level) + " assignement:" + std::to_string(current_ex->get_assignement()) + "\"";
    system(tmp.c_str());
    up_lvl();
    std::cout << "(Pressione enter para continuar...)" << std::endl;
    std::string input;
    if(!std::getline(std::cin, input))
        sigd();
    level_per_ex += level_per_ex_save;
    if(level_per_ex > 100)
        end_exam();
    backup = 0;
    if(file_exists("rendu/"))
    {
        if(!file_exists("success"))
            system("mkdir success");
        system("cp -r rendu/* success/ 2> /dev/null");
    }
    start_new_exam();
}

void exam::end_exam()
{
    std::string tmp;
    if(student)
        tmp = "bash .system/data_sender.sh \"exam_success_end: examrank0" + std::to_string(exam_number) + "\"";
    else
        tmp = "bash .system/data_sender.sh \"exam_success_end: examweek0" + std::to_string(exam_number) + "\"";
        system(tmp.c_str());
        remove(".system/exam_token/actuel_token.txt");
        std::cout << WHITE << BOLD << "Congratulation! You have finished the Exam Rank 0" << exam_number << " !" << std::endl;
        std::cout << "Love" << MAGENTA << " 42_EXAM" << WHITE << BOLD <<" ? Scan this QRCode to star Github repository 👋" << std::endl << std::endl;
        // show file .system/qrcode 
        system("cat .system/qrcode");
        std::cout << std::endl << "Opening Github repository? (y/n)" << std::endl;
        char c;
        std::cin >> c;
        if(c == 'y' || c == 'Y')
        {
        int is_linux = 0;
        #ifdef __linux__
            is_linux = 1;
        #endif
        if(is_linux)
            system("xdg-open http://git.grademe.fr");
        else
            system("open http://git.grademe.fr");
        }
        std::cout << WHITE << BOLD << "Thanks for studying with us "  << LIME << BOLD << username << WHITE << BOLD << " ❤️" << std::endl;
        exit(0);
}

// ==> Grademe function call by enter grademe on prompt
void exam::grademe(void)
{
    // std::cout << "time before grading: " << current_ex->time_bef_grade << " seconds" << std::endl;
    std::cout << std::endl
              << "Antes de continuar, faça " << RED << "CERTEZA ABSOLUTA" << RESET << " que você está no diretório certo," << std::endl;
    std::cout << "que você não esqueceu nada, etc..." << std::endl;
    std::cout << "Se sua atribuição estiver errada, você terá a mesma atribuição" << std::endl;
    std::cout << std::endl
              << " mas com menos pontos de potencial para ganhar !" << std::endl;
    std::cout << RED << "Tem certeza?" << RESET << " [y/N] ";
    std::string input;
    if (!std::getline(std::cin, input))
        sigd();
    if (input == "y" || input == "Y")
    {
        if (current_ex->time_bef_grade > time(NULL) && waiting_time)
        {
            std::cout << RED << "ERRO: " << RESET << "Você deve esperar pelo menos " << YELLOW << BOLD;
            if ((((current_ex->time_bef_grade - time(NULL))) / 60) >= 1)
                std::cout << (current_ex->time_bef_grade - time(NULL)) / 60 << " munutos" << RESET << " e " << YELLOW << BOLD << (current_ex->time_bef_grade - time(NULL)) % 60 << " seconds" << RESET;
            else
                std::cout << (current_ex->time_bef_grade - time(NULL)) << " segundos" << RESET;
            std::cout << " até a próxima solicitação de avaliação, então reserve um tempo para fazer mais testes e tenha certeza de que terá sucesso na próxima tentativa" << std::endl;
            return;
        }
        std::cout << "Ok, making grading request to server now." << std::endl;
        grade_request(0);
    }
    else
    {
        std::cout << " Abort" << std::endl;
    }
}

// ==> Functiion that call the bash grade system
void exam::grade_request(bool i)
{
    if(!i)
    {
    std::cout << std::endl << "Agora vamos aguardar a conclusão do trabalho." << std::endl << "Por Favor seja " << LIME << "paciente" << RESET << ", isto " << LIME << "CAN" << RESET " levar vários minutos..." << std::endl;
    std::cout << "(10 segundos é rápido, 30 segundos são esperados, 3 minutos é o máximo)" << std::endl;
    srand(time(NULL));
    int waiting_seconds = rand() % 5 + 1;
    for (int i = 0; i < waiting_seconds; i++)
    {
        srand(time(NULL));
        double random = rand() % 6500000 + 250000;
        std::cout << "waiting..." << std::endl;
        usleep(random);
    }
    }

    if (!file_exists(".system/grading/tester.sh"))
    {
        std::cout << "Erro: Não foi possível encontrar o script de avaliação para este exercício, ele será lançado em breve. Você pode usar \"force_success\" para passar este ex." << std::endl;
        return;
    }

    system("bash .system/grading/tester.sh");

    if (file_exists(".system/grading/passed"))
    {
        success_ex();
    }
    else
    {
        std::cout << RED <<  ">>>>>>>>>> FALHOU <<<<<<<<<<" << RESET << std::endl; 
        sleep(1);
        std::cout << "Você falhou na atribuição." << std::endl;

        // se houver um arquivo traceback, crie uma pasta traces e copie o arquivo para ela com o bom nome
        if (file_exists("traceback"))
        {
            system("mkdir -p traces");
            std::string trace_name = std::to_string(level) + "-" + std::to_string(current_ex->get_assignement()) + "_" + current_ex->get_name() + ".trace";
            std::string cmd_system_call = "mv traceback traces/" + trace_name;
            system(cmd_system_call.c_str());
            std::cout << "Trace saved to " << LIME << current_path() << "/traces/" <<  trace_name << RESET << std::endl << std::endl;
        }
        else
        {
            std::cout << "No traceback found." << std::endl << std::endl;
        }
        fail_ex();
        std::cout << "(Pressione enter para continuar...)" << std::endl;
        std::string input;
        if(!std::getline(std::cin, input))
            sigd();
        info();
    }
}
