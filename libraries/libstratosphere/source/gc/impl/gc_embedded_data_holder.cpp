/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>

namespace ams::gc::impl {

    namespace {

        constexpr const u8 LibraryEmbeddedCa1Modulus[GcCrypto::GcRsaKeyLength] = {
            0xAF, 0xB6, 0xA0, 0x1F, 0x7F, 0x8C, 0xEC, 0xD6, 0x62, 0xC9, 0xF9, 0x83, 0x69, 0x4F, 0x0E, 0x8E,
            0xC3, 0x71, 0x70, 0x60, 0x63, 0xEB, 0x90, 0x2F, 0x1E, 0x3B, 0xA2, 0xCD, 0xD5, 0x7A, 0xAA, 0x45,
            0x27, 0x61, 0xF9, 0x10, 0xC0, 0x0C, 0x72, 0xE7, 0xBD, 0x70, 0x2E, 0x32, 0xA0, 0xD0, 0x8A, 0x29,
            0x43, 0x19, 0x12, 0x8C, 0x8D, 0x10, 0xE4, 0x04, 0xE0, 0x13, 0x7F, 0x26, 0x02, 0x1B, 0xFD, 0x44,
            0xAF, 0x70, 0xD7, 0xBF, 0xDF, 0x97, 0xD2, 0x34, 0xFD, 0xBB, 0x51, 0x8D, 0x7C, 0x04, 0x9D, 0x30,
            0xFF, 0xB1, 0xB4, 0xD3, 0xEF, 0x2C, 0xEE, 0xAD, 0x4C, 0x4A, 0x26, 0x94, 0x15, 0x13, 0xA9, 0xDA,
            0xF4, 0xA4, 0x22, 0xE7, 0x11, 0x8A, 0xE4, 0xB0, 0xE9, 0x66, 0x23, 0xA7, 0xED, 0x7D, 0x73, 0x8B,
            0x32, 0xE5, 0xE1, 0x19, 0x34, 0x15, 0x06, 0x5D, 0xA6, 0xCD, 0x80, 0xA5, 0xC0, 0xD0, 0xCE, 0x7C,
            0x3E, 0x3D, 0x1F, 0x2B, 0x65, 0x26, 0xBE, 0xAE, 0x55, 0xC7, 0x03, 0xCF, 0x4A, 0xD3, 0xDA, 0x54,
            0x13, 0x1F, 0x20, 0x05, 0xE9, 0x9B, 0x3F, 0xDE, 0x73, 0xD3, 0xA0, 0xFF, 0xA0, 0x7E, 0xA9, 0x6A,
            0xBC, 0xF8, 0x6C, 0xF1, 0x3C, 0x72, 0x4E, 0x6F, 0xA1, 0x3C, 0x20, 0xCD, 0x3A, 0x1A, 0x65, 0xE0,
            0xF0, 0xF8, 0x84, 0xEB, 0x6B, 0x38, 0x49, 0xB6, 0xF2, 0x5B, 0x81, 0x16, 0x8B, 0x1A, 0xE0, 0x4F,
            0x18, 0x88, 0xF1, 0xAD, 0x66, 0xA9, 0xC8, 0xE0, 0x9D, 0xD0, 0x9B, 0xFA, 0xAD, 0xBE, 0xC9, 0x5C,
            0xE5, 0x54, 0x2A, 0xF4, 0x59, 0xE2, 0xFA, 0xD4, 0xC9, 0x58, 0x1C, 0x83, 0xD9, 0x23, 0x77, 0xDC,
            0x78, 0xBE, 0xCA, 0x7B, 0xC1, 0x69, 0xF3, 0x9C, 0xFE, 0xF7, 0xB9, 0x9E, 0xD6, 0x44, 0x70, 0x1C,
            0x8B, 0x08, 0xB0, 0x44, 0xE5, 0x63, 0xFB, 0xB8, 0x45, 0x40, 0xDA, 0xA2, 0x3C, 0xB9, 0xFB, 0x75,
        };

