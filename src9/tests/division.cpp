// tests NDS division IO registers
#include "../inc.h"

struct DIVtest
{
    u32 numLo, numHi, denomLo, denomHi, resLo, resHi, remLo, remHi;
    bool divby0errorbit;
};

// very unstructured test cases
const DIVtest div32_32_tests[] = {
    {67,    0,     10,      0,    6,    0,    7,    0,     false}, //    @   67 , 10 ->  6,   7
    {31463,    0,    456,      0,   68,    0,  455,    0,     false},
    {31464,    0,    456,      0,   69,    0,    0,    0,    false},
    {31465,    0,    456,      0,   69,    0,    1,    0,     false},

    {       -140,   44,     18,      1,   -7,   -1,  -14,   -1,     false},  // -140 , 18 -> -7, -14  (-140/18 = -7.77** -> -7, -140 - 18*(-7) = -14)
    {       -809,  123,     18,    188,  -44,   -1,  -17,   -1,     false},
    {       -810, -123,     18,     11,  -45,   -1,    0,    0,     false},
    {       -811,    0,     18,    -51,  -45,   -1,   -1,   -1,     false},

    {        817,   -5,    -10,     -5,  -81,   -1,    7,    0,     false},
    {        810,   -5,    -10,     -5,  -81,   -1,    0,    0,     false},
    {        811,   -6,    -10,    531,  -81,   -1,    1,    0,     false},
    {        809,    5,    -10,    252,  -80,   -1,    9,    0,     false},

    {        -54,  244,    -10,    428,    5,    0,   -4,   -1,     false},
    {        -59,  244,    -10,    428,    5,    0,   -9,   -1,     false},
    {        -60,  244,    -10,    428,    6,    0,    0,    0,     false},
    {        -61,  244,    -10,    428,    6,    0,   -1,   -1,     false},

    {        188,    0,      1,      0,  188,    0,    0,    0,     false},
    {       -123,    0,      1,      0, -123,   -1,    0,    0,     false},
    {        614,    0,     -1,      0, -614,   -1,    0,    0,     false},
    {       -555,    0,     -1,      0,  555,    0,    0,    0,     false},

    { 0x80000000,    0,     -1,      0,  0x80000000, 0, 0, 0, false}, // overflow (-MAX/-1 -> -MAX instead of MAX, not sign extended)
    {        123,    0,      0,    555,   -1,    0,  123,    0,     false}, // div by 0: res also sign extended incorrectly
    {       -123,    0,      0,    555,    1,   -1, -123,   -1,     false}, // '
    {          0,    0,      0,    555,   -1,    0,    0,    0,     false},
    {        456,    0,      0,      0,   -1,    0,  456,    0,     true}, // 64-bit denom = 0: error bit
    {       -456,    0,      0,      0,    1,   -1, -456,   -1,     true}, // '
    {          0,    0,      0,      0,   -1,    0,    0,    0,     true} // '
};

const DIVtest div64_32_tests[] = {
    {  3433982045 , 1495661067 , 236509398 , 2881185925 , 1391126937 , 6 , 48106103 , 0 , false},
    {  2154647668 , 3521289250 , 425590710 , 3127514378 , 782146911 , 4294967294 , 3932474858 , 4294967295 , false},
    {  1922168281 , 1858073932 , 120466744 , 3903510642 , 1820883341 , 15 , 84121089 , 0 , false},
    {  2205423745 , 1140703687 , 1341204594 , 3681089015 , 3652899083 , 0 , 759056795 , 0 , false},
    {  3178427586 , 166340665 , 2977347259 , 3917866833 , 3752756569 , 4294967295 , 1176782527 , 0 , false},
    {  3642771976 , 2744890040 , 3689984914 , 1961992904 , 2414569531 , 2 , 4205680482 , 4294967295 , false},
    {  2953990472 , 228525878 , 595492953 , 3003833644 , 1648233065 , 0 , 144585415 , 0 , false},
    {  4122067872 , 2331242851 , 3841153440 , 3927033522 , 1405134676 , 4 , 4267768608 , 4294967295 , false},
    {  1298567073 , 541382732 , 442302378 , 2613613729 , 962117552 , 1 , 239531201 , 0 , false},
    {  409708380 , 978081371 , 2832999784 , 2915952732 , 1421560420 , 4294967295 , 212138684 , 0 , false},
    {  1930979202 , 1105853070 , 3119598546 , 578578769 , 254020341 , 4294967295 , 447521672 , 0 , false},
    {  16225604 , 2021597761 , 1519863446 , 54293766 , 1417846109 , 1 , 1099756230 , 0 , false},
    {  3966856588 , 3456041294 , 1659113464 , 4073113242 , 2123229307 , 4294967295 , 3737977188 , 4294967295 , false},
    {  2503201751 , 3761051594 , 4111858164 , 1880989876 , 3933473996 , 2 , 4117312871 , 4294967295 , false},
    {  3497809569 , 2352592527 , 1310362933 , 265449947 , 2223426585 , 4294967294 , 3382726772 , 4294967295 , false},
    {  179874632 , 3182602627 , 302931460 , 626862639 , 1408744342 , 4294967292 , 4123306224 , 4294967295 , false},

    {    0, 0x80000000,     -1,      0,    0, 0x80000000,    0,    0,     false}, // overflow (-MAX/-1 -> -MAX instead of MAX)
    {  123,          6,      0,  0x555,   -1,         -1,  123,    6,     false}, // div by 0: res sign extended correctly
    { -123,         -8,      0,  0x555,    1,          0, -123,   -8,     false}, // '
    {    0,          0,      0,  0x555,   -1,         -1,    0,    0,     false},
    {  456,          6,      0,      0,   -1,         -1,  456,    6,     true}, // 64-bit denom = 0: error bit
    { -456,         -8,      0,      0,    1,          0, -456,   -8,     true}, // '
    {    0,          0,      0,      0,   -1,         -1,    0,    0,     true}  // '
};

