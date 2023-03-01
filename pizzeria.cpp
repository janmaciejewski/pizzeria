#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>   

using namespace std;

struct MenuItem
{
    int id;
    string name;
    int price;
    string ingredients;
    int amount = 0;
};

//wyswietla powitanie i dane pizzeri
void powitanie()
{
    cout << "============================================================" << endl;
    cout << "                       PIZZERIA  PEPINO                " << endl;
    cout << "============================================================" << endl;
    cout << "----------------------Poznan ul. Szczygla 20---------------" << endl;
    cout << "------------------------Tel. 824 751 421--------------------" << endl;
    cout << "----------WITAMY W NASZEJ APLIKACJI DO ZAMOWIEN------------- " << endl << endl;
}

//pobiera godzine systemowa
time_t nowdataint = time(0);
tm* timenow = localtime(&nowdataint);
char* nowdatastring = ctime(&nowdataint);
unsigned int godzinyDostawy;

//-----------------------------------------------------------------------------------------

int numerStolika;
bool statusZamowienia;
string nr_telefonu;
string adresKlienta;
string name;

//pizzeria jest zamknieta
void godzinyOtwarcia()
{
    if (timenow->tm_hour < 12 || timenow->tm_hour>24 || timenow->tm_wday == 1)
    {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-    Pizzeria jest otwrta od poniedzialku do soboty od 12 do 24      -" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "Data i godzina teraz: " << nowdatastring << endl;
        cout << "Pizzeria jest zamknieta" << endl;

        if (statusZamowienia == 0)
        {
            cout << "Dlaczego wszedles do zamknietej pizzeri? Dzwonie po policje" << endl;
        }
    }
    else
    {
        if (statusZamowienia == 1)
        {
            cout << "-------------------------------------------------------------------" << endl;
            cout << "    Pizzeria jest otwrta od poniedzialku do soboty od 12 do 24      " << endl;
            cout << "-------------------------------------------------------------------" << endl;
            cout << "---------------------Aby wybrac czas oczekiwania--------------------" << endl;
            cout << "--------wpisz liczbe godzin do dostawy i wcisnij enter---" << endl;
            cout << "-------------------------------------------------------------------" << endl;
            cin >> godzinyDostawy;
            time_t laterdataint = nowdataint + godzinyDostawy * 3600;
            char* laterdatastring = ctime(&laterdataint);
            cout << "Dostawa: " << laterdatastring << endl;
            tm* timelater = localtime(&laterdataint);
            if (timelater->tm_hour < 12 || timelater->tm_hour>24 || timelater->tm_wday == 1)
            {
                cout << "Restauracja jest jeszcze zamknieta" << endl;
            }
        }

    }
}

//podanie imienia
//wybor na miejscu czy dostawa
void wyborZamowienia()
{
    cout << "Wprowadz swoje imie i nacisnij enter: " << endl;
    cin >> name;
    if (name.length() == 0) {
        cout << "Podano zle imie, sprobuj jeszcze raz!";
        wyborZamowienia();
    }
    cout << "\t\t" << name << " Zloz zamowienie: " << endl;
    cout << "---------------------------DOSTAWA---------------------------" << endl << endl;
    cout << "\t\t Na miejscu czy na dowoz?" << endl;
    cout << "\t [0] Na miejscu \t\t [1] Na dowoz" << endl;
    cout << "Wpisz 0, jesli na miejscu. Wpisz 1, jesli na dowoz i wcisnij enter " << endl;
    cin >> statusZamowienia;
    if (statusZamowienia == 0)
    {
        cout << "Wybrales opcje jedzenie na miejscu " << endl;
        cout << "Wybierz numer stolika od 1 do 10" << endl;
        cin >> numerStolika;
        if ((numerStolika <= 0) || (numerStolika > 10)) {
            cout << "Nie mamy takich stolikow" << endl;
            wyborZamowienia();
        }
        else
        {
            void godzinyOtwarcia();
        }
    }
    else
    {
        cout << "Wybrales opcje na dowoz " << endl;
        void godzinyOtwarcia();
        cout << "Wprowadz numer telefonu: " << endl;
        cin >> nr_telefonu;

        if (nr_telefonu.length() != 9) {
            cout << "Podano zły numer telefonu!" << endl;

            wyborZamowienia();
        }

        cout << "Wpisz adres i wcisnij enter " << endl;
        cin.ignore();
        getline(cin, adresKlienta);

        if (adresKlienta.empty()) {
            cout << "Podaj poprawny adres nastepnym razem";
            wyborZamowienia();
        }
        godzinyOtwarcia();

    }

}
//wektor menu jednej linii
vector<string> splitRawMenuItem(string rawMenuItemStr, char divider) {
    vector<string> temp;
    stringstream ss(rawMenuItemStr);
    string tok;

    while (getline(ss, tok, divider)) {
        temp.push_back(tok);
    }
    return temp;
}


