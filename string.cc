#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstring>
#include <cctype>

#define STRING_DEFAULT_SIZE 64

class String
{
private:
	int length;
	int currSize;
	char *str;

public:
	String(int size)
	{
		currSize = size;
		// printf("Ceating a String instance!\n");
		length = 0;
		str = (char *)malloc(sizeof(char) * currSize);
		str[0] = 0;
	}

	String() : String(STRING_DEFAULT_SIZE) {}

	~String()
	{
		free(str);
	}

	void Print()
	{
		printf("%s\n", str);
	}

	void Set(int i, char c)
	{
		while (i + 1 >= currSize)
		{
			char *tstr = (char *)realloc((void *)str, currSize * 2);
			// enlarge the buffer size

			if (!tstr)
			{
				fprintf(stderr, "ERR: Unable to allocate memory!");
				return;
			}
			this->str = tstr;
			currSize = currSize * 2;
		}

		if (i > length)
			for (int j = length; j < i; j++)
				str[j] = ' ';

		str[i] = c;
		if (i >= length)
		{
			length = i + 1;
			str[length] = 0;
		}
	}

	void Set(int i, const String &s)
	{
		int len = s.GetLength();
		for (int j = 0; j < len; j++)
		{
			this->Set(i + j, s.GetChar(j));
		}
	}

	void Set(int i, const char *str)
	{
		for (int j = 0; j < strlen(str); j++)
		{
			this->Set(i + j, str[j]);
		}
	}

	void Append(char c)
	{
		Set(this->length, c);
	}

	void Append(const String &s)
	{
		Set(this->length, s);
	}

	void Append(const char *str)
	{
		Set(this->length, str);
	}

	void Flush()
	{
		this->str[0] = 0;
		this->length = 0;
	}

	char Pop()
	{
		char c = str[length - 1];
		str[--length] = 0;
		return c;
	}

	// optimize?
	void Ltrim()
	{

		int i = 0;
		while (i < length && isspace(str[i]))
			i++;
		if (i > 0)
		{
			for (int j = i; j < length; j++)
				str[j - i] = str[j];
			length -= i;
			str[length] = 0;
		}
	}

	void Rtrim()
	{
		int i = length - 1;
		while (i >= 0 && isspace(str[i]))
			i--;
		str[i + 1] = 0;
		length = i + 1;
	}

	char GetChar(int i) const
	{
		if (i >= length)
		{
			return 0;
		}
		return str[i];
	}

	int GetLength() const
	{
		return this->length;
	}
};

int main()
{
	String *s1 = new String(128);
	String *s2 = new String(128);

	s2->Append(" world!         ");
	s1->Append(" Hello");
	s2->Print();
	s2->Ltrim();
	s2->Rtrim();
	s2->Print();

	s2->Append(*s1);
	s2->Print();
	return 0;
}
