#include <cstdlib>
#include <cstdio>

int maximum(int a, int b, int c) {
    int t = a;
    if (b > a) {
        t = b;
    }
    if (c > t) {
        return c;
    }
    return t;
}

int find(int t, int* array, int size)
{
    for (int i = 0; i < size; i++) {
        if (array[i] == t) {
            return i;
        }
    }
    return -1;
}

char* fgetw(char* string, int size, FILE* file, char splitter = ' ')
{
    if (size <= 1 || file == NULL || string == NULL) {
        return NULL;
    }
    int i = 0;
    int ch = getc(file);
    if (ch == EOF) {
        return NULL;
    }
    while ((unsigned char)ch != splitter && (unsigned char)ch != '\n' && ch != EOF && i < size - 2) {
        string[i] = (unsigned char)ch;
        i++;
        ch = getc(file);
    }
    // Now ch = -1 OR splitter OR '\n' OR last char to be saved
    if (ch == EOF) {
        string[i] ='\0';
    } else {
        string[i] = (unsigned char)ch;
        string[i + 1] = '\0';
    }
    return string;
}

void closeFile(FILE* file)
{
    try {
        // Closing file
        int re = fclose(file);
        if (re != 0) {
            throw -12;
        }
    } catch (int error_code_) {
        switch (error_code_) {
        case -12:
            fprintf(stderr, "%s", "    !Error: Unable to close file\n");
            exit(EXIT_FAILURE);
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }
}

unsigned getStrLength(char* in)
{
    if (in == NULL) {
        return 0;
    }
    unsigned out = 0;
    while(*(in + out) != '\0') {
        try {
            out++;
            // Checking length to prevent overflow
            if (out == -1) {
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
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

char* copyStr(char* in, unsigned long length = 0)
{
    if (in == NULL) {
        return NULL;
    }
    if (length == 0) {
        length = getStrLength(in);
    }
    char* out = new char[length + 1];
    for (unsigned long i = 0; i < length; i++) {
        out[i] = in[i];
    }
    out[length] = '\0';
    return out;
}

char* merge(char* in1, char* in2)
{
    if (in1 == NULL || in2 == NULL) {
        fprintf(stderr, "%s", "    !Error: NULL input. Returned NULL\n");
        return NULL;
    }
    unsigned l1 = getStrLength(in1);
    unsigned l2 = getStrLength(in2);

    char* out = new char[l1 + l2 + 1];
    out[l1 + l2] = '\0';

    unsigned i = 0;
    for (; i < l1; i++) {
        out[i] = in1[i];
    }
    for (; i < l1 + l2; i++) {
        out[i] = in2[i - l1];
    }

    delete [] in1;
    delete [] in2;

    return out;
};

char* merge(unsigned amount, char* in, ...)
{
    if (amount == 0) {
        fprintf(stderr, "%s", "    !Error: Wrong input. Returned NULL\n");
        return NULL;
    } else if (amount == 1) {
        return in;
    }
    char* out = new char[1];
    out[0] = '\0';
    char** curr = &in;
    for (unsigned i = 0; i < amount; i++) {
        out = merge(out, *curr);
        curr++;
    }
    return out;
}

typedef class sequence
{
    char** list_;
    unsigned length_;
    unsigned list_size_; // Amount of strings
    static const unsigned STRING_SIZE_ = 5; // Size of each string
    void addStr (unsigned amount)
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
            delete [] list_;
        }
        list_ = newList;
        list_size_ += amount;
    }
public:
    void clear()
    {
        if (list_ != NULL) {
            for (unsigned i = 0; i < list_size_; i++) {
                delete [] list_[i];
            }
            delete [] list_;
            list_ = NULL;
        }
        length_ = 0;
        list_size_ = 0;
    }

    unsigned getLength()
    {
        return length_;
    }

    void print (FILE* stream = stdout)
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
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
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
                throw -12;
            }
            if (l >= STRING_SIZE_) { // Actually that would never happen
                throw -13;
            }
        } catch (int error_code) {
            switch(error_code) {
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
        } else { // Have to create new string because last one is full
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
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
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
        } else { // Have to create 1+ new strings
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

    sequence (char* in)
    {

        try {
            if (in == NULL) {
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
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

    sequence (const sequence& in)
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

typedef class FASTA
{
    char*         id_; // id
    unsigned      id_length_; // id length
    char*         description_; // description
    unsigned      description_length_; // description length
    sequence      sequence_; // sequence
public:
    void clear()
    {
        if (id_ != NULL) {
            delete [] id_;
            id_ = NULL;
        }
        id_length_ = 0;
        if (description_ != NULL) {
            delete [] description_;
            description_ = NULL;
        }
        description_length_ = 0;
    }

    void print (FILE* stream = stdout)
    {
        const char LINE_END = ';';
        if (id_ != NULL) {
            fprintf(stream, "%s%s%c%c", "Id: ", id_, LINE_END, '\n');
        } else {
            fprintf(stream, "%s%s%c%c", "Id: ", "No Id", LINE_END, '\n');
        }
        if (description_ != NULL) {
            fprintf(stream, "%s%s%c%c", "Description: ", description_, LINE_END, '\n');
        } else {
            fprintf(stream, "%s%s%c%c", "Description: ", "No description", LINE_END, '\n');
        }
        fprintf(stream, "%s", "Sequence: ");
        sequence_.print(stream);
        fprintf(stream, "%c%c%s%lu%c%c", LINE_END, '\n', "Sequence size: ", sequence_.getLength(), LINE_END, '\n');
    }

    void save (const char* path)
    {

        try {
            if (id_ == NULL || sequence_.getLength() == 0) {
                throw -11;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -11:
                fprintf(stderr, "%s", "    !Error: Nothing to save\n");
                return;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }

        FILE* file = fopen(path, "wt");

        try {
            if (file == NULL) {
                throw -12;
            }
            if (fputc((int)'>', file) == EOF) {
                throw -13;
            }
            if (fputs(id_, file) == EOF) {
                throw -13;
            }
            if (description_ != NULL) {
                if (fputc((int)' ', file) == EOF) {
                    throw -13;
                }
                if (fputs(description_, file) == EOF) {
                    throw -13;
                }
            }
            if (fputc((int)'\n', file) == EOF) {
                throw -13;
            }
            sequence_.print(file);
            closeFile(file);
        } catch (int error_code) {
            switch (error_code) {
            case -12:
                fprintf(stderr, "%s", "    !Error: Can not open file\n");
                return;
                break;
            case -13:
                fprintf(stderr, "%s", "    !Error: Error occured while writing\n");
                closeFile(file);
                return;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }
        fprintf(stdout, "%s", "    File successfully saved.\n");
    }

    void read (const char* path)
    {
        const unsigned BUFFER_SIZE = 512; // Equal to maximum size of Id and description
        FILE* file = fopen(path, "rt");

        try {
            // Checking if succeed opening file
            if (file == NULL) {
                throw -11;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -11:
                fprintf(stderr, "%s", "    !Error: Can not open file\n");
                return;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }

        unsigned char firstSymbol = getc(file); // Must be '>'

        try {
            // Checking if file matches standart
            if (firstSymbol != '>') {
                throw -13;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -13:
                fprintf(stderr, "%s", "    !Error: Not a FASTA file (please check file format documentation)\n");
                closeFile(file);
                return;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }

        clear(); // Now ready to work with file. Deleting previous data
        char* buffer = new char[BUFFER_SIZE];

        // Copying ID
        char* id_r = fgetw(buffer, 256, file, ' '); // Response

        try {
            // Checking if successfully read string
            if (id_r == NULL) {
                throw -14;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -14:
                fprintf(stderr, "%s", "    !Error: Error occured while reading id\n");
                closeFile(file);
                clear();
                delete [] buffer;
                return;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }

        id_length_ = getStrLength(buffer);
        char skip = buffer[id_length_ - 1]; // Taking last buffer symbol (normally ' ' or '\n') to check whether description exists later
        buffer[id_length_ - 1] = '\0';
        id_length_--;
        id_ = copyStr(buffer);

        // Copying description
        if (skip == ' ') { // ' ' - description exists
            char* description_r = fgets(buffer, BUFFER_SIZE, file); // Response

            try {
                // Checking if succeed reading string
                if (description_r == NULL) {
                    throw -14;
                }
            } catch (int error_code) {
                switch (error_code) {
                case -14:
                    fprintf(stderr, "%s", "    !Error: Error occured while reading id\n");
                    closeFile(file);
                    clear();
                    delete [] buffer;
                    return;
                    break;
                default:
                    fprintf(stderr, "%s", "    !Error: Unknown error\n");
                    exit(EXIT_FAILURE);
                }
            }

            description_length_ = getStrLength(buffer);
            if (buffer[description_length_ - 1] == '\n') { // Must remove \n at the end of string if it exists. Normally it should
                buffer[description_length_ - 1] = '\0';
                description_length_--;
            } else { // No end of the line symbol. Occures when description is too big
                fprintf(stderr, "%s", "    !Error: Too big description. Description can't be bigger than 500 characters\n");
                clear();
                delete [] buffer;
                return;
            }
            description_ = copyStr(buffer);
        } else if (skip == '\n') { // '\n' - no description
            description_ = NULL;
            description_length_ = 0;
        } else { // Unexcepted symbol. Occures when id is too big
            fprintf(stderr, "%s", "    !Error: Probably too big Id. Id can't be bigger than 500 characters\n");
            clear();
            delete [] buffer;
            return;
        }

        // Copying sequence
        int acceptedSymbols[18] = {65, 67, 71, 84, 86, 82, 89, 75, 77, 83, 87, 66, 68, 72, 86, 78, 88, 45};

        int ch = 1;
        unsigned long l = 0;
        while (ch > 0) {
            // Reading sequence by blocks size of BUFFER_SIZE - 1. When met \0 - stop
            while (ch > 0 && l < BUFFER_SIZE - 1) {
                ch = getc(file); // if \0 met. Nothing will happen (watch "if" below)
                if (find(ch, acceptedSymbols, 18) != -1) { // Adding only symbols, which allowed by documentation
                    buffer[l] = (unsigned char)ch;
                    // Here may parse with parse(buffer[l]);
                    l++;
                } else if (ch != 32 && ch != 0 && ch != 10 && ch != -1) { // Space, line end, \0 and EOF are possible
                    fprintf(stderr, "%s%c%s%d%s", "    !Error: Unexcepted symbol '", (unsigned char) ch, "' with code ", ch, " met\n");
                }
            }
            buffer[l] = '\0';
            l = 0;
            sequence_.add(buffer);
            // if ch == 0 || ch == EOF => exiting "while"
        }

        closeFile(file);
        delete [] buffer;
        fprintf(stdout, "%s", "    File successfully read.\n");
    }

    void setId (char* in)
    {
        if (id_ != NULL) {
            delete [] id_;
            id_ = NULL;
            id_length_ = 0;
        }
        id_ = copyStr(in);
        id_length_ = getStrLength(id_);
    }

    void setDescription (char* in)
    {
        if (description_ != NULL) {
            delete [] description_;
            description_ = NULL;
            description_length_ = 0;
        }
        description_ = copyStr(in);
        description_length_ = getStrLength(description_);
    }

    void setSequence (char* in)
    {
        sequence_.clear();
        sequence_.add(in);
    }

    void align (FASTA in)
    {
        int gap = 0;
        //                    A    G    C    T
        int S[4][4] = {{/*A*/  1,   0,   0,   0},
                       {/*G*/  0,   1,   0,   0},
                       {/*C*/  0,   0,   1,   0},
                       {/*T*/  0,   0,   0,   1}};

        int**  F  = new  int*[sequence_.getLength() + 1];
        char** F_ = new char*[sequence_.getLength() + 1];
        for (unsigned long i = 0; i < sequence_.getLength() + 1; i++) {
            F[i]  = new  int[in.sequence_.getLength() + 1];
            F_[i] = new char[in.sequence_.getLength() + 1];
            F[i][0] = gap * i;
        }

        for (unsigned long i = 0; i < in.sequence_.getLength() + 1; i++) {
            F[0][i] = gap * i;
        }

        for (unsigned long i = 1; i < sequence_.getLength() + 1; i++) {
            for (unsigned long j = 1; j < in.sequence_.getLength() + 1; j++) {
                int f, s;
                switch (sequence_[i - 1]) {
                case 'A':
                    f = 0;
                    break;
                case 'G':
                    f = 1;
                    break;
                case 'C':
                    f = 2;
                    break;
                case 'T':
                case 'U':
                    f = 3;
                    break;
                default:
                    f = 0;
                }
                switch (in.sequence_[j - 1]) {
                case 'A':
                    s = 0;
                    break;
                case 'G':
                    s = 1;
                    break;
                case 'C':
                    s = 2;
                    break;
                case 'T':
                case 'U':
                    s = 3;
                    break;
                default:
                    s = 0;
                }

                int match = F[i - 1][j - 1] + S[f][s];
                int del = F[i - 1][j] + gap;
                int ins = F[i][j - 1] + gap;

                F[i][j] = maximum(match, del, ins);
                if (F[i][j] == match) {
                    F_[i][j] = 'M';
                } else if (F[i][j] == del) {
                    F_[i][j] = 'D';
                } else if (F[i][j] == ins) {
                    F_[i][j] = 'I';
                } else {
                    F_[i][j] = '0';
                }
                // Uncomment this for table: fprintf(stdout, "%d%c%c", F[i][j], F_[i][j], ' ');
            }
            // Uncomment this for table: fprintf(stdout, "%c", '\n');
        }

        char* alignment = new char[1];
        alignment[0] = '\0';
        char* inAlignment = new char[1];
        inAlignment[0] = '\0';

        unsigned long i = sequence_.getLength();
        unsigned long j = in.sequence_.getLength();

        unsigned long matches = 0;

        while (i > 0 && j > 0) {
            if (F_[i][j] == 'M') {
                char s1[2] = {sequence_[i - 1], '\0'};
                alignment = merge(s1, alignment);
                char s2[2] = {in.sequence_[j - 1], '\0'};
                inAlignment = merge(s2, inAlignment);
                i--;
                j--;
            } else if (F_[i][j] == 'D') {
                char s1[2] = {sequence_[i - 1], '\0'};
                alignment = merge(s1, alignment);
                char s2[2] = {'-', '\0'};
                inAlignment = merge(s2, inAlignment);
                i--;
            } else {
                char s1[2] = {'-', '\0'};
                alignment = merge(s1, alignment);
                char s2[2] = {in.sequence_[j - 1], '\0'};
                inAlignment = merge(s2, inAlignment);
                j--;
            }
        }
        while (i > 0) {
            char s1[2] = {sequence_[i - 1], '\0'};
            alignment = merge(s1, alignment);
            char s2[2] = {'-', '\0'};
            inAlignment = merge(s2, inAlignment);
            i--;
        }
        while (j > 0) {
            char s1[2] = {'-', '\0'};
            alignment = merge(s1, alignment);
            char s2[2] = {in.sequence_[j - 1], '\0'};
            inAlignment = merge(s2, inAlignment);
            j--;
        }
        unsigned l = getStrLength(alignment);
        char* matchesLine = new char[l + 1];
        matchesLine[l] = '\0';
        for (unsigned i = 0; i < l; i++) {
            if (alignment[i] == inAlignment[i]) {
                matchesLine[i] = '+';
                matches++;
            } else if (alignment[i] == '-' || inAlignment[i] == '-') {
                matchesLine[i] = '.';
            } else {
                matchesLine[i] = '-';
            }
        }

        fprintf(stdout, "%s%c", alignment,   '\n');
        fprintf(stdout, "%s%c", inAlignment, '\n');
        fprintf(stdout, "%s%c", matchesLine, '\n');
        fprintf(stdout, "%s%d%c", "Amount of matching nucleotides: ", matches, '\n');

        delete [] alignment;
        delete [] inAlignment;
        delete [] matchesLine;
        delete [] F;
        delete [] F_;
    }

    FASTA()
    {
        id_ = NULL;
        id_length_ = 0;
        description_ = NULL;
        description_length_ = 0;
    }

    FASTA (const FASTA& in)
    {
        id_ = copyStr(in.id_);
        id_length_ = in.id_length_;
        description_ = copyStr(in.description_);
        description_length_ = in.description_length_;
        sequence_ = in.sequence_;
    }

    ~FASTA()
    {
        clear();
    }
};