//wektor menu wszystkie linie
vector<MenuItem> getMenuMapped() {
    vector<MenuItem> items;
    ifstream file("menu.txt");
    string str;
    int i = 0;
    while (getline(file, str)) {
        vector<string> formattedStr = splitRawMenuItem(str, ';');
        MenuItem item;
        item.id = stoi(formattedStr[0]);
        item.name = formattedStr[1];
        item.price = stoi(formattedStr[2]);
        item.ingredients = formattedStr[3];
        items.push_back(item);
        i++;
    }
    return items;
}

//dodaje odstepy do tabelki
string getBeautyStringFrom(MenuItem item) {
    string beautyName = item.name;
    for (int i = item.name.length(); beautyName.length() <= 18; i++)
        beautyName += ' ';

    string beautyIgridients = item.ingredients;
    if (item.ingredients == "None")
        beautyIgridients = "";

    for (int i = beautyIgridients.length(); beautyIgridients.length() <= 30; i++)
        beautyIgridients += ' ';

    string beautyId = to_string(item.id);
    if (item.id < 10)
        beautyId += " ";

    string beautyPrice = to_string(item.price);
    for (int i = beautyPrice.length(); beautyPrice.length() <= 2; i++)
        beautyPrice += ' ';

    string beautyAmount = "";
    if (item.amount != 0)
        beautyAmount = "x" + to_string(item.amount);
    return beautyId + " " + beautyName + beautyPrice + "       " + beautyIgridients + beautyAmount;
}

//pokazuje menu pizzeri
void stworzMenu(vector<MenuItem> menuItems) {
    cout << "------------------------------MENU---------------------------------" << endl << endl;
    cout << "-----------------------------Pizza---------------------------------" << endl;
    cout << "Nr---Nazwa---------Cena-----------------Skladniki------------------" << endl;
    for (int i = 0; i < menuItems.size(); i++)
    {
        string test = getBeautyStringFrom(menuItems[i]);
        cout << test << endl;
    }
}
vector<MenuItem> orderItems;

//podlicza cene
int podsumowanieZamowienia() {
    int totalPrice = 0;
    for (int i = 0; i < orderItems.size(); i++)
    {
        MenuItem orderItem = orderItems[i];
        totalPrice += (orderItem.amount * orderItem.price);
    }
    return totalPrice;
}

//wypisuje podsumnowanie zamowienia
void wypiszPodsumowanie() {
    if (orderItems.empty())
    {
        cout << "Twoje zamowienie jest na razie puste, sprobuj cos dodac :D" << endl;
        return;
    }

    cout << "-------------------------------------TWOJE ZAMOWIENIE------------------------------------" << endl;
    cout << "Twoje produkty:" << endl;
    cout << "Nr---Nr w menu---Nazwa---------Cena-----------------Skladniki-----------------------------------Ilosc-------------" << endl;
    for (int i = 0; i < orderItems.size(); i++)
    {
        string orderitemstring = to_string(i + 1) + ".       " + getBeautyStringFrom(orderItems[i]);
        cout << orderitemstring << endl;
    }
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "Suma: " + to_string(podsumowanieZamowienia()) + "zl" << endl;
}

