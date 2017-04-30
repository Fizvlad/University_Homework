#include <cstdio>
#include <cstdlib>

using namespace std;

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
}

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

int main()
{
    fprintf(stdout, "%s", "\nCurrent Directory:\n");
    system("echo %cd%");
    fprintf(stdout, "%s", "\nFile list\n");
    system("dir /a-d /oe");
    fprintf(stdout, "%s", "\nPlease set extension of files to be separated into subfolder:\n");
    char* ext = new char[10];
    fscanf(stdin, "%s", ext);
    char* command = merge("mkdir ", copyStr(ext));
    system(command);
    delete command;
    command = merge(5, "move /y *.", copyStr(ext), " ./", ext, "/");
    system(command);
    system("pause");
    return 0;
}
