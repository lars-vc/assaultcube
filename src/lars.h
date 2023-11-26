#pragma once
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
// TODO: fix with lars.cpp
// int get_val_lars(int val);
// void set_val_lars(int &val, int set);
// void add_val_lars(int &val, int add);

///////////////////////////////////////////
// simple add
///////////////////////////////////////////
// inline int val_mask = 1;
//
// inline int get_val_lars(int val) { return val - val_mask; }
// inline void set_val_lars(int &val, int set) { val = set + val_mask; }
//
// inline void add_val_lars(int &val, int add) {
//     set_val_lars(val, get_val_lars(val) + add);
// }

///////////////////////////////////////////
// simple xor
///////////////////////////////////////////
// inline int val_mask = rand();
//
// inline int get_val_lars(int val) { return val ^ val_mask; }
// inline void set_val_lars(int &val, int set) { val = set ^ val_mask; }
//
// inline void add_val_lars(int &val, int add) {
//     set_val_lars(val, get_val_lars(val) + add);
// }

///////////////////////////////////////////
// protector
///////////////////////////////////////////

class Protector {
  public:
    std::vector<int> masks;
    Protector() {}
};

class ProtectedInt {
  protected:
    virtual int deobfuscate() const { return 3; };
    virtual void obfuscate(int val){};
    virtual void obfuscateAdd(int add){};

  public:
    ProtectedInt() { obfuscate(0); }
    ProtectedInt(int val) { obfuscate(val); }
    virtual ~ProtectedInt() = default;

    // OPERATORS
    ProtectedInt &operator=(int val) {
        obfuscate(val);
        return *this;
    }
    ProtectedInt &operator=(ProtectedInt val) {
        return operator=(val.deobfuscate());
    }
    ProtectedInt operator+(ProtectedInt add) {
        int val = deobfuscate();
        int valadd = add.deobfuscate();
        ProtectedInt p(val + valadd);
        return p;
    }
    ProtectedInt operator+(int add) {
        int val = deobfuscate();
        ProtectedInt p(val + add);
        return p;
    }
    ProtectedInt &operator+=(int add) {
        obfuscateAdd(add);
        return *this;
    }
    ProtectedInt &operator++(int) { return this->operator+=(1); }
    ProtectedInt &operator-=(int add) { return this->operator+=(-add); }
    ProtectedInt &operator--(int) { return this->operator-=(1); }
    operator bool() const { return deobfuscate() != 0; }
    // Maybe this can work?
    // operator int() const { return deobfuscate(); }

    int val() { return deobfuscate(); }
};

class XORInt : public ProtectedInt {
  private:
    int mask = rand();
    int obfuscated_val;
    int deobfuscate() const override { return mask ^ obfuscated_val; }
    void obfuscate(int val) override { obfuscated_val = mask ^ val; }
    void obfuscateAdd(int add) override { obfuscate(deobfuscate() + add); }

  public:
    XORInt() : ProtectedInt() {}
    XORInt(int val) : ProtectedInt(val) {}
    ~XORInt() = default;
};

class SplitInt : public ProtectedInt {
  private:
    int val0;
    int val1;
    int val2;
    int val3;
    int deobfuscate() const override { return val0 + val1 + val2 + val3; }
    void obfuscate(int val) override {
        val0 = val - rand();
        val1 = val / 2;
        val2 = val / 3;
        val3 = val - val0 - val1 - val2;
    }
    void obfuscateAdd(int add) override { val3 += add; }

  public:
    SplitInt() : ProtectedInt() {}
    SplitInt(int val) : ProtectedInt(val) {}
    ~SplitInt() = default;
};

inline int get_val_lars(ProtectedInt &val) { return val.val(); }
inline void set_val_lars(ProtectedInt &val, int set) {
    printf("set_val_lars: %d, %d\n", val.val(), set);
    val = set;
}

inline void add_val_lars(ProtectedInt &val, int add) {
    printf("add_val_lars: %d, %d\n", val.val(), add);
    val += add;
}
