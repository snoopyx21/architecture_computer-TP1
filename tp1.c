#include <stdio.h>
#include <stdlib.h>

// Récupérer le nième bit dans une chaîne de caractères
int n_b (char *addr, int i){
  return (char)0x1 & ((char) *(addr + i/8))>>i%8;
}

// n_b décomposé par instruction
int n_b2 (char *addr, int i) {
  // On choisit le caractère qui contient le bit concerné (8 bits par caractère)
  unsigned char quel_caractere_contient_le_bit = addr[i/8];

  // On fait en sorte que le bit en question se retrouve en première position
  // >> étant l'opérateur décalage de bit vers la droite
  // exemple : 10101010 >> 3 = 00010101
  //           11111111 >> 2 = 00111111
  quel_caractere_contient_le_bit = quel_caractere_contient_le_bit >> i%8;

  // Récupérer uniquement le bit qui nous intéresse (le 1er)
  // Le "et" bit à bit : a & b
  // exemple : xxxxxxx1 &       xxxxxxx0 &    // Avec x un bit à 1 ou 0
  //           00000001         00000001      // 0x1
  //           --------         --------
  //           00000001         00000000
  return quel_caractere_contient_le_bit & 0x1;
}

char d2c (int n){
  return ( n<0? '?': (n<10? '0'+n : (n<36 ? 'A' + (n-10) : '?') ));
}

long long int P2(int n) {
  return (n<0?0:(n==0?1 : 2*P2(n-1)));
}

void base2(int n) {
  if (n > 0) {
    base2(n/2);
    printf("%d", n%2);
  }
}

void baseB(int B, int n){
  if (B > 0 && B <= 36) {
    if (n > 0) {
      baseB(B, n/B);
      printf("%c", d2c(n%B));
    }
  }
}

// Copie des bits 0 à 22 dans le tableau result
void mantisse(float f, int result[23]) {
  for(unsigned int i = 0; i < 23; ++i) {
    result[i] = n_b((char*)&f, i);
  }
}

// Récupération de tout les bits de la mantisse dans le tableau mantisse_val
// puis calcul de sa valeur :
// 1 + bit22 * 2^(-1) + bit21 * 2^(-2) + ... + bit0 * 2^(-23)
float mantisseNormalisee(float f) {
  int mantisse_val[23];
  mantisse(f, mantisse_val);
  float mantisse_normal = 1.f;
  float puissance_2 = 1.f/2.f;
  for (int i = 22; i > 0; --i) {
    mantisse_normal += puissance_2 * mantisse_val[i];
    puissance_2 /= 2.f;
  }
  return mantisse_normal;
}

// Copie des bits 23 à 31 dans le tableau result
void exposant(float f, int result[8]) {
  for(unsigned int i = 23; i < 31; ++i) {
    result[i-23] = n_b((char*)&f, i);
  }
}

// Récupération de tout les bits de l'exposant dans le tableau exposant_val
// puis calcul de la valeur de l'exposant en le multipliant par sa puissance de
// 2 correspondante.
// On soustrait 127 car l'exposant est codé avec un décalage de 127.
// Cette représentation permets une comparaison d'exposants (a>b) immédiate
// contrairement au complément à deux ou il faut vérifier le signe.
int exposantSansExces(float f) {
  int exposant_val[8];
  exposant(f, exposant_val);
  float exposant = 0.f;
  float puissance_2 = 1.f;
  for (unsigned int i = 0; i < 8; ++i) {
    exposant += puissance_2 * exposant_val[i];
    puissance_2 *= 2.f;
  }
  return exposant - 127.f;
}

// Retourne 1 si le signe est négatif et 0 sinon
int signe(float f) {
  return n_b((char*)&f, 31);
}

int main(int argc, char **argv){
  int valeur;
  if (argc > 1) {
    valeur = atoi(argv[1]);
  } else {
    valeur = 10;
  }

  printf("La base2 de %d est : ", valeur);
  base2(valeur);
  printf(
      "\n\nVoici la representation machine de %d : "
      , valeur);

  for (unsigned int i = 0; i < sizeof(int)*8; ++i) {
    printf("%d", n_b((char*)&valeur, i));
  }
  printf(
      "\n"
      "Si cette valeur est inversee par rapport à sa base2 c'est qu'on observe"
      "\n\tune representation little endian (petit boutiste) et inversement.\n"
      "\n");
  for (int i = 4; i <= 36; i += 4) {
    printf("Le chiffre %d en base %d se represente ainsi : ", valeur, i);
    baseB(i, valeur);
    printf("\n");
  }

  float valeur_flottante = (float) valeur;
  float mantisse_val = mantisseNormalisee(valeur_flottante);
  int exposant_val = exposantSansExces(valeur_flottante);

  printf(
      "\nLa mantisse de %f est %f\n"
      "L'exposant sans exces de %f est %d\n"
      , valeur_flottante, mantisse_val
      , valeur_flottante, exposant_val);

  const char *signe_char;
  if (signe(valeur_flottante)) {
    signe_char = "negatif";
  } else {
    signe_char = "positif";
  }
  printf("Le signe de %f est %s\n", valeur_flottante, signe_char);
  printf("La mantisse (%f) * 2^(exposant (%d)) = %f\n",
      mantisse_val, exposant_val, mantisse_val * (float)P2(exposant_val));

  return EXIT_SUCCESS;
}
