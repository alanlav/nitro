/* =========================================================================
 * This file is part of NITRO
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * NITRO is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include <import/nitf.h>
#include "Test.h"

TEST_CASE( testField)
{
    char fhdr_str[NITF_FHDR_SZ + 1];
    char ubin_str[10];
    char hl_str[NITF_HL_SZ + 1];
    double doubleData;
    float floatData;
    nitf_Field *fhdr = NULL, *ubin = NULL, *hl = NULL, *realField = NULL;

    nitf_Error error;
    int32_t int32 = 16801;
    fhdr = nitf_Field_construct(NITF_FHDR_SZ, NITF_BCS_A, &error);
    ubin = nitf_Field_construct(4, NITF_BINARY, &error);
    hl = nitf_Field_construct(NITF_HL_SZ, NITF_BCS_N, &error);
    realField = nitf_Field_construct(NITF_HL_SZ, NITF_BCS_N, &error);

    TEST_ASSERT(fhdr);
    TEST_ASSERT(ubin);
    TEST_ASSERT(hl);
    TEST_ASSERT(realField);

    printf("%d\n", int32);
    nitf_Field_setRawData(fhdr, "NIT", 3, &error);
    nitf_Field_setRawData(ubin, &int32, 4, &error);
    nitf_Field_setRawData(hl, "000142", 6, &error);
    nitf_Field_setRawData(realField, "142.56", 6, &error);

    TEST_ASSERT(nitf_Field_get(fhdr, fhdr_str, NITF_CONV_STRING, NITF_FHDR_SZ
            + 1, &error));
    printf("FHDR: [%s]\n", fhdr_str);
    TEST_ASSERT_EQ_STR(fhdr_str, "NIT ");

    TEST_ASSERT(nitf_Field_get(hl, hl_str, NITF_CONV_STRING, NITF_HL_SZ + 1,
                               &error));
    printf("HL (str): [%s]\n", hl_str);
    TEST_ASSERT_EQ_STR(hl_str, "000142");

    TEST_ASSERT(nitf_Field_get(hl, &int32, NITF_CONV_INT, 4, &error));
    printf("HL: [%d]\n", int32);
    TEST_ASSERT_EQ_INT(int32, 142);

    TEST_ASSERT(nitf_Field_get(realField, hl_str, NITF_CONV_STRING, NITF_HL_SZ
            + 1, &error));
    printf("REAL (str): [%s]\n", hl_str);
    TEST_ASSERT_EQ_STR(hl_str, "142.56");

    TEST_ASSERT(nitf_Field_get(realField, &doubleData, NITF_CONV_REAL,
                               sizeof(double), &error));
    printf("REAL (double): [%f]\n", doubleData);
    TEST_ASSERT_EQ_FLOAT((float)doubleData, 142.56f);

    TEST_ASSERT(nitf_Field_get(realField, &floatData, NITF_CONV_REAL,
                               sizeof(float), &error));
    printf("REAL (float): [%f]\n", floatData);
    TEST_ASSERT_EQ_FLOAT(floatData, 142.56f);

    TEST_ASSERT(nitf_Field_get(ubin, &int32, NITF_CONV_INT, 4, &error));
    printf("UBIN: [%d]\n", int32);
    TEST_ASSERT_EQ_INT(int32, 16801);

    TEST_ASSERT(nitf_Field_get(ubin, ubin_str, NITF_CONV_STRING, 10, &error));
    printf("UBIN (str): [%s]\n", ubin_str);
    TEST_ASSERT_EQ_STR(ubin_str, "16801");

    nitf_Field_destruct(&fhdr);
    nitf_Field_destruct(&ubin);
    nitf_Field_destruct(&hl);
    nitf_Field_destruct(&realField);

    TEST_ASSERT_NULL(fhdr);
    TEST_ASSERT_NULL(ubin);
    TEST_ASSERT_NULL(hl);
    TEST_ASSERT_NULL(realField);
}

TEST_CASE(setReal)
{
    const size_t length = 14;
    nitf_Error errorObj;  /* Error object */
    nitf_Error* error = &errorObj;    /* Pointer to error object */

    /* Note:

        NITF real fields are usually NITF_BCS_A because of the decimal place
        or exponent
     */
    nitf_Field* field = nitf_Field_construct(length, NITF_BCS_A, error);
    TEST_ASSERT(field);

    #define BUF_SIZE 256
    char buffer[BUF_SIZE];

    /*  Test fields */
    NITF_BOOL ret = nitf_Field_setReal(field, "f", 1, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "+12.3456000000");

    ret = nitf_Field_setReal(field, "e", 1, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "+1.2345600e+01");

    ret = nitf_Field_setReal(field, "E", 1, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "+1.2345600E+01");


    ret = nitf_Field_setReal(field, "f", 0, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "12.34560000000");

    ret = nitf_Field_setReal(field, "e", 0, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "1.23456000e+01");

    ret = nitf_Field_setReal(field, "E", 0, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "1.23456000E+01");


    field = nitf_Field_construct(8, NITF_BCS_A, error);
    TEST_ASSERT(field);
    ret = nitf_Field_setReal(field, "f", 1, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "+12.3456");
    ret = nitf_Field_setReal(field, "f", 0, 12.3456, error);
    TEST_ASSERT(ret);
    nitf_Field_snprint(buffer, BUF_SIZE, field);
    TEST_ASSERT_EQ_STR(buffer, "12.34560");

}

