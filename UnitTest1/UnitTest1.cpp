#include "pch.h"
#include "CppUnitTest.h"
#include "../lb.12.3/lb.12.3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::string testFilename = "testFile.dat";

			// Створюємо тестовий запис
			Zodiac testRecord;
			strcpy_s(testRecord.surname, "Іваненко");
			strcpy_s(testRecord.name, "Іван");
			strcpy_s(testRecord.sign, "Овен");
			testRecord.birthdate[0] = 1;
			testRecord.birthdate[1] = 4;
			testRecord.birthdate[2] = 1990;

			// Записуємо запис у файл
			writeRecordToFile(testFilename, testRecord);

			// Відкриваємо файл для перевірки
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"Файл не вдалося відкрити для читання!");

			Zodiac readRecord;
			file.read(reinterpret_cast<char*>(&readRecord), sizeof(Zodiac));

			// Перевіряємо, чи запис відповідає очікуваному
			Assert::AreEqual(std::string(testRecord.surname), std::string(readRecord.surname));
			Assert::AreEqual(std::string(testRecord.name), std::string(readRecord.name));
			Assert::AreEqual(std::string(testRecord.sign), std::string(readRecord.sign));
			Assert::AreEqual(testRecord.birthdate[0], readRecord.birthdate[0]);
			Assert::AreEqual(testRecord.birthdate[1], readRecord.birthdate[1]);
			Assert::AreEqual(testRecord.birthdate[2], readRecord.birthdate[2]);

			// Закриваємо файл
			file.close();

			// Видаляємо тестовий файл
			std::remove(testFilename.c_str());
		}

		// Тест для перевірки сортування
		TEST_METHOD(TestSortFileBySurname)
		{
			// Створюємо тимчасовий файл для тестування
			std::string testFilename = "testFile.dat";

			// Створюємо кілька записів
			Zodiac records[3] = {
				{"Петренко", "Петро", "Телець", {5, 5, 1995}},
				{"Іваненко", "Іван", "Овен", {1, 4, 1990}},
				{"Сидоренко", "Сидір", "Близнюки", {10, 6, 1985}}
			};

			// Записуємо їх у файл
			for (const auto& record : records) {
				writeRecordToFile(testFilename, record);
			}

			// Викликаємо функцію сортування
			sortFileBySurname(testFilename);

			// Відкриваємо файл і перевіряємо порядок
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"Файл не вдалося відкрити для читання!");

			Zodiac sortedRecords[3];
			for (int i = 0; i < 3; ++i) {
				file.read(reinterpret_cast<char*>(&sortedRecords[i]), sizeof(Zodiac));
			}

			// Очікуваний порядок: Іваненко, Петренко, Сидоренко
			Assert::AreEqual(std::string("Іваненко"), std::string(sortedRecords[0].surname));
			Assert::AreEqual(std::string("Петренко"), std::string(sortedRecords[1].surname));
			Assert::AreEqual(std::string("Сидоренко"), std::string(sortedRecords[2].surname));

			// Закриваємо файл
			file.close();

			// Видаляємо тестовий файл
			std::remove(testFilename.c_str());
		}

		// Тест для перевірки пошуку за знаком Зодіаку
		TEST_METHOD(TestSearchBySign)
		{
			// Створюємо тимчасовий файл для тестування
			std::string testFilename = "testFile.dat";

			// Створюємо кілька записів
			Zodiac records[3] = {
				{"Петренко", "Петро", "Телець", {5, 5, 1995}},
				{"Іваненко", "Іван", "Овен", {1, 4, 1990}},
				{"Сидоренко", "Сидір", "Близнюки", {10, 6, 1985}}
			};

			// Записуємо їх у файл
			for (const auto& record : records) {
				writeRecordToFile(testFilename, record);
			}

			// Відкриваємо файл для пошуку
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"Файл не вдалося відкрити для читання!");

			// Шукаємо "Овен"
			std::string searchSign = "Овен";
			Zodiac foundRecord;
			bool found = false;

			while (file.read(reinterpret_cast<char*>(&foundRecord), sizeof(Zodiac))) {
				if (std::string(foundRecord.sign) == searchSign) {
					found = true;
					break;
				}
			}

			// Перевіряємо результат пошуку
			Assert::IsTrue(found, L"Запис із знаком Зодіаку 'Овен' не знайдено!");
			Assert::AreEqual(std::string("Іваненко"), std::string(foundRecord.surname));

			// Закриваємо файл
			file.close();

			// Видаляємо тестовий файл
			std::remove(testFilename.c_str());
		}
	};
}
