#include <stdio.h>
#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
// #pragma warning(disable : 4996)
#include <string.h>

#include <unordered_map>
using namespace std;
#define KEY_UP 38
#define KEY_DOWN 0x28
#define VK_RETURN 0x0D
int id = 1;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Employee
{
public:
    Employee(const std::string& name, int age, const std::string& department)
      : name(name)
      , age(age)
      , department(department)
    {
    }

    void work()
    {
        std::cout << "Выполняет обязанности сотрудника отдела " << department << std::endl;
    }

protected:
    std::string name;
    int age;
    std::string department;
};

class ChiefEditor : public Employee
{
public:
    ChiefEditor(const std::string& name, int age)
      : Employee(name, age, "Редакция")
    {
    }

    /*void manage() {
        std::cout << name << " управляет работой редакции" << std::endl;
    }*/
};

class Journalist : public Employee
{
public:
    Journalist(const std::string& name, int age)
      : Employee(name, age, "Журналистика")
    {
    }

    /*void writeArticle() {
        std::cout << name << " пишет статью" << std::endl;
    }*/
};

class LiteraryEditor : public Employee
{
public:
    LiteraryEditor(const std::string& name, int age)
      : Employee(name, age, "Литература")
    {
    }

    /*void editText() {
        std::cout << name << " редактирует текст" << std::endl;
    }*/
};

class Photographer : public Employee
{
public:
    Photographer(const std::string& name, int age)
      : Employee(name, age, "Фотография")
    {
    }

    /*void takePhoto() {
        std::cout << name << " делает фотографию" << std::endl;
    }*/
};

class Designer : public Employee
{
public:
    Designer(const std::string& name, int age)
      : Employee(name, age, "Дизайн")
    {
    }

    /*void designLayout() {
        std::cout << name << " разрабатывает дизайн макета" << std::endl;
    }*/
};

class Document
{
public:
    Document(const std::string& title, const std::string& author)
      : title(title)
      , author(author)
    {
    }

    /*void displayInfo() {
        std::cout << "Документ: " << title << ", Автор: " << author << std::endl;
    }*/

protected:
    std::string title;
    std::string author;
};

class MagazineLayout : public Document
{
public:
    MagazineLayout(const std::string& title, const std::string& author)
      : Document(title, author)
    {
    }

    /*void createLayout() {
        std::cout << "Создается макет для журнала: " << title << std::endl;
    }*/
};

class Article : public Document
{
public:
    Article(const std::string& title, const std::string& author)
      : Document(title, author)
    {
    }

    /*void writeContent() {
        std::cout << "Написана статья: " << title << " авторства " << author << std::endl;
    }*/
};

class PhotoReportage : public Document
{
public:
    PhotoReportage(const std::string& title, const std::string& author)
      : Document(title, author)
    {
    }

    /*void capturePhotos() {
        std::cout << "Создается фото-репортаж: " << title << " авторства " << author << std::endl;
    }*/
};

struct Abonent
{
    int id;
    int cost;
    string name;
    string author;
    string date;
    string book_name;
    string publisher;
    int deleted = 0;
};

struct Saver
{
    int id;
    int cost;
    char name[256];
    char author[256];
    char date[256];
    char book_name[256];
    char publisher[256];
    int deleted;
};

struct Debtor
{
    string name;
    int count = 0;
};

struct double_list
{
    Abonent abonent;
    double_list* next = nullptr;
    double_list* prev = nullptr;
};

Abonent Init(
    string firstname,
    string secondname,
    string surname,
    int cost,
    string author,
    string book_name,
    string publisher,
    string date
)
{
    Abonent new_abonent;
    new_abonent.id = id;
    id++;
    new_abonent.cost = cost;
    ostringstream oss1;

    new_abonent.date = date;
    new_abonent.author = author;
    oss1 << secondname << " " << firstname << " " << surname;
    new_abonent.name = (string) oss1.str().c_str();
    new_abonent.publisher = publisher;
    new_abonent.book_name = book_name;
    return new_abonent;
}

Abonent calculateAbonentBookCount(double_list& list);
double_list* Init_double_list(double_list* list, Abonent abonent1);
double_list* Sort_list_name(double_list* list, int a, double_list* beg);
double_list* Sort_list_cost(double_list* list, int a, double_list* beg);
double_list* Sort_list_id(double_list* list, int a, double_list* beg);
void Show_list(double_list* list, int n, int page);
void Show_abonent(Abonent people, int n);
void processDoubleList(double_list* list);
Abonent Get_param_from_keyboard();
void Save_txt(double_list* list);
void Save_bin(double_list* list);
double_list* Load_bin(double_list* list, double_list* beg);
double_list* find_abonent(double_list* list, int id);
void edit_abonent(double_list* pClient);
void delete_abonent(double_list* pClient);
int Age(string str1);
long avarage_age(double_list* list);
vector<string> menu1 = {
    "добавление элемента",
    "просмотр списка",
    "сохранение списка",
    "загрузка списка",
    "сортировка",
    "редактировать абонента",
    "удалить абонента",
    "найти кол-во задолженостей",
    "Найти абонента",
    "Выход",
    ""
};
vector<string> menu2 = {"Следующая", "Предыдущая", "Выход", ""};
vector<string> menu3 = {"ID", "ФИО", "ЦЕНА", ""};
void print_menu(int switcher, vector<string> menu);