TEST_CASE(nitfBinaryConversions)
{
    // Declare Variables
    nitf_Error error;
    NITF_BOOL ret = FALSE;

    // Construct & Test Fields (Case 1)
    nitf_Field* bin1 = NULL, * ubin1 = NULL;
    bin1 = nitf_Field_construct(1, NITF_BINARY, &error);
    ubin1 = nitf_Field_construct(1, NITF_BINARY, &error);

    TEST_ASSERT(bin1);
    TEST_ASSERT(ubin1);

    // Set Field Data (Case 1)
    int8_t int8 = 0x7F;
    uint8_t uint8 = 0b1111111;
    nitf_Field_setRawData(bin1, &int8, 1, &error);
    nitf_Field_setRawData(ubin1, &uint8, 1, &error);

    // toInt (Case 1)
    int8_t out = 0;
    ret = nitf_Field_get(bin1, &out, NITF_CONV_INT, 1, &error);
    TEST_ASSERT(ret);
    printf("toInt (1): [%d]\n", out);
    TEST_ASSERT_EQ_INT(out, 127);

    // toUint (Case 1)
    uint8_t uout = 0;
    ret = nitf_Field_get(ubin1, &uout, NITF_CONV_UINT, 1, &error);
    TEST_ASSERT(ret);
    printf("toUint (1): [%d]\n", uout);
    TEST_ASSERT_EQ_INT(uout, 127);

    // intToString (Case 1)
    char bin1_str[6];
    ret = nitf_Field_get(bin1, bin1_str, NITF_CONV_STRING, 6, &error);
    TEST_ASSERT(ret);
    printf("fromIntToString (1): [%s]\n", bin1_str);
    TEST_ASSERT_EQ_STR(bin1_str, "127");

    // Construct & Test Fields (Case 3)
    nitf_Field* bin3 = NULL, * ubin3 = NULL;
    bin3 = nitf_Field_construct(3, NITF_BINARY, &error);
    ubin3 = nitf_Field_construct(3, NITF_BINARY, &error);

    TEST_ASSERT(bin3);
    TEST_ASSERT(ubin3);

    // Set Field Data (Case 3)
    int32_t int32 = 0b11111111111111111111111;
    uint32_t uint32 = 0x7FFFFF;
    nitf_Field_setRawData(bin3, &int32, 3, &error);
    nitf_Field_setRawData(ubin3, &uint32, 3, &error);

    // toInt (Case 3)
    int32_t out3 = 0;
    ret = nitf_Field_get(bin3, &out3, NITF_CONV_INT, 3, &error);
    TEST_ASSERT(ret);
    printf("toInt (3): [%d]\n", out3);
    TEST_ASSERT_EQ_INT(out3, 8388607);

    // toUint (Case 3)
    uint32_t uout3 = 0;
    ret = nitf_Field_get(ubin3, &uout3, NITF_CONV_UINT, 3, &error);
    TEST_ASSERT(ret);
    printf("toUint (3): [%d]\n", uout3);
    TEST_ASSERT_EQ_INT(uout3, 8388607);

    // intToString (Case 3)
    char bin3_str[14];
    ret = nitf_Field_get(bin3, bin3_str, NITF_CONV_STRING, 14, &error);
    TEST_ASSERT(ret);
    printf("fromIntToString (3): [%s]\n", bin3_str);
    TEST_ASSERT_EQ_STR(bin3_str, "8388607");

    // Construct & Test String Field (Case 3)
    nitf_Field* sbin3 = NULL;
    sbin3 = nitf_Field_construct(7, NITF_BCS_A, &error);

    TEST_ASSERT(sbin3);

    // Set Field Data (Case 3)
    nitf_Field_setRawData(sbin3, "8388607", 7, &error);

    // stringToInt (Case 3)
    int32_t sout3 = 0;
    ret = nitf_Field_get(sbin3, &sout3, NITF_CONV_INT, 3, &error);
    TEST_ASSERT(ret);
    printf("fromStringToInt (3): [%d]\n", sout3);
    TEST_ASSERT_EQ_INT(sout3, 8388607);

    // stringToUint (Case 3)
    uint32_t suout3 = 0;
    ret = nitf_Field_get(sbin3, &suout3, NITF_CONV_INT, 3, &error);
    TEST_ASSERT(ret);
    printf("fromStringToUint (3): [%d]\n", suout3);
    TEST_ASSERT_EQ_INT(suout3, 8388607);
}

TEST_MAIN(
    (void)argc;
    (void)argv;
    CHECK(testField);
    CHECK(setReal);
    CHECK(nitfBinaryConversions);
    )