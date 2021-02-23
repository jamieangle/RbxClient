#pragma once
#ifdef DLLDIR_EX
#define DLLDIR  __declspec(dllexport)
#else
#define DLLDIR  __declspec(dllimport)
#endif

#include <Windows.h>

class DLLDIR tryDll {
public:

    tryDll(void){
        MessageBox(nullptr, "Constructor", "Ran", NULL);
    }

    ~tryDll(void){
        MessageBox(nullptr, "Destructor", "Ran", NULL);
    }

    void hello(){
        MessageBox(nullptr, "Non-Static Method", "Ran", NULL);
    }

    static void staticMeth(){
        MessageBox(nullptr, "Static Method", "Ran", NULL);
    }
};