int main()
{
    setlocale(LC_ALL, "Russian");
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0;
    DWORD InputsRead = 0;
    bool running = true;
    int switcher = 1;
    bool autorisation = true;
    string login;
    string password;
    INPUT_RECORD irInput;
    GetNumberOfConsoleInputEvents(hInput, &NumInputs);

    double_list* list = new double_list;
    list = NULL;
    Abonent a;
    Saver* s = new Saver;
    do
    {
        std::cout << "Aвторизация\nЛогин:";
        std::cin >> login;
        std::cout << "Пароль:";
        std::cin >> password;
        // cout<<login;
    }
    while (autorisation == false);
    while (running)
    {
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
        switch (irInput.EventType)
        {
        case KEY_EVENT:
            if (irInput.Event.KeyEvent.bKeyDown)
            {
                if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                {
                    if (switcher > 2)
                    {
                        switcher--;
                    }
                    system("cls");
                    print_menu(switcher, menu1);
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                {
                    if (switcher < menu1.size())
                    {
                        switcher++;
                    }
                    system("cls");
                    print_menu(switcher, menu1);
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                {
                    switch (switcher - 1)
                    {
                    case 1:
                        a = Get_param_from_keyboard();
                        list = Init_double_list(list, a);
                        system("cls");
                        Show_list(list, 0, 1);
                        break;
                    case 2:
                        system("cls");
                        printf(
                            "| %-5s | %-5s | %-39s | %-20s | %-10s | %-10s |\n",
                            "ID",
                            "Цена",
                            "ФИО",
                            "Даты выдачи",
                            "Книга",
                            "Издательство"
                        );
                        printf(
                            "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
                        );
                        Show_list(list, 0, 1);
                        {
                            int page = 1;
                            bool ans = 1;
                            switcher = 2;
                            while (ans)
                            {
                                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                switch (irInput.EventType)
                                {
                                case KEY_EVENT:
                                    if (irInput.Event.KeyEvent.bKeyDown)
                                    {
                                        if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                        {
                                            if (switcher > 2)
                                            {
                                                switcher--;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }
                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                        {
                                            if (switcher < menu2.size())
                                            {
                                                switcher++;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }

                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                        {
                                            switch (switcher - 1)
                                            {
                                            case 1:
                                            {
                                                page++;
                                                system("cls");
                                                printf(
                                                    "|%-7s| %-40s | %-20s | %-10s | %-6s |\n",
                                                    "ID",
                                                    "Цена",
                                                    "ФИО",
                                                    "Даты выдачи",
                                                    "Книга",
                                                    "Издательство"
                                                );
                                                printf(
                                                    "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
                                                );
                                                Show_list(list, 0, page);
                                                print_menu(switcher, menu2);
                                                system("pause");
                                                break;
                                            }
                                            case 2:
                                            {
                                                if (page != 1)
                                                {
                                                    page--;
                                                }
                                                system("cls");
                                                printf(
                                                    "|%-7s| %-40s | %-20s | %-10s | %-6s |\n",
                                                    "ID",
                                                    "ID",
                                                    "Цена",
                                                    "ФИО",
                                                    "Даты выдачи",
                                                    "Книга",
                                                    "Издательство"
                                                );
                                                printf(
                                                    "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
                                                );
                                                Show_list(list, 0, page);
                                                print_menu(switcher, menu2);
                                                system("pause");
                                                break;
                                            }
                                            default:
                                                ans = 0;
                                                system("cls");
                                                system("pause");
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    case 3:
                    {
                        system("cls");
                        Save_txt(list);
                        Save_bin(list);
                        break;
                    }
                    case 4:
                        list = NULL;
                        system("cls");
                        list = Load_bin(list, list);
                        break;
                    case 5:
                    {
                        system("cls");
                        int ans = 1;
                        int poriador = 1;
                        switcher = 1;
                        while (ans)
                        {
                            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                            switch (irInput.EventType)
                            {
                            case KEY_EVENT:
                                if (irInput.Event.KeyEvent.bKeyDown)
                                {
                                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                    {
                                        if (switcher > 2)
                                        {
                                            switcher--;
                                        }
                                        system("cls");
                                        print_menu(switcher, menu3);
                                    }
                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                    {
                                        if (switcher < menu3.size())
                                        {
                                            switcher++;
                                        }
                                        system("cls");
                                        print_menu(switcher, menu3);
                                    }
                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                    {
                                        switch (switcher - 1)
                                        {
                                        case 1:
                                        {
                                            Sort_list_id(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        case 2:
                                        {
                                            Sort_list_name(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        case 3:
                                        {
                                            Sort_list_cost(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        default:
                                            ans = 0;
                                            system("cls");
                                            system("pause");
                                            break;
                                        }
                                    }
                                }
                            default:
                                break;
                            }
                        }
                    }
                    break;
                    case 6:
                    {
                        int id_s;
                        system("cls");
                        cout << "Введите id\n", cin >> id_s;
                        edit_abonent(find_abonent(list, id_s));
                        break;
                    }
                    case 7:
                    {
                        int id_s;
                        system("cls");
                        cout << "Введите id\n", cin >> id_s;
                        delete_abonent(find_abonent(list, id_s));
                        break;
                    }
                    case 8:
                    {
                        system("cls");
                        processDoubleList(list);
                        system("cls");
                        break;
                    }
                    case 9:
                    {
                        int id_ss;
                        cout << "Введите искомое id" << endl, cin >> id_ss;
                        if (find_abonent(list, id_ss) == nullptr)
                        {
                            break;
                        }
                        Show_abonent(find_abonent(list, id_ss)->abonent, 1);
                        break;
                    }
                    case 10:
                        running = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void print_menu(int switcher, vector<string> menu)
{
    for (int i = 0; i < menu.size(); i++)
    {
        if (i + 2 == switcher)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            WORD saved_attributes = consoleInfo.wAttributes;
            SetConsoleTextAttribute(
                hConsole,
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE
                    | BACKGROUND_INTENSITY
            );
            cout << ">>" << menu[i] << endl;
            SetConsoleTextAttribute(hConsole, saved_attributes);
        }
        else
        {
            cout << "  " << menu[i] << endl;
        }
    }
}

double_list* Init_double_list(double_list* list, Abonent abonent1)
{
    if (list == NULL)
    {
        list = new double_list;
        list->next = NULL;
        list->prev = NULL;
        list->abonent = abonent1;
        return list;
    }
    else
    {
        double_list* p = list;
        list->next = Init_double_list(list->next, abonent1);
        list->next->prev = list;
    }
    return list;
}

void Show_abonent(Abonent people, int n)
{
    setlocale(LC_ALL, "ru");
    if (people.deleted != 1)
    {
        printf(
            "|%-7s|%-7s|%-40s | %-20s | %-10s | %-10s |\n",
            to_string(people.id).c_str(),
            to_string(people.cost).c_str(),
            people.name.c_str(),
            people.author.c_str(),
            people.book_name.c_str(),
            people.publisher.c_str()
        );
        printf(
            "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
        );
    }
}

Abonent Get_param_from_keyboard()
{
    string firstname, secondname, surname, date, book_name, author, publisher;
    int cost;
    cout << "\nИмя:\n";
    cin >> firstname;
    cout << "\nОтчество:\n";
    cin >> secondname;
    cout << "\nФамилия:\n";
    cin >> surname;
    cout << "\nДата выдачи:\n";
    cin >> date;
    cout << "\nИздатель:\n";
    cin >> publisher;
    cout << "\nНазвание книги\n";
    cin >> book_name;
    cout << "\nАвтор:\n";
    cin >> author;
    cout << "\nЦена:\n";
    cin >> cost;
    Abonent ne = Init(firstname, secondname, surname, cost, author, book_name, publisher, date);
    return ne;
}

void Save_txt(double_list* list)
{
    system("cls");
    string name;
    cout << "Введите имя файла для сохранения txt(вписать без .txt)\n", cin >> name;
    name.append(".txt");
    ofstream saver(name);
    if (!saver)
    {
        cout << "Error" << endl;
    }
    double_list* beg = list;
    int counter = 0;
    saver << "--------------------------------------------------------------" << endl;
    while (list)
    {
        counter++;
        saver << "ID: " << list->abonent.id << endl;
        saver << "COST:" << list->abonent.cost << endl;
        saver << "NAME: " << list->abonent.name << endl;
        saver << "BOOK_NAME: " << list->abonent.book_name << endl;
        saver << "AUTHOR: " << list->abonent.author << endl;
        saver << "DATE: " << list->abonent.date << endl;
        saver << "PUBLISHER: " << list->abonent.publisher << endl;
        saver << "DELETED: " << list->abonent.deleted << endl;
        saver << "--------------------------------------------------------------" << endl;
        list = list->next;
    }
    list = beg;
    saver.close();
    cout << "Сохранено";
}

void Save_bin(double_list* list)
{
    Saver element;
    system("cls");
    string name;
    cout << "Введите имя файла для сохранения bin(вписать без .bin)\n", cin >> name;
    name.append(".bin");
    ofstream saver(name, ios::binary);
    if (!saver)
    {
        cout << "Ошибка" << endl;
        return;
    }
    double_list* beg = list;
    while (list)
    {
        element.id = list->abonent.id;
        strncpy_s(element.name, const_cast<char*>(list->abonent.name.c_str()), 256);
        strncpy_s(element.author, const_cast<char*>(list->abonent.author.c_str()), 256);
        strncpy_s(element.date, const_cast<char*>(list->abonent.date.c_str()), 256);
        strncpy_s(element.book_name, const_cast<char*>(list->abonent.book_name.c_str()), 256);
        strncpy_s(element.publisher, const_cast<char*>(list->abonent.publisher.c_str()), 256);
        element.deleted = list->abonent.deleted;
        saver.write((char*) &element, sizeof(Saver));
        list = list->next;
    }
    list = beg;
    saver.close();
    cout << "Сохранено";
}

double_list* Load_bin(double_list* list, double_list* beg)
{
    id = 1;
    system("cls");
    string name;
    cout << "Введите имя файла для загрузки\n", cin >> name;
    name.append(".bin");
    ifstream loader(name);
    if (!loader)
    {
        cout << "Ошибка";
        return beg;
    }
    Saver element;
    Abonent el;
    loader.seekg(0);
    while (loader.read((char*) &element, sizeof(Saver)))
    {
        el.id = element.id;
        el.name = element.name;
        el.date = element.date;
        el.book_name = element.book_name;
        el.author = element.author;
        el.publisher = element.publisher;
        el.cost = element.cost;
        el.deleted = element.deleted;
        list = Init_double_list(list, el);
        id++;
    }
    return list;
}

void Show_list(double_list* list, int n, int page)
{
    if (list)
    {
        n += 1;
        if (n <= page * 20 && n > page * 20 - 20)
        {
            Show_abonent(list->abonent, n);
            Show_list(list->next, n, page);
        }
        else
        {
            if (n > page * 20)
            {
                return;
            }
            Show_list(list->next, n, page);
        }
    }
}

double_list* Sort_list_name(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.name > list->next->abonent.name)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_name(beg, a, beg);
            }
            else
            {
                return Sort_list_name(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* Sort_list_cost(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.cost > list->next->abonent.cost)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_cost(beg, a, beg);
            }
            else
            {
                return Sort_list_cost(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* Sort_list_id(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.id > list->next->abonent.id)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_id(beg, a, beg);
            }
            else
            {
                return Sort_list_id(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* find_abonent(double_list* list, int id)
{
    double_list* list1 = list;
    while (list1)
    {
        if (list1->abonent.id == id)
        {
            return list1;
        }
        list1 = list1->next;
    }
    cout << "Не найден\n";
    return nullptr;
}

void edit_abonent(double_list* pAbonent)
{
    if (pAbonent)
    {
        pAbonent->abonent = Get_param_from_keyboard();
    }
    else
    {
        cout << "Не найден";
    }
}

void delete_abonent(double_list* pAbonent)
{
    if (pAbonent)
    {
        pAbonent->abonent.deleted = 1;
        cout << "Удален" << endl;
    }
}

void processDoubleList(double_list* list)
{
    std::unordered_map<std::string, int> debtorCount;
    double_list* current = list;

    while (current != nullptr)
    {
        if (current->abonent.deleted == 0)
        {
            debtorCount[current->abonent.name]++;
        }
        current = current->next;
    }

    std::vector<Debtor> debtors;
    for (const auto& entry : debtorCount)
    {
        debtors.push_back({entry.first, entry.second});
    }

    std::sort(
        debtors.begin(),
        debtors.end(),
        [](const Debtor& a, const Debtor& b)
        {
            return a.count > b.count;
        }
    );
    for (int i = 0; i < 3 && i < debtors.size(); ++i)
    {
        std::cout << debtors[i].name << ": " << debtors[i].count << " задолженностей" << std::endl;
    }
    std::string filename;
    std::cout << "Введите имя файла для сохранения списка должников: ";
    std::cin >> filename;

    if (filename.find(".txt") == std::string::npos)
    {
        filename += ".txt";
    }
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        for (const auto& debtor : debtors)
        {
            outfile << debtor.name << ": " << debtor.count << " debts" << std::endl;
        }
        outfile.close();
        std::cout << "Список должников сохранен в файле '" << filename << "'" << std::endl;
    }
    else
    {
        std::cerr << "Не удалось открыть файл для записи" << std::endl;
    }
}
