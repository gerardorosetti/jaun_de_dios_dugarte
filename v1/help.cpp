#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
// #include <thread>
// #include <chrono>

// using namespace std::literals::chrono_literals;

#ifdef _WIN32
    #include <windows.h>
    void clear() {
        system("cls");
    }
    void wait(int time)
    {
        Sleep(time * 1000);
    }
    #define LINUX false
#else
    #include <unistd.h>
    void clear() {
        system("clear");
    }
    void wait(int time)
    {
        sleep(time);
    }
    #define LINUX true
#endif

struct Question
{
    std::string title;
    std::string hint;
    std::string answer;
};

void load()
{
    // for (size_t i = 0; i < 3; ++i)
    // {
    //     // std::this_thread::sleep_for(1s);
    //     wait(1);
    //     std::cout << ".";
    // }
            wait(1);
        std::cout << ".";
                wait(1);
        std::cout << ".";
                wait(1);
        std::cout << ".";
}

int main ()
{
    std::vector<Question> questions;

    std::ifstream data_file{"helper.txt"};

    std::string temp[3];
    size_t index = 0;
    for (std::string line; std::getline(data_file, line); ++index)
    {
        if (line.length() == 0)
        {
            index = -1;
            continue;
        }

        temp[index] = line;
        if (index == 2)
        {
            Question q{temp[0], temp[1], temp[2]};
            questions.push_back(q);
        }
    }

    // for (Question q : questions)
    // {
    //     std::cout << q.title << std::endl << q.hint << std::endl << q.answer << std::endl << std::endl;
    // }

    while (true)
    {
        clear();
        std::cout << "\tBienvenido al Asistente de Estudio\n\t\tJuan de Dios Dugarte\n" << std::endl;
        std::cout << "1) Modo Ordenado\n2) Modo Aleatorio\n3) Salir\n\nIngrese su opcion [1, 2 o 3]: ";
        std::string option;
        std::cin >> option;
        if (option == "1")
        {
            std::getchar();
            for (index = 0; index < questions.size(); ++index)
            {
                clear();
                Question q = questions[index];
                std::cout << index + 1 << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
                if (q.answer == ans)
                {
                    clear();
                    std::cout << "\n\n\tRESPUESTA CORRECTA!! Quedan: "<< questions.size() - index - 1 << " Preguntas. Avanzando a la siguiente pregunta" << std::endl;
                        if(!LINUX) load();
                        else wait(3);
                    // clear();
                }
                else
                {
                    clear();
                    std::cout << "\n\n\tRESPUESTA INCORRECTA";
                        if(!LINUX) load();
                        else wait(3);
                    std::cout << "\n\tTu respuesta es: " << ans << std::endl;
                    std::cout << "\tLa respuesta correcta es: " << q.answer << std::endl << std::endl;
                    std::cout << "\n1) Repetir Pregunta, 2) Avanzar a la Siguiente Pregunta\n\nIngrese su opcion [1 o 2]: ";
                    std::cin >> option;
                    if (option == "1")
                    {
                        index = index - 1;
                        std::cout << "\nRepitiendo Pregunta";
                        if(!LINUX) load();
                        else wait(3);
                        clear();
                        std::getchar();
                    }
                }
            }
        }
        else if(option == "2")
        {

            std::random_device rd{};
            std::mt19937_64 rng(rd());

            std::getchar();

            std::vector<Question> questions_temp{questions};
            bool again = false;
            Question q;
            for (index = 0; questions_temp.size() > 0; ++index)
            {
                clear();
                std::uniform_int_distribution<size_t> dis{0, questions_temp.size() - 1};
                size_t num = dis(rng);
                // std::cout <<"\n\n\n" << num  << "\n\n\n";
                // std::this_thread::sleep_for(2s);
                if (!again)
                {
                    q = questions_temp[num];
                    std::vector<Question> q_temp_vec{};
                    Question q_aux;
                    while (true)
                    {
                        Question q_aux = questions_temp[questions_temp.size() - 1];
                        questions_temp.pop_back();
                        if (q_aux.title == q.title && q_aux.answer == q.answer && q.hint == q_aux.hint)
                        {
                            for (Question temp : q_temp_vec)
                            {
                                questions_temp.push_back(temp);
                            }
                            break;
                        }
                        q_temp_vec.push_back(q_aux);
                    }
                }
                std::cout << index + 1 << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
                if (q.answer == ans)
                {
                    clear();
                    std::cout << "\n\n\tRESPUESTA CORRECTA!! Quedan: "<< questions_temp.size() << " Preguntas. Avanzando a la siguiente pregunta" << std::endl;
                        if(!LINUX) load();
                        else wait(3);
                    again = false;
                }
                else
                {
                    clear();
                    std::cout << "\n\n\tRESPUESTA INCORRECTA";
                        if(!LINUX) load();
                        else wait(3);
                    std::cout << "\n\tTu respuesta es: " << ans << std::endl;
                    std::cout << "\tLa respuesta correcta es: " << q.answer << std::endl << std::endl;
                    std::cout << "\n1) Repetir Pregunta, 2) Saltar Pregunta\n\nIngrese su opcion [1 o 2]: ";
                    std::cin >> option;
                    if (option == "1")
                    {
                        --index;
                        again = true;
                        std::cout << "\nRepitiendo Pregunta";
                        if(!LINUX) load();
                        else wait(3);
                        std::getchar();
                        continue;
                    }
                    again = false;
                    std::getchar();
                }
            }
        }
        else if(option == "3")
        {
            break;
        }
        else
        {
            clear();
            std::cout << "\n\n\tOpcion no valida, intente nuevamente\n\n";
            // std::this_thread::sleep_for(2s);
            wait(2);
        }
    }

    return EXIT_SUCCESS;
}