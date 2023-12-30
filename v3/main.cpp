#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

//Especial Chars
#include <wchar.h>
#include <locale.h>

//Regular Expression
#include <regex>

const std::regex regexp{"[a-zA-Z0-9áÁéÉíÍóÓúÚñÑĺ]"};
const std::regex regexp_only_letters1{"[a-zA-ZáÁéÉíÍóÓúÚñÑĺÚ]"};
const std::regex regexp_only_letters2{"[a-zA-Z]"};
// const std::regex regexp{"[a-zA-Z0-9áéíóúñÑĺ]"};
using namespace std::literals::chrono_literals;

#ifdef _WIN32
    #include <windows.h>
    void clear() {
        system("cls");
    }
    #define LINUX false
#else
    #include <unistd.h>
    void clear() {
        system("clear");
    }
    #define LINUX true
#endif

struct Question
{
    std::string title;
    std::string hint;
    std::vector<std::string> answer;
    std::vector<std::string> answer_original;
    size_t number;
};

std::string to_lower(std::string s)
{
    std::string result = "";

    for (size_t i = 0; i < s.length(); ++i)
    {
        result += std::tolower(s[i]);
    }

    return result;
}

bool check_only_numbers(std::string s)
{
    bool result = true;

    for (size_t i = 0; i < s.length(); ++i)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            result = false;
            break;
        }
    }
    
    return result;
}

std::string only_letters(std::string s)
{
    std::string result = "";

    for (size_t i = 0; i < s.length(); ++i)
    {
        if ((std::regex_match(std::string{s[i]}, regexp) || s[i] == ' ') && s[i] != '-')
        {
            result += s[i];
        }
    }

    return result;
}

std::vector<std::string> split(std::string string, char break_char = ' ')
{
    std::vector<std::string> result{};
    std::string temp = "";
    for (size_t i = 0; i < string.length(); ++i)
    {
        if (string[i] == break_char)
        {
            result.push_back(temp);
            temp = "";
            continue;
        }
        temp += string[i];
    }
    result.push_back(temp);
    return result;
}

void print_questions_vector_data(std::vector<Question>& questions)
{
    std::cout << "\nNumero de Preguntas: " << questions.size() << std::endl << std::endl;
    
    for (Question q: questions)
    {
        size_t index = q.number;
        std::cout << index << ". ";
        std::cout << q.title << std::endl;

        if (q.hint.length() > 0)
        {
            std::cout << index << ". ";
            std::cout << q.hint  << std::endl;
        }

        if (q.answer.size() == 0)
        {
            ++index;
            continue;
        }

        if (q.answer.size() > 1)
        {
            std::cout << index << ". Respuesta: " << std::endl;
            char letter = 'a';
            for (std::string s : q.answer)
            {
                std::cout << "\t" << letter++ << ") " << s << std::endl;
            }
        }
        else
        {
            std::cout << index << ". Respuesta: " << q.answer[0]  << std::endl  << std::endl;
        }

        std::cout << std::endl;

        ++index;
    }
}

void print_no_valid_option_message()
{
    std::cout << "\n\tOpcion no valida, intente nuevamente\n\n";
    std::this_thread::sleep_for(2s);
};

bool ask_clear_terminal()
{
    bool clear_terminal = false;
    do
    {
        std::string s_ans;
        std::cout << "\nDesea Borrar el Terminal? (S/N): ";
        std::cin >> s_ans;
        if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
        {
            clear_terminal = true;
            break;
        }
        else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
        {
            break;
        }
        print_no_valid_option_message();
    } while (true);
    return clear_terminal;
}

