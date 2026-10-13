#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

struct Cvor {
  bool je_list;
  // brojevi su binarni - tako da sigurno imamo najvise 2 potomka (cifra 1 ili cifra 0)
  Cvor *potomci[2];
};

Cvor *napravi_cvor() {
  // ovde moze alokacija pomocu malloc-a, jer imamo niz fiksne duzine 2
  Cvor *novi_cvor = (Cvor*)malloc(sizeof(Cvor));
  novi_cvor->je_list = false;
  // oba potomka svakog cvora su nullptr tj. ne postoje dok ne dodjemo u situaciju da moramo da ih kreiramo
  novi_cvor->potomci[0] = novi_cvor->potomci[1] = nullptr;
  return novi_cvor;
}

// funkcija za racunanje vrednosti broja iz string-a koji predstavlja binarnu reprezentaciju
int izracunaj_vrednost(std::string& binarni_broj) {
  int n = binarni_broj.size();
  int vrednost = 0;
  for (int i = 0, j = n - 1; i < n; i++, j--)
    // oduzimamo '0' da bismo dobili cifru, a ne ASCII vrednost
    vrednost += std::pow(2, i) * (binarni_broj[j] - '0');
  return vrednost;
}

void dodaj_broj(Cvor *koren, std::string& broj, int i) {
  // ukoliko smo dosli do kraja broja to znaci da je cvor u kome se trenutno nalazimo list
  // (predstavlja kraj broja), oznacavamo to i zavrsavamo rekurziju
  if (i == (int)broj.size()) {
    koren->je_list = true;
    return;
  }
  // uzimamo trenutnu cifru (0 ili 1), oduzimamo '0' da bismo dobili celobrojnu vrednost
  int cifra = broj[i] - '0';
  // ukoliko ne postoji cvor za tu cifru, kreiramo ga
  if (koren->potomci[cifra] == nullptr)
    koren->potomci[cifra] = napravi_cvor();
  // rekurzivno nastavljamo za ostatak broja na odgovarajucem potomku
  dodaj_broj(koren->potomci[cifra], broj, i + 1);
}

// funkcija koja trazi maksimalnu vrednost xor-adatog broja sa svim brojevima koji su vec u stablu
void pronadji_xor(Cvor *koren, std::string& rezultat_xor, std::string& broj, int i) {
  if (koren->je_list) {
    return;
  }
  // trenutna cifra broja koju razmatramo
  int cifra = broj[i] - '0';
  // logika za xor: zelimo tj. cilj nam je da dobijemo sto vise jedinica na pocecima (najznacajnijim pozicijama)
  // zato pokusavamo da izaberemo SUPROTNU cifru od nase trenutne koju razmatramo (ako je nasa 0 hocemo 1, i obrnuto)
  int suprotna_cifra = std::abs(cifra - 1);
  // ukoliko dodjemo do cvora koji cuva suprotnu cifru od nase trenutne posmatrane cifra idemo tim putem i u
  // rezultat xor-a dodajemo '1'
  if (koren->potomci[suprotna_cifra]) {
    rezultat_xor += '1';
    pronadji_xor(koren->potomci[suprotna_cifra], rezultat_xor, broj, i + 1);
  } else {
    // ako suprotna cifra ne postoji moramo da idemo istim putem pa u rezultat dodajemo '0'
    rezultat_xor += '0';
    pronadji_xor(koren->potomci[cifra], rezultat_xor, broj, i + 1);
  }
}

void oslobodi_stablo(Cvor *koren) {
  if (koren == nullptr)
    return;
  for (auto& potomak : koren->potomci)
    oslobodi_stablo(potomak);
  free(koren);
}

int main() {

  std::vector<std::string> brojevi = {"101", "000", "011"};
  int maksimalna_vrednost = 0;
  std::string maks_xor;
  std::string trenutni_xor;
  
  Cvor *koren = napravi_cvor();
  
  for (std::string& broj : brojevi) {
    trenutni_xor = "";
    dodaj_broj(koren, broj, 0);
    pronadji_xor(koren, trenutni_xor, broj, 0);
    // pretvaramo binarni rezultat u broj i pratimo maksimum
    if (izracunaj_vrednost(trenutni_xor) > maksimalna_vrednost) {
      maksimalna_vrednost = izracunaj_vrednost(trenutni_xor);
      maks_xor = trenutni_xor;
    }
  }
  
  std::cout << maks_xor << std::endl;
  
  oslobodi_stablo(koren);

  return 0;
}