//dodaje element do zamowienia
void dodajDoZamowienia(MenuItem item) {
    bool isOrderContainsItem = false;
    int amount;
    cout << "Ilosc?" << endl;
    cin >> amount;
    if (amount > 0)
    {
        for (int i = 0; i < orderItems.size(); i++)
        {
            if (orderItems[i].id == item.id)
            {
                orderItems[i].amount += amount;
                isOrderContainsItem = true;
                break;
            }
        }
        if (!isOrderContainsItem)
        {
            item.amount = amount;
            orderItems.push_back(item);
        }
    }
    else
        cout << "Nie mozesz podac takiej ilosci!";
}

//usuwa element z zamowienia
void usunzZamowienia(int id) {
    if (id <= orderItems.size()) {
        string amountStr;
        cout << "Podaj jaka ilosc chcesz usunac? (jezeli chcesz usunac wszystkie, po prostu wpisz 'wszystkie' i nacisnij enter)" << endl;
        cin >> amountStr;
        if (amountStr == "wszystkie")
            orderItems.erase(orderItems.begin() + (id - 1));
        else
        {
            int amount = stoi(amountStr);
            if (orderItems[id - 1].amount <= amount)
            {
                orderItems.erase(orderItems.begin() + (id - 1));
            }
            else
            {
                orderItems[id - 1].amount = orderItems[id - 1].amount - amount;
            }
        }
    }
    else
        cout << "Ten element nie znajduje się w zamowieniu!" << endl;
}

//pokazuje menu wyboru
void menuOpcji() {
    cout << endl << "-------------------------------------------------------------------" << endl;
    cout << endl << "Aby dodac danie do listy wybierz [0] i wcisnij enter" << endl;
    cout << "Aby usunac danie z listy wybierz [1] i wcisnij enter" << endl;
    cout << "Aby zlozyc zamowienie wybierz    [2] i wcisnij enter" << endl;
    cout << "Aby wyswietlic menu wybierz      [3] i wcisnij enter" << endl;
    cout << "Aby wyswietlic zamowienie wybierz[4] i wcisnij enter" << endl;
    cout << "Aby wyjsc wybierz                [q] i wcisnij enter" << endl;
    cout << "-------------------------------------------------------------------" << endl << endl;
}

//dodaje pozycje do zamowienia
void dodajPozycje(vector<MenuItem> menuItems) {
    int id;
    cout << "Wybierz numer dania do zamowienia i wcisnij enter" << endl;
    cin >> id;
    if (0 < id && id <= 15)
    {
        dodajDoZamowienia(menuItems[id - 1]);
        cout << "Dodano produkt do zamowienia." << endl;
    }
    else
    {
        cout << "Wprowadzono nieprawidlowa liczbe!" << endl;
    }
}

//usuwa pozycje z zamowienia
void usunPozycje() {
    if (!orderItems.empty())
    {
        wypiszPodsumowanie();
        cout << "Wybierz numer dania do usuniecia z zamowienia i wcisnij enter " << endl;
        int id;
        cin >> id;
        if (0 < id)
        {
            usunzZamowienia(id);
            cout << "Usunieto produkty." << endl;
        }
        else
            cout << "Nie ma takiego numeru" << endl;
    }
    else
        cout << "Twoje zamowienie jest puste, sprobuj najpierw cos dodac! :D" << endl;
}


//opcja zamkniecia do wrzucenia w funkcje
void pozegnanie()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << "----------------------ZAPRASZAMY PONOWNIE--------------------------" << endl;
    cout << "-------------------------------------------------------------------" << endl;
}

