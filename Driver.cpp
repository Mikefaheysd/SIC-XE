//
// Created by Michael Fahey on 8/9/18.
//

#include <iostream>
#include <stdio.h>
#include "sicxe_asm.h"

using namespace std;

int main(int argc, char *argv[]) {

//    // check for file
//    if(argc != 2) {
//        cout << "Error, you must supply the name of the file " <<
//             "to process at the command line." << endl;
//        exit(1);
//    }

    string filename = "1source.asm";//argv[1];
    sicxe_asm obj;
    obj.appMain(filename);

    return 0;
}