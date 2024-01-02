#include "pch.h"

zmogus::zmogus(std::istream& is) : egz(0), vidur(0), median(0), galutinis(0) {
    readzmogus(is);
}

istream& zmogus::readzmogus(istream& is) {
    is >> vardas >> pavarde >> egz;
    float paz;
    while (is >> paz) {
        nd.push_back(paz);
    }
    return is;
}

istream& operator>>(istream& is, zmogus& student)
{
    cout << "Iveskite varda ir pavarde: ";
    is >> student.vardas >> student.pavarde;

    int ndskaicius;
    cout << "Iveskite kiek namu darbu turi zmogus (jei nenorite ivesti, "
            "palikite tuscia ir spauskite Enter): ";
    cin.ignore();
    string nInput;
    getline(is, nInput);
    if (!nInput.empty()) {
        istringstream iss(nInput);
        iss >> ndskaicius;

        if (ndskaicius > 0) {
          cout << "Iveskite namu darbu pazymius (atskirkite ivertinimus "
                  "tarpais): ";
          for (int i = 0; i < ndskaicius; ++i) {
            int pazymys;
            is >> pazymys;
            student.addPazymys(pazymys);
          }
        }
    } else {
        cout << "Ar sugeneruoti namu darbu ir egzamino pazymius? (1 - Taip, 0 "
                "- Ne): ";
        int generate;
        is >> generate;
        if (generate) {
          cout
            << "Ar norite pasirinkti namu darbu skaiciu? (1 - Taip, 0 - Ne): ";
          int chooseN;
          is >> chooseN;
          if (chooseN) {
            cout << "Iveskite namu darbu skaiciu: ";
            is >> ndskaicius;
          } else {
            ndskaicius = rand() % 10 + 1;
          }
          cout << "Sugeneruotas namu darbu kiekis: " << ndskaicius << endl;
          cout << "Sugeneruoti namu darbu pazymiai: ";
          generateRandomGrades(student, ndskaicius);
          

          cout << endl;

          student.setEgzaminas(rand() % 10 + 1);
          cout << "Sugeneruotas egzamino pazymys: " << student.getEgzaminas()
               << endl;
         calculateAverage(student);
         calculateGalutinis(student);

        } else {
          cout << "Iveskite namu darbu pazymius (atskirkite ivertinimus "
                  "tarpais baigti - spauskite Enter): ";
          int k;

          while (is >> k) {
            if (k < 0 || k > 10) {
              throw out_of_range(
                "Invalid input. Please enter a number between 0 and 10.");
            }
            student.addPazymys(k);

            if (is.peek() == '\n') {
              is.ignore(); // Ignore the newline character
              break;
            }
          }
          calculateAverage(student);
        }
    }
    if (student.getEgzaminas() == 0) {
        // If not, prompt for exam score
        cout << "Iveskite egzamino bala: ";
        is >> student.egz;

        // Additional logic for calculating average, median, and final grade if
        // needed
        calculateMedian(student);
        calculateAverage(student);
        calculateGalutinis(student);
    }

    return is;
}

void processFileData(list<zmogus>& grupe) {

    string fileName;
    std::cout << "Irasykite failo pavadinima (iskaitant ir failo tipa): ";
    cin.ignore();
    getline(cin, fileName);

    ifstream input(fileName);

    if (!input.is_open()) {
        throw invalid_argument("Netinkamas failo pavadinimas arba failas neegzistuoja.");
    }

    string line;
    while (getline(input, line)) {
        processLine(line, grupe);
    }

    input.close();
}
void processLine(const string& line, list<zmogus>& grupe)
{
    if (line.find("Vardas") != string::npos ||
        line.find("Pavarde") != string::npos)
        return;
    istringstream ss(line);
    zmogus laikinas;

    string vardas, pavarde;
    int nd1, nd2, nd3, nd4, egz;

    if (ss >> vardas >> pavarde >> nd1 >> nd2 >> nd3 >> nd4 >> egz) {
        laikinas.setVardas(vardas);
        laikinas.setPavarde(pavarde);
        laikinas.addPazymys(nd1);
        laikinas.addPazymys(nd2);
        laikinas.addPazymys(nd3);
        laikinas.addPazymys(nd4);
        laikinas.setEgzaminas(egz);

        grupe.push_back(laikinas);

        // Processing inside the if block
        /* calculateAverage(laikinas);
        calculateMedian(laikinas);
        laikinas.clearND();*/

    } else {
        cerr << "Invalid input format in the file." << endl;
        return;
    }
}