const DIVtest div64_64_tests[] = {
    {  1811435369 , 2899071174 , 378838206 , 1972369566 , 0 , 0 , 1811435369 , 2899071174 , false},
    {  3561088202 , 2795612862 , 2670550186 , 1972501369 , 0 , 0 , 3561088202 , 2795612862 , false},
    {  3544901067 , 876673192 , 95990780 , 0 , 570756327 , 9 , 88386919 , 0 , false},
    {  3297557998 , 60728899 , 1831216766 , 1559128996 , 0 , 0 , 3297557998 , 60728899 , false},
    {  3830564669 , 1873812420 , 3924560227 , 2604699052 , 4294967295 , 4294967295 , 3460157600 , 183544177 , false},
    {  1045560126 , 865902829 , 4206723589 , 1021554562 , 0 , 0 , 1045560126 , 865902829 , false},
    {  335827131 , 894068399 , 1504592406 , 26560618 , 33 , 0 , 2223885285 , 17567993 , false},
    {  2206612439 , 2104078770 , 1144569821 , 1841111763 , 1 , 0 , 1062042618 , 262967007 , false},
    {  2672951752 , 2052516775 , 641911416 , 1967816049 , 1 , 0 , 2031040336 , 84700726 , false},
    {  402058462 , 2425767789 , 3651716241 , 1878602569 , 0 , 0 , 402058462 , 2425767789 , false},
    {  2018647722 , 3185867357 , 1924563228 , 677692115 , 4294967295 , 4294967295 , 3943210950 , 3863559472 , false},
    {  737594227 , 1343895311 , 3045244374 , 96026225 , 13 , 0 , 4099090325 , 95554376 , false},
    {  2732658177 , 3839563371 , 3060215123 , 512049618 , 0 , 0 , 2732658177 , 3839563371 , false},
    {  1699776401 , 961636287 , 516156236 , 0 , 3706866484 , 1 , 29194273 , 0 , false},
    {  1464896284 , 2465942470 , 1950357861 , 0 , 267192719 , 4294967295 , 3105672881 , 4294967295 , false},
    {  1884637257 , 3069229139 , 2129770961 , 3165750172 , 1 , 0 , 4049833592 , 4198446262 , false},

    {    0, 0x80000000,     -1,     -1,    0, 0x80000000,    0,    0,     false}, // overflow (-MAX/-1 -> -MAX instead of MAX)
    {  456,          6,      0,      0,   -1,         -1,  456,    6,     true}, // 64-bit denom = 0: error bit
    { -456,         -8,      0,      0,    1,          0, -456,   -8,     true}, // '
    {    0,          0,      0,      0,   -1,         -1,    0,    0,     true} // '
};

void check_div(const DIVtest array[], int test_count)
{
    for(int i = 0; i < test_count; i++)
    {
        REG_DIVNUMERLO = array[i].numLo;
        REG_DIVNUMERHI = array[i].numHi;
        REG_DIVDENOMLO = array[i].denomLo;
        REG_DIVDENOMHI = array[i].denomHi;

        wait_bit_value(i, DIVCNT, 15, false); // wait for bit 15 = busy bit to be turned off

        u32 fail = 0;
        fail |= (array[i].resLo != REG_DIVRESLO);
        fail |= (array[i].resHi != REG_DIVRESHI) << 1;
        fail |= (array[i].remLo != REG_DIVREMLO) << 2;
        fail |= (array[i].remHi != REG_DIVREMHI) << 3;
        fail |= (array[i].divby0errorbit != ((REG_DIVCNT >> 14) & 1)) << 4;
        if (fail) fail_test(i);
    }
}

void run_tests_ds_maths_div_32_32()
{
    REG_DIVCNT = 0; // 32/32
    check_div(div32_32_tests, 27);
}

void run_tests_ds_maths_div_64_32()
{
    REG_DIVCNT = 1; // 64/32
    check_div(div64_32_tests, 23);
}

void run_tests_ds_maths_div_64_64()
{   
    REG_DIVCNT = 2; // 64/64
    check_div(div64_64_tests, 20);
}
