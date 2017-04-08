template <typename T1, typename T2, typename T3> class triad {
    T1 First;
    T2 Second;
    T3 Third;
public:
    T1& first() {
        return First;
    }
    T2& second() {
        return Second;
    }
    T3& third() {
        return Third;
    }

    void set(const T1& in1, const T2& in2, const T3& in3) {
        First = in1;
        Second = in2;
        Third = in3;
    }

    void swap(triad& in) {
        triad temp(*this);
        First = in.First;
        Second = in.Second;
        Third = in.Third;
        in.First = temp.First;
        in.Second = temp.Second;
        in.Third = temp.Third;
    }

    triad () {}
    triad (const triad& in): First(in.First), Second(in.Second), Third(in.Third) {}
    triad (const T1& in1, const T2& in2, const T3& in3): First(in1), Second(in2), Third(in3) {}
    ~triad() {}
    triad& operator=(triad in) {
        swap(in);
    }
};