void calculateMedian(zmogus& zmog) {
    list<int> sort_nd = zmog.getPaz();
    sort_nd.sort();

    if (sort_nd.size() % 2 == 0) {
        auto it1 = sort_nd.begin();
        std::advance(it1, sort_nd.size() / 2 - 1);
        int mid1 = *it1;

        auto it2 = sort_nd.begin();
        std::advance(it2, sort_nd.size() / 2);
        int mid2 = *it2;

        zmog.setMed((mid1 + mid2) / 2.0);
    }
    else {
        auto it = sort_nd.begin();
        std::advance(it, sort_nd.size() / 2);
        zmog.setMed (*it);
    }
}
void calculateAverage(zmogus& zmog) {
    float vidurkis = 0;
    for (int paz : zmog.getPaz()) {
        vidurkis += paz;
    }
    vidurkis = (vidurkis / zmog.getPaz().size());
    zmog.setVid(vidurkis);
    cout << "Debug: Calculate Average for " << zmog.getVardas() << " "
         << zmog.getPavarde() << ": " << zmog.getVid() << endl;
}

/* void inputStudentData(list<zmogus>& grupe)
{
    int zmoniu_sk;
    std::cout << "Iveskite mokiniu skaiciu: ";
    cin >> zmoniu_sk;

    for (int j = 0; j < zmoniu_sk; j++) {
        zmogus laikinas;

        std::cout << "Iveskite varda ir pavarde " << endl;
        string vardas, pavarde;
        cin >> vardas >> pavarde;
        laikinas.setVardas(vardas);
        laikinas.setPavarde(pavarde);

        int ndskaicius = -1;
        std::cout << "Iveskite kiek namu darbu turi zmogus (jei nenorite ivesti, palikite tuscia ir spauskite Enter): ";
        cin.ignore();
        string nInput;
        getline(cin, nInput);
        if (!nInput.empty()) {
            istringstream iss(nInput);
            iss >> ndskaicius;
        }

        if (ndskaicius == -1) {
            std::cout << "Ar sugeneruoti namu darbu ir egzamino pazymius? (1 - Taip, 0 - Ne): ";
            int generate;
            cin >> generate;
            if (generate) {
                std::cout << "Ar norite pasirinkti namu darbu skaiciu? (1 - Taip, 0 - Ne): ";
                int chooseN;
                cin >> chooseN;
                if (chooseN) {
                    std::cout << "Iveskite namu darbu skaiciu: ";
                    cin >> ndskaicius;
                }
                else {
                    ndskaicius = rand() % 10 + 1;
                }
                std::cout << "Sugeneruotas namu darbu kiekis: " << ndskaicius << endl;
                std::cout << "Sugeneruoti namu darbu pazymiai: ";
                generateRandomGrades(laikinas, ndskaicius);

                std::cout << endl;

                laikinas.setEgzaminas(rand() % 10 + 1);
                std::cout << "Sugeneruotas egzamino pazymys: " << laikinas.getEgzaminas() << endl;
            }
            else {
                while (true) {
                    try {
                        std::cout << "Iveskite namu darbu pazymius (atskirkite ivertinimus tarpais baigti - spauskite Enter): ";
                        int k;

                        while (cin >> k) {
                            if (k < 0 || k > 10) {
                                throw out_of_range("Invalid input. Please enter a number between 0 and 10.");
                            }
                            laikinas.addPazymys(k);

                            if (cin.peek() == '\n') {
                                cin.ignore(); // Ignore the newline character
                                break;
                            }
                        }

                        break; // Exit the loop if all inputs were valid
                    }
                    catch (const out_of_range& e) {
                        cerr << e.what() << endl;
                        laikinas.clearND(); // Clear the vector if an error occurred
                    }
                    catch (const exception& e) {
                        cerr << "An exception occurred: " << e.what() << endl;
                    }
                }
                int egz;
                ndskaicius = laikinas.getPaz().size();
                std::cout << "Iveskite egzamino bala " << endl;
                cin >> egz;
                    laikinas.setEgzaminas(egz);
            }
        }
        else {
            for (int i = 0; i < ndskaicius; i++) {
                int k;
                std::cout << "Iveskite " << i + 1 << " pazymi " << endl;
                cin >> k;
                laikinas.addPazymys(k);
            }
            int egz;
            std::cout << "Iveskite egzamino bala " << endl;
            cin >> egz;
            laikinas.setEgzaminas(egz);
        }

        
        calculateAverage(laikinas);
        calculateMedian(laikinas);

        laikinas.setGalutinis(laikinas.getVid());

        grupe.push_back(laikinas);
        laikinas.clearND();
    }
}*/

