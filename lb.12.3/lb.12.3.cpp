#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

struct Zodiac {
    char surname[50];
    char name[50];
    char sign[20];
    int birthdate[3]; // [0] - ����, [1] - �����, [2] - ��
};

// ������� ��� �������� ������ ������
static Zodiac inputRecord() {
    Zodiac record{};

    std::cout << "������ �������: ";
    std::cin.ignore();
    std::cin.getline(record.surname, 50);

    std::cout << "������ ��'�: ";
    std::cin.getline(record.name, 50);

    std::cout << "������ ���� ������: ";
    std::cin.getline(record.sign, 20);

    std::cout << "������ ���� ���������� (���� ����� ��): ";
    while (!(std::cin >> record.birthdate[0] >> record.birthdate[1] >> record.birthdate[2])) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cerr << "�������! ������ ���� � ������: ���� ����� ��.\n";
    }

    return record;
}

// ������� ��� ������ ������ ������ � ����
static void writeRecordToFile(const std::string& filename, const Zodiac& record) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "������� �������� ����� ��� ������!\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&record), sizeof(Zodiac));
    file.close();
}

// ������� ��� ������ ����� �� ������ ������
static void searchBySign(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "������� �������� ����� ��� �������!\n";
        return;
    }

    char searchSign[20];
    std::cout << "������ ���� ������ ��� ������: ";
    std::cin.ignore();
    std::cin.getline(searchSign, 20);

    bool found = false;
    Zodiac record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Zodiac))) {
        if (std::string(record.sign) == searchSign) {
            std::cout << "�������: " << record.surname << ", ��'�: " << record.name
                << ", ���� ����������: " << record.birthdate[0] << "/"
                << record.birthdate[1] << "/" << record.birthdate[2] << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "����� �� ������ \"" << searchSign << "\" �� ��������.\n";
    }

    file.close();
}

// ������� ��� ������������� ������ � ���� �� ��������
static void sortFileBySurname(const std::string& filename) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "������� �������� ����� ��� ����������!\n";
        return;
    }

    // ������� ����� �����
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    int recordCount = fileSize / sizeof(Zodiac);

    for (int i = 0; i < recordCount - 1; ++i) {
        for (int j = 0; j < recordCount - i - 1; ++j) {
            Zodiac record1, record2;

            // ������ ��� ������ ������
            file.seekg(j * sizeof(Zodiac));
            file.read(reinterpret_cast<char*>(&record1), sizeof(Zodiac));
            file.read(reinterpret_cast<char*>(&record2), sizeof(Zodiac));

            // ��������� �� �������������, ���� �������
            if (std::string(record1.surname) > std::string(record2.surname)) {
                file.seekp(j * sizeof(Zodiac));
                file.write(reinterpret_cast<const char*>(&record2), sizeof(Zodiac));
                file.write(reinterpret_cast<const char*>(&record1), sizeof(Zodiac));
            }
        }
    }

    std::cout << "���� ������ ������������ �� ���������.\n";

    // ������ ��������� ������������ ������
    file.seekg(0, std::ios::beg); // ����������� �� ������� �����
    Zodiac record;
    std::cout << "\n³����������� ������:\n";
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Zodiac))) {
        std::cout << "�������: " << record.surname
            << ", ��'�: " << record.name
            << ", ���� ������: " << record.sign
            << ", ���� ����������: " << record.birthdate[0] << "/"
            << record.birthdate[1] << "/" << record.birthdate[2] << "\n";
    }

    file.close();
}

// ���� ��������
static void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string filename;
    std::cout << "������ ��'� ����� ��� ������: ";
    std::cin >> filename;

    int choice;
    do {
        std::cout << "\n����:\n";
        std::cout << "1. �������� ����� � ����\n";
        std::cout << "2. ������������� ������ �� ���������\n";
        std::cout << "3. ����� �� ������ ������\n";
        std::cout << "4. �����\n";
        std::cout << "��� ����: ";

        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cerr << "�������! ������ ����� �� 1 �� 4.\n";
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
            std::cout << "�������� ��������� ������.\n";
            break;
        default:
            std::cout << "������������ ����! ��������� �� ���.\n";
        }
    } while (choice != 4);
}

// ������� �������
int main() {
    menu();
    return 0;
}
