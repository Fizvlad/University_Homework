#include <cstdio>
using namespace std;

float min(float a, float b, float c) {
    return min(min(a, b), c);
}
float min(float a, float b, float c, float d) {
    return min(min(a, b), min(c, d));
}
float max(float a, float b, float c) {
    return max(max(a, b), c);
}
float max(float a, float b, float c, float d) {
    return max(max(a, b), max(c, d));
}

char* floatToString(float in) {
    int MAX_LENGTH = 14;
    char* temp = new char[MAX_LENGTH];
    int l;
    l = sprintf(temp, "%.3f", in);
    if (l == EOF || l > MAX_LENGTH) {
        exit(EXIT_FAILURE);
    }
    char* out = new char[l + 1];
    out[l] = '\0';
    for (int i = 0; i < l; i++) {
        out[i] = temp[i];
    }
    delete temp;
    return out;
}

char* merge(char* in1, char* in2) {
    unsigned l1 = 0;
    unsigned l2 = 0;
    while (in1[l1] != '\0') {
        l1++;
    }
    while (in2[l2] != '\0') {
        l2++;
    }
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

char* merge(unsigned amount, char* in, ...) {
    char* out = "";
    char** curr = &in;
    for (unsigned i = 0; i < amount; i++) {
        out = merge(out, *curr);
        curr++;
    }
    return out;
}
