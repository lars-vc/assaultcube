#pragma once
#include <stdlib.h>
#include <vector>
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
inline int val_mask = rand();

inline int get_val_lars(int val) { return val ^ val_mask; }
inline void set_val_lars(int &val, int set) { val = set ^ val_mask; }

inline void add_val_lars(int &val, int add) {
    set_val_lars(val, get_val_lars(val) + add);
}

class Protector {
    std::vector<int> masks;
    Protector() {}
};