        constexpr const u8 LibraryEmbeddedCa9Modulus[GcCrypto::GcRsaKeyLength] = {
            0xCD, 0xF3, 0x2C, 0xB0, 0xF5, 0x14, 0x78, 0x34, 0xE5, 0x02, 0xD0, 0x29, 0x6A, 0xA5, 0xFD, 0x97,
            0x6A, 0xE0, 0xB0, 0xBB, 0xB0, 0x3B, 0x1A, 0x80, 0xB7, 0xD7, 0x58, 0x92, 0x79, 0x84, 0xC0, 0x36,
            0xB1, 0x55, 0x23, 0xD8, 0xA5, 0x60, 0x91, 0x26, 0x48, 0x1A, 0x80, 0x4A, 0xEA, 0x00, 0x98, 0x2A,
            0xEC, 0x52, 0x17, 0x72, 0x92, 0x4D, 0xF5, 0x42, 0xA7, 0x8A, 0x6F, 0x7F, 0xD2, 0x48, 0x51, 0x8E,
            0xDF, 0xCB, 0xBF, 0x77, 0xF6, 0x18, 0xBD, 0xE5, 0x00, 0xD9, 0x70, 0x8C, 0xEF, 0x57, 0xB2, 0x96,
            0xD0, 0x36, 0x83, 0x88, 0x9C, 0xC5, 0xFB, 0xA0, 0x33, 0x81, 0xA2, 0x12, 0x23, 0xC6, 0xC7, 0x86,
            0x0A, 0x98, 0x57, 0x4D, 0x2E, 0xB5, 0xAE, 0x64, 0xE4, 0x6F, 0xC2, 0xC5, 0xAC, 0x6A, 0x1D, 0xDB,
            0xA5, 0xAF, 0x12, 0x22, 0xAB, 0x1F, 0x51, 0xC8, 0x0E, 0x0D, 0xC9, 0xF5, 0x03, 0xE8, 0xD2, 0xFC,
            0x84, 0x62, 0x26, 0x55, 0xA4, 0xC3, 0xE2, 0xA8, 0x98, 0x05, 0x67, 0x23, 0xFD, 0xA5, 0x46, 0x40,
            0x78, 0x51, 0x09, 0x3D, 0x91, 0x74, 0xD6, 0xD0, 0x54, 0x23, 0x0D, 0xA0, 0xFB, 0x07, 0xD0, 0xAA,
            0x9D, 0x50, 0x4E, 0x2B, 0x26, 0x9A, 0x14, 0xE5, 0x6C, 0x73, 0x66, 0x24, 0x18, 0xA1, 0x93, 0x9C,
            0x2A, 0x40, 0x40, 0x05, 0x6B, 0xF1, 0x45, 0xDF, 0x22, 0x8B, 0x40, 0x61, 0xA4, 0x11, 0x06, 0x03,
            0xA5, 0x53, 0x84, 0xC0, 0x12, 0xE1, 0x88, 0x9D, 0x55, 0x55, 0x07, 0x40, 0x88, 0x01, 0x8C, 0xAB,
            0xA2, 0xFD, 0xFD, 0x19, 0x48, 0x25, 0xAB, 0x59, 0x59, 0x28, 0x63, 0x68, 0x69, 0x1B, 0x99, 0x73,
            0x8D, 0xAB, 0x5A, 0xFA, 0x71, 0x60, 0x1B, 0x12, 0xE7, 0x99, 0x70, 0xF1, 0x99, 0x2A, 0x50, 0x18,
            0x8B, 0x6B, 0x61, 0x90, 0xE2, 0x7E, 0x8B, 0x90, 0xD4, 0xD5, 0xC0, 0xCB, 0x7C, 0x08, 0x06, 0xD9,
        };

