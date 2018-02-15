#include <cstddef>

using namespace std;

class long_number {
    char* data; // data[0] - first digit
    bool sign;
    unsigned length; // size of data array
    void setLength (unsigned newLength) {
        if (newLength != length) {
            char* newData = new char[newLength + 1];
            for (int i = 0; i < newLength; i++) {
                if (i < length) {
                    newData[i] = data[i];
                } else {
                    newData[i] = '0';
                }
            }
            newData[newLength] = '\0';
            length = newLength;
            if (data != NULL) {
                delete data;
            }
            data = newData;
        }
    }
    void fixLength() {
        if (data == NULL || length == 0) {
            return;
        }
        int i = length - 1;
        while (data[i] == '0' && i > 0) {
            i--;
        }
        if (i == 0 && data[0] == '0' && sign == false) {
            sign = true;
        }
        setLength(i + 1);
    }
    bool isBiggerThan (long_number* num) {
        int i;
        if (length > num->length) {
            i = length - 1;
        } else {
            i = num->length - 1;
        }
        for (; i > -1; i--) {
            int s1, s2;
            if (i >= length) {
                s1 = '0';
            } else {
                s1 = data[i];
            }
            if (i >= num->length) {
                s2 = '0';
            } else {
                s2 = num->data[i];
            }
            if (s1 > s2) {
                return true;
            } else if (s1 < s2 ||(i == 0 && s1 == s2)) {
                return false;
            }
        }
    }
    void multipleByTenDegree (unsigned degree) {
        unsigned oldLength = length;
        setLength(oldLength + degree);
        for (int i = 0; i < oldLength ; i++) {
            data[length - 1 - i] = data[oldLength - 1 - i];
            data[oldLength - 1 - i] = '0';
        }
    }
public :
    void set (long_number* input) {
        if (data != NULL) {
            delete data;
        }
        sign = input->sign;
        length = input->length;
        data = new char[length + 1];
        for(int i = 0; i < length; i++) {
            data[i] = input->data[i];
        }
        data[length] = '\0';
    }
    void set (char* input) {
        if (input == NULL) {
            return;
        }
        if (data != NULL) {
            delete data;
        }
        if (input[0] == '-') {
            sign = false;
        } else {
            sign = true;
        }
        length = 0;
        while (input[length] != '\0') {
            length++;
        }
        length -= !sign;

        data = new char[length + 1];
        for (int i = 0; i < length; i++) {
            data[i] = input[length + !sign - 1 - i];
        }
        data[length] = '\0';
    }
    char* print () {
        if (data == NULL || length == 0) {
            char* zero = new char[2];
            zero[0] = '0';
            zero[1] = '\0';
            return zero;
        }
        char* output = new char[length + !sign + 1];
        if (!sign) {
            output[0] = '-';
        }
        for (int i = !sign; i < length + !sign; i++) {
            output[i] = data[length - 1 + !sign - i];
        }
        output[length + !sign] = '\0';
        return output;
    }
    void add(long_number* input) {
        long_number* num = new long_number;
        num->set(input);
        if (length < num->length) { // Need to expand number
            setLength(num->length + 1);
        } else {
            setLength(length + 1);
        }
        if (sign == num->sign) {
            bool extra = false;
            for (int i = 0; i < length; i++) {
                int s, sum;
                if (i >= num->length) {
                    s = 48;
                } else {
                    s = num->data[i];
                }
                sum = data[i] - 48 + s - 48 + extra;
                if (sum > 9) {
                    extra = true;
                    sum %= 10;
                } else {
                    extra = false;
                }
                data[i] = 48 + sum;
            }
        } else {
            if (isBiggerThan(num)) {
                bool lack = false;
                for (int i = 0; i < length; i++) {
                    int s, sum;
                    if (i >= num->length) {
                        s = 48;
                    } else {
                        s = num->data[i];
                    }
                    sum = data[i]- s - lack;
                    if (sum < 0) {
                        lack = true;
                        sum += 10;
                    } else {
                        lack = false;
                    }
                    data[i] = 48 + sum;
                }
            } else {
                sign = num->sign;
                bool lack = false;
                for (int i = 0; i < length; i++) {
                    int s, sum;
                    if (i >= num->length) {
                        s = 48;
                    } else {
                        s = num->data[i];
                    }
                    sum = s - data[i] - lack;
                    if (sum < 0) {
                        lack = true;
                        sum += 10;
                    } else {
                        lack = false;
                    }
                    data[i] = 48 + sum;
                }
            }
        }
        fixLength();
        delete num;
    }
    void substract(long_number* input) {
        long_number* num = new long_number;
        num->set(input);

        num->sign = !num->sign;
        add(num);
        delete num;
    }
    void multiple(long_number* input) {
        long_number* num = new long_number;
        num->set(input);
        num->fixLength();
        char* old;
        old = print();

        bool endSign = sign == num->sign;
        sign = true;
        num->sign = true;

        if (length == 1 && num->length == 1) {
            int result = (data[0] - 48) * (num->data[0] - 48);
            setLength(2);
            data[1] = result / 10 + 48;
            data[0] = result % 10 + 48;
        } else {
            unsigned commonLength = 1;
            while (commonLength < length || commonLength < num->length) {
                commonLength *= 2;
            }
            setLength(commonLength);
            num->setLength(commonLength);

            char* temp;
            long_number a1;
            long_number b1;
            if (length == 1) {
                a1.sign = true;
                a1.length = 1;
                a1.data = new char[1];
                a1.data[0] = '0';
                b1.set(this);
                b1.sign = true;
            } else {
                temp = print() + !sign;
                temp[length / 2] = '\0';
                a1.set(temp);
                a1.sign = true;
                b1.set(this);
                b1.sign = true;
                b1.setLength(length / 2);
            }
            a1.fixLength();
            b1.fixLength();
            delete temp;
            long_number a2;
            long_number b2;
            if (num->length == 1) {
                a2.sign = true;
                a2.length = 1;
                a2.data = new char[1];
                a2.data[0] = '0';
                b2.set(num);
                b2.sign = true;
            } else {
                temp = num->print() + !num->sign;
                temp[length / 2] = '\0';
                a2.set(temp);
                a2.sign = true;
                b2.set(num);
                b2.sign = true;
                b2.setLength(length / 2);
            }
            a2.fixLength();
            b2.fixLength();
            delete temp;

            long_number p_a;
            p_a.set(&a1);
            p_a.multiple(&a2);
            long_number p_b;
            p_b.set(&b1);
            p_b.multiple(&b2);

            long_number s_1;
            s_1.set(&a1);
            s_1.add(&b1);
            long_number s_2;
            s_2.set(&a2);
            s_2.add(&b2);

            long_number s_p;
            s_p.set(&s_1);
            s_p.multiple(&s_2);
            s_p.substract(&p_a);
            s_p.substract(&p_b);

            s_p.multipleByTenDegree(commonLength / 2);
            p_a.multipleByTenDegree(commonLength);

            long_number result;
            result.set(&p_b);
            result.add(&s_p);
            result.add(&p_a);

            set(&result);
            sign = endSign;
        }
        fixLength();
        num->fixLength();

        delete old;
        delete num;
    }
    long_number () {
        data = NULL;
        length = 0;
        sign = true;
    }
    ~long_number () {
        delete data;
    }
};
