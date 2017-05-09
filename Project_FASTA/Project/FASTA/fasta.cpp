#include <cstdlib>
#include <cstdio>
#include "utility.h"
#include "sequence.h"

class statistic
{
    friend class FASTA;
    static const unsigned ACCEPTED_SYMBOLS_AMOUNT_ = 18;
    static const int ACCEPTED_SYMBOLS_[ACCEPTED_SYMBOLS_AMOUNT_]; // Static member must be defined out of class
    unsigned symbols_amounts_[ACCEPTED_SYMBOLS_AMOUNT_]; // Contains amount of ACCEPTED_SYMBOLS_[i]

    void clear()
    {
        for (unsigned i = 0; i < ACCEPTED_SYMBOLS_AMOUNT_; i++) {
            symbols_amounts_[i] = 0;
        }
    }

    statistic()
    {
        clear();
    }

    statistic(const statistic& in)
    {
        for (unsigned i = 0; i < ACCEPTED_SYMBOLS_AMOUNT_; i++) {
            symbols_amounts_[i] = in.symbols_amounts_[i];
        }
    }
};

const int statistic::ACCEPTED_SYMBOLS_[18] = {65, 67, 71, 84, 86, 82, 89, 75, 77, 83, 87, 66, 68, 72, 86, 78, 88, 45};

typedef class FASTA
{
    char*         id_; // id
    unsigned      id_length_; // id length
    char*         description_; // description
    unsigned      description_length_; // description length
    sequence      sequence_; // sequence
    statistic     statistic_; // statistic of nucleotide frequency

    void updateStatistics ()
    {
        statistic_.clear();
        for (unsigned i = 0; i < sequence_.getLength(); i++) {
            int ch = (int) sequence_[i];
            int p = find(ch, statistic_.ACCEPTED_SYMBOLS_, statistic_.ACCEPTED_SYMBOLS_AMOUNT_);
            if (p != -1) { // Counting only symbols, which allowed by documentation
                statistic_.symbols_amounts_[p]++;
            } else {
                fprintf(stderr, "%s%c%s%d%s", "    !Error: Unexcepted symbol '", (unsigned char) ch, "' with code ", ch, " met\n");
            }
        }
    }
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
        sequence_.clear();
        statistic_.clear();
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

    void printStat (FILE* stream = stdout)
    {
        fprintf(stream, "%s", "Nucleotide frequencies: \n");
        const char LINE_END = ';';
        for (unsigned i = 0; i < statistic_.ACCEPTED_SYMBOLS_AMOUNT_; i++) {
            unsigned a = statistic_.symbols_amounts_[i];
            if (a != 0) {
                fprintf(stream, "%s%c%s%d%c%c", "Amount of symbol ", (unsigned char) statistic_.ACCEPTED_SYMBOLS_[i], ": ", a, LINE_END, '\n');
            }
        }
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
        int ch = 1;
        unsigned long l = 0;
        while (ch > 0) {
            // Reading sequence by blocks size of BUFFER_SIZE - 1. When met \0 - stop
            while (ch > 0 && l < BUFFER_SIZE - 1) {
                ch = getc(file); // if \0 met. Nothing will happen (watch "if" below)
                int p = find(ch, statistic_.ACCEPTED_SYMBOLS_, statistic_.ACCEPTED_SYMBOLS_AMOUNT_);
                if (p != -1) { // Adding only symbols, which allowed by documentation
                    buffer[l] = (unsigned char)ch;
                    statistic_.symbols_amounts_[p]++;
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
        updateStatistics();
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
        statistic_ = in.statistic_;
    }

    ~FASTA()
    {
        clear();
    }
};
