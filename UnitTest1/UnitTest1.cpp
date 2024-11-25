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

			// ��������� �������� �����
			Zodiac testRecord;
			strcpy_s(testRecord.surname, "��������");
			strcpy_s(testRecord.name, "����");
			strcpy_s(testRecord.sign, "����");
			testRecord.birthdate[0] = 1;
			testRecord.birthdate[1] = 4;
			testRecord.birthdate[2] = 1990;

			// �������� ����� � ����
			writeRecordToFile(testFilename, testRecord);

			// ³�������� ���� ��� ��������
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"���� �� ������� ������� ��� �������!");

			Zodiac readRecord;
			file.read(reinterpret_cast<char*>(&readRecord), sizeof(Zodiac));

			// ����������, �� ����� ������� �����������
			Assert::AreEqual(std::string(testRecord.surname), std::string(readRecord.surname));
			Assert::AreEqual(std::string(testRecord.name), std::string(readRecord.name));
			Assert::AreEqual(std::string(testRecord.sign), std::string(readRecord.sign));
			Assert::AreEqual(testRecord.birthdate[0], readRecord.birthdate[0]);
			Assert::AreEqual(testRecord.birthdate[1], readRecord.birthdate[1]);
			Assert::AreEqual(testRecord.birthdate[2], readRecord.birthdate[2]);

			// ��������� ����
			file.close();

			// ��������� �������� ����
			std::remove(testFilename.c_str());
		}

		// ���� ��� �������� ����������
		TEST_METHOD(TestSortFileBySurname)
		{
			// ��������� ���������� ���� ��� ����������
			std::string testFilename = "testFile.dat";

			// ��������� ����� ������
			Zodiac records[3] = {
				{"��������", "�����", "������", {5, 5, 1995}},
				{"��������", "����", "����", {1, 4, 1990}},
				{"���������", "����", "��������", {10, 6, 1985}}
			};

			// �������� �� � ����
			for (const auto& record : records) {
				writeRecordToFile(testFilename, record);
			}

			// ��������� ������� ����������
			sortFileBySurname(testFilename);

			// ³�������� ���� � ���������� �������
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"���� �� ������� ������� ��� �������!");

			Zodiac sortedRecords[3];
			for (int i = 0; i < 3; ++i) {
				file.read(reinterpret_cast<char*>(&sortedRecords[i]), sizeof(Zodiac));
			}

			// ���������� �������: ��������, ��������, ���������
			Assert::AreEqual(std::string("��������"), std::string(sortedRecords[0].surname));
			Assert::AreEqual(std::string("��������"), std::string(sortedRecords[1].surname));
			Assert::AreEqual(std::string("���������"), std::string(sortedRecords[2].surname));

			// ��������� ����
			file.close();

			// ��������� �������� ����
			std::remove(testFilename.c_str());
		}

		// ���� ��� �������� ������ �� ������ ������
		TEST_METHOD(TestSearchBySign)
		{
			// ��������� ���������� ���� ��� ����������
			std::string testFilename = "testFile.dat";

			// ��������� ����� ������
			Zodiac records[3] = {
				{"��������", "�����", "������", {5, 5, 1995}},
				{"��������", "����", "����", {1, 4, 1990}},
				{"���������", "����", "��������", {10, 6, 1985}}
			};

			// �������� �� � ����
			for (const auto& record : records) {
				writeRecordToFile(testFilename, record);
			}

			// ³�������� ���� ��� ������
			std::ifstream file(testFilename, std::ios::binary);
			Assert::IsTrue(file.is_open(), L"���� �� ������� ������� ��� �������!");

			// ������ "����"
			std::string searchSign = "����";
			Zodiac foundRecord;
			bool found = false;

			while (file.read(reinterpret_cast<char*>(&foundRecord), sizeof(Zodiac))) {
				if (std::string(foundRecord.sign) == searchSign) {
					found = true;
					break;
				}
			}

			// ���������� ��������� ������
			Assert::IsTrue(found, L"����� �� ������ ������ '����' �� ��������!");
			Assert::AreEqual(std::string("��������"), std::string(foundRecord.surname));

			// ��������� ����
			file.close();

			// ��������� �������� ����
			std::remove(testFilename.c_str());
		}
	};
}
