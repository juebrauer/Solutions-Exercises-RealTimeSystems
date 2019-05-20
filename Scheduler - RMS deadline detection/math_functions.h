#pragma once

/// see https://de.wikibooks.org/wiki/C%2B%2B-Programmierung/_Br%C3%BCche/_Die_Methoden 


/// English: GCD = Greatest common divisor
/// German : ggT = größter gemeinsamer Teiler
/// 
/// --> GCD(12,18)
/// --> GCD(18,12%18)=GCD(a,b)
/// --> GCD(12,18%12)=GCD(a,b)
/// --> GCD(6,12%6)=GCD(a,b)
/// --> return a=6
unsigned int GCD(unsigned int a, unsigned int b) {

  if (b == 0)

    return a;

  else return GCD(b, a % b);

}


/// English: LCM = Least Common Multiple
/// German : kgV = kleinstes gemeinsames Vielfaches
unsigned int LCM(unsigned int a, unsigned int b) {

  return (a * b) / GCD(a, b);

}
