#include <cstdlib>
#include <cstdio>

char* copyStr(char* in) {
    if (in == NULL) {
        return NULL;
    }

    unsigned long length = 0;
    while(*(in + length) != '\0') {
        try {
            length++;
            if (length == -1) {
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
            case -11:
                fprintf(stderr, "%s", "    !Error: String is too big. Returned NULL\n");
                return NULL;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    char* out = new char[length + 1];
    for (unsigned long i = 0; i < length; i++) {
        out[i] = in[i];
    }
    out[length] = '\0';
    return out;
}

unsigned long getStrLength(char* in) {
    try {
        if (in == NULL) {
            throw -11;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -11:
            fprintf(stderr, "%s", "    !Error: NULL input. Returned 0\n");
            return 0;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }
    unsigned long out = 0;
    while (in[out] != '\0') {
        out++;
    }
    return out;
}

typedef class FASTA {
    char*         id_; // id
    unsigned      id_length_; // id length
    unsigned      version_; // version
    char*         description_; // description
    unsigned      description_length_; // description length
    char*         sequence_; // sequence
    unsigned long sequence_length_; // sequence_length_
    void clear() {
        if (id_ != NULL) {
            delete id_;
        }
        id_length_ = 0;
        if (description_ != NULL) {
            delete description_;
        }
        description_length_ = 0;
        if (sequence_ != NULL) {
            delete sequence_;
        }
        sequence_length_ = 0;
    }
public:
    void print(FILE* stream = stdout) {
        if (id_ != NULL) {
            fprintf(stream, "%s%s%c", "Id: ", id_, '\n');
        } else {
            fprintf(stream, "%s%s%c", "Description: ", "No Id", '\n');
        }
        if (description_ != NULL) {
            fprintf(stream, "%s%s%c", "Description: ", description_, '\n');
        } else {
            fprintf(stream, "%s%s%c", "Description: ", "No description", '\n');
        }
        if (sequence_ != NULL) {
            fprintf(stream, "%s%s%c", "Sequence: ", sequence_, '\n');
        } else {
            fprintf(stream, "%s%s%c", "Description: ", "No data", '\n');
        }
        fprintf(stream, "%s%lu%c", "Sequence size: ", sequence_length_, '\n');
    }

    void save (const char* path) { // !!! MUST DO
    }

    void open (const char* path) {
        FILE* file = fopen(path, "rt");
        try {
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
                fprintf(stderr, "%s", "    !Error: Unknown error");
                exit(EXIT_FAILURE);
            }
        }

        unsigned char firstSymbol = getc(file); // Must be '>'
        try {
            if (firstSymbol != '>') {
                throw -13;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -13:
                fprintf(stderr, "%s", "    !Error: Not a FASTA file or have no description\n");
                // Closing file
                try {
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
                        fprintf(stderr, "%s", "    !Error: Unknown error");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error");
                exit(EXIT_FAILURE);
            }
        }

        clear(); // Now ready to work with file. Deleting previous data


        // Copying ID
        char* id = new char[256];
        int id_r = fscanf(file, "%s", id); // Response
        try {
            if (id_r != 1) {
                throw -14;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -14:
                fprintf(stderr, "%s", "    !Error: Error occured while reading id\n");
                //Closing file
                try {
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
                        fprintf(stderr, "%s", "    !Error: Unknown error");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error");
                exit(EXIT_FAILURE);
            }
        }

        id_ = copyStr(id);
        id_length_ = getStrLength(id_);
        delete id;

        // Copying description
        int skip = getc(file); // Skipping one symbol and checking whether description exists
        if (skip == 32) { // ' ' - description exists
            unsigned DESCRIPTION_MAX_LENGTH = 256;
            char* description = new char[DESCRIPTION_MAX_LENGTH];
            char* description_r = fgets(description, DESCRIPTION_MAX_LENGTH, file); // Response

            try {
                if (description_r == NULL) {
                    throw -14;
                }
            } catch (int error_code) {
                switch (error_code) {
                case -14:
                    fprintf(stderr, "%s", "    !Error: Error occured while reading id\n");
                    //Closing file
                    try {
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
                            fprintf(stderr, "%s", "    !Error: Unknown error");
                            exit(EXIT_FAILURE);
                        }
                    }
                    break;
                default:
                    fprintf(stderr, "%s", "    !Error: Unknown error");
                    exit(EXIT_FAILURE);
                }
            }

            description_length_ = getStrLength(description);
            if (description[description_length_ - 1] == '\n') { // Must delete \n at the end of string if it exists. Normally it should
                description[description_length_ - 1] = '\0';
                description_length_--;
            } else {
                /*
                    !!! MUST PROCESS CASE OF TOO BIG DESCRIPTION
                */
            }
            description_ = copyStr(description);
            delete description;
        } else if (skip == 10) { // '\n' - no description
            description_ = NULL;
            description_length_ = 0;
        } else { // Unexcepted symbol. This occures when id is too big
            /*
                !!! MUST PROCESS CASE OF TOO BIG ID
            */
        }

        //Copying sequence
        unsigned long SEQUENCE_MAX_LENGTH = 1024; // !!! MUST GET RID OF THIS RESTRICTION
        sequence_length_ = 0;
        char* sequence = new char[SEQUENCE_MAX_LENGTH];
        int ch; // Will step-by-step read each char
        ch = getc(file);
        while (ch > 0) {
            if ((ch >= 65 && ch <= 90) || ch == 42 || ch == 45) { // Adding only symbols, which allowed by documentation
                sequence[sequence_length_] = (unsigned char)ch;
                sequence_length_++;
            }
            ch = getc(file);
        }
        sequence_ = copyStr(sequence);
        delete sequence;

        // Closing file
        try {
            int re = fclose(file);
            if (re != 0) {
                throw -12;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -12:
                fprintf(stderr, "%s", "    !Error: Unable to close file\n");
                exit(EXIT_FAILURE);
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error");
                exit(EXIT_FAILURE);
            }
        }
    }
    FASTA() {
        id_ = NULL;
        id_length_ = 0;
        description_ = NULL;
        description_length_ = 0;
        sequence_ = NULL;
        sequence_length_ = 0;
    }
    FASTA(const FASTA& in) {
        id_ = copyStr(in.id_);
        id_length_ = in.id_length_;
        description_ = copyStr(in.description_);
        description_length_ = in.description_length_;
        sequence_ = copyStr(in.sequence_);
        sequence_length_ = in.sequence_length_;
    }
    ~FASTA() {
        clear();
    }
};
