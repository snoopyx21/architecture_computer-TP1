/*
 * Copyright (c) 2016, Maxime Schmitt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const unsigned int gros_entier = 1e8;
int main(void) {

  // Question 5
  // Quelle est la valeur de a après cette boucle ?
  //
  //    Normalement 10^8 !!
  //    Mais on observe la valeur correspondant à 2^24 (16,777,216)
  //
  // D'où provient cette différence selon vous ?
  //
  //    Erreur d'arrondis des flottants :
  //    Représentation de 2^24 est 1,00000000000000000000000 * 2^24
  //    Représentation de 1    est 1,00000000000000000000000 * 2^0
  //    Addition :
  //    Traduction sous le même exposant pour l'addition (le plus petit des
  //    deux) avec un exemple d'addition sur 25 bits
  //
  //    1000000000000000000000000
  //    0000000000000000000000001 +
  //    -------------------------
  //    1000000000000000000000001
  //     |       Mantisse      |
  //
  //    Repasser sous la représentation normalisée :
  //
  //    1,00000000000000000000000 * 2^24 <- Ici le 1 est jeté durant l'arrondis
  //    car la mantisse ne peut pas contenir plus de 23 bits

  float a = 0.f;
  for(unsigned int i = 0; i < gros_entier; ++i) {
    a += 1.f;
  }
  printf("La valeur de a est %.2f\n", a);


  // Question 6
  // Mais que se passe-t-il ?
  // Mais qu'est-ce qui se passe ?
  // Mais what ?
  //
  //    Le même problème d'arrondis à partir de 2^24 apparaît.
  //    La valeur exacte n'est pas représentable en flottant !
  //    L'addition de la plus petite valeur à la plus grande à permis de mitiger
  //    le problème de précision en additionnant de plus petites valeurs
  //    ensemble et non de très grosses valeurs avec de très petites.

static const unsigned int plus_gros_carre = 500;
static const unsigned int plus_petit_carre = 411;

  if (1) { // Remplacer 0 par 1 pour que le code puisse s'exécuter
           // (pensez à recompiler -> make)
    float b = 0.f;
    float c = 0.f;
    unsigned int d = 0u;

    for(unsigned int i = plus_petit_carre; i <= plus_gros_carre; ++i) {
      b += (float) (i * i);
      d += i*i;
    }

    for(unsigned int i = plus_gros_carre; i >= plus_petit_carre; --i) {
      c += (float) (i*i);
    }

    if (c == b)
      printf("\nLes deux valeurs sont egales à %f\n", c);
    else
      printf("\nLes deux valeurs different : %f et %f\n", b, c);
    printf("La valeur exacte est de %u et sa représentation en flottant est %f\n",
        d, (float)d);
  }


  // Aide question 6
  if (1) { // Remplacer 0 par 1 pour que le code puisse s'exécuter
           // (pensez à recompiler -> make)
    unsigned int p = 0;
    float q = 0.f;
    for(unsigned int i = plus_petit_carre; i <= plus_gros_carre; ++i) {
      p += i*i;
      q += (float) (i*i);
      if (p != (unsigned int) q) {
        printf(
            "\nLa difference apparait pour i = %u\n"
            "\ti*i = %u, p = %u et q = %.2f\n"
            "\tla representation de p en flottant:%.2f\n"
            "\tla puissance de 2 requise pour representer p est %.f\n",
            i, i*i, p, q, (float)p, floorf(logf((float)p)/logf(2.f)));
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