void inputStudentData(list<zmogus>& grupe)
{
    int zmoniu_sk;
    cout << "Iveskite mokiniu skaiciu: ";
    cin >> zmoniu_sk;

    for (int j = 0; j < zmoniu_sk; ++j) {
        zmogus laikinas;
        cin >> laikinas; // Panaudoti ivedimo operatoriu
        grupe.push_back(laikinas);
    }
}


 void printStudentData(const list<zmogus>& grupe, int choice)
{
    std::cout << std::left << setw(15) << "Vardas" << setw(15) << "Pavarde" << setw(10) << "Galutinis (";

    if (choice == 1) {
        std::cout << setw(10) << std::left << "vid.)";
    }
    else if (choice == 2) {
        std::cout << setw(10) << std::left << "med.)";
    }

    std::cout << setw(15) << "Address" << endl;

    std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ') << std::endl;

    for (const auto& zmogus : grupe) {
        std::cout << std::left << setw(15) << zmogus.getVardas() << setw(15) << zmogus.getPavarde() << setw(20);

        if (choice == 1) {
            float galutinis = zmogus.getVid() * 0.4 + zmogus.getEgzaminas() * 0.6;
            std::cout << fixed << galutinis << setprecision(2);
        }
        else if (choice == 2) {
            float galutinis = zmogus.getMed() * 0.4 + zmogus.getEgzaminas() * 0.6;
            std::cout << fixed << galutinis << setprecision(2);
        }

        // Get the address of the object and convert it to a string
        uintptr_t address = reinterpret_cast<uintptr_t>(&zmogus);
        std::cout << " 0x" << std::hex << address << std::dec;

        std::cout << endl;
    }
}
void generateRandomGrades(zmogus& zmog, int ndskaicius) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    for (int i = 0; i < ndskaicius; i++) {
        int k = dis(gen);
        std::cout << k << " ";
        zmog.addPazymys(k);
    }
}
void printStudentDataToFile( const list<zmogus>& grupe, int choice, ofstream& outputFile)
{
    outputFile << std::left << setw(15) << "Vardas" << setw(15) << "Pavarde"
               << setw(10) << "Galutinis (";

    if (choice == 1) {
        outputFile << std::left << "vid.)";
    } else if (choice == 2) {
        outputFile << std::left << "med.)";
    }

    outputFile << endl;

    for (const zmogus& student : grupe) {
        outputFile << std::left << setw(15) << student.getVardas() << setw(15)
                   << student.getPavarde() << setw(20);

        if (choice == 1) {
            calculateAverage(const_cast<zmogus&>(student));
            float vid = student.getVid();
            int egz = student.getEgzaminas();
            float galutinis = vid * 0.4 + egz * 0.6;

            // Debug prints
            cout << "Debug: Vardas: " << student.getVardas()
                 << " Pavarde: " << student.getPavarde() << " Vid: " << vid
                 << " Egzaminas: " << egz << " Galutinis: " << galutinis
                 << endl;

            outputFile << fixed <<  galutinis << setprecision(2);
        } else if (choice == 2) {
            float med = student.getMed();
            int egz = student.getEgzaminas();
            float galutinis = med * 0.4 + egz * 0.6;

            // Debug prints
            cout << "Debug: Vardas: " << student.getVardas()
                 << " Pavarde: " << student.getPavarde() << " Mediana: " << med
                 << " Egzaminas: " << egz << " Galutinis: " << galutinis
                 << endl;

            outputFile << fixed << galutinis << setprecision(2);
        }
        outputFile << endl;
    }
}


