#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

struct Zodiac {
    char surname[50];
    char name[50];
    char sign[20];
    int birthdate[3]; // [0] - день, [1] - місяць, [2] - рік
};

// Функція для введення одного запису
static Zodiac inputRecord() {
    Zodiac record{};

    std::cout << "Введіть прізвище: ";
    std::cin.ignore();
    std::cin.getline(record.surname, 50);

    std::cout << "Введіть ім'я: ";
    std::cin.getline(record.name, 50);

    std::cout << "Введіть знак Зодіаку: ";
    std::cin.getline(record.sign, 20);

    std::cout << "Введіть день народження (день місяць рік): ";
    while (!(std::cin >> record.birthdate[0] >> record.birthdate[1] >> record.birthdate[2])) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cerr << "Помилка! Введіть дату в форматі: день місяць рік.\n";
    }

    return record;
}

// Функція для запису одного запису у файл
static void writeRecordToFile(const std::string& filename, const Zodiac& record) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для запису!\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&record), sizeof(Zodiac));
    file.close();
}

// Функція для пошуку людей за знаком Зодіаку
static void searchBySign(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для читання!\n";
        return;
    }

    char searchSign[20];
    std::cout << "Введіть знак Зодіаку для пошуку: ";
    std::cin.ignore();
    std::cin.getline(searchSign, 20);

    bool found = false;
    Zodiac record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Zodiac))) {
        if (std::string(record.sign) == searchSign) {
            std::cout << "Прізвище: " << record.surname << ", Ім'я: " << record.name
                << ", Дата народження: " << record.birthdate[0] << "/"
                << record.birthdate[1] << "/" << record.birthdate[2] << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "Людей із знаком \"" << searchSign << "\" не знайдено.\n";
    }

    file.close();
}

// Функція для впорядкування записів у файлі за прізвищем
static void sortFileBySurname(const std::string& filename) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для сортування!\n";
        return;
    }

    // Зчитуємо розмір файлу
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    int recordCount = fileSize / sizeof(Zodiac);

    for (int i = 0; i < recordCount - 1; ++i) {
        for (int j = 0; j < recordCount - i - 1; ++j) {
            Zodiac record1, record2;

            // Читаємо два сусідніх записи
            file.seekg(j * sizeof(Zodiac));
            file.read(reinterpret_cast<char*>(&record1), sizeof(Zodiac));
            file.read(reinterpret_cast<char*>(&record2), sizeof(Zodiac));

            // Порівнюємо та переставляємо, якщо потрібно
            if (std::string(record1.surname) > std::string(record2.surname)) {
                file.seekp(j * sizeof(Zodiac));
                file.write(reinterpret_cast<const char*>(&record2), sizeof(Zodiac));
                file.write(reinterpret_cast<const char*>(&record1), sizeof(Zodiac));
            }
        }
    }

    std::cout << "Файл успішно впорядковано за прізвищами.\n";

    // Додаємо виведення відсортованих записів
    file.seekg(0, std::ios::beg); // Повертаємось на початок файлу
    Zodiac record;
    std::cout << "\nВідсортований список:\n";
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Zodiac))) {
        std::cout << "Прізвище: " << record.surname
            << ", Ім'я: " << record.name
            << ", Знак Зодіаку: " << record.sign
            << ", Дата народження: " << record.birthdate[0] << "/"
            << record.birthdate[1] << "/" << record.birthdate[2] << "\n";
    }

    file.close();
}

// Меню програми
static void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string filename;
    std::cout << "Введіть ім'я файлу для роботи: ";
    std::cin >> filename;

    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Введення даних у файл\n";
        std::cout << "2. Впорядкування записів за прізвищами\n";
        std::cout << "3. Пошук за знаком Зодіаку\n";
        std::cout << "4. Вихід\n";
        std::cout << "Ваш вибір: ";

        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cerr << "Помилка! Введіть число від 1 до 4.\n";
        }

        switch (choice) {
        case 1: {
            Zodiac record = inputRecord();
            writeRecordToFile(filename, record);
            break;
        }
        case 2:
            sortFileBySurname(filename);
            break;
        case 3:
            searchBySign(filename);
            break;
        case 4:
            std::cout << "Програма завершила роботу.\n";
            break;
        default:
            std::cout << "Неправильний вибір! Спробуйте ще раз.\n";
        }
    } while (choice != 4);
}

// Головна функція
int main() {
    menu();
    return 0;
}
