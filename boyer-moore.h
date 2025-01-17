#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h> // exist
#include <unistd.h>

// #include <time.h>

#define ALPHABET_LEN 256
#define max(a, b) ((a < b) ? b : a)


void make_delta1(ptrdiff_t *delta1, uint8_t *pat, size_t patlen) {
    for (int i=0; i < ALPHABET_LEN; i++) {
        delta1[i] = patlen;
    }
    for (int i=0; i < patlen; i++) {
        delta1[pat[i]] = patlen-1 - i;
    }
}


bool is_prefix(uint8_t *word, size_t wordlen, ptrdiff_t pos) {
    int suffixlen = wordlen - pos;
    // could also use the strncmp() library function here
    // return ! strncmp(word, &word[pos], suffixlen);
    for (int i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos+i]) {
            return false;
        }
    }
    return true;
}


size_t suffix_length(uint8_t *word, size_t wordlen, ptrdiff_t pos) {
    size_t i;
    // increment suffix length i to the first mismatch or beginning
    // of the word
    for (i = 0; (word[pos-i] == word[wordlen-1-i]) && (i <= pos); i++);
    return i;
}


void make_delta2(ptrdiff_t *delta2, uint8_t *pat, size_t patlen) {
    ssize_t p;
    size_t last_prefix_index = 1;

    // first loop
    for (p=patlen-1; p>=0; p--) {
        if (is_prefix(pat, patlen, p+1)) {
            last_prefix_index = p+1;
        }
        delta2[p] = last_prefix_index + (patlen-1 - p);
    }

    // second loop
    for (p=0; p < patlen-1; p++) {
        size_t slen = suffix_length(pat, patlen, p);
        if (pat[p - slen] != pat[patlen-1 - slen]) {
            delta2[patlen-1 - slen] = patlen-1 - p + slen;
        }
    }
}


uint8_t* boyer_moore (uint8_t *string, size_t stringlen, uint8_t *pat, size_t patlen) {
    ptrdiff_t delta1[ALPHABET_LEN];
    ptrdiff_t delta2[patlen]; // C99 VLA
    make_delta1(delta1, pat, patlen);
    make_delta2(delta2, pat, patlen);

    // The empty pattern must be considered specially
    if (patlen == 0) {
        return string;
    }

    size_t i = patlen - 1;        // str-idx
    while (i < stringlen) {
        ptrdiff_t j = patlen - 1; // pat-idx
        while (j >= 0 && (string[i] == pat[j])) {
            --i;
            --j;
        }
        if (j < 0) {
            return &string[i+1];
        }

        ptrdiff_t shift = max(delta1[string[i]], delta2[j]);
        i += shift;
    }
    return NULL;
}


// int main() {
//     uint8_t string[] = "asdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw++++test++++3yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9basdfa vehkfvgj lekh4trke gklaerhjlkjker jhgelkjrth aerdfg reagtrgaergaergaerg34te5yw4232343yiow45uyp8349u8 paetoi3u[adl______f34mt9b";
//     uint8_t pattern[] = "test";

//     clock_t start, end;
//     double time;

//     start = clock();

//     uint8_t *result = boyer_moore(string, strlen((char *)string), pattern, strlen((char *)pattern));

//     if (result != NULL) {
//         printf("Pattern found at index: %ld\n", result - string);
//     } else {
//         printf("Pattern not found in the string.\n");
//     }

//     end = clock();
//     time = ((double) (end - start)) / CLOCKS_PER_SEC;
//     printf("boyer time : %f \n", time);


//     start = clock();
//     // Find first occurrence of s2 in s1
//     char* p = strstr(string, pattern);

//     // Prints the result
//     if (p) {
//         printf("String found\n");
//     }
//     else
//         printf("String not found\n");

//     end = clock();
//     time = ((double) (end - start)) / CLOCKS_PER_SEC;
//     printf("strstr time : %f \n", time);


//     return 0;
// }
