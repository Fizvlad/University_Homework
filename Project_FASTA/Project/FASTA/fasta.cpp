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
                throw -12;
            }
        } catch (int error_code) {
            switch(error_code) {
            case -12:
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

typedef class FASTA {
    char*    id_; // id
    unsigned version_; // version
    char*    description_; // description
    char*    sequence_; // sequence
    void clear() {
        if (id_ != NULL) {
            delete id_;
        }
        if (description_ != NULL) {
            delete description_;
        }
        if (sequence_ != NULL) {
            delete sequence_;
        }
    }
public:
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

        unsigned char firstSymbol = getc(file);
        try {
            if (firstSymbol != '>') {
                throw -13;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -13:
                fprintf(stderr, "%s", "    !Error: Not a FASTA file or have no description\n");
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

        clear();

        char* t = new char[128];
        int t_r = fscanf(file, "%s", t);

        // Need to check whether read string or not

        id_ = copyStr(t);
        delete t;

        //description_ = description;
        //sequence_ = sequence;
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
        description_ = NULL;
        sequence_ = NULL;
    }
    FASTA(const FASTA& in) {
        id_ = copyStr(in.id_);
        description_ = copyStr(in.description_);
        sequence_ = copyStr(in.sequence_);
    }
    ~FASTA() {
        clear();
    }
};
