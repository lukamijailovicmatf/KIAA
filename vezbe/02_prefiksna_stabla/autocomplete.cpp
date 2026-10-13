#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

struct Cvor {
  bool je_list;
  std::string rec;
  std::map<char, Cvor*> potomci;
};

Cvor* napravi_cvor() {
  Cvor* novi_cvor = new Cvor();
  novi_cvor->je_list = false;
  novi_cvor->rec = "";
  return novi_cvor;
}

void dodaj_rec(Cvor* koren, std::string& rec_za_dodavanje, int i) {
  if (i == (int)rec_za_dodavanje.size()) {
    koren->je_list = true;
    koren->rec = rec_za_dodavanje;
    return;
  }
  auto iterator = koren->potomci.find(rec_za_dodavanje[i]);
  if (iterator == koren->potomci.end()) {
    koren->potomci[rec_za_dodavanje[i]] = napravi_cvor();
  }
  dodaj_rec(koren->potomci[rec_za_dodavanje[i]], rec_za_dodavanje, i + 1);
}

// funkcija koja vraca pokazivac na cvor u kome se zavrsava odgovarajuci prefiks
// ispod tog cvora se nalaze sve reci koje pocinju tim prefiksom
Cvor* automatsko_dovrsavanje(Cvor* koren, std::string& prefiks, int i) {
  // ako smo dosli do kraja prefiksa to znaci da smo nasli trazeni cvor i vracamo ga
  if (i == (int)prefiks.size()) {
    return koren;
  }
  // proveravamo da li u trenutnom cvoru postoji grana za sledece slovo prefiksa
  auto iterator = koren->potomci.find(prefiks[i]);
  // ukoliko grana ne postoji znaci da u stablu nema nijedne reci sa ovim prefiksom
  if (iterator == koren->potomci.end()) {
    return nullptr;
  }
  return automatsko_dovrsavanje(koren->potomci[prefiks[i]], prefiks, i + 1);
}

// funkcija za ispisivanje svih reci koje se nalaze ispod trenutnog cvora
void ispisi(Cvor* koren) {
  // ako dodjemo do cvora koji je oznacen kao list, znaci da se tu nalazi cela rec pa je ispisujemo
  if (koren->je_list) {
    std::cout << koren->rec << std::endl;
  }
  // prolazimo kroz sve potomke trenutnog cvora
  auto pocetak = koren->potomci.begin();
  auto kraj = koren->potomci.end();
  // rekurzivno pozivamo 'ispisi' za svakog potomka
  while (pocetak != kraj) {
    ispisi(pocetak->second);
    pocetak++;
  }
}

void oslobodi_stablo(Cvor* koren) {
  if (koren == nullptr) {
    return;
  }
  for (auto& par : koren->potomci) {
    oslobodi_stablo(par.second);
  }
  delete koren;
}

int main() {

  std::vector<std::string> reci = {"cod", "coder", "codecs", "coding", "code"};
  
  Cvor* koren = napravi_cvor();
  
  std::string prefiks = "code";
  
  for (std::string& rec : reci) {
    dodaj_rec(koren, rec, 0);
  }
  
  // pozivamo funkciju koja nas dovodi do cvora gde se zavrsava prefiks "code"
  Cvor* pomocni_cvor = automatsko_dovrsavanje(koren, prefiks, 0);
  
  // ako je vracen nullptr znaci da nijedna rec u stablu ne pocinje tim prefiksom
  if (pomocni_cvor == nullptr) {
    std::cout << -1 << std::endl;
    return 0;
  }
  
  // ispisujemo sve reci koje se nalaze ispod pronadjenog cvora
  ispisi(pomocni_cvor);
  
  oslobodi_stablo(koren);

  return 0;
}
