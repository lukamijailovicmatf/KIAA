#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <vector>

struct Cvor {
  bool je_list;
  // mapa u kojoj cuvamo potomke cvora
  // kljuc u mapi (char) je karakter koji vodi ka potomku
  // vrednost (Cvor*) je pokazivac na taj cvor potomak
  // koristimo unordered_map (hes mapu) jer su operacije pretrage u proseku brzine O(1)
  std::unordered_map<char, Cvor*> potomci;
};

Cvor* napravi_cvor() {
  // kada se radi sa unordered_map alokacija se MORA vrsiti pomocu 'new'
  // ukoliko se koristi 'malloc' dolazi do gresaka pri pokretanju programa
  Cvor* novi_cvor = new Cvor();
  novi_cvor->je_list = false;
  return novi_cvor;
}

// 'koren' - trenutni cvor; 'rec' - rec koju dodajemo; 'i' - indeks trenutnog slova
void dodaj_rec(Cvor* koren, std::string& rec, int i) {
  if (i == (int)rec.size()) {
    koren->je_list = true;
    return;
  }
  // trazimo da li u mapi trenutnog cvora postoji grana za trenutno slovo (rec[i])
  auto iterator = koren->potomci.find(rec[i]);
  // ukoliko grana za to slovo ne postoji, kreiramo novi cvor
  if (iterator == koren->potomci.end()) {
    koren->potomci[rec[i]] = napravi_cvor();
  }
  // rekurzivno pozivamo funkciju za sledece slovo (i + 1)
  // a kao novi 'koren' prosledjujemo cvor na koji vodi trenutno slovo
  dodaj_rec(koren->potomci[rec[i]], rec, i + 1);
}

// funkcija koja trazi najduzi zajednicki prefiks
void najduzi_prefiks(Cvor* koren, std::string& prefiks) {
  // spustamo se niz stablo sve dok ne dodjemo do prvog lista
  // ili dok god cvor ima tacno jednog potomka
  while (koren != nullptr && koren->je_list == false && koren->potomci.size() == 1) {
    // uzimamo taj jedini element (granu) iz mape
    auto element = koren->potomci.begin();
    // element->first je kljuc (karakter/slovo), dodajemo ga u nas rezultat
    prefiks += element->first;
    // element->second je vrednost (pokazivac na sledeci cvor), prelazimo na njega
    koren = element->second;
  }
}

void oslobodi_stablo(Cvor* koren) {
  if (koren == nullptr) {
    return;
  }
  // prvo oslobadjamo sva podstabla
  for (auto& par : koren->potomci) {
    oslobodi_stablo(par.second);
  }
  // na kraju brisemo sam koren, posto smo koristili 'new' ovde mora 'delete'
  delete koren;
}

int main() {

  std::vector<std::string> reci = {"code", "coder", "coding", "codable", "codec", "codecs", "coded",
                                   "codeless", "codec", "codecs", "codependence", "codex", "codify",
                                   "codependents", "codes", "code", "coder", "codesign", "codec",
                                   "codeveloper", "codrive", "codec", "codecs", "codiscovered"};
                                   
  Cvor* koren = napravi_cvor();
  
  for (std::string& rec : reci) {
    dodaj_rec(koren, rec, 0);
  }
  
  std::string prefiks = "";
  
  najduzi_prefiks(koren, prefiks);
  
  std::cout << "Najduzi zajednicki prefiks je: " << prefiks << std::endl;
  
  oslobodi_stablo(koren);
  
  return 0;
}
