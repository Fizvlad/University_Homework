#ifndef TRIAD_H_INCLUDED
#define TRIAD_H_INCLUDED

using namespace std;

template <typename T1, typename T2, typename T3> class triad{
    T1 First;
    T2 Second;
    T3 Third;
public:
    T1& first();
    T2& second();
    T3& third();
    void set(const T1& in1, const T2& in2, const T3& in3);

    triad();
    triad(const T1& in1, const T2& in2, const T3& in3);
    ~triad();
};

#endif // TRIAD_H_INCLUDED
