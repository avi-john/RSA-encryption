#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>

using namespace std;

bool isPrime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}
uint64_t generateRandomPrime(uint64_t lower, uint64_t upper) {
    while (true) {
        uint64_t num = lower + rand() % (upper - lower);
        if (isPrime(num)) return num;
    }
}
int gcd(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}
uint64_t modExp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

void savePublicKey(uint64_t n, uint64_t e) {
    ofstream publicKeyFile("public_key.txt");
    if (!publicKeyFile) {
        cerr << "Error opening public key file!" << endl;
        return;
    }
    publicKeyFile << "Public Key: (n = " << n << ", e = " << e << ")" << endl;
    publicKeyFile.close();
}

void savePrivateKey(uint64_t n, uint64_t d) {
    ofstream privateKeyFile("private_key.txt");
    if (!privateKeyFile) {
        cerr << "Error opening private key file!" << endl;
        return;
    }
    privateKeyFile << "Private Key: (n = " << n << ", d = " << d << ")" << endl;
    privateKeyFile.close();
}

void encrypt(const string& inputFile, const string& outputFile, uint64_t n, uint64_t e) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error opening input file!" << endl;
        return;
    }

    string message((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();
    vector<uint64_t> encryptedMessage;
    for (char c : message) {
        uint64_t encryptedChar = modExp((uint64_t)c, e, n);
        encryptedMessage.push_back(encryptedChar);
    }

    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "\nError opening output file!" ;
        return;
    }

    for (size_t i = 0; i < encryptedMessage.size(); i++) {
        outFile << encryptedMessage[i];
        if (i != encryptedMessage.size() - 1) outFile << " ";
    }
cout<<"\nEncryption has been completed ";
    outFile.close();
}
void decrypt(const string& inputFile, const string& outputFile, uint64_t n, uint64_t d) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "\nError opening input file!" ;
        return;
    }
    vector<uint64_t> encryptedMessage;
    uint64_t num;
    while (inFile >> num) {
        encryptedMessage.push_back(num);
    }
    inFile.close();

    stringstream decryptedMessage;
    for (uint64_t c : encryptedMessage) {
        char decryptedChar = (char)modExp(c, d, n);
        decryptedMessage << decryptedChar;
    }

    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "\nError opening output file!" ;
        return;
    }

    outFile << decryptedMessage.str();
    outFile.close();
    cout << "\nDecryption has been completed" ;
}
int main() {
    srand(time(0));
    uint64_t p = generateRandomPrime(4096, 65535);
    uint64_t q = generateRandomPrime(4096, 65535);
    while (p == q) {
        q = generateRandomPrime(4096, 65535);
    }
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);
    uint64_t e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1) break;
        e++;
    }

    uint64_t d = 1;
    while ((d * e) % phi != 1) {
        d++;
    }
    cout << "Public key: (n = " << n << ", e = " << e << ")" ;
    cout << "\nPrivate key: (n = " << n << ", d = " << d << ")" ;
    savePublicKey(n, e);
    savePrivateKey(n, d);
    encrypt("input.txt", "encrypted.txt", n, e);
    decrypt("encrypted.txt", "decrypted.txt", n, d);
    return 0;
}
