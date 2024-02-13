#pragma once
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#define protectfunc /*PROTECTOR_FUNCTION_PRIO=5*/
///////////////////////////////////////////
// protector
///////////////////////////////////////////
class ProtectedInt {
  protected:
    int unprotected_val;
    virtual int deobfuscate() const { return unprotected_val; };
    virtual void obfuscate(int val) { unprotected_val = val; }

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
    void obfuscateAdd(int add) { obfuscate(deobfuscate() + add); }
    void obfuscateAdd(int add, int linenr) { obfuscate(deobfuscate() + add); }
};

class XORInt : public ProtectedInt {
  private:
    int mask = rand();
    int obfuscated_val;
    int deobfuscate() const override { return mask ^ obfuscated_val; }
    void obfuscate(int val) override { obfuscated_val = mask ^ val; }
    void obfuscateAdd(int add) { obfuscated_val += add; }

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

  public:
    SplitInt() : ProtectedInt() {}
    SplitInt(int val) : ProtectedInt(val) {}
    ~SplitInt() = default;

    void obfuscateAdd(int add) { val3 += add; }
    void obfuscateAdd(int add, int linenr) {
        if (linenr % 4 == 0) {
            val0 += add;
        } else if (linenr % 4 == 1) {
            val1 += add;
        } else if (linenr % 4 == 2) {
            val2 += add;
        } else if (linenr % 4 == 3) {
            val3 += add;
        }
    }
};

class ChainInt : public ProtectedInt {
  private:
    int size;
    int **p;
    int **start;
    int deobfuscate() const override { return **p; }
    void obfuscate(int val) override { **p = val; }
    void create_new_path(int val) {
        start = (int **)malloc(sizeof(int *));
        p = start;
        size = rand() % 10 + 1;
        for (int i = 0; i < size; i++) {
            int *next = (int *)malloc(sizeof(int *));
            *p = next;
            p = (int **)next;
        }
        int *next = (int *)malloc(sizeof(int *));
        *p = next;
        **p = val;
    }
    void free_path() {
        // clear value out of memory, might not work thanks to compiler
        obfuscate(0xdeadbeef);
        p = start;
        for (int i = 0; i < size + 1; i++) {
            int *next = *p;
            free(p);
            p = (int **)next;
        }
        free(p);
    }

  public:
    ChainInt() { create_new_path(0); }
    ChainInt(int val) { create_new_path(val); }
    ~ChainInt() { free_path(); }

    void newpath() {
        int val = deobfuscate();
        printf("TEST\n");
        free_path();
        printf("TEST2\n");
        create_new_path(val);
    }
};

protectfunc inline int get_val_lars(ProtectedInt &val) { return val.val(); }
inline void set_val_lars(ProtectedInt &val, int set) {
    printf("set_val_lars: %d, %d\n", val.val(), set);
    val = set;
}

// inline void add_val_lars(ProtectedInt &val, int add) {
//     printf("add_val_lars: %d, %d\n", val.val(), add);
//     val += add;
// }