        constexpr const u8 LibraryEmbeddedCaPublicExponent[GcCrypto::GcRsaPublicExponentLength] = {
            0x01, 0x00, 0x01
        };

        constexpr const u8 LibraryEmbeddedCa10Modulus[2][GcCrypto::GcRsaKeyLength] = {
            {
                0x98, 0xC7, 0x26, 0xB6, 0x0D, 0x0A, 0x50, 0xA7, 0x39, 0x21, 0x0A, 0xE3, 0x2F, 0xE4, 0x3E, 0x2E,
                0x5B, 0xA2, 0x86, 0x75, 0xAA, 0x5C, 0xEE, 0x34, 0xF1, 0xA3, 0x3A, 0x7E, 0xBD, 0x90, 0x4E, 0xF7,
                0x8D, 0xFA, 0x17, 0xAA, 0x6B, 0xC6, 0x36, 0x6D, 0x4C, 0x9A, 0x6D, 0x57, 0x2F, 0x80, 0xA2, 0xBC,
                0x38, 0x4D, 0xDA, 0x99, 0xA1, 0xD8, 0xC3, 0xE2, 0x99, 0x79, 0x36, 0x71, 0x90, 0x20, 0x25, 0x9D,
                0x4D, 0x11, 0xB8, 0x2E, 0x63, 0x6B, 0x5A, 0xFA, 0x1E, 0x9C, 0x04, 0xD1, 0xC5, 0xF0, 0x9C, 0xB1,
                0x0F, 0xB8, 0xC1, 0x7B, 0xBF, 0xE8, 0xB0, 0xD2, 0x2B, 0x47, 0x01, 0x22, 0x6B, 0x23, 0xC9, 0xD0,
                0xBC, 0xEB, 0x75, 0x6E, 0x41, 0x7D, 0x4C, 0x26, 0xA4, 0x73, 0x21, 0xB4, 0xF0, 0x14, 0xE5, 0xD9,
                0x8D, 0xB3, 0x64, 0xEE, 0xA8, 0xFA, 0x84, 0x1B, 0xB8, 0xB8, 0x7C, 0x88, 0x6B, 0xEF, 0xCC, 0x97,
                0x04, 0x04, 0x9A, 0x67, 0x2F, 0xDF, 0xEC, 0x0D, 0xB2, 0x5F, 0xB5, 0xB2, 0xBD, 0xB5, 0x4B, 0xDE,
                0x0E, 0x88, 0xA3, 0xBA, 0xD1, 0xB4, 0xE0, 0x91, 0x81, 0xA7, 0x84, 0xEB, 0x77, 0x85, 0x8B, 0xEF,
                0xA5, 0xE3, 0x27, 0xB2, 0xF2, 0x82, 0x2B, 0x29, 0xF1, 0x75, 0x2D, 0xCE, 0xCC, 0xAE, 0x9B, 0x8D,
                0xED, 0x5C, 0xF1, 0x8E, 0xDB, 0x9A, 0xD7, 0xAF, 0x42, 0x14, 0x52, 0xCD, 0xE3, 0xC5, 0xDD, 0xCE,
                0x08, 0x12, 0x17, 0xD0, 0x7F, 0x1A, 0xAA, 0x1F, 0x7D, 0xE0, 0x93, 0x54, 0xC8, 0xBC, 0x73, 0x8A,
                0xCB, 0xAD, 0x6E, 0x93, 0xE2, 0x19, 0x72, 0x6B, 0xD3, 0x45, 0xF8, 0x73, 0x3D, 0x2B, 0x6A, 0x55,
                0xD2, 0x3A, 0x8B, 0xB0, 0x8A, 0x42, 0xE3, 0x3D, 0xF1, 0x92, 0x23, 0x42, 0x2E, 0xBA, 0xCC, 0x9C,
                0x9A, 0xC1, 0xDD, 0x62, 0x86, 0x9C, 0x2E, 0xE1, 0x2D, 0x6F, 0x62, 0x67, 0x51, 0x08, 0x0E, 0xCF,
            },
            {
                0xC8, 0x65, 0x8D, 0x9D, 0x15, 0xF4, 0xCC, 0x35, 0x7D, 0x3C, 0x7B, 0xBF, 0xA3, 0x7D, 0xA9, 0xFE,
                0x93, 0xD9, 0x3A, 0x64, 0x7C, 0x12, 0x81, 0xB8, 0xA7, 0x6D, 0xE6, 0x76, 0xA5, 0x9F, 0x95, 0xB1,
                0x0B, 0xC5, 0x93, 0x9F, 0x48, 0xE9, 0x4F, 0x3D, 0xD1, 0x94, 0x0F, 0x78, 0x70, 0x5A, 0x2C, 0x82,
                0x6C, 0xE9, 0xB0, 0xA7, 0x6C, 0xEA, 0xB5, 0xC1, 0x20, 0xD0, 0x2A, 0x29, 0x42, 0xA6, 0x33, 0x70,
                0x75, 0x53, 0x3E, 0x88, 0x4A, 0xEF, 0x35, 0x0E, 0x79, 0xE4, 0xB0, 0x0F, 0x90, 0xA2, 0xAC, 0xF8,
                0x31, 0x02, 0xA3, 0x8E, 0x99, 0x7E, 0xF4, 0x72, 0x5A, 0x0B, 0xE8, 0x23, 0x4E, 0x87, 0xFB, 0x2F,
                0x22, 0x22, 0x57, 0xF6, 0xE1, 0x43, 0xFD, 0x11, 0xDA, 0x2D, 0xE6, 0x25, 0x96, 0x4C, 0x6B, 0x3B,
                0x54, 0x0C, 0x22, 0x8C, 0xB5, 0x82, 0xDB, 0x49, 0x5C, 0xB0, 0x36, 0x13, 0x31, 0x6F, 0x1A, 0xFF,
                0xA5, 0x1F, 0x70, 0x15, 0xAC, 0xDA, 0xF5, 0xD6, 0xE5, 0x71, 0x2F, 0x47, 0x43, 0xAB, 0x00, 0x03,
                0xCE, 0x9C, 0x70, 0xEB, 0x58, 0x6C, 0xE1, 0x3F, 0xC8, 0xD7, 0x43, 0xDA, 0x34, 0xDD, 0x23, 0x76,
                0xE3, 0x39, 0xB6, 0x8E, 0x5D, 0x63, 0xD6, 0xDD, 0x42, 0x5B, 0xB4, 0x58, 0xCF, 0x2D, 0x47, 0x61,
                0x2F, 0x3F, 0xC3, 0x20, 0xF5, 0xD6, 0xDB, 0xFD, 0x75, 0xCB, 0x06, 0xBC, 0x94, 0x4E, 0xE5, 0x3D,
                0xC8, 0x70, 0xC6, 0xCB, 0xB9, 0xE0, 0x9B, 0x0F, 0x32, 0xA4, 0xC3, 0xCA, 0x46, 0x8C, 0x44, 0x2D,
                0x2E, 0x71, 0xC8, 0xF0, 0x51, 0x17, 0x94, 0x5D, 0x40, 0xE2, 0x31, 0x9A, 0x24, 0x9F, 0x7C, 0xC5,
                0xDC, 0xB9, 0xB4, 0x43, 0x23, 0x70, 0xF7, 0x73, 0x0A, 0x5A, 0x6B, 0x8D, 0x9C, 0x76, 0xB1, 0x23,
                0x49, 0x35, 0x4E, 0x3E, 0x92, 0x22, 0xDF, 0xBB, 0x5E, 0xF4, 0x9E, 0x98, 0xCF, 0x51, 0xBE, 0xDF,
            },
        };

