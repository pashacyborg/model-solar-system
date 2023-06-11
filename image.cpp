#include <assert.h>
#include <fstream>
#include "image.h"

using namespace std;

Image::Image(char* _pixels, int _width, int _height)
{
	pixels = _pixels;
	width = _width;
	height = _height;
}

Image::~Image() {
	delete[] pixels;
}


	//использовано с https://stackoverflow.com/questions/27073283/converting-unsigned-char-to-int-and-short
	//нужны побитовые операции для работы с картинкой
//и весь код загрузчика изображений взят с
//https://coderoad.ru/5674512/%D0%A6%D0%B2%D0%B5%D1%82-%D1%82%D0%B5%D0%BA%D1%81%D1%82%D1%83%D1%80%D1%8B-%D0%BE%D1%82%D0%BE%D0%B1%D1%80%D0%B0%D0%B6%D0%B0%D0%B5%D1%82%D1%81%D1%8F-%D0%BD%D0%B5%D0%BF%D1%80%D0%B0%D0%B2%D0%B8%D0%BB%D1%8C%D0%BD%D0%BE

	// Преобразует четырехсимвольный массив в целое число с прямым порядком байтов.
	int toInt(const char* bytes) 
	{
		return (int)(((unsigned char)bytes[3] << 24)|((unsigned char)bytes[2] << 16)|((unsigned char)bytes[1] << 8)|(unsigned char)bytes[0]);
	}

	// Преобразует двухсимвольный массив в короткий, используя прямой порядок байтов
	short toShort(const char* bytes) 
	{
		return (short)(((unsigned char)bytes[1] << 8)|(unsigned char)bytes[0]);
	}

	// Читает следующие четыре байта как целое число с прямым порядком байтов
	int readInt(ifstream &input) 
	{
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	// Считывает следующие два байта как короткие, используя прямой порядок байтов
	short readShort(ifstream &input) 
	{
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	// Как auto_ptr, но для массивов
	template<class T>
	class auto_array 
	{

	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) : array(array_), isReleased(false) { }

		auto_array(const auto_array<T> &aarray) 
		{
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		~auto_array() 
		{
			if (!isReleased && array != NULL) 
			{
				delete[] array;
			}
		}

		T* get() const 
		{
			return array;
		}

		T &operator*() const 
		{
			return *array;
		}

		void operator=(const auto_array<T> &aarray) 
		{
			if (!isReleased && array != NULL) 
			{
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		T* operator->() const 
		{
			return array;
		}

		T* release() 
		{
			isReleased = true;
			return array;
		}

		void reset(T* array_ = NULL) 
		{
			if (!isReleased && array != NULL) 
			{
				delete[] array;
			}
			array = array_;
		}

		T* operator+(int i)
		{
			return array + i;
		}

		T &operator[](int i) 
		{
			return array[i];
		}
	};

	//функция для установки пикч в качестве текстуры
Image* loadBMP(const char* filename) 
{
	ifstream input;
	input.open(filename, ifstream::binary);
	char buffer[2];
	input.read(buffer, 2);
	input.ignore(8);
	int dataOffset = readInt(input);

	// Считываем заголовок
	int headerSize = readInt(input);
	int width = readInt(input);
	int height = readInt(input);


	//Считываем данные
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	//Получение данных при верном формате
	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			for (int c = 0; c < 3; c++) 
			{
				pixels2[3 * (width * y + x) + c] = pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	return new Image(pixels2.release(), width, height);
}