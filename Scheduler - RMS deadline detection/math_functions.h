#pragma once

/// see https://de.wikibooks.org/wiki/C%2B%2B-Programmierung/_Br%C3%BCche/_Die_Methoden 


/// English: GCD = Greatest common divisor
/// German : ggT = Größter gemeinsamer Teiler
unsigned int GCD(unsigned int a, unsigned int b) {

  if (b == 0)

    return a;

  else return GCD(b, a % b);

}


/// English: LCM = Least Common Multiple
/// German : kgV = Kleinstes Gemeinsames Vielfaches
unsigned int LCM(unsigned int a, unsigned int b) {

  return (a * b) / GCD(a, b);

}