        constexpr const u8 LibraryEmbeddedCa10CertificateModulus[2][GcCrypto::GcRsaKeyLength] = {
            {
                0xAA, 0x9B, 0x6C, 0xE2, 0x50, 0xE5, 0xEC, 0x25, 0xEE, 0x2D, 0x21, 0x9C, 0xB7, 0x4F, 0xA2, 0x72,
                0x1E, 0x44, 0xB7, 0xFC, 0x65, 0x86, 0xAC, 0x81, 0xCC, 0x09, 0xDC, 0xAD, 0xB7, 0x68, 0x37, 0x52,
                0x72, 0x81, 0xD5, 0xBA, 0x72, 0x11, 0x41, 0x71, 0x98, 0x46, 0xA9, 0x47, 0xF6, 0x95, 0x9D, 0x9B,
                0x5E, 0xCA, 0x07, 0x5A, 0x57, 0xE0, 0xAB, 0x2E, 0xDB, 0xE5, 0xF3, 0x01, 0x3B, 0xBB, 0x2B, 0x2E,
                0x44, 0x31, 0xA3, 0x0B, 0x2F, 0x3A, 0x51, 0xC4, 0x6B, 0x64, 0xD8, 0xF1, 0x01, 0x2D, 0xE9, 0xE8,
                0x86, 0x30, 0xAC, 0xF8, 0x02, 0xA3, 0x5A, 0xBE, 0x60, 0xFB, 0x5C, 0x1C, 0x39, 0x7C, 0x8B, 0x4F,
                0xBF, 0xE2, 0xDF, 0x1E, 0xF2, 0x69, 0x4E, 0xA3, 0x6A, 0x6C, 0x69, 0x97, 0xDD, 0xF1, 0xB2, 0x14,
                0x63, 0x8F, 0xDD, 0x94, 0xC5, 0x7D, 0x73, 0xF6, 0xE1, 0xDA, 0x0C, 0xD5, 0x8B, 0x69, 0x76, 0x06,
                0xC1, 0xE7, 0x61, 0x1C, 0x4B, 0xF2, 0x5B, 0x18, 0x6B, 0xB0, 0x05, 0x34, 0x2C, 0x4C, 0xAB, 0x45,
                0xF3, 0x88, 0x2E, 0x71, 0xFD, 0x7A, 0x7F, 0xC3, 0x0D, 0xB4, 0xB4, 0x71, 0xDF, 0xEE, 0x9A, 0xAA,
                0x1E, 0x26, 0xD5, 0x17, 0x43, 0x6A, 0x6B, 0x4E, 0x93, 0xA2, 0xEE, 0x88, 0xAB, 0x5E, 0xFB, 0x68,
                0x32, 0x78, 0xB3, 0xF7, 0xA5, 0x16, 0x1C, 0x19, 0x6A, 0x66, 0xA4, 0xE3, 0x97, 0x8F, 0x7B, 0x19,
                0x1D, 0xE4, 0x2D, 0x50, 0x09, 0x41, 0x4A, 0x77, 0xF7, 0xA0, 0xBD, 0xEE, 0x99, 0x18, 0x9B, 0xA7,
                0x67, 0xFF, 0x67, 0xF6, 0xDA, 0xD0, 0x31, 0xEF, 0x8E, 0x4F, 0x1C, 0xC6, 0xBA, 0xC6, 0xC4, 0x3D,
                0x81, 0xB9, 0xFD, 0x9F, 0x2E, 0xF0, 0x4C, 0x50, 0x05, 0x9D, 0x08, 0x45, 0xA2, 0x15, 0x35, 0xE9,
                0xC2, 0xED, 0xFD, 0x2F, 0xF7, 0xD3, 0xA8, 0x39, 0xD5, 0xD2, 0xF4, 0x79, 0x58, 0x76, 0x43, 0xCB,
            },
            {
                0xC5, 0x42, 0x7B, 0x81, 0x51, 0x70, 0x8B, 0x84, 0xBD, 0x16, 0x21, 0x37, 0x9E, 0xBC, 0x54, 0xEC,
                0x97, 0xFB, 0x16, 0x81, 0x77, 0x5B, 0x67, 0x02, 0xE5, 0x7E, 0x06, 0x60, 0xC9, 0x3B, 0x4B, 0x98,
                0xB1, 0xEB, 0xE2, 0xA6, 0x46, 0xA5, 0xBB, 0xD1, 0x8A, 0xF4, 0xAB, 0x6D, 0x60, 0xD0, 0xC3, 0xFD,
                0xE5, 0x9F, 0x80, 0xA4, 0xA5, 0xDF, 0xD2, 0xAD, 0x64, 0x8E, 0xB4, 0x72, 0x22, 0x95, 0xDB, 0x5F,
                0xB1, 0x38, 0x43, 0x1C, 0x25, 0xFE, 0x73, 0x10, 0xF4, 0xB8, 0xBD, 0xAA, 0xCF, 0x1A, 0x12, 0x1F,
                0x7C, 0xED, 0x72, 0x3E, 0xCC, 0xF9, 0x75, 0x28, 0x21, 0x83, 0x74, 0x92, 0x72, 0xD4, 0xD5, 0x01,
                0x59, 0x2A, 0x7A, 0x6F, 0x80, 0xA3, 0xA5, 0x63, 0xD5, 0x09, 0x36, 0xCE, 0x0C, 0x3F, 0xCF, 0x08,
                0x10, 0x29, 0xEE, 0xB9, 0xB1, 0xE2, 0x79, 0x02, 0xEC, 0xE8, 0x51, 0x72, 0x4D, 0x60, 0xE4, 0xAC,
                0x76, 0x23, 0x06, 0x45, 0x6F, 0x02, 0xDB, 0x7A, 0xBC, 0x46, 0xC4, 0xF8, 0x3E, 0xC2, 0x1B, 0x9C,
                0x6A, 0xC3, 0x37, 0xE7, 0xC2, 0x85, 0x80, 0xA6, 0xB1, 0x41, 0xC6, 0x43, 0x2A, 0xD9, 0x45, 0x63,
                0x4D, 0x8E, 0xCB, 0xA2, 0x79, 0x54, 0x94, 0x54, 0xE7, 0x34, 0xEA, 0xAD, 0xE9, 0x47, 0x52, 0x6C,
                0x96, 0x22, 0xF2, 0xD6, 0xDC, 0xB7, 0x45, 0x03, 0xB6, 0xC8, 0x36, 0x92, 0x10, 0x4A, 0x40, 0x2B,
                0x05, 0x34, 0x78, 0x2A, 0xAD, 0x6A, 0x8E, 0x7F, 0xA1, 0x22, 0x3A, 0xC5, 0xD1, 0x0A, 0x4D, 0xD8,
                0x7A, 0x9A, 0x53, 0x9A, 0x00, 0xAF, 0x70, 0x76, 0xC1, 0xF9, 0x9C, 0x98, 0x02, 0xCB, 0x4C, 0xF5,
                0x9E, 0x51, 0x29, 0x72, 0x4C, 0x13, 0x45, 0xCA, 0xB1, 0xA4, 0x4A, 0x4E, 0x32, 0xCC, 0x23, 0xA8,
                0x69, 0xBE, 0x82, 0xD5, 0x86, 0x22, 0xA5, 0xEE, 0x97, 0x1A, 0xFF, 0x11, 0xF0, 0xE7, 0x66, 0x0B,
            },
        };

