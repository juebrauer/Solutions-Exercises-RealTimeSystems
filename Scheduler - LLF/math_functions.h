#pragma once

/// siehe https://de.wikibooks.org/wiki/C%2B%2B-Programmierung/_Br%C3%BCche/_Die_Methoden 


unsigned int ggT(unsigned int a, unsigned int b) {

  if (b == 0)

    return a;

  else return ggT(b, a % b);

}



unsigned int kgV(unsigned int a, unsigned int b) {

  return (a * b) / ggT(a, b);

}
