#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#define BROJ_KARAKTERA 128

struct Cvor {
  // zbog specificnosti pref. stabla i cvor koji ima potomke moze da bude list, zbog toga cuvamo info
  // o tome da li je cvor list, odnosno da li se u njemu zavrsava neka rec
  bool je_list;
  // niz u kome cemo cuvati potomke datog cvora, ukoliko potomak na nekoj poziciji ne postoji
  // imacemo nullptr na toj poziciji
  Cvor* potomci[BROJ_KARAKTERA];
};

// funkcija koja kreira i vraca novi cvor
Cvor* napravi_cvor() {
  Cvor* novi_cvor = (Cvor*)malloc(sizeof(Cvor));
  novi_cvor->je_list = false;
  for (int i = 0; i < BROJ_KARAKTERA; i++) {
    novi_cvor->potomci[i] = nullptr;
  }
  return novi_cvor;
}

// funkcija za dodavanje reci u stablo
void dodaj_rec(Cvor* koren, std::string& rec) {
  int duzina = rec.length();
  for (int i = 0; i < duzina; i++) {
    int indeks_slova = (int)rec[i];
    // ukoliko ne postoji grana za to slovo, kreiramo novi cvor
    if (koren->potomci[indeks_slova] == nullptr) {
      koren->potomci[indeks_slova] = napravi_cvor();
    }
    // krecemo se niz stablo
    koren = koren->potomci[indeks_slova];
  }
  // kada dodjemo do kraja reci, poslednji cvor je list
  koren->je_list = true;
}

// funkcija koja proverava da li se rec nalazi u stablu
bool pronadji_rec(Cvor* koren, std::string& rec) {
  if (koren == nullptr) {
    return false;
  }
  int duzina = rec.length();
  for (int i = 0; i < duzina; i++) {
    int indeks_slova = (int)rec[i];
    koren = koren->potomci[indeks_slova];
    if (koren == nullptr) {
      return false;
    }
  }
  return koren->je_list;
}

// funkcija za brisanje stabla iz memorije
void oslobodi_stablo(Cvor* koren) {
  if (koren == nullptr) {
    return;
  }
  // prvo se rekurzivno oslobadjaju sva deca (potomci)
  for (auto& cvor_potomak : koren->potomci) {
    oslobodi_stablo(cvor_potomak);
  }
  // na kraju se oslobadja i roditeljski cvor
  free(koren);
}

int main() {

  std::vector<std::string> reci = {"kod", "koder", "kodiranje", "kodeks"};
  
  Cvor* koren = napravi_cvor();
  
  for (std::string& rec : reci) {
    dodaj_rec(koren, rec);
  }
  
  std::string trazena_rec = "kod";
  
  std::cout << "Da li postoji rec '" << trazena_rec << "'?" << std::endl << 
  std::boolalpha << pronadji_rec(koren, trazena_rec) << std::endl;
  
  oslobodi_stablo(koren);

  return 0;
}