int main ()
{
    //Allow especial chars
    setlocale(LC_ALL, "");

    std::vector<Question> questions;

    std::ifstream data_file{"data.txt"};

    bool menu_flag = true;

    bool reading_answers = false;

    Question q{"", "", std::vector<std::string>{}};

    std::vector<std::string> initial_menu_lines{""};

    //Getting the questions
    size_t question_number = 0;
    bool auto_hint{false};
    while(!data_file.eof())
    {
        
        std::string line;
        std::getline(data_file, line);
        if(menu_flag && line == "MENU FIN")
        {
            menu_flag = false;
            continue;
        }
        if(menu_flag)
        {
            initial_menu_lines.push_back(line);
            continue;
        }
        if (line.length() == 0)
        {
            if (q.title != "")
            {
                q.number = ++question_number;
                questions.push_back(q);
            }

            q = Question{"", "", std::vector<std::string>{}};
            reading_answers = false;
            auto_hint = false;
            continue;
        }

        std::vector<std::string> temp{split(line)};

        auto return_letter = [&] (std::string temp_ans, size_t g)
        {
            std::string result_string{""};
            if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters2))
            {
                if (g + 1 < temp_ans.length())
                {
                    if (std::regex_match(std::string{temp_ans[g + 1]}, regexp_only_letters1) || temp_ans[g + 1] == ' ')
                    {                                
                        // q.hint += temp_ans[g];
                        // q.hint += ' ';
                        result_string += temp_ans[g];
                        result_string += ' ';
                        // break;
                    }
                }
            }
            else if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters1))
            {
                if (g + 1 < temp_ans.length())
                {
                    if (std::regex_match(std::string{temp_ans[g + 2]}, regexp_only_letters1) || temp_ans[g + 2] == ' ')
                    {                                
                        // q.hint += std::string{temp_ans[g]} + std::string{temp_ans[g + 1]};
                        // q.hint += ' ';
                        result_string += std::string{temp_ans[g]} + std::string{temp_ans[g + 1]};
                        result_string += ' ';
                        // break;
                    }
                }
            }
            return result_string;
        };

        for (size_t i = 0; i < temp.size(); ++i)
        {

            // auto take_letter = [&] (std::string temp_ans, size_t g)
            // {
            //     if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters2))
            //     {
            //         if (g + 1 < temp_ans.length())
            //         {
            //             if (std::regex_match(std::string{temp_ans[g + 1]}, regexp_only_letters1) || temp_ans[g] == ' ')
            //             {                                
            //                 q.hint += temp_ans[g];
            //                 q.hint += ' ';
            //                 // break;
            //             }
            //         }
            //     }
            //     else if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters1))
            //     {
            //         if (g + 1 < temp_ans.length())
            //         {
            //             if (std::regex_match(std::string{temp_ans[g + 2]}, regexp_only_letters1) || temp_ans[g] == ' ')
            //             {                                
            //                 q.hint += std::string{temp_ans[g]} + std::string{temp_ans[g + 1]};
            //                 q.hint += ' ';
            //                 // break;
            //             }
            //         }
            //     }
            // };

            std::string s = temp[i];
            if (s == "Pregunta:")
            {
                for (size_t j = i; j < temp.size(); ++j)
                {
                    q.title += temp[j];
                    q.title += " ";
                }
                q.title.pop_back();
                q.title.pop_back();
                break;
            }
            else if (s == "Pista:")
            {
                for (size_t j = i; j < temp.size(); ++j)
                {
                    q.hint += temp[j];
                    q.hint += " ";
                }

                q.hint.pop_back();

                if (q.hint == "Pista:" || q.hint == "Pista: ")
                {
                    q.hint = "";
                }

                if (q.hint == "Pista: \"LLENAR PISTA\"")
                {
                    q.hint = "Pista: ";
                    auto_hint = true;
                }
                
                break;
            }
            else if (s == "Respuesta:")
            {
                if (i != temp.size() - 1)
                {
                    std::string temp_ans = "";
                    for (size_t j = i + 1; j < temp.size(); ++j)
                    {
                        temp_ans += temp[j];
                        temp_ans += " ";
                    }
                    temp_ans.pop_back();
                    q.answer_original.push_back(temp_ans);
                    q.answer.push_back(only_letters(to_lower(temp_ans)));

                    if (auto_hint)
                    {
                        // q.hint += temp_ans[0];
                        bool take{true};
                        for (size_t g = 0; g < temp_ans.length(); ++g)
                        {
                            if (take)
                            {
                                if (temp_ans[g] == '(' && (g + 1) < temp_ans.length())
                                {
                                    q.hint += "(";
                                    // q.hint +=  temp_ans[g + 1];
                                    q.hint +=  return_letter(temp_ans, g + 1);
                                    q.hint +=  ") ";   
                                    ++g;
                                    take = false;
                                    continue;
                                }
                                // q.hint += temp_ans[g];
                                // q.hint += " ";
                                q.hint += return_letter(temp_ans, g);
                                take = false;
                            }
                            if (temp_ans[g] == ' ')
                            {
                                take = true;
                            }
                            if (temp_ans[g] == ',' || temp_ans[g] == ';' || temp_ans[g] == ':' || temp_ans[g] == '.' || temp_ans[g] == '?' || temp_ans[g] == '!' || temp_ans[g] == '$')
                            {
                                q.hint.pop_back();
                                q.hint += temp_ans[g];
                                q.hint += ' ';
                            }
                        }
                        auto_hint = false;
                    }
                    

                    break;
                }
                reading_answers = true;
            }
        }
        if (reading_answers)
        {
            if (temp[1] == "Respuesta:")
            {
                continue;
            }

            std::string temp_ans = "";
            for (size_t i = 0; i < temp.size(); ++i)
            {
                temp_ans += temp[i];
                temp_ans += " ";
            }
            temp_ans.pop_back();
            q.answer_original.push_back(temp_ans);
            q.answer.push_back(only_letters(to_lower(temp_ans)));


            if (auto_hint)
            {
                for (size_t g = 0; g < temp_ans.length(); ++g)
                {
                    // if (std::tolower(temp_ans[g]) >= 97 && std::tolower(temp_ans[g]) <= 122)
                    // if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters2))
                    // {
                    //     if (g + 1 < temp_ans.length())
                    //     {
                    //         if (std::regex_match(std::string{temp_ans[g + 1]}, regexp_only_letters1) || temp_ans[g] == ' ')
                    //         {                                
                    //             q.hint += temp_ans[g];
                    //             q.hint += ' ';
                    //             break;
                    //         }
                    //     }
                    // }
                    // else if (std::regex_match(std::string{temp_ans[g]}, regexp_only_letters1))
                    // {
                    //     if (g + 1 < temp_ans.length())
                    //     {
                    //         if (std::regex_match(std::string{temp_ans[g + 2]}, regexp_only_letters1) || temp_ans[g] == ' ')
                    //         {                                
                    //             q.hint += std::string{temp_ans[g]} + std::string{temp_ans[g + 1]};
                    //             q.hint += ' ';
                    //             break;
                    //         }
                    //     }
                    // }
                    std::string lambda_result{return_letter(temp_ans, g)};
                    if (lambda_result != "")
                    {
                        q.hint += lambda_result;
                        break;
                    }
                    
                }
            }
        }
    }

    q.number = ++question_number;
    questions.push_back(q);

    data_file.close();

    auto print_initial_menu = [&initial_menu_lines] ()
    {
        // clear();

        // std::cout << "\tBienvenido al Asistente de Estudio de\n\t    Juan De Dios Dugarte Masso" << std::endl;
        std::cout << std::endl;
        for (std::string line : initial_menu_lines)
        {
            std::cout << line << std::endl;
        }
    };

    std::string option{""};

    do
    {
        if (option == "")
        {            
            clear();
            std::cout << "\tBienvenido al Asistente de Estudio de\n\t    Juan De Dios Dugarte Masso" << std::endl;
            print_initial_menu();
            std::cout << "Seleccione de que manera le gustaria trabajar:" << std::endl;
            std::cout << "1) Modo Ordenado\n2) Modo Aleatorio\n3) Salir\n\nIngrese su opcion [1, 2 o 3]: ";
            std::cin >> option;
            std::getchar();
        }
        if (option == "1")
        {
            option = "";
            std::string start{""}, end{""};

            do
            {
                std::cout << "Seleccione con cual pregunta desea empezar: ";
                std::cin >> start;
                if (check_only_numbers(start))
                {
                    if (std::stoi(start) > 0)
                    {
                        break;
                    }
                }
                print_no_valid_option_message();
            } while (true);

            do
            {
                std::cout << "Seleccione con cual pregunta desea terminar: ";
                std::cin >> end;
                if (check_only_numbers(end))
                {
                    if (std::stoi(end) <= questions.size())
                    {
                        break;
                    }
                }
                print_no_valid_option_message();
            } while (true);

            size_t start_index = std::stoi(start) - 1;
            size_t end_index = std::stoi(end);

            std::getchar();
            bool clear_terminal = true;
            for (size_t index = start_index; index < end_index; ++index)
            {
                if(clear_terminal){
                    clear();
                }
                else 
                    clear_terminal = true;
                Question q = questions[index];
                std::cout << index + 1 << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
                
                size_t words_match_counter = 0;
                std::string to_show = "";

                auto check_sentences_match = [&words_match_counter, &to_show] (std::string s1, std::string s2)
                {
                    //Lo mejor es que s1 sea el string de la respuesta ya correcta.
                    std::vector<std::string> v1{split(only_letters(to_lower(s1)))};
                    std::vector<std::string> v2{split(only_letters(to_lower(s2)))};
                    bool result = true;
                    for (std::string s1_temp : v1)
                    {
                        if (std::any_of(v2.begin(), v2.end(), [&s1_temp] (std::string s2_temp) { return s1_temp == s2_temp; }))
                        {
                            ++words_match_counter;
                            to_show += s1_temp;
                            to_show += ' ';
                            continue;
                        }
                        for (size_t i = 0; i < s1_temp.length(); ++i)
                        {
                            to_show += "_ ";
                        }
                    }
                    to_show.pop_back();
                    return words_match_counter == v1.size();
                };

                std::string big_ans = "";

                for (std::string ans_item : q.answer)
                {
                    big_ans += ans_item + ' ';
                }

                big_ans.pop_back();

                if (!check_sentences_match(big_ans, ans))
                {
                    std::cout << "\nIncorrecto... ";
                }
                else std::cout << "\nExcelente!!! ";

                std::cout << "Numero de palabras acertadas: " << words_match_counter << " / " << split(big_ans).size() << std::endl << std::endl;
                
                std::cout << "Coincidencias de tu Respuesta: " << to_show << std::endl << std::endl;

                std::string original_ans = "";
                if (q.answer_original.size() > 1)
                {
                    original_ans += "\n\n";
                }

                for (std::string temp_s: q.answer_original)
                {
                    std::string no_space = temp_s;
                    no_space.replace(0, 0, "");
                    original_ans += no_space + "\n";
                }

                std::cout << q.title << ": " << original_ans << std::endl;

                std::string opt{""};

                std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";

                bool menu = false;
                do
                {
                    std::cin >> opt;

                    bool flag = false;

                    if (check_only_numbers(opt))
                    {
                        switch (std::stoi(opt))
                        {
                            case 1: //Continuar
                                flag = true;
                                break;
                            case 2: //Ir al Menu
                                menu = true;
                                flag = true;
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        menu = true;
                                        do
                                        {
                                            std::cout << "\nDesea Solo Imprimir el Menu? (S/N): ";
                                            std::cin >> s_ans;
                                            if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                            {
                                                menu = false;
                                                flag = false;
                                                // std::cout << std::endl;
                                                // for (std::string line : initial_menu_lines)
                                                // {
                                                //     std::cout << line << std::endl << std::endl;
                                                // }
                                                print_initial_menu();
                                                std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";
                                                break;
                                            }
                                            else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                            {
                                                break;
                                            }
                                            std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                        } while (true);
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        menu = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            case 3: //Repetir
                                --index;
                                flag = true;
                                clear_terminal = ask_clear_terminal();
                                std::cout << std::endl;
                                break;
                            case 4: //Volver
                                index -= 2;
                                flag = true;
                                break;
                            case 5: //Reiniciar
                                
                                flag = true;
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        index = start_index - 1;
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            case 6: //Ir a
                                flag = true;
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        do
                                        {
                                            std::string idx;
                                            std::cout << "\n\nIngrese a donde desea ir [" << start_index + 1 << " - " << end_index << "]: ";
                                            std::cin >> idx;
                                            if (check_only_numbers(idx))
                                            {
                                                size_t test_idx = std::stoi(idx) - 1; 
                                                if (test_idx >= start_index && test_idx < end_index)
                                                {
                                                    index = test_idx - 1;
                                                    break;
                                                }
                                            }
                                            std::cout << "\nOpcion no valida, Ingresa nueva opcion [" << start_index + 1 << " - " << end_index << "]: ";
                                        } while (true);
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            case 7: //Redefinir Rango
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        menu = true;
                                        option = "1";
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu\t3) Repetir" << std::endl << "4) Volver\t5) Reiniciar\t6) Ir a"  << std::endl << "7) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 7]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            default:
                                std::cout << "\nOpcion no valida, Ingresa nueva opcion [1, 2, ..., 7]: ";
                                break;
                        }
                    }
                    else
                    {
                        std::cout << "\nOpcion no valida, Ingresa nueva opcion [1, 2, ..., 7]: ";
                    }

                    if (flag)
                    {
                        break;
                    }

                } while (true);

                if (menu)
                {
                    break;
                }  
                std::getchar();
            }
        }
        else if(option == "2")
        {
            option = "";
            std::random_device rd{};
            std::mt19937_64 rng(rd());

            std::string start{""}, end{""};

            do
            {
                std::cout << "Seleccione con cual pregunta desea empezar: ";
                std::cin >> start;
                if (check_only_numbers(start))
                {
                    if (std::stoi(start) > 0)
                    {
                        break;
                    }
                }
                print_no_valid_option_message();
            } while (true);

            do
            {
                std::cout << "Seleccione con cual pregunta desea terminar: ";
                std::cin >> end;
                if (check_only_numbers(end))
                {
                    if (std::stoi(end) <= questions.size())
                    {
                        break;
                    }
                }
                print_no_valid_option_message();
            } while (true);

            size_t start_index = std::stoi(start) - 1;
            size_t end_index = std::stoi(end);

            std::getchar();

            std::vector<Question> questions_temp{};
            for (size_t g = start_index; g < end_index; ++g)
            {
                questions_temp.push_back(questions[g]);
            }
            
            bool again = false;
            Question q;

            bool clear_terminal = true;
            while (questions_temp.size() > 0)
            {
                if (clear_terminal) clear();
                else clear_terminal = true;
                size_t index;
                std::uniform_int_distribution<size_t> dis{0, questions_temp.size() - 1};
                size_t num = dis(rng);
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

                    index = q.number;
                }

                std::cout << index << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
                
                size_t words_match_counter = 0;
                std::string to_show = "";

                auto check_sentences_match = [&words_match_counter, &to_show] (std::string s1, std::string s2)
                {
                    //Lo mejor es que s1 sea el string de la respuesta ya correcta.
                    std::vector<std::string> v1{split(only_letters(to_lower(s1)))};
                    std::vector<std::string> v2{split(only_letters(to_lower(s2)))};
                    bool result = true;
                    for (std::string s1_temp : v1)
                    {
                        if (std::any_of(v2.begin(), v2.end(), [&s1_temp] (std::string s2_temp) { return s1_temp == s2_temp; }))
                        {
                            ++words_match_counter;
                            to_show += s1_temp;
                            to_show += ' ';
                            continue;
                        }
                        for (size_t i = 0; i < s1_temp.length(); ++i)
                        {
                            to_show += "_ ";
                        }
                    }
                    to_show.pop_back();
                    return words_match_counter == v1.size();
                };

                std::string big_ans = "";

                for (std::string ans_item : q.answer)
                {
                    big_ans += ans_item + ' ';
                }

                big_ans.pop_back();

                if (!check_sentences_match(big_ans, ans))
                {
                    std::cout << "\nIncorrecto... ";
                }
                else std::cout << "\nExcelente!!! ";

                std::cout << "Numero de palabras acertadas: " << words_match_counter << " / " << split(big_ans).size() << std::endl << std::endl;
                
                std::cout << "Coincidencias de tu Respuesta: " << to_show << std::endl << std::endl;

                
                std::string original_ans = "";

                if (q.answer_original.size() > 1)
                {
                    original_ans += "\n\n";
                }
                
                for (std::string temp_s: q.answer_original)
                {
                    std::string no_space = temp_s;
                    no_space.replace(0, 0, "");
                    original_ans += no_space + "\n";
                }
                
                std::cout << q.title << ": " << original_ans << std::endl;

                std::string opt{""};

                std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu" << std::endl << "3) Repetir\t4) Reiniciar" << std::endl << "5) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 5]: ";

                bool menu = false;
                do
                {
                    std::cin >> opt;

                    bool flag = false;

                    if (check_only_numbers(opt))
                    {
                        switch (std::stoi(opt))
                        {
                            case 1: //Continuar
                                flag = true;
                                again = false;
                                break;
                            case 2: //Ir al Menu
                                menu = true;
                                flag = true;
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        menu = true;
                                        do
                                        {
                                            std::cout << "\nDesea Solo Imprimir el Menu? (S/N): ";
                                            std::cin >> s_ans;
                                            if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                            {
                                                menu = false;
                                                flag = false;
                                                // std::cout << std::endl;
                                                // for (std::string line : initial_menu_lines)
                                                // {
                                                //     std::cout << line << std::endl;dasdasd
                                                // }
                                                print_initial_menu();
                                                std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu" << std::endl << "3) Repetir\t4) Reiniciar" << std::endl << "5) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 5]: ";
                                                break;
                                            }
                                            else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                            {
                                                break;
                                            }
                                            std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                        } while (true);
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        menu = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu" << std::endl << "3) Repetir\t4) Reiniciar" << std::endl << "5) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 5]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            case 3: //Repetir
                                flag = true;
                                clear_terminal = ask_clear_terminal();
                                std::cout << std::endl;
                                break;
                            case 4: //Reiniciar
                                flag = true;
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        again = false;
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu" << std::endl << "3) Repetir\t4) Reiniciar" << std::endl << "5) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 5]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            case 5: //Redefinir Rango
                                do
                                {
                                    std::string s_ans;
                                    std::cout << "\nEstas seguro? (S/N): ";
                                    std::cin >> s_ans;
                                    if (to_lower(s_ans) == "si" || to_lower(s_ans) == "s")
                                    {
                                        flag = true;
                                        menu = true;
                                        option = "2";
                                        break;
                                    }
                                    else if (to_lower(s_ans) == "no" || to_lower(s_ans) == "n")
                                    {
                                        flag = false;
                                        std::cout << "Que desea hacer?: " << std::endl << "1) Continuar\t2) Ir al Menu" << std::endl << "3) Repetir\t4) Reiniciar" << std::endl << "5) Redefinir Rango" << std::endl <<"Ingrese su opcion [1, 2, ..., 5]: ";
                                        break;
                                    }
                                    std::cout << "\nOpcion no valida, Ingresa nueva opcion: ";
                                } while (true);
                                break;
                            default:
                                std::cout << "\nOpcion no valida, Ingresa nueva opcion [1, 2, ..., 5]: ";
                                break;
                        }
                    }
                    else
                    {
                        std::cout << "\nOpcion no valida, Ingresa nueva opcion [1, 2, ..., 5]: ";
                    }

                    if (flag)
                    {
                        break;
                    }

                } while (true);

                if (menu)
                {
                    break;
                }  
                std::getchar();
            }
        }
        else if(option == "3")
        {
            break;
        }
        else
        {
            print_no_valid_option_message();
            option = "";
        }
    } while (true);
    

    return EXIT_SUCCESS;
}
