#include "pch.h"


int main() {

  //Person student;
  
  zmogus student("Jonas", "Girdys");
  student.display();
  cout << "----------" << endl;

  zmogus obj1;
  obj1.setVardas("Petras");
  obj1.setPavarde("Giedraitis");
  obj1.addPazymys(2);
  obj1.addPazymys(3);
  obj1.addPazymys(5);
  obj1.addPazymys(1);
  obj1.setEgzaminas(6);


  obj1.calculateGalutinis(obj1);

  zmogus obj2 = obj1;
  zmogus obj3;
  obj3 = obj1;

  cout << "Informacija apie obj1 " << obj1 << endl;
  cout << "Informacija apie obj2 " << obj2 << endl;
  cout << "Informacija apie obj3 " << obj3 << endl;

  //----------------------

    string choice;
    cout << "Pasirinkite operacija (auto/file): ";
    cin >> choice;

    if (choice == "auto") {
        generateStudentFilesAutomatically();
    }
    else if (choice == "file") {
        srand(time(0));

        zmogus laikinas;
        list<zmogus> grupe;
        int zmoniu_sk = 0;

        string dataChoice;
        cout << "Ar norite skaityti duomenis is failo (taip/ne): ";
        cin >> dataChoice;

        if (dataChoice == "taip") {
            processFileData(grupe);

            grupe.sort([](const zmogus& a, const zmogus& b) {
                return rikiavimas(a, b);
                });
            int choice;
            cout << "Pasirinkite skaiciavimo buda (1 - Vidurkis, 2 - Mediana): ";
            cin >> choice;

            ofstream outputFile("output.txt");
            printStudentDataToFile(grupe, choice, outputFile);
            outputFile.close();

            cout << "Atsakymai irasyti i output.txt faila." << endl;
        }
        else {
            inputStudentData(grupe);
            int choice;
            cout << "Pasirinkite skaiciavimo buda (1 - Vidurkis, 2 - Mediana): ";
            cin >> choice;

            grupe.sort([](const zmogus& a, const zmogus& b) {
                return rikiavimas(a, b);
                });
            printStudentData(grupe, choice);
        }
    }
    else {
        cout << "Neteisinga pasirinkimo operacija." << endl;
    }
    return 0;
};
