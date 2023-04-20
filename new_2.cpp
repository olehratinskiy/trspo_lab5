#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

extern "C" __global__ void AnalyzeLetters(bool *d_found, char **d_vowels, char *d_text, int Size) {
    int i = blockIdx.x*blockDim.x+threadIdx.x;
    int k;
    d_c[i]=0;
    for (k=0;k<Size;k++)
    {
        d_c[i]+=d_a[i*Size+k]*d_b[k];
    }
}

int main() {
    if(!InitCUDA()) { return 0; }
    int Size = 1000;

    string vowels[] = { "а", "е", "є", "и", "і", "ї", "о", "у", "ю", "я" };
    const int num_vowels = sizeof(vowels) / sizeof(vowels[0]);

    bool found[num_vowels] = { false };

    string text = "Вивести всі голосні, які відсутні у даному тексті.";
    cout << "String: " << text << endl;

    // DECLARE SPACE IN VIDEO MEMORY
    bool *d_found;
    char **d_vowels;
    char *d_text;
    cudaMalloc((void **)&d_found, sizeof(bool) * num_vowels);
    cudaMalloc((void**)&d_vowels, sizeof(char*) * 10);
    cudaMalloc((void**)&d_text, sizeof(char) * (strlen(text) + 1));

    for (int i = 0; i < 10; i++) {
        cudaMalloc((void**)&dev_vowels[i], sizeof(char) * (strlen(vowels[i]) + 1));
    }

    // COPY ENTRY
    CUDA_SAFE_CALL(cudaMemcpy(d_found, found, sizeof(bool) * num_vowels, cudaMemcpyHostToDevice));

    for (int i = 0; i < num_vowels; i++) {
        CUDA_SAFE_CALL(cudaMemcpy(d_vowels[i], vowels[i].c_str(), sizeof(char) * (strlen(vowels[i].c_str()) + 1), cudaMemcpyHostToDevice));
    }

    CUDA_SAFE_CALL(cudaMemcpy(d_text, text.c_str(), sizeof(char) * (strlen(text.c_str()) + 1), cudaMemcpyHostToDevice));

    // DEFINE BLOCK AND ITS SIZE
    dim3 threadsPerBlock(256);
    dim3 numBlocks((Size + 255) / 256);

    // CALL CUDA FUNC
    AnalizeLetters <<< numBlocks, threadsPerBlock >>> (d_found, d_vowels, d_text,Size);

    // COPY RESULTS
    CUDA_SAFE_CALL(cudaMemcpy(found, d_found, sizeof(bool) * num_vowels, cudaMemcpyHostToDevice));



//    for (size_t i = 0; i < text.length(); i++)
//    {
//        if ((text[i] & 0xC0) == 0xC0) {
//            string utf8_char = text.substr(i, 2);
//            if ((text[i] & 0xE0) == 0xE0) utf8_char = text.substr(i, 3);
//            if ((text[i] & 0xF0) == 0xF0) utf8_char = text.substr(i, 4);
//            i += utf8_char.length() - 1;
//
//            for (int j = 0; j < num_vowels; j++) {
//                if (vowels[j] == utf8_char) {
//                    found[j] = true;
//                    break;
//                }
//            }
//        }
//    }

    cout << "Not found vowel letters: ";
    for (int i = 0; i < num_vowels; i++) {
        if (!found[i]) cout << vowels[i] << " ";
    }
    cout << endl;

    return 0;
}