        constexpr const u8 LibraryEmbeddedCardHeaderKey[2][GcCrypto::GcAesKeyLength] = {
            { 0x01, 0xC5, 0x8F, 0xE7, 0x00, 0x2D, 0x13, 0x5A, 0xB2, 0x9A, 0x3F, 0x69, 0x33, 0x95, 0x74, 0xB1, },
            { 0xCB, 0xA7, 0xB8, 0x75, 0xEB, 0x67, 0x05, 0xFB, 0x46, 0x0A, 0x33, 0xFD, 0x34, 0x09, 0x13, 0xB4, },
        };

        constexpr const u8 LibraryEmbeddedConcatenatedGcKeys[2][0x70] = {
            {
                0x98, 0x42, 0xD1, 0x45, 0x92, 0xEE, 0x79, 0xAE, 0xE3, 0xAA, 0xC9, 0xEA, 0x6A, 0x67, 0xC4, 0xB4,
                0x5E, 0x18, 0x1E, 0x0C, 0xC0, 0xA2, 0x1C, 0x0E, 0x05, 0xA4, 0x49, 0x30, 0x53, 0x7F, 0xC8, 0xE2,
                0xB9, 0xFB, 0x97, 0x31, 0x0A, 0x4E, 0x28, 0xE7, 0x1E, 0x69, 0x8C, 0xEE, 0xED, 0x26, 0x20, 0x14,
                0x63, 0x76, 0xBC, 0x1D, 0x86, 0xED, 0x11, 0x01, 0x4D, 0xB0, 0xFC, 0x88, 0xD4, 0x64, 0x15, 0x03,
                0x68, 0x95, 0x4D, 0x5A, 0x87, 0x57, 0x81, 0xB6, 0x6C, 0xD1, 0xEF, 0x40, 0x9D, 0x74, 0xF1, 0xA5,
                0xDA, 0xCA, 0x1F, 0x3E, 0x78, 0x96, 0xCA, 0x2F, 0x1A, 0x47, 0xA3, 0x19, 0x47, 0x47, 0xC4, 0x54,
                0x5C, 0x97, 0x02, 0x74, 0xF2, 0x69, 0xA2, 0x14, 0x46, 0xFC, 0x5B, 0x21, 0x85, 0x29, 0xCB, 0x16,
            },
            {
                0xE8, 0x17, 0xE6, 0x0B, 0xE2, 0x6C, 0x32, 0x30, 0x45, 0xF7, 0xBA, 0x8D, 0xBD, 0x99, 0x15, 0x62,
                0xD1, 0x1C, 0x1C, 0x2C, 0x42, 0xC1, 0x2E, 0x1B, 0x4A, 0xF1, 0x65, 0x3B, 0x0D, 0x37, 0xF3, 0xC6,
                0x91, 0xB2, 0x5C, 0x22, 0xB9, 0x47, 0xF1, 0x15, 0xB0, 0xEE, 0x16, 0xC5, 0x3F, 0xCC, 0x58, 0xD6,
                0xA6, 0xAC, 0x06, 0x47, 0x3A, 0xA0, 0x9B, 0x12, 0xE2, 0x50, 0x80, 0x13, 0x49, 0x2C, 0x3C, 0xED,
                0x35, 0x5B, 0xA3, 0x6D, 0x26, 0x1E, 0xF6, 0xC9, 0xFA, 0xD2, 0x43, 0x81, 0x5A, 0xD0, 0x22, 0x75,
                0x78, 0x5A, 0x92, 0xE4, 0x91, 0x49, 0xD8, 0x28, 0x57, 0x57, 0x49, 0x68, 0x01, 0x0E, 0xA4, 0x10,
                0x5B, 0x05, 0x47, 0x03, 0xD1, 0x1B, 0xA7, 0xCA, 0xD9, 0x06, 0x10, 0x02, 0x85, 0xA3, 0x99, 0x26,
            },
        };

