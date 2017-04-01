using namespace std;

void strSwap (char** s1, char** s2) {
    char* t = *s2;
    *s2 = *s1;
    *s1 = t;
};

bool isFirstBigger (char* s1, char* s2) {
    int i = 0;
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0') {
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0') { // equal
        return false;
    }
    if (s1[i] == '\0' && s2[i] != '\0') { // 2nd is longer
        return false;
    }
    if (s1[i] != '\0' && s2[i] == '\0') { // 1st is longer
        return true;
    }
    if (s1[i] > s2[i]) {
        return true;
    }
    if (s1[i] < s2[i]) {
        return false;
    }
    return false;
};
