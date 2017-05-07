#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

unsigned long getStrLength(char* in)
{
	if (in == NULL) {
		return 0;
	}
	unsigned long out = 0;
	while (*(in + out) != '\0') {
		try {
			out++;
			// Checking length to prevent overflow
			if (out == -1) {
				throw - 11;
			}
		}
		catch (int error_code) {
			switch (error_code) {
			case -11:
				fprintf(stderr, "%s", "    !Error: String is too big. Returned 0\n");
				return 0;
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return out;
}

char* copyStr(char* in)
{
	if (in == NULL) {
		return NULL;
	}

	unsigned long length = getStrLength(in);
	char* out = new char[length + 1];
	for (unsigned long i = 0; i < length; i++) {
		out[i] = in[i];
	}
	out[length] = '\0';
	return out;
}


typedef class sequence
{
	char** list_;
	unsigned length_;
	unsigned list_size_; // Amount of strings
	static const unsigned STRING_SIZE_ = 5; // Size of each string
	void addStr(unsigned amount)
	{
		char** newList = new char*[list_size_ + amount];
		for (int i = 0; i < list_size_; i++) {
			newList[i] = list_[i];
		}
		for (int i = 0; i < amount; i++) {
			newList[list_size_ + i] = new char[STRING_SIZE_ + 1];
			newList[list_size_ + i][0] = '\0';
		}
		if (list_ != NULL) {
			delete[] list_;
		}
		list_ = newList;
		list_size_ += amount;
	}
public:
	void clear()
	{
		if (list_ != NULL) {
			for (unsigned i = 0; i < list_size_; i++) {
				delete[] list_[i];
			}
			delete[] list_;
			list_ = NULL;
		}
		length_ = 0;
		list_size_ = 0;
	}

	unsigned getLength()
	{
		return length_;
	}

	void print(FILE* stream = stdout)
	{
		for (unsigned i = 0; i < list_size_; i++) {
			fprintf(stream, "%s", list_[i]);
		}
	}

	char& operator[](unsigned in)
	{
		char nullOut = '\0';

		try {
			if (in >= length_) {
				throw - 11;
			}
		}
		catch (int error_code) {
			switch (error_code) {
			case -11:
				fprintf(stderr, "%s", "    !Error: Out of boundaries\n");
				return nullOut;
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}

		unsigned k = in / STRING_SIZE_; // Number of string
		unsigned l = in % STRING_SIZE_; // Number of char in string

		try {
			if (k >= list_size_) {
				throw - 12;
			}
			if (l >= STRING_SIZE_) { // Actually that would never happen
				throw - 13;
			}
		}
		catch (int error_code) {
			switch (error_code) {
			case -12:
				fprintf(stderr, "%s%d%c", "    !Error: No string with number ", k, '\n');
				return nullOut;
				break;
			case -13:
				fprintf(stderr, "%s%d%c", "    !Error: No char with number ", l, '\n');
				return nullOut;
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}

		return list_[k][l];
	}

	void add(char in)
	{
		unsigned l = length_ - (list_size_ - 1) * STRING_SIZE_; // Size of last string
		if (l != STRING_SIZE_) { // Last string is not full yet
			list_[list_size_ - 1][l] = in;
			list_[list_size_ - 1][l + 1] = '\0';
		}
		else { // Have to create new string because last one is full
			addStr(1);
			list_[list_size_ - 1][0] = in;
			list_[list_size_ - 1][1] = '\0';
		}
		length_++;
	}

	void add(char* in)
	{

		try {
			if (in == NULL) {
				throw - 11;
			}
		}
		catch (int error_code) {
			switch (error_code) {
			case -11:
				fprintf(stderr, "%s", "    !Error: NULL input\n");
				return;
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}

		unsigned l = length_ - (list_size_ - 1) * STRING_SIZE_; // Size of last string
		unsigned t = getStrLength(in);
		if (STRING_SIZE_ - l >= t) { // No need to create new string
			for (unsigned i = 0; i < t; i++) {
				list_[list_size_ - 1][l + i] = in[i];
			}
			list_[list_size_ - 1][l + t] = '\0';
		}
		else { // Have to create 1+ new strings
			unsigned j = list_size_; // Number of string being filled
			unsigned strNeeded = (t - (STRING_SIZE_ - l)) / STRING_SIZE_;
			if ((t - (STRING_SIZE_ - l)) % STRING_SIZE_ != 0) {
				strNeeded++;
			}
			addStr(strNeeded);
			unsigned k = 0; // Current symbol in input string
			for (; k < STRING_SIZE_ - l; k++) {
				list_[j - 1][l + k] = in[k];
			}
			unsigned i = 0; // Current symbol in string
			for (; j < list_size_; j++) {
				for (; i < STRING_SIZE_ && k < t; i++) {
					list_[j][i] = in[k];
					k++;
				}
				list_[j][i] = '\0';
				if (k != t) {
					i = 0;
				}
			}
		}
		length_ += t;
	}

	sequence()
	{
		list_ = NULL;
		length_ = 0;
		list_size_ = 0;
	}

	sequence(char* in)
	{

		try {
			if (in == NULL) {
				throw - 11;
			}
		}
		catch (int error_code) {
			switch (error_code) {
			case -11:
				fprintf(stderr, "%s", "    !Error: NULL input\n");
				length_ = 0;
				list_size_ = 0;
				return;
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}

		length_ = getStrLength(in);
		list_size_ = length_ / STRING_SIZE_;
		if (length_ % STRING_SIZE_ != 0) { // Need one more string which will have less than STRING_SIZE_ symbols
			list_size_++;
		}
		list_ = new char*[list_size_];

		unsigned k = 0; // number of symbol currently being copied from in
		for (unsigned i = 0; i < list_size_; i++) {
			list_[i] = new char[STRING_SIZE_ + 1];
			unsigned j = 0; // number of current symbol in current string
			for (; j < STRING_SIZE_ && k < length_; j++) {
				list_[i][j] = in[k];
				k++;
			}
			list_[i][j] = '\0'; // when stopped copying (when j == STRING_SIZE_ || k == length_)
		}
	}

	sequence(const sequence& in)
	{
		length_ = in.length_;
		list_size_ = in.list_size_;
		list_ = new char*[list_size_];
		for (unsigned i = 0; i < list_size_; i++) {
			list_[i] = copyStr(in.list_[i], STRING_SIZE_);
		}
	}

	~sequence()
	{
		clear();
	}
};

/*typedef class sequence
{
	char** list;
	size_t list_size;  //Number of rows in sequence
public:
	sequence(size_t list_size) {
		list = new char *[list_size];
		list[0] = new char[list_size * 70];
		for (size_t i = 1; i != list_size; ++i)
			list[i] = list[i - 1] + 70;
	};
	~sequence() {
		delete[] list[0];
		delete[] list;
	};
	char operator[](size_t n) {
		try {
			if (n > list_size)
				throw - 15;
		}
		catch (int error_code) {
			switch (error_code) {
			case -15:
				fprintf(stderr, "%s", "    !Error: Out of sequence\n");				
				break;
			default:
				fprintf(stderr, "%s", "    !Error: Unknown error\n");
				exit(EXIT_FAILURE);
			}
		}
		return *(list[0] + n - 1);
	};
	sequence(sequence const& a)                //Copy constructor
		: list_size(a.list_size), list(new char *[list_size]) {
		list[0] = new char[list_size * 70];
		for (size_t i = 1; i != list_size; ++i)
			list[i] = list[i - 1] + 70;
		for (size_t i = 0; i != list_size; ++i)
			for (size_t j = 0; j != 71; ++j)
				*(list[i] + j) = *(a.list[i] + j);
	};
};*/

//PARSING
struct pars {
	unsigned long n;    //How many times symbol s occurs in the sequence
	char s;
};
typedef class parsed {

	size_t b;           //Where starts parsing
	size_t e;           //Where ends parsing
public:
	pars* row;
	parsed() {                      //Constructor
		row = 0;
		b = 0;
		e = 0;
	}
	parsed(size_t b, size_t e) {     //Consrtuctor with arguments
		row = new pars[e - b + 1];   
	};

	~parsed() {                     //Distructor
		delete[] row;
	};

	parsed(parsed const& a)         //Copy constructor
		: b(a.b), e(a.e), row(new pars[e - b + 1]) {
		for (size_t i = 0; i != e - b + 2; ++i) {
			row[i].n = a.row[i].n;
			row[i].s = a.row[i].s;
		}
	};
};

pars* parsing(sequence a, size_t n, size_t m) {     //This function makes parsed rows
	parsed myparsed(n,m);
	size_t j = 0;
	myparsed.row[j].n = 1;
	myparsed.row[j].s = a[n];
	for (size_t i = n+1; i != m+1; ++i)
		if (a[i-1] == a[i])
			myparsed.row[j].n += 1;
		else
		{
			j++;
			myparsed.row[j].n = 1;
			myparsed.row[j].s = a[i];
		}
	return myparsed.row;
}