void generateStudentFilesAutomatically() {
    int numStudents[] = { 1000, 10000, 100000, 1000000, 10000000 };
    int numHomeworks = 4;
    string filenames[] = { "students_1000.txt", "students_10000.txt", "students_100000.txt",
                          "students_1000000.txt", "students_10000000.txt" };

    std::cout << "Pasirinkite pagal ka surusiuoti duomenis (v - vardas, p - pavarde, g - galutinis): ";
    string rusiavimoKriterijus;
    cin >> rusiavimoKriterijus;

    std::cout << "Pasirinkite strategija (1 - Strategija 1, 2 - Strategija 2, 3 - Strategija 3): ";
    int strategija;
    cin >> strategija;
    std::cout << '\n';

    for (int i = 0; i < 5; i++) {

        ifstream fileCheck(filenames[i]);
        if (fileCheck.good()) {
            std::cout << "File " << filenames[i] << " already exists. Skipping generation." << endl;
        }
        else {
            auto start = chrono::high_resolution_clock::now();
            generateStudentFile(numStudents[i], numHomeworks, filenames[i]);
            auto end = std::chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            std::cout << "Failo kurimo laikas (" << filenames[i] << "): " << duration.count() << " sekundes" << endl;
        }

        calculateGalutinisForFile(filenames[i], rusiavimoKriterijus, strategija);
    }
}
void generateStudentFile(int numStudents, int numHomeworks, const std::string& filename) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> examDist(1, 10);
    uniform_int_distribution<> ndDist(1, 10);

    ofstream outputFile(filename);

    if (outputFile.is_open()) {
        outputFile << std::left << std::setw(22) << "Vardas" << std::setw(22) << "Pavarde";

        for (int i = 1; i <= numHomeworks; ++i) {
            outputFile << std::setw(10) << "ND" + std::to_string(i);
        }

        outputFile << std::setw(10) << " Egzaminas\n";

        for (int i = 0; i < 94; ++i) {
            outputFile << "-";
        }
        outputFile << "\n";
        for (int i = 1; i <= numStudents; ++i) {
            std::stringstream studentData;
            studentData << std::left << std::setw(22) << ("Vardenis" + std::to_string(i));
            studentData << std::left << std::setw(22) << ("Pavardenis" + std::to_string(i));

            for (int j = 0; j < numHomeworks; ++j) {
                studentData << std::setw(10) << ndDist(gen);
            }

            studentData << std::setw(11) << examDist(gen) << std::endl;
            outputFile << studentData.str();
        }

        outputFile.close();
        std::cout << "Sukurtas: " << filename << std::endl;
    }
    else {
        std::cerr << "Nepavyko atidaryti failo: " << filename << std::endl;
    }

}
void calculateGalutinisForFile(const string& filename, string rusiavimoKriterijus, int strategija) {

    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        auto startRead = chrono::high_resolution_clock::now();
        string line;
        list<zmogus> students;

        getline(inputFile, line); // Read the first line (header)
        getline(inputFile, line);

        while (getline(inputFile, line)) {
            istringstream iss(line);
            zmogus student;

            string vardas, pavarde;
            int nd1, nd2, nd3, nd4, egz;

            if (iss >> vardas >> pavarde >> nd1 >> nd2 >> nd3 >> nd4 >> egz) {
                student.setVardas(vardas);
                student.setPavarde(pavarde);
                student.addPazymys(nd1);
                student.addPazymys(nd2);
                student.addPazymys(nd3);
                student.addPazymys(nd4);
                student.setEgzaminas(egz);

            }
            else {
                std::cerr << "Error: Unable to read line correctly. Skipping the line." << std::endl;
                continue;
            }

            calculateGalutinis(student);

            students.push_back(student);
        }

        inputFile.close();
        auto endRead = std::chrono::high_resolution_clock::now();

        chrono::duration<double> durationRead = endRead - startRead;
        std::cout << "Failo (" << filename << ") nuskaitymo laikas: " << durationRead.count() << " sekundes" << std::endl;

        /*string sortingCriteria;

        cout << "Pasirinkite pagal ka surusiuoti duomenis (vardas/pavarde/galutinis): ";
        cin >> sortingCriteria;*/

        auto startSort = std::chrono::high_resolution_clock::now();

        if (rusiavimoKriterijus == "v") {
            students.sort([](const zmogus& a, const zmogus& b) {
                return rikiavimas(a, b);
                });
        }
        else if (rusiavimoKriterijus == "p") {
            students.sort([](const zmogus& a, const zmogus& b) {
                return rikiavimaspav(a, b);
                });
        }
        else if (rusiavimoKriterijus == "g") {
            students.sort(compareStudents);
        }
        else {
            std::cout << "Netinkamas rusiavimo kriterijus. Naudojamas numatytasis (galutinis)." << endl;
            students.sort(compareStudents);
        }
        auto endSort = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> durationSort = endSort - startSort;
        std::cout << "Failo (" << filename << ") rusiavimo laikas : " << durationSort.count() << " sekundes" << std::endl;

        /*std::cout << "Choose an option (1 - Strategija 1, 2 - Strategija 2): ";
        int option;
        std::cin >> option;*/

        if (strategija == 1) {

            list<zmogus> kietiakaiStudents;
            list<zmogus> vargsiukaiStudents;

            auto startDivision = std::chrono::high_resolution_clock::now();

            for (const auto& student : students) {
                if (student.getGalutinis() >= 5) {
                    kietiakaiStudents.push_back(student);
                }
                else {
                    vargsiukaiStudents.push_back(student);
                }
            }

            auto endDivision = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationDivision = endDivision - startDivision;
            std::cout << "Division time: " << durationDivision.count() << " sekundes" << std::endl;

            ofstream kietiakai("kietiakai_" + filename);
            ofstream vargsiukai("vargsiukai_" + filename);

            kietiakai << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";
            vargsiukai << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";

            auto startWriteKietiakai = std::chrono::high_resolution_clock::now();
            for (const auto& student : kietiakaiStudents) {
                kietiakai << std::left << std::setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }
            auto endWriteKietiakai = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationWriteKietiakai = endWriteKietiakai - startWriteKietiakai;
            kietiakai.close();

            std::cout << "Irasimo i faila (kietiakai): " << durationWriteKietiakai.count() << " sekundes" << std::endl;

            auto startWriteVargsiukai = std::chrono::high_resolution_clock::now();
            for (const auto& student : vargsiukaiStudents) {
                vargsiukai << std::left << std::setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }
            auto endWriteVargsiukai = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationWriteVargsiukai = endWriteVargsiukai - startWriteVargsiukai;

            vargsiukai.close();
            std::cout << "Irasimo i faila (vargsiukai): " << durationWriteVargsiukai.count() << " sekundes" << std::endl;

            double totalTime = durationRead.count() + durationSort.count() + durationDivision.count() + durationWriteKietiakai.count() + durationWriteVargsiukai.count();
            std::cout << "Bendras failo apdorojimo laikas: " << totalTime << " sekundes" << std::endl;
            std::cout << '\n';
        }

        else if (strategija == 2) {
            list<zmogus> vargsiukai;

            auto startDivision = std::chrono::high_resolution_clock::now();

            for (auto it = students.begin(); it != students.end(); ) {
                if (it->getGalutinis() < 5) {
                    vargsiukai.push_back(*it);
                    it = students.erase(it);
                }
                else {
                    ++it;
                }
            }

            auto endDivision = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationDivision = endDivision - startDivision;
            std::cout << "Division time: " << durationDivision.count() << " sekundes" << std::endl;

            ofstream studentsFile("students_" + filename);
            ofstream vargsiukaiFile("vargsiukai2_" + filename);

            studentsFile << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";
            vargsiukaiFile << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";

            auto startStudent = std::chrono::high_resolution_clock::now();

            for (const auto& student : students) {
                studentsFile << std::left << std::setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }

            auto endStudent = chrono::high_resolution_clock::now();


            chrono::duration<double> durationStudent = endStudent - startStudent;
            std::cout << "Irasimo i faila (Student): " << durationStudent.count() << " sekundes" << endl;

            auto startVargsiukai = chrono::high_resolution_clock::now();

            for (const auto& student : vargsiukai) {
                vargsiukaiFile << left << setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }

            auto endVargsiukai = chrono::high_resolution_clock::now();

            chrono::duration<double> durationVargsiukai = endVargsiukai - startVargsiukai;
            std::cout << "Irasimo i faila (vargsiukai): " << durationVargsiukai.count() << " sekundes" << endl;

            studentsFile.close();
            vargsiukaiFile.close();

            double totalTime = durationRead.count() + durationSort.count() + durationDivision.count() + durationStudent.count() + durationVargsiukai.count();
            std::cout << "Bendras failo apdorojimo laikas: " << totalTime << " sekundes" << endl;
            std::cout << '\n';
        }
        else if (strategija == 3) {

            //list<zmogus> vargsiukai;


            auto startDivision = std::chrono::high_resolution_clock::now();

            auto partitionIt = std::partition(students.begin(), students.end(), [](const zmogus& student) {
                return student.getGalutinis() < 5;
                });

            list<zmogus> vargsiukai(partitionIt, students.end());
            students.erase(partitionIt, students.end());

            /*
            auto newEnd = remove_if(students.begin(), students.end(), [](const zmogus& student) {
                return student.galutinis < 5;
                });

            copy_if(students.begin(), newEnd, back_inserter(vargsiukai), [](const zmogus& student) {
                return student.galutinis < 5;
                });

            students.erase(newEnd, students.end());
            */

            auto endDivision = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationDivision = endDivision - startDivision;
            std::cout << "Division time: " << durationDivision.count() << " sekundes" << std::endl;

            ofstream studentsFile("students_" + filename);
            ofstream vargsiukaiFile("vargsiukai2_" + filename);

            studentsFile << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";
            vargsiukaiFile << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde" << std::setw(10) << "Galutinis\n";

            auto startStudent = std::chrono::high_resolution_clock::now();

            for (const auto& student : students) {
                studentsFile << std::left << std::setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }

            auto endStudent = chrono::high_resolution_clock::now();


            chrono::duration<double> durationStudent = endStudent - startStudent;
            std::cout << "Irasimo i faila (Student): " << durationStudent.count() << " sekundes" << endl;

            auto startVargsiukai = chrono::high_resolution_clock::now();

            for (const auto& student : vargsiukai) {
                vargsiukaiFile << left << setw(20) << student.getVardas() << std::setw(20) << student.getPavarde() << std::setw(10) << student.getGalutinis() << "\n";
            }

            auto endVargsiukai = chrono::high_resolution_clock::now();

            chrono::duration<double> durationVargsiukai = endVargsiukai - startVargsiukai;
            std::cout << "Irasimo i faila (vargsiukai): " << durationVargsiukai.count() << " sekundes" << endl;

            studentsFile.close();
            vargsiukaiFile.close();

            double totalTime = durationRead.count() + durationSort.count() + durationDivision.count() + durationStudent.count() + durationVargsiukai.count();
            std::cout << "Bendras failo apdorojimo laikas: " << totalTime << " sekundes" << endl;
            std::cout << '\n';
        }

        else {
            std::cerr << "Ivestas netinkamas pasirinkimas: " << filename << std::endl;
        }
    }
    else {
        std::cerr << "Nepavyko atidaryti failo: " << filename << std::endl;
    }

}
void calculateGalutinis(zmogus& student) {
    if (student.getPaz().size() > 0) {
        float sum = std::accumulate(student.getPaz().begin(), student.getPaz().end(), 0.0);
        student.setVid( (sum / static_cast<float>(student.getPaz().size())));
        student.setGalutinis( student.getVid() * 0.4 + student.getEgzaminas() * 0.6);
        //std::cout << "Debug: " << student.getVardas() << " " << student.getPavarde() << " " << student.getVid() << " " << student.getEgzaminas() << " " << student.getGalutinis() << std::endl;
    }
    else {
        student.setVid(0.0);
        student.setGalutinis(student.getEgzaminas() * 0.6);
    }
}
bool compareStudents(const zmogus& a, const zmogus& b) {
    if (a.getGalutinis() == b.getGalutinis()) {
        return rikiavimas(a, b);
    }
    return a.getGalutinis() > b.getGalutinis();
}
bool rikiavimas(const zmogus& a, const zmogus& b) {
    size_t i = 0, j = 0;
    while (i < a.getVardas().size() && j < b.getVardas().size()) {
        if (isdigit(a.getVardas()[i]) && isdigit(b.getVardas()[j])) {
            int numA = 0, numB = 0;
            while (i < a.getVardas().size() && isdigit(a.getVardas()[i])) {
                numA = numA * 10 + (a.getVardas()[i] - '0');
                i++;
            }
            while (j < b.getVardas().size() && isdigit(b.getVardas()[j])) {
                numB = numB * 10 + (b.getVardas()[j] - '0');
                j++;
            }
            if (numA != numB)
                return numA < numB;
        }
        else {
            if (a.getVardas()[i] != b.getVardas()[j])
                return a.getVardas()[i] < b.getVardas()[j];
            i++;
            j++;
        }
    }
    return a.getVardas().size() < b.getVardas().size();

}
bool rikiavimaspav(const zmogus& a, const zmogus& b) {
    size_t i = 0, j = 0;
    while (i < a.getPavarde().size() && j < b.getPavarde().size()) {
        if (isdigit(a.getPavarde()[i]) && isdigit(b.getPavarde()[j])) {
            int numA = 0, numB = 0;
            while (i < a.getPavarde().size() && isdigit(a.getPavarde()[i])) {
                numA = numA * 10 + (a.getPavarde()[i] - '0');
                i++;
            }
            while (j < b.getPavarde().size() && isdigit(b.getPavarde()[j])) {
                numB = numB * 10 + (b.getPavarde()[j] - '0');
                j++;
            }
            if (numA != numB)
                return numA < numB;
        }
        else {
            if (a.getPavarde()[i] != b.getPavarde()[j])
                return a.getPavarde()[i] < b.getPavarde()[j];
            i++;
            j++;
        }
    }
    return a.getPavarde().size() < b.getPavarde().size();

}


/*
void zmogus::calculateMedian() {
    nd.sort();

    if (nd.size() % 2 == 0) {
        auto it1 = nd.begin();
        std::advance(it1, nd.size() / 2 - 1);
        int mid1 = *it1;

        auto it2 = nd.begin();
        std::advance(it2, nd.size() / 2);
        int mid2 = *it2;

        med = (mid1 + mid2) / 2.0;
    }
    else {
        auto it = nd.begin();
        std::advance(it, nd.size() / 2);
        med = *it;
    }
}

void zmogus::calculateAverage() {
    float nd_sum = 0;
    for (int k : nd) {
        nd_sum += k;
    }
    vid = (nd_sum / nd.size());
}

void zmogus::calculateGalutinis() {
    if (nd.size() > 0) {
        float sum = std::accumulate(nd.begin(), nd.end(), 0.0);
        vid = (sum / static_cast<float>(nd.size()));
        galutinis = vid * 0.4 + egz * 0.6;
    }
    else {
        vid = 0.0;
        galutinis = egz * 0.6;
    }
}*/
