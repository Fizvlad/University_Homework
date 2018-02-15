#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

char* createNumber(bool ifOwn) { // creating string with number
    unsigned length;
    scanf("%d", &length);
    
    bool sign;
    if (ifOwn) {
        char* in = new char[length + 1 + 1]; // +1 for '-' and +1 for '\0'
        in[length + 1] = '\0';
        scanf("%s", in);
        if (in[0] == '-') {
            sign = false;
        } else {
            sign = true;
            in[length] = '\0';
        }
        return in;
    } else {
        srand(time(0));
        sign = rand() % 2;

        char* in = new char[length + !sign + 1];
        if (!sign) {
            in[0] = '-';
        }
        for(int i = !sign; i < length + !sign; i++) {
            in[i] = char(48 + (rand() % 10));
        }
        in[length + !sign] = '\0';
        return in;
    }    
}