        constexpr const u8 LibraryEmbeddedSplEncryptedKek[GcCrypto::GcAesKeyLength] = {
            0x42, 0xF1, 0xEB, 0xCB, 0xDD, 0xED, 0x82, 0xAF, 0x32, 0x4E, 0x0D, 0xF4, 0x84, 0xF2, 0xAB, 0x57,
        };

        constexpr const u8 LibraryEmbeddedIvForKek[2][GcCrypto::GcAesCbcIvLength] = {
            { 0xC6, 0x09, 0x0A, 0x32, 0x51, 0xA5, 0x26, 0xEC, 0x8F, 0x2B, 0xA9, 0x3E, 0xCC, 0x62, 0xF0, 0x92 },
            { 0x23, 0xA6, 0xFE, 0x53, 0xE7, 0x16, 0x86, 0xFB, 0x9B, 0xB5, 0x72, 0x32, 0x42, 0x01, 0xA3, 0xC7 },
        };

    }

    constinit bool EmbeddedDataHolder::s_is_dev = false;

    constinit const void *EmbeddedDataHolder::s_ca_public_exponent       = LibraryEmbeddedCaPublicExponent;
    constinit const void *EmbeddedDataHolder::s_ca1_modulus              = LibraryEmbeddedCa1Modulus;
    constinit const void *EmbeddedDataHolder::s_ca9_modulus              = LibraryEmbeddedCa9Modulus;
    constinit const void *EmbeddedDataHolder::s_ca10_modulus             = LibraryEmbeddedCa10Modulus[0];
    constinit const void *EmbeddedDataHolder::s_ca10_certificate_modulus = LibraryEmbeddedCa10CertificateModulus[0];
    constinit const void *EmbeddedDataHolder::s_card_header_key          = LibraryEmbeddedCardHeaderKey[0];