void rachunek()
{
    fstream save;
    save.open("rachunek.txt");
    save << "---------------------------------------Potwierdzenie-------------------------------------" << endl;
    save << "Pizzeria Pepino ul. Szczygla 20" << endl;
    time_t nowdataint = time(0);
    char* nowdatastring = ctime(&nowdataint);
    save << nowdatastring << endl;
    save << "-------------------------------------TWOJE ZAMOWIENIE------------------------------------" << endl;
    save << "Twoje produkty:" << endl;
    save << "Nr---Nr w menu---Nazwa---------Cena-----------------Skladniki----------Ilosc-------------" << endl;
    for (int i = 0; i < orderItems.size(); i++)
    {
        string orderitemstring = to_string(i + 1) + ".       " + getBeautyStringFrom(orderItems[i]);
        save << orderitemstring << endl;
    }
    save << "-----------------------------------------------------------------------------------------" << endl;
    save << "Suma: " + to_string(podsumowanieZamowienia()) + "zl" << endl;
    if (statusZamowienia = 0)
    {
        save << "Na miejscu" << endl;
        save << "Stolik numer" << numerStolika << endl;
    }
    else
    {
        save << "Dowoz     " << endl;
        save << "Telefon   ";
        save << nr_telefonu << endl;
        save << "Adres";
        save << adresKlienta << endl;

        time_t laterdataint = nowdataint + godzinyDostawy * 3600;
        char* laterdatastring = ctime(&laterdataint);
        save << "Dostawa: " << laterdatastring << endl;

    }
    save << "-------------------------------------ZAPRASZAMY PONOWNIE---------------------------------" << endl;
    save.close();
    cout << "Zapisano" << endl;

    pozegnanie();
}

void wyborRachunku()
{
    bool choose;
    cout << endl << "--------------------------------Czy zapisac potwierdzenie do pliku?----------------------------" << endl;
    cout << "Aby zakonczyc wybierz 0 i enter" << endl;
    cout << "Aby wydrukowac do pliku wybier 1 i enter" << endl;
    cin >> choose;
    if (choose == 1)
    {
        rachunek();
    }
    else
    {
        pozegnanie();
    }
}


void podsumowanie()
{
    if (!orderItems.empty()) {
        cout << "---------------------------------------Rachunek---------------------------------------" << endl;
        time_t nowdataint = time(0);
        char* nowdatastring = ctime(&nowdataint);
        cout << nowdatastring << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        cout << "Pizzeria Pepino ul. Szczygla 20" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        cout << name << endl;
        cout << "-----------------------------------rodzaj zamowienia------------------------------------" << endl;
        if (statusZamowienia == 0)
        {
            cout << "Na miejscu" << endl;
            cout << "Stolik numer" << numerStolika << endl;
        }
        else
        {
            cout << "Dowoz     " << endl;
            cout << "Telefon   ";
            cout << nr_telefonu << endl;
            cout << "Adres   ";
            cout << adresKlienta << endl;

            time_t laterdataint = nowdataint + godzinyDostawy * 3600;
            char* laterdatastring = ctime(&laterdataint);
            cout << "Dostawa: " << laterdatastring << endl;

        }
        wypiszPodsumowanie();
        wyborRachunku();
    }
    else
        cout << "Nic nie zostalo zamowione" << endl;
}

//----------------------------------------------------------------------------------------------------------------

//funkcja wyboru przy zamoweniu
void menuUzytkownika(vector<MenuItem> menuItems) {
    for (;;)
    {
        menuOpcji();
        char choice;
        cin >> choice;
        switch (choice)
        {
        case '0': 
            dodajPozycje(menuItems); 
            break;
        case '1': 
            usunPozycje(); 
            break;
        case '2': 
            podsumowanie(); 
            return;
        case '3': 
            stworzMenu(menuItems);
            break;
        case '4': 
            wypiszPodsumowanie(); 
            break;
        case 'q': 
            return;
        }
    }
}


//pokazuje menu pizzeri i menu wboru
void pokazMenu()
{
    vector<MenuItem> menuItems = getMenuMapped();
    stworzMenu(menuItems);
    menuUzytkownika(menuItems);
}

int main()
{
    powitanie();
    wyborZamowienia();
    pokazMenu();
    return 0;
}