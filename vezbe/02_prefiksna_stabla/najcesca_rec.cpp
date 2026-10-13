#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

struct Cvor {
  // rec koja se nalazi u cvoru; samo listovi (krajevi reci) ce imati ovu promenljivu
  // popunjenu; ostali cvorovi kroz koje samo prolazimo cuvaju samo prazan string
  std::string rec;
  // promenljiva koja cuva broj pojavljivanja odgovarajuce reci
  // kad god stignemo do kraja neke reci tj. u list uvecavamo obaj brojac
  int brojac;
  // mapa u kojoj cuvamo za svaki cvor njegove potomke, kljuc u mapi nam predstavlja
  // karakter koji vodi ka cvoru potomku, a vrednost je bas taj cvor potomak tj.
  // pokazivac na njega
  std::map<char, Cvor*> potomci;
};

Cvor* napravi_cvor() {
  // kada se radi sa map alokacija se MORA vrsiti pomocu 'new'
  // ukoliko se koristi 'malloc' dolazi do gresaka pri pokretanju programa
  Cvor* novi_cvor = new Cvor();
  // na pocetku cvor ne sadrzi nikakvu rec
  novi_cvor->rec = "";
  // na pocetku rec se pojavljuje 0 puta
  novi_cvor->brojac = 0;
  return novi_cvor;
}

void dodaj_rec(Cvor* koren, std::string& rec_za_dodavanje, int i) {
  // ako je 'i' stigao do kraja reci to znaci da smo u listu tj. da su sva slova obradjena
  if (i == (int)rec_za_dodavanje.size()) {
    koren->rec = rec_za_dodavanje;
    koren->brojac++;
    return;
  }
  // trazimo da li vec postoji grana za trenutno slovo (rec_za_dodavanje[i])
  auto iterator = koren->potomci.find(rec_za_dodavanje[i]);
  // ukoliko ne postoji pravimo novi cvor za to slovo
  if (iterator == koren->potomci.end()) {
    koren->potomci[rec_za_dodavanje[i]] = napravi_cvor();
  }
  // idemo rekurzivno dalje, prelazimo na sledeci cvor i sledece slovo (i + 1)
  dodaj_rec(koren->potomci[rec_za_dodavanje[i]], rec_za_dodavanje, i + 1);
}

// funkcija koja pronalazi rec koja se javlja najvise puta
// 'maksimum' se prosledjuje po referenci (&), sto je kljucno da bi sve grane stabla delile isti maksimum
void nadji_najcescu_rec(Cvor* koren, std::string& najcesca_rec, int& maksimum) {
  // ako rec u cvoru nije prazna, znaci da smo u listu (na kraju neke reci)
  if (koren->rec != "") {
    // proveravamo da li je broj pojavljivanja ove reci veci od trenutnog globalnog maksimuma
    if (koren->brojac > maksimum) {
      maksimum = koren->brojac;
      najcesca_rec = koren->rec;
    }
  }
  // prolazimo kroz svu decu (sve potomke) trenutnog cvora
  auto pocetak = koren->potomci.begin();
  auto kraj = koren->potomci.end();
  // idemo redom kroz mapu
  while (pocetak != kraj) {
    // pocetak->second je pokazivac na cvor potomak
    // pozivamo rekurziju za taj cvor kako bismo pretrazili celo stablo
    nadji_najcescu_rec(pocetak->second, najcesca_rec, maksimum);
    // pomeramo se na sledece slovo (sledeceg potomka) u mapi
    pocetak++;
  }
}

// funkcija za oslobadjanje memorije
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

  std::vector<std::string> reci = {"cod", "codecs", "coding", "coder", "coding", "coder", "coder"};
  
  Cvor* koren = napravi_cvor();
  
  for (std::string& rec : reci) {
    dodaj_rec(koren, rec, 0);
  }
  
  std::string najcesca_rec = "";
  int maksimum = 0;
  
  nadji_najcescu_rec(koren, najcesca_rec, maksimum);
  
  std::cout << "Najcesca rec je: " << najcesca_rec << std::endl;
  
  oslobodi_stablo(koren);

  return 0;
}
