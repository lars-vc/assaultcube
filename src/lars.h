#pragma once
#include <stdlib.h>
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
    Protector *protector;
    int obfuscated_val;
    int id;
    int deobfuscate() { return protector->masks[id] ^ obfuscated_val; }
    int obfuscate(int val) { return protector->masks[id] ^ val; }

  public:
    ProtectedInt operator=(int val) {
        obfuscated_val = obfuscate(val);
        return *this;
    }
    ProtectedInt operator=(ProtectedInt val) {
        obfuscated_val = val.obfuscated_val;
        return *this;
    }
};
