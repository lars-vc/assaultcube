#include "lars.h"

int val_mask = 1;
int get_val_lars(int val) { return val - val_mask; }
void set_val_lars(int &val, int set) { val = set + val_mask; }
void add_val_lars(int &val, int add) {
    set_val_lars(val, get_val_lars(val) + add);
}
