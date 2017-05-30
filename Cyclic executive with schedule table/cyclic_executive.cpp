#include <conio.h>
#include <iostream>
#include <thread>
using namespace std;

void f1() { cout << "f1 "; }
void f2() { cout << "f2 "; }
void f3() { cout << "f3 "; }

const int nr_schemes = 3;
const int nr_slots   = 3;
void(*calltable[nr_schemes][nr_slots])() =
{ {f1,f2,f3},
  {f2,f2,f2},
  {f1,f3,f1} };
 
int main() {
  while (true) {
    for (int s = 0; s < nr_schemes; s++) {
      for (int slot = 0; slot < nr_slots; slot++) {
        (*calltable[s][slot])();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
      cout << endl;
    }
  }
  cout << "Press key to exit." << endl;
  _getch();
}