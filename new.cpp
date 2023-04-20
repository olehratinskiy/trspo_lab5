#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

int main() {
    string vowels[] = { "а", "е", "є", "и", "і", "ї", "о", "у", "ю", "я" };
    const int num_vowels = sizeof(vowels) / sizeof(vowels[0]);

    bool found[num_vowels] = { false };

    string text = "Вивести всі голосні, які відсутні у даному тексті.";
    cout << "String: " << text << endl;

    for (size_t i = 0; i < text.length(); i++)
    {
        if ((text[i] & 0xC0) == 0xC0) {
            string utf8_char = text.substr(i, 2);
            if ((text[i] & 0xE0) == 0xE0) utf8_char = text.substr(i, 3);
            if ((text[i] & 0xF0) == 0xF0) utf8_char = text.substr(i, 4);
            i += utf8_char.length() - 1;

            for (int j = 0; j < num_vowels; j++) {
                if (vowels[j] == utf8_char) {
                    found[j] = true;
                    break;
                }
            }
        }
    }

    cout << "Not found vowel letters: ";
    for (int i = 0; i < num_vowels; i++) {
        if (!found[i]) cout << vowels[i] << " ";
    }
    cout << endl;

    return 0;
}
