#include <cstdlib>
#include <cstdio>

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

unsigned long getStrLength(char* in)
{
    if (in == NULL) {
        return 0;
    }
    unsigned long out = 0;
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

    delete in1;
    delete in2;

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

typedef class FASTA
{
    char*         id_; // id
    unsigned      id_length_; // id length
    unsigned      version_; // version
    char*         description_; // description
    unsigned      description_length_; // description length
    char*         sequence_; // sequence
    unsigned long sequence_length_; // sequence_length
public:
    void clear()
    {
        if (id_ != NULL) {
            delete id_;
            id_ = NULL;
        }
        id_length_ = 0;
        if (description_ != NULL) {
            delete description_;
            description_ = NULL;
        }
        description_length_ = 0;
        if (sequence_ != NULL) {
            delete sequence_;
            sequence_ = NULL;
        }
        sequence_length_ = 0;
    }

    void print(FILE* stream = stdout)
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
        if (sequence_ != NULL) {
            fprintf(stream, "%s%s%c%c", "Sequence: ", sequence_, LINE_END, '\n');
        } else {
            fprintf(stream, "%s%s%c%c", "Sequence: ", "No data", LINE_END, '\n');
        }
        fprintf(stream, "%s%lu%c%c", "Sequence size: ", sequence_length_, LINE_END, '\n');
    }

    void save (const char* path)
    {

        try {
            if (id_ == NULL || sequence_ == NULL) {
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
            if (fputs(sequence_, file) == EOF) {
                throw -13;
            }
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
                delete buffer;
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
                    delete buffer;
                    return;
                    break;
                default:
                    fprintf(stderr, "%s", "    !Error: Unknown error\n");
                    exit(EXIT_FAILURE);
                }
            }

            description_length_ = getStrLength(buffer);
            if (buffer[description_length_ - 1] == '\n') { // Must delete \n at the end of string if it exists. Normally it should
                buffer[description_length_ - 1] = '\0';
                description_length_--;
            } else { // No end of the line symbol. Occures when description is too big
                fprintf(stderr, "%s", "    !Error: Too big description. Description can't be bigger than 500 characters\n");
                clear();
                delete buffer;
                return;
            }
            description_ = copyStr(buffer);
        } else if (skip == '\n') { // '\n' - no description
            description_ = NULL;
            description_length_ = 0;
        } else { // Unexcepted symbol. Occures when id is too big
            fprintf(stderr, "%s", "    !Error: Probably too big Id. Id can't be bigger than 500 characters\n");
            clear();
            delete buffer;
            return;
        }

        // Copying sequence
        sequence_ = new char[1];
        sequence_[0] = '\0';
        sequence_length_ = 0;
        int ch = 1;
        unsigned long l = 0;
        while (ch > 0) {
            // Reading sequence by blocks size of BUFFER_SIZE - 1. When met \0 - stop
            while (ch > 0 && l < BUFFER_SIZE - 1) {
                ch = getc(file); // if \0 met. Nothing will happen (watch "if" below)
                if ((ch >= 65 && ch <= 90) || ch == 42 || ch == 45) { // Adding only symbols, which allowed by documentation
                    buffer[l] = (unsigned char)ch;
                    sequence_length_++;
                    l++;
                }
            }
            buffer[l] = '\0';
            char* temp = copyStr(buffer); // Need to create temp, so buffer won't be deleted after merging
            l = 0;
            sequence_ = merge(sequence_, temp);
            // if ch == 0 => exiting "while"
        }

        closeFile(file);
        delete buffer;
        fprintf(stdout, "%s", "    File successfully read.\n");
    }

    void setId (char* in)
    {
        if (id_ != NULL) {
            delete id_;
            id_ = NULL;
            id_length_ = 0;
        }
        id_ = copyStr(in);
        id_length_ = getStrLength(id_);
    }

    void setDescription (char* in)
    {
        if (description_ != NULL) {
            delete description_;
            description_ = NULL;
            description_length_ = 0;
        }
        description_ = copyStr(in);
        description_length_ = getStrLength(description_);
    }

    void setSequence (char* in)
    {
        if (sequence_ != NULL) {
            delete sequence_;
            sequence_ = NULL;
            sequence_length_ = 0;
        }
        sequence_ = copyStr(in);
        sequence_length_ = getStrLength(sequence_);
    }

    FASTA()
    {
        id_ = NULL;
        id_length_ = 0;
        description_ = NULL;
        description_length_ = 0;
        sequence_ = NULL;
        sequence_length_ = 0;
    }

    FASTA(const FASTA& in)
    {
        id_ = copyStr(in.id_);
        id_length_ = in.id_length_;
        description_ = copyStr(in.description_);
        description_length_ = in.description_length_;
        sequence_ = copyStr(in.sequence_);
        sequence_length_ = in.sequence_length_;
    }

    ~FASTA()
    {
        clear();
    }
};
