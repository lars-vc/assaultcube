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
  private:
  public:
    Protector *protector;
    int obfuscated_val;
    int id;
    int deobfuscate() const { return protector->masks[id] ^ obfuscated_val; }
    void obfuscate(int val) { obfuscated_val = protector->masks[id] ^ val; }
    // int deobfuscate() const { return 1111111 ^ obfuscated_val; }
    // int obfuscate(int val) { return 1111111 ^ val; }

    ProtectedInt() {
        protector = new Protector();
        id = 0;
        protector->masks.push_back(rand());
        obfuscate(0);
    }
    ProtectedInt(int val) {
        protector = new Protector();
        id = 0;
        protector->masks.push_back(rand());
        obfuscate(val);
    }

    // OPERATORS
    ProtectedInt &operator=(int val) {
        printf("=1 val: %d\n", val);
        obfuscate(val);
        return *this;
    }
    ProtectedInt &operator=(ProtectedInt val) {
        printf("=2 val: %d\n", val.val());
        obfuscated_val = val.obfuscated_val;
        id = val.id;
        return *this;
    }
    ProtectedInt operator+(ProtectedInt add) {
        printf("+1 val: %d\n", add.val());
        int val = deobfuscate();
        int valadd = add.deobfuscate();
        ProtectedInt p(val + valadd);
        return p;
    }
    ProtectedInt operator+(int add) {
        printf("+2 val: %d\n", add);
        int val = deobfuscate();
        ProtectedInt p(val + add);
        return p;
    }
    ProtectedInt &operator+=(int add) {
        printf("+= val: %d\n", add);
        int val = deobfuscate();
        obfuscate(val + add);
        return *this;
    }
    ProtectedInt &operator++(int) {
        printf("+=\n");
        return this->operator+=(1);
    }
    ProtectedInt &operator-=(int add) {
        printf("+= val: %d\n", add);
        return this->operator+=(-add);
    }
    ProtectedInt &operator--(int) {
        printf("+= val\n");
        return this->operator-=(1);
    }
    operator bool() const { return deobfuscate() != 0; }
    // Maybe this can work?
    // operator int() const { return deobfuscate(); }

    int val() { return deobfuscate(); }
};

inline int get_val_lars(ProtectedInt val) { return val.val(); }
inline void set_val_lars(ProtectedInt &val, int set) {
    printf("set_val_lars: %d, %d\n", val.val(), set);
    val = set;
}

inline void add_val_lars(ProtectedInt &val, int add) {
    printf("add_val_lars: %d, %d\n", val.val(), add);
    val += add;
}