    Result EmbeddedDataHolder::SetLibraryEmbeddedKeys(bool is_dev) {
        ConcatenatedGcLibraryEmbeddedKeys embedded_keys;
        R_TRY(DecryptoEmbeddedKeys(std::addressof(embedded_keys), sizeof(embedded_keys), is_dev));

        {
            /* TODO: Set hmac/cv keys. */
            AMS_UNUSED(embedded_keys);
        }

        R_SUCCEED();
    }

    Result EmbeddedDataHolder::DecryptoEmbeddedKeys(ConcatenatedGcLibraryEmbeddedKeys *out, size_t out_size, bool is_dev) {
        /* Determine key index. */
        const auto key_idx = is_dev ? 1 : 0;

        /* Set global pointers. */
        s_is_dev                   = is_dev;
        s_ca10_modulus             = LibraryEmbeddedCa10Modulus[key_idx];
        s_ca10_certificate_modulus = LibraryEmbeddedCa10CertificateModulus[key_idx];
        s_card_header_key          = LibraryEmbeddedCardHeaderKey[key_idx];

        /* Get the keys/iv. */
        ConcatenatedGcLibraryEmbeddedKeys keys;
        std::memcpy(std::addressof(keys), LibraryEmbeddedConcatenatedGcKeys[key_idx], sizeof(keys));
        static_assert(sizeof(LibraryEmbeddedConcatenatedGcKeys[0]) == sizeof(*out));

        const void *iv_for_kek = LibraryEmbeddedIvForKek[key_idx];

        /* Generate the kek. */
        u8 kek[GcCrypto::GcAesKeyLength] = {};
        crypto::Aes128CtrDecryptor aes_ctr;
        ON_SCOPE_EXIT {
            crypto::ClearMemory(kek, sizeof(kek));
            aes_ctr.Initialize(kek, sizeof(kek), iv_for_kek, GcCrypto::GcAesCbcIvLength);
        };

        constexpr const auto KeyGeneration = 4;
        R_TRY(GcCrypto::DecryptAesKeySpl(kek, sizeof(kek), LibraryEmbeddedSplEncryptedKek, sizeof(LibraryEmbeddedSplEncryptedKek), KeyGeneration, 0));

        /* Decrypt the embedded keys. */
        aes_ctr.Initialize(kek, sizeof(kek), iv_for_kek, GcCrypto::GcAesCbcIvLength);
        aes_ctr.Update(out, out_size, std::addressof(keys), sizeof(keys));

        R_SUCCEED();
    }

}