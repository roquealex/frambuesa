///////////////////////////////////////


//char sine_wav_capture[] = {
int sine_wav_capture[] = {
-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,0x01,0x01,0xfe,
0x52,0x49,0x46,0x46,0xc4,0x0f,0x00,0x00,
0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,
0x44,0xac,0x00,0x00,0x10,0xb1,0x02,0x00,
0x04,0x00,0x10,0x00,0x64,0x61,0x74,0x61,
0xa0,0x0f,0x00,0x00,0x00,0x00,0x01,0x00,
0x6a,0x06,0xcc,0x0c,0xcf,0x0c,0x6b,0x19,
0x24,0x13,0x9c,0x25,0x6a,0x19,0x3c,0x31,
0x92,0x1f,0x11,0x3c,0x9d,0x25,0xfa,0x45,
0x83,0x2b,0xc2,0x4e,0x3a,0x31,0x59,0x56,
0xc2,0x36,0x89,0x5c,0x13,0x3c,0x4d,0x61,
0x24,0x41,0x87,0x64,0xfa,0x45,0x2a,0x66,
0x84,0x4a,0x3c,0x66,0xc4,0x4e,0xa7,0x64,
0xb9,0x52,0x88,0x61,0x53,0x56,0xd9,0x5c,
0x9f,0x59,0xb9,0x56,0x8b,0x5c,0x3d,0x4f,
0x19,0x5f,0x7e,0x46,0x50,0x61,0xad,0x3c,
0x1b,0x88,0x01,0x02,0xfd,0x17,0x63,0xda,
0x31,0x8b,0x64,0x4f,0x26,0x88,0x65,0x19,
0x1a,0x31,0x66,0x8b,0x0d,0x63,0x66,0xb9,
0x00,0x3c,0x66,0xeb,0xf3,0xa1,0x65,0x4d,
0xe7,0xab,0x64,0x10,0xdb,0x4a,0x63,0x6a,
0xcf,0x84,0x61,0x85,0xc4,0x62,0x5f,0x91,
0xba,0xd9,0x5c,0xb2,0xb1,0xf7,0x59,0x0f,
0xaa,0xbb,0x56,0xc7,0xa3,0x22,0x53,0xee,
0x9e,0x3e,0x4f,0x9e,0x9b,0x03,0x4b,0xe1,
0x99,0x80,0x46,0xbb,0x99,0xb6,0x41,0x36,
0x9b,0xa9,0x3c,0x42,0x9e,0x5f,0x37,0xd8,
0xa2,0xe0,0x31,0xe3,0xa8,0x28,0x2c,0x50,
0xb0,0x4c,0x26,0xf6,0xb8,0x44,0x20,0xc4,
0xc2,0x1e,0x1a,0x7d,0xcd,0xdc,0x13,0x09,
0xd9,0x87,0x0d,0x2e,0xe5,0x25,0x07,0xbf,
0xf1,0xb9,0x00,0x8c,0xfe,0x53,0xfa,0x5a,
0x0b,0xe9,0xf3,0xff,0x17,0x51,0x2b,0x01,
0x03,0xfc,0x94,0xed,0x40,0x24,0x4b,0xe7,
0xf3,0x2f,0x1f,0xe1,0xe2,0x3a,0x11,0xdb,
0xe5,0x44,0x27,0xd5,0xd5,0x4d,0x6b,0xcf,
0x8b,0x55,0xda,0xc9,0xe7,0x5b,0x87,0xc4,
0xd7,0x60,0x69,0xbf,0x3b,0x64,0x93,0xba,
0x13,0x66,0xf8,0xb5,0x4c,0x66,0xb9,0xb1,
0xea,0x64,0xb1,0xad,0xf6,0x61,0x15,0xaa,
0x75,0x5d,0xba,0xa6,0x7e,0x57,0xc7,0xa3,
0x27,0x50,0x2b,0xa1,0x8c,0x47,0xee,0x9e,
0xd5,0x3d,0x15,0x9d,0x23,0x33,0x9e,0x9b,
0xa4,0x27,0x8a,0x9a,0x87,0x1b,0xe3,0x99,
0xf8,0x0e,0x98,0x99,0x31,0x02,0xc0,0x99,
0x5e,0xf5,0x44,0x9a,0xb7,0xe8,0x36,0x9b,
0x70,0xdc,0x89,0x9c,0xb1,0xd0,0x43,0x9e,
0xb9,0xc5,0x5b,0xa0,0xa3,0xbb,0xda,0xa2,
0xa7,0xb2,0xae,0xa5,0xdc,0xaa,0xe6,0xa8,
0x6b,0xa4,0xbe,0xe9,0x01,0x04,0xfb,0x6e,
0xac,0x68,0x9f,0x4f,0xb0,0xe9,0x9b,0x7d,
0xb4,0x00,0x9a,0xfa,0xb8,0xaa,0x99,0xb9,
0xbd,0xf8,0x9a,0xc3,0xc2,0xd4,0x9d,0x03,
0xc8,0x3f,0xa2,0x7f,0xcd,0x23,0xa8,0x2e,
0xd3,0x64,0xaf,0x07,0xd9,0xef,0xb7,0x0b,
0xdf,0x96,0xc1,0x2e,0xe5,0x3e,0xcc,0x6d,
0xeb,0xad,0xd7,0xbf,0xf1,0xca,0xe3,0x22,
0xf8,0x4a,0xf0,0x89,0xfe,0x18,0xfd,0xf8,
0x04,0xe6,0x09,0x58,0x0b,0x94,0x16,0xb8,
0x11,0xe3,0x22,0xfd,0x17,0xa5,0x2e,0x30,
0x1e,0xb1,0x39,0x41,0x24,0xcd,0x43,0x2d,
0x2a,0xe3,0x4c,0xf4,0x2f,0xb8,0x54,0x83,
0x35,0x44,0x5b,0xe4,0x3a,0x57,0x60,0x04,
0x40,0xef,0x63,0xe4,0x44,0xf1,0x65,0x85,
0x49,0x5a,0x66,0xd2,0x4d,0x27,0x65,0xdb,
0x51,0x62,0x62,0x87,0x55,0x08,0x5e,0x5d,
0x12,0x01,0x05,0xfa,0xeb,0x58,0x41,0x58,
0xe2,0x5b,0x0a,0x51,0x96,0x5e,0x98,0x48,
0xce,0x60,0xfc,0x3e,0xc0,0x62,0x66,0x34,
0x3a,0x64,0xfa,0x28,0x5a,0x65,0xef,0x1c,
0x13,0x66,0x68,0x10,0x60,0x66,0xa8,0x03,
0x4f,0x66,0xd0,0xf6,0xcc,0x65,0x25,0xea,
0xed,0x64,0xcd,0xdd,0xa2,0x63,0xff,0xd1,
0xf4,0x61,0xed,0xc6,0xe8,0x5f,0xbb,0xbc,
0x73,0x5d,0x9e,0xb3,0xa8,0x5a,0xad,0xab,
0x7e,0x57,0x15,0xa5,0xfa,0x53,0xe6,0x9f,
0x2a,0x50,0x3b,0x9c,0xfd,0x4b,0x23,0x9a,
0x90,0x47,0x9f,0x99,0xd0,0x42,0xbe,0x9a,
0xd6,0x3d,0x6a,0x9d,0x96,0x38,0xaf,0xa1,
0x26,0x33,0x60,0xa7,0x78,0x2d,0x86,0xae,
0xa5,0x27,0xe4,0xb6,0xa7,0x21,0x71,0xc0,
0x84,0x1b,0xfa,0xca,0x4c,0x15,0x5a,0xd6,
0xf9,0x0e,0x60,0xe2,0x18,0xb6,0x01,0x06,
0xf9,0x97,0x08,0xde,0xee,0x33,0x02,0xa0,
0xfb,0xc3,0xfb,0x72,0x08,0x60,0xf5,0x28,
0x15,0x02,0xef,0x81,0x21,0xb6,0xe8,0x5a,
0x2d,0x86,0xe2,0x79,0x38,0x6b,0xdc,0xb6,
0x42,0x7e,0xd6,0xe6,0x4b,0xb3,0xd0,0xe9,
0x53,0x1a,0xcb,0x93,0x5a,0xb7,0xc5,0xdb,
0x5f,0x8f,0xc0,0x98,0x63,0xa2,0xbb,0xcb,
0x65,0x04,0xb7,0x64,0x66,0xa3,0xb2,0x5e,
0x65,0x99,0xae,0xc5,0x62,0xdd,0xaa,0x9d,
0x5e,0x76,0xa7,0xf8,0x58,0x6c,0xa4,0xf0,
0x51,0xb8,0xa1,0x9b,0x49,0x6b,0x9f,0x22,
0x40,0x74,0x9d,0xa4,0x35,0xed,0x9b,0x52,
0x2a,0xbf,0x9a,0x52,0x1e,0xff,0x99,0xd9,
0x11,0xa3,0x99,0x1d,0x05,0xaa,0x99,0x44,
0xf8,0x20,0x9a,0x93,0xeb,0xf4,0x9a,0x2e,
0xdf,0x33,0x9c,0x4d,0xd3,0xd6,0x9d,0x25,
0xc8,0x25,0x51,0x01,0x07,0xf8,0xda,0x9f,
0xd6,0xbd,0x3e,0xa2,0x99,0xb4,0x05,0xa5,
0x83,0xac,0x1e,0xa8,0xc3,0xa5,0x9d,0xab,
0x6a,0xa0,0x63,0xaf,0x91,0x9c,0x85,0xb3,
0x4c,0x9a,0xee,0xb7,0x99,0x99,0x9e,0xbc,
0x89,0x9a,0x9b,0xc1,0x09,0x9d,0xc8,0xc6,
0x1d,0xa1,0x40,0xcc,0xab,0xa6,0xda,0xd1,
0xa2,0xad,0xb5,0xd7,0xe2,0xb5,0xa5,0xdd,
0x4f,0xbf,0xcb,0xe3,0xba,0xc9,0xfb,0xe9,
0x07,0xd5,0x51,0xf0,0xfb,0xe0,0xac,0xf6,
0x6d,0xed,0x15,0xfd,0x2c,0xfa,0x81,0x03,
0xff,0x06,0xe7,0x09,0xb7,0x13,0x46,0x10,
0x22,0x20,0x93,0x16,0x07,0x2c,0xc8,0x1c,
0x43,0x37,0xe4,0x22,0x96,0x41,0xd8,0x28,
0xed,0x4a,0xa8,0x2e,0x0c,0x53,0x44,0x34,
0xe8,0x59,0xaf,0x39,0x4f,0x5f,0xe1,0x3e,
0x44,0x63,0xcc,0x43,0x9c,0x65,0xe7,0x17,
0x01,0x08,0xf7,0x81,0x48,0x6a,0x66,0xda,
0x4c,0x8f,0x65,0xfb,0x50,0x26,0x63,0xb7,
0x54,0x28,0x5f,0x2d,0x58,0xb0,0x59,0x40,
0x5b,0xcd,0x52,0xfd,0x5d,0x9e,0x4a,0x5a,
0x60,0x42,0x41,0x54,0x62,0xe2,0x36,0xef,
0x63,0xa4,0x2b,0x21,0x65,0xb6,0x1f,0xf1,
0x65,0x4a,0x13,0x5a,0x66,0x8f,0x06,0x59,
0x66,0xbc,0xf9,0xf6,0x65,0xff,0xec,0x26,
0x65,0x90,0xe0,0xf8,0x63,0xa2,0xd4,0x5d,
0x62,0x5b,0xc9,0x68,0x60,0xf8,0xbe,0x09,
0x5e,0x96,0xb5,0x55,0x5b,0x5d,0xad,0x3b,
0x58,0x79,0xa6,0xd2,0x54,0xee,0xa0,0x0b,
0x51,0xf1,0x9c,0xf9,0x4c,0x76,0x9a,0x97,
0x48,0x9b,0x99,0xeb,0x43,0x58,0x9a,0xfd,
0x3e,0xac,0x9c,0xcd,0x39,0x94,0xa0,0x67,
0x34,0xf6,0xa5,0xc7,0x2e,0xc6,0xac,0xfc,
0x28,0xe3,0xb4,0x64,0xe8,0x01,0x09,0xf6,
0x05,0x23,0x2d,0xbe,0xed,0x1c,0x82,0xc8,
0xb8,0x16,0xb4,0xd3,0x6a,0x10,0x97,0xdf,
0x0d,0x0a,0x00,0xec,0xa5,0x03,0xb5,0xf8,
0x3b,0xfd,0x8c,0x05,0xd2,0xf6,0x47,0x12,
0x73,0xf0,0xc0,0x1e,0x25,0xea,0xb4,0x2a,
0xe7,0xe3,0x06,0x36,0xd1,0xdd,0x77,0x40,
0xcf,0xd7,0xec,0x49,0x02,0xd2,0x30,0x52,
0x5b,0xcc,0x31,0x59,0xeb,0xc6,0xc7,0x5e,
0xb6,0xc1,0xe2,0x62,0xbb,0xbc,0x70,0x65,
0x0a,0xb8,0x63,0x66,0x9a,0xb3,0xbf,0x65,
0x7f,0xaf,0x7f,0x63,0xad,0xab,0xb2,0x5f,
0x36,0xa8,0x60,0x5a,0x13,0xa5,0xa7,0x53,
0x4e,0xa2,0x9b,0x4b,0xe8,0x9f,0x61,0x42,
0xde,0x9d,0x1c,0x38,0x3d,0x9c,0xf4,0x2c,
0xfa,0x9a,0x1a,0x21,0x24,0x9a,0xb6,0x14,
0xab,0x99,0x06,0x08,0xa4,0x99,0x2e,0xfb,
0xbe,0xaf,0x01,0x0a,0xf5,0xf8,0x99,0x71,
0xee,0xbe,0x9a,0xf4,0xe1,0xe0,0x9b,0xf4,
0xd5,0x70,0x9d,0x99,0xca,0x59,0x9f,0x1c,
0xc0,0xae,0xa1,0x95,0xb6,0x58,0xa4,0x41,
0xae,0x65,0xa7,0x29,0xa7,0xc9,0xaa,0x82,
0xa1,0x80,0xae,0x4f,0x9d,0x8e,0xb2,0xaa,
0x9a,0xe5,0xb6,0xa1,0x99,0x8a,0xbb,0x2a,
0x9a,0x71,0xc0,0x58,0x9c,0x97,0xc5,0x0b,
0xa0,0xfc,0xca,0x4a,0xa5,0x91,0xd0,0xec,
0xab,0x5a,0xd6,0xe9,0xb3,0x4c,0xdc,0x10,
0xbd,0x5e,0xe2,0x49,0xc7,0x95,0xe8,0x65,
0xd2,0xdc,0xee,0x35,0xde,0x3a,0xf5,0x93,
0xea,0xa0,0xfb,0x40,0xf7,0x0b,0x02,0x17,
0x04,0x73,0x08,0xd8,0x10,0xd5,0x0e,0x59,
0x1d,0x25,0x15,0x62,0x29,0x63,0x1b,0xc5,
0x34,0x82,0x21,0x55,0x3f,0x82,0x27,0xe5,
0x48,0x59,0x2d,0x51,0x51,0xe0,0xc8,0x01,
0x0b,0xf4,0x02,0x33,0x78,0x58,0x7a,0x38,
0x36,0x5e,0xb6,0x3d,0x7e,0x62,0xb5,0x42,
0x3a,0x65,0x75,0x47,0x5c,0x66,0xe4,0x4b,
0xe8,0x65,0x12,0x50,0xd3,0x63,0xe5,0x53,
0x34,0x60,0x6c,0x57,0x0e,0x5b,0x93,0x5a,
0x7c,0x54,0x69,0x5d,0x96,0x4c,0xd5,0x5f,
0x7b,0x43,0xed,0x61,0x53,0x39,0x98,0x63,
0x42,0x2e,0xe6,0x64,0x7a,0x22,0xca,0x65,
0x25,0x16,0x4b,0x66,0x78,0x09,0x63,0x66,
0xa6,0xfc,0x13,0x66,0xdf,0xef,0x61,0x65,
0x5a,0xe3,0x43,0x64,0x4c,0xd7,0xc6,0x62,
0xd6,0xcb,0xe0,0x60,0x43,0xc1,0x9e,0x5e,
0x9c,0xb7,0xf8,0x5b,0x21,0xaf,0xf9,0x58,
0xeb,0xa7,0xa0,0x55,0x0e,0xa2,0xec,0x51,
0xb9,0x9d,0xf0,0x4d,0xe1,0x9a,0x9a,0x49,
0xab,0x99,0x03,0x45,0x06,0x9a,0x21,0x40,
0x03,0x9c,0xd0,0xf0,0x01,0x0c,0xf3,0xff,
0x3a,0x8d,0x9f,0xa7,0x35,0x9d,0xa4,0x12,
0x30,0x1b,0xab,0x50,0x2a,0xef,0xb2,0x66,
0x24,0xf8,0xbb,0x4f,0x1e,0x15,0xc6,0x27,
0x18,0x14,0xd1,0xd6,0x11,0xda,0xdc,0x84,
0x0b,0x23,0xe9,0x18,0x05,0xce,0xf5,0xb2,
0xfe,0xa1,0x02,0x45,0xf8,0x66,0x0f,0xe6,
0xf1,0xf3,0x1b,0x8f,0xeb,0x0c,0x28,0x54,
0xe5,0x82,0x33,0x2d,0xdf,0x31,0x3e,0x2b,
0xd9,0xd9,0x47,0x50,0xd3,0x6f,0x50,0x9d,
0xcd,0xb7,0x57,0x25,0xc8,0xa2,0x5d,0xdf,
0xc2,0x17,0x62,0xd6,0xbd,0xfd,0x64,0x17,
0xb9,0x51,0x66,0x93,0xb4,0x09,0x66,0x69,
0xb0,0x26,0x64,0x83,0xac,0xae,0x60,0xf8,
0xa8,0xbb,0x5b,0xc2,0xa5,0x48,0x55,0xe7,
0xa2,0x8f,0x4d,0x6c,0xa0,0x91,0x44,0x4a,
0x9e,0x86,0x3a,0x95,0x9c,0x91,0x2f,0xb1,
0x93,0x01,0x0d,0xf2,0x3a,0x9b,0xd5,0x23,
0x4a,0x9a,0x94,0x17,0xc0,0x99,0xea,0x0a,
0x9a,0x99,0x1b,0xfe,0xde,0x99,0x51,0xf1,
0x86,0x9a,0xc2,0xe4,0x98,0x9b,0xa1,0xd8,
0x07,0x9d,0x1d,0xcd,0xe8,0x9e,0x68,0xc2,
0x17,0xa1,0xa8,0xb8,0xbb,0xa3,0x08,0xb0,
0xa7,0xa6,0xaa,0xa8,0xfe,0xa9,0xa9,0xa2,
0x9f,0xad,0x1f,0x9e,0x9d,0xb1,0x25,0x9b,
0xe2,0xb5,0xb3,0x99,0x74,0xba,0xed,0x99,
0x50,0xbf,0xb1,0x9b,0x63,0xc4,0x14,0x9f,
0xc0,0xc9,0xf6,0xa3,0x46,0xcf,0x4d,0xaa,
0x08,0xd5,0xfa,0xb1,0xec,0xda,0xe4,0xba,
0xfd,0xe0,0xe2,0xc4,0x26,0xe7,0xc9,0xcf,
0x6e,0xed,0x7d,0xdb,0xc8,0xf3,0xb5,0xe7,
0x28,0xfa,0x5d,0xf4,0x99,0x00,0x2b,0x01,
0xfd,0x06,0xf5,0x0d,0x63,0x0d,0x8a,0x1a,
0xb8,0x13,0xb4,0x26,0x1d,0x68,0x01,0x0e,
0xf1,0xf8,0x19,0x3e,0x32,0x22,0x20,0x05,
0x3d,0x28,0x26,0xd0,0x46,0x0a,0x2c,0x83,
0x4f,0xbc,0x31,0xf5,0x56,0x41,0x37,0x09,
0x5d,0x8a,0x3c,0xa9,0x61,0x9a,0x41,0xbb,
0x64,0x64,0x46,0x42,0x66,0xec,0x4a,0x23,
0x66,0x22,0x4f,0x72,0x64,0x11,0x53,0x29,
0x61,0xa4,0x56,0x5a,0x5c,0xe5,0x59,0x1b,
0x56,0xcc,0x5c,0x7a,0x4e,0x4f,0x5f,0xaa,
0x45,0x7f,0x61,0xb4,0x3b,0x3c,0x63,0xdb,
0x30,0xa6,0x64,0x34,0x25,0x9d,0x65,0xfc,
0x18,0x38,0x66,0x5f,0x0c,0x66,0x66,0x91,
0xff,0x30,0x66,0xc1,0xf2,0x90,0x65,0x2d,
0xe6,0x8f,0x64,0xf8,0xd9,0x24,0x63,0x65,
0xce,0x58,0x61,0x92,0xc3,0x2a,0x5f,0xb6,
0xb9,0x99,0x5c,0xf5,0xb0,0xb0,0x59,0x6c,
0xa9,0x6a,0x56,0x48,0xa3,0xcc,0x52,0x8e,
0x9e,0xec,0x61,0x01,0x0f,0xf0,0xde,0x4e,
0x67,0x9b,0x9d,0x4a,0xca,0x99,0x14,0x46,
0xcf,0x99,0x42,0x41,0x6d,0x9b,0x32,0x3c,
0x99,0x9e,0xdf,0x36,0x59,0xa3,0x5f,0x31,
0x80,0xa9,0xa0,0x2b,0x0b,0xb1,0xc3,0x25,
0xd4,0xb9,0xb4,0x1f,0xae,0xc3,0x8f,0x19,
0x87,0xce,0x49,0x13,0x1b,0xda,0xf3,0x0c,
0x50,0xe6,0x8f,0x06,0xe7,0xf2,0x27,0x00,
0xb6,0xff,0xb9,0xf9,0x84,0x0c,0x59,0xf3,
0x20,0x19,0xfd,0xec,0x58,0x25,0xbe,0xe6,
0xfa,0x30,0x8f,0xe0,0xd4,0x3b,0x86,0xda,
0xc3,0x45,0xa0,0xd4,0x95,0x4e,0xe5,0xce,
0x2c,0x56,0x5f,0xc9,0x6d,0x5c,0x0c,0xc4,
0x33,0x61,0xf5,0xbe,0x79,0x64,0x26,0xba,
0x28,0x66,0x92,0xb5,0x3c,0x66,0x55,0xb1,
0xb7,0x64,0x5e,0xad,0x9e,0x61,0xbf,0xa9,
0xf8,0x5c,0x75,0xa6,0xe2,0x56,0xf2,0x95,
0x01,0x10,0xef,0x85,0xa3,0x6a,0x4f,0xf2,
0xa0,0xb8,0x46,0xc0,0x9e,0xe3,0x3c,0xee,
0x9c,0x21,0x32,0x80,0x9b,0x8f,0x26,0x7a,
0x9a,0x66,0x1a,0xd2,0x99,0xd1,0x0d,0x9f,
0x99,0x07,0x01,0xc0,0x99,0x32,0xf4,0x5b,
0x9a,0x99,0xe7,0x4e,0x9b,0x52,0xdb,0xae,
0x9c,0xae,0xcf,0x6f,0x9e,0xc2,0xc4,0x91,
0xa0,0xc7,0xba,0x18,0xa3,0xe4,0xb1,0xf6,
0xa5,0x35,0xaa,0x34,0xa9,0xeb,0xa3,0xc4,
0xac,0x02,0x9f,0xae,0xb0,0xb1,0x9b,0xe1,
0xb4,0xe2,0x99,0x68,0xb9,0xbc,0x99,0x29,
0xbe,0x26,0x9b,0x3d,0xc3,0x2c,0x9e,0x7e,
0xc8,0xba,0xa2,0x04,0xce,0xbb,0xa8,0xb2,
0xd3,0x21,0xb0,0x93,0xd9,0xc3,0xb8,0x97,
0xdf,0x84,0xc2,0xc1,0xe5,0x3f,0xcd,0xfb,
0xeb,0xc2,0xd8,0x58,0xf2,0xe8,0xe4,0xb3,
0xf8,0x73,0xf1,0xac,0x25,0x01,0x11,0xee,
0x20,0xff,0x44,0xfe,0x8d,0x05,0x0d,0x0b,
0xed,0x0b,0xb8,0x17,0x4b,0x12,0xfa,0x23,
0x8e,0x18,0xb0,0x2f,0xbd,0x1e,0xa6,0x3a,
0xce,0x24,0xae,0x44,0xb6,0x2a,0xa3,0x4d,
0x76,0x30,0x63,0x55,0x04,0x36,0xc5,0x5b,
0x5c,0x3b,0xc0,0x60,0x78,0x40,0x2b,0x64,
0x54,0x45,0x0c,0x66,0xeb,0x49,0x4f,0x66,
0x34,0x4e,0xf9,0x64,0x33,0x52,0x09,0x62,
0xdc,0x55,0x96,0x5d,0x2f,0x59,0xa2,0x57,
0x2b,0x5c,0x56,0x50,0xc6,0x5e,0xc3,0x47,
0x08,0x61,0x0f,0x3e,0xde,0x62,0x65,0x33,
0x60,0x64,0xe7,0x27,0x6a,0x65,0xd1,0x1b,
0x21,0x66,0x3f,0x0f,0x62,0x66,0x7f,0x02,
0x47,0x66,0xa5,0xf5,0xbb,0x65,0x03,0xe9,
0xd5,0x64,0xb3,0xdc,0x7e,0x63,0xf5,0xd0,
0xca,0x61,0xf6,0xc5,0xb0,0x5f,0xda,0xbb,
0x07,0xf6,0x01,0x12,0xed,0x38,0x5d,0xd9,
0xb2,0x60,0x5a,0x05,0xab,0x32,0x57,0x8c,
0xa4,0xa4,0x53,0x82,0x9f,0xcb,0x4f,0xf8,
0x9b,0x9a,0x4b,0x07,0x9a,0x23,0x47,0xa8,
0x99,0x62,0x42,0xea,0x9a,0x5a,0x3d,0xc1,
0x9d,0x1f,0x38,0x20,0xa2,0x9e,0x32,0xfc,
0xa7,0xf6,0x2c,0x37,0xaf,0x1b,0x27,0xbb,
0xb7,0x17,0x21,0x58,0xc1,0xf8,0x1a,0x01,
0xcc,0xb6,0x14,0x65,0xd7,0x66,0x0e,0x83,
0xe3,0x05,0x08,0x02,0xf0,0x9a,0x01,0xcb,
0xfc,0x30,0xfb,0x9e,0x09,0xca,0xf4,0x49,
0x16,0x6f,0xee,0x9e,0x22,0x26,0xe8,0x61,
0x2e,0xf6,0xe1,0x75,0x39,0xe0,0xdb,0x94,
0x43,0xf5,0xd5,0xb1,0x4c,0x2e,0xd0,0x91,
0x54,0x9b,0xca,0x1d,0x5b,0x3d,0xc5,0x43,
0x60,0x18,0xc0,0xda,0x63,0x38,0xbb,0xed,
0x65,0x95,0xb6,0x5a,0x66,0xdb,0x2f,0x01,
0x13,0xec,0x46,0xb2,0x36,0x65,0x3c,0xae,
0x71,0x62,0x8c,0xaa,0x2c,0x5e,0x2b,0xa7,
0x60,0x58,0x29,0xa4,0x3e,0x51,0x7f,0xa1,
0xc8,0x48,0x38,0x9f,0x3a,0x3f,0x4e,0x9d,
0xa3,0x34,0xcc,0x9b,0x42,0x29,0xaa,0x9a,
0x32,0x1d,0xf3,0x99,0xb8,0x10,0x9d,0x99,
0xeb,0x03,0xb4,0x99,0x22,0xf7,0x2b,0x9a,
0x68,0xea,0x10,0x9b,0x17,0xde,0x55,0x9c,
0x41,0xd2,0xfe,0x9d,0x2a,0xc7,0x11,0xa0,
0xf6,0xbc,0x78,0xa2,0xcc,0xb3,0x4a,0xa5,
0xdb,0xab,0x6e,0xa8,0x35,0xa5,0xee,0xab,
0x01,0xa0,0xc3,0xaf,0x4d,0x9c,0xe6,0xb3,
0x29,0x9a,0x59,0xb8,0x9e,0x99,0x10,0xbd,
0xb2,0x9a,0x10,0xc2,0x58,0x9d,0x46,0xc7,
0x8f,0xa1,0xbf,0xcc,0x40,0xa7,0x62,0xd2,
0x53,0xae,0x3b,0xd8,0xb3,0xb6,0x36,0xde,
0x35,0xc0,0x0f,0xc3,0x01,0x14,0xeb,0x56,
0xe4,0xbc,0xca,0x91,0xea,0x14,0xd6,0xe2,
0xf0,0x1b,0xe2,0x43,0xf7,0x92,0xee,0xa8,
0xfd,0x57,0xfb,0x17,0x04,0x27,0x08,0x7c,
0x0a,0xdf,0x14,0xd9,0x10,0x3a,0x21,0x25,
0x17,0x18,0x2d,0x57,0x1d,0x39,0x38,0x70,
0x23,0x7f,0x42,0x61,0x29,0xb3,0x4b,0x2c,
0x2f,0xbe,0x53,0xc6,0x34,0x71,0x5a,0x29,
0x3a,0xbf,0x5f,0x56,0x3f,0x87,0x63,0x3e,
0x44,0xc4,0x65,0xe6,0x48,0x63,0x66,0x42,
0x4d,0x6a,0x65,0x52,0x51,0xd9,0x62,0x0d,
0x55,0xb8,0x5e,0x77,0x58,0x1e,0x59,0x85,
0x5b,0x1c,0x52,0x37,0x5e,0xd0,0x49,0x8c,
0x60,0x5b,0x40,0x7e,0x62,0xe4,0x35,0x0e,
0x64,0x95,0x2a,0x3a,0x65,0x9a,0x1e,0xfe,
0x65,0x24,0x12,0x5c,0x66,0x66,0x05,0x58,
0x66,0x90,0xf8,0xe3,0x65,0xda,0xeb,0xd2,
0x34,0x01,0x15,0xea,0x13,0x65,0x76,0xdf,
0xd3,0x63,0x90,0xd3,0x36,0x62,0x65,0xc8,
0x33,0x60,0x0e,0xbe,0xd1,0x5d,0xcc,0xb4,
0x0c,0x5b,0xaf,0xac,0xf4,0x57,0xe5,0xa5,
0x79,0x54,0x86,0xa0,0xb3,0x50,0xa3,0x9c,
0x95,0x4c,0x55,0x9a,0x2d,0x48,0x98,0x99,
0x7b,0x43,0x7f,0x9a,0x87,0x3e,0xf6,0x9c,
0x52,0x39,0x01,0xa1,0xe6,0x33,0x88,0xa6,
0x40,0x2e,0x74,0xad,0x76,0x28,0xb0,0xb5,
0x76,0x22,0x14,0xbf,0x60,0x1c,0x7b,0xc9,
0x24,0x16,0xc5,0xd4,0xd7,0x0f,0xb1,0xe0,
0x78,0x09,0x28,0xed,0x11,0x03,0xdd,0xf9,
0xa4,0xfc,0xb7,0x06,0x3e,0xf6,0x6d,0x13,
0xe0,0xef,0xdb,0x1f,0x91,0xe9,0xc5,0x2b,
0x5b,0xe3,0x01,0x37,0x41,0xdd,0x60,0x41,
0x48,0xd7,0xb7,0x4a,0x7c,0xd1,0xe3,0x52,
0xda,0xcb,0xc1,0x59,0x3f,0xdb,0x01,0x16,
0xe9,0x70,0xc6,0x37,0x5f,0x3e,0xc1,0x2d,
0x63,0x4e,0xbc,0x98,0x65,0x9c,0xb7,0x64,
0x66,0x3c,0xb3,0x9e,0x65,0x1d,0xaf,0x34,
0x63,0x5e,0xab,0x47,0x5f,0xe5,0xa7,0xd1,
0x59,0xd3,0xa4,0xfb,0x52,0x0f,0xa2,0xcf,
0x4a,0xb7,0x9f,0x7e,0x41,0xb2,0x9d,0x1f,
0x37,0x1d,0x9c,0xe8,0x2b,0xe2,0x9a,0xfe,
0x1f,0x14,0x9a,0x92,0x13,0xa7,0x99,0xda,
0x06,0xa6,0x99,0x08,0xfa,0x08,0x9a,0x45,
0xed,0xd1,0x9a,0xdc,0xe0,0x04,0x9c,0xe0,
0xd4,0x94,0x9d,0x9f,0xc9,0x8f,0x9f,0x30,
0xbf,0xe6,0xa1,0xc9,0xb5,0x9b,0xa4,0x8c,
0xad,0xb1,0xa7,0x97,0xa6,0x1b,0xab,0x11,
0xa1,0xdc,0xae,0xff,0x9c,0xef,0xb2,0x84,
0x9a,0x4f,0xb7,0x99,0x99,0xf9,0xbb,0x4f,
0x9a,0xe5,0xc0,0x9a,0x9c,0x14,0xc6,0x7a,
0xa0,0x24,0x66,0x01,0x17,0xe8,0x7a,0xcb,
0xd1,0xa5,0x18,0xd1,0x9d,0xac,0xe2,0xd6,
0xad,0xb4,0xd7,0xdc,0xf7,0xbd,0xef,0xe2,
0x41,0xc8,0x25,0xe9,0x71,0xd3,0x70,0xef,
0x52,0xdf,0xd0,0xf5,0xb4,0xeb,0x33,0xfc,
0x6e,0xf8,0xa2,0x02,0x3d,0x05,0x07,0x09,
0x02,0x12,0x69,0x0f,0x74,0x1e,0xb7,0x15,
0x75,0x2a,0xf3,0x1b,0xc2,0x35,0x0f,0x22,
0x40,0x40,0x0c,0x28,0xb5,0x49,0xde,0x2d,
0x06,0x52,0x85,0x33,0x0b,0x59,0xf4,0x38,
0xab,0x5e,0x2f,0x3e,0xce,0x62,0x26,0x43,
0x65,0x65,0xdf,0x47,0x64,0x66,0x49,0x4c,
0xc5,0x65,0x6d,0x50,0x93,0x63,0x3c,0x54,
0xca,0x5f,0xb8,0x57,0x84,0x5a,0xdb,0x5a,
0xd3,0x53,0xa3,0x5d,0xcb,0x4b,0x0b,0x60,
0x9d,0x42,0x17,0x62,0x57,0x38,0xbc,0x63,
0x3a,0x2d,0xfd,0x64,0x5d,0x21,0xe8,0x9a,
0x01,0x18,0xe7,0xdc,0x65,0x03,0x15,0x50,
0x66,0x4e,0x08,0x5f,0x66,0x7a,0xfb,0x0c,
0x66,0xba,0xee,0x44,0x65,0x3b,0xe2,0x2b,
0x64,0x39,0xd6,0x99,0x62,0xd9,0xca,0xb2,
0x60,0x55,0xc0,0x65,0x5e,0xcb,0xb6,0xb3,
0x5b,0x6b,0xae,0xb2,0x58,0x53,0xa7,0x4b,
0x55,0x9b,0xa1,0x94,0x51,0x65,0x9d,0x8e,
0x4d,0xb5,0x9a,0x31,0x49,0xa0,0x99,0x95,
0x44,0x26,0x9a,0xac,0x3f,0x43,0x9c,0x86,
0x3a,0xf5,0x9f,0x26,0x35,0x24,0xa5,0x8e,
0x2f,0xc4,0xab,0xc9,0x29,0xb6,0xb3,0xd7,
0x23,0xd7,0xbc,0xc5,0x1d,0x0d,0xc7,0x91,
0x17,0x1e,0xd2,0x48,0x11,0xf4,0xdd,0xea,
0x0a,0x46,0xea,0x87,0x04,0xf7,0xf6,0x19,
0xfe,0xce,0x03,0xb3,0xf7,0x8a,0x10,0x50,
0xf1,0x16,0x1d,0xff,0xea,0x1a,0x29,0xc1,
0xe4,0x87,0x34,0x0e,0x03,0x01,0x19,0xe6,
0xa2,0xde,0x1a,0x3f,0x9f,0xd8,0xaf,0x48,
0xcb,0xd2,0x27,0x51,0x1b,0xcd,0x4d,0x58,
0xa8,0xc7,0x1d,0x5e,0x67,0xc2,0x68,0x62,
0x65,0xbd,0x2e,0x65,0xab,0xb8,0x5c,0x66,
0x2f,0xb4,0xec,0x65,0x0a,0xb0,0xe7,0x63,
0x2e,0xac,0x4c,0x60,0xa9,0xa8,0x30,0x5b,
0x7c,0xa5,0xa7,0x54,0xaa,0xa2,0xc6,0x4c,
0x33,0xa0,0xb5,0x43,0x24,0x9e,0x8f,0x39,
0x6b,0x9c,0x86,0x2e,0x25,0x9b,0xbe,0x22,
0x36,0x9a,0x71,0x16,0xba,0x99,0xc0,0x09,
0x9b,0x99,0xf3,0xfc,0xea,0x99,0x25,0xf0,
0x99,0x9a,0xa6,0xe3,0xb6,0x9b,0x8b,0xd7,
0x31,0x9d,0x1e,0xcc,0x11,0x9f,0x77,0xc1,
0x59,0xa1,0xd6,0xb7,0xf2,0xa3,0x4b,0xaf,
0xf8,0xa6,0x12,0xa8,0x4b,0xaa,0x2f,0xa2,
0xfa,0xad,0xc9,0x9d,0xfe,0xb1,0xf2,0x9a,
0x33,0xa7,0x01,0x1a,0xe5,0x48,0xb6,0xaa,
0x99,0xe2,0xba,0x00,0x9a,0xc4,0xbf,0xf4,
0x9b,0xde,0xc4,0x72,0x9f,0x3e,0xca,0x7d,
0xa4,0xcb,0xcf,0xf2,0xaa,0x8d,0xd5,0xbc,
0xb2,0x79,0xdb,0xc2,0xbb,0x8c,0xe1,0xd5,
0xc5,0xb6,0xe7,0xd5,0xd0,0x03,0xee,0x90,
0xdc,0x59,0xf4,0xdd,0xe8,0xc2,0xfa,0x83,
0xf5,0x29,0x01,0x55,0x02,0x95,0x07,0x1f,
0x0f,0xf7,0x0d,0xa9,0x1b,0x49,0x14,0xc7,
0x27,0x8b,0x1a,0x45,0x33,0xae,0x20,0xef,
0x3d,0xb3,0x26,0xab,0x47,0x8f,0x2c,0x3c,
0x50,0x42,0x32,0x92,0x57,0xba,0x37,0x84,
0x5d,0x06,0x3d,0x00,0x62,0x0a,0x42,0xf1,
0x64,0xd2,0x46,0x50,0x66,0x51,0x4b,0x0c,
0x66,0x81,0x4f,0x36,0x64,0x67,0x53,0xc9,
0x60,0xf3,0x56,0xd8,0x5b,0x2e,0x5a,0x76,
0x55,0x09,0x5d,0xbd,0x4d,0xfe,0xc7,0x01,
0x1b,0xe4,0x87,0x5f,0xc8,0x44,0xaa,0x61,
0xc6,0x3a,0x62,0x63,0xcf,0x2f,0xc1,0x64,
0x20,0x24,0xaf,0x65,0xd9,0x17,0x40,0x66,
0x36,0x0b,0x65,0x66,0x66,0xfe,0x26,0x66,
0x99,0xf1,0x7d,0x65,0x0c,0xe5,0x73,0x64,
0xe5,0xd8,0xfc,0x62,0x5f,0xcd,0x2c,0x61,
0xa2,0xc2,0xee,0x5e,0xde,0xb8,0x5f,0x5c,
0x38,0xb0,0x63,0x59,0xcf,0xa8,0x1c,0x56,
0xca,0xa2,0x73,0x52,0x35,0x9e,0x7d,0x4e,
0x30,0x9b,0x39,0x4a,0xba,0x99,0xa5,0x45,
0xe4,0x99,0xd0,0x40,0xa3,0x9b,0xb8,0x3b,
0xfd,0x9e,0x62,0x36,0xd5,0xa3,0xd9,0x30,
0x24,0xaa,0x1c,0x2b,0xcb,0xb1,0x34,0x25,
0xab,0xba,0x2b,0x1f,0xa5,0xc4,0xf9,0x18,
0x89,0xcf,0xba,0x12,0x35,0xdb,0x5c,0x0c,
0x6f,0xe7,0xfd,0x05,0x11,0xf4,0x8d,0xff,
0xe0,0x00,0xaa,0xed,0x01,0x1c,0xe3,0x29,
0xf9,0xac,0x0d,0xc0,0xf2,0x43,0x1a,0x6f,
0xec,0x6b,0x26,0x2a,0xe6,0x01,0x32,0x02,
0xe0,0xc5,0x3c,0xfb,0xd9,0x9d,0x46,0x19,
0xd4,0x53,0x4f,0x63,0xce,0xcd,0x56,0xdf,
0xc8,0xea,0x5c,0x93,0xc3,0x92,0x61,0x85,
0xbe,0xaf,0x64,0xb5,0xb9,0x3c,0x66,0x2f,
0xb5,0x2a,0x66,0xf4,0xb0,0x80,0x64,0x06,
0xad,0x3f,0x61,0x70,0xa9,0x7c,0x5c,0x2c,
0xa6,0x41,0x56,0x45,0xa3,0xad,0x4e,0xbc,
0xa0,0xde,0x45,0x90,0x9e,0xf2,0x3b,0xcb,
0x9c,0x1c,0x31,0x62,0x9b,0x78,0x25,0x67,
0x9a,0x47,0x19,0xcb,0x99,0xa7,0x0c,0x99,
0x99,0xdc,0xff,0xd0,0x99,0x0d,0xf3,0x68,
0x9a,0x72,0xe6,0x6d,0x9b,0x40,0xda,0xd1,
0x9c,0xa6,0xce,0x9c,0x9e,0xcd,0xc3,0xcb,
0xa0,0xf0,0xb9,0x54,0xa3,0x1f,0xb1,0xdb,
0xdb,0x01,0x1d,0xe2,0x40,0xa6,0x9b,0xa9,
0x81,0xa9,0x61,0xa3,0x1d,0xad,0xac,0x9e,
0x0c,0xb1,0x6e,0x9b,0x49,0xb5,0xd4,0x99,
0xd1,0xb9,0xc9,0x99,0xa0,0xbe,0x5d,0x9b,
0xb2,0xc3,0x87,0x9e,0xfe,0xc8,0x33,0xa3,
0x85,0xce,0x5c,0xa9,0x3a,0xd4,0xdc,0xb0,
0x1d,0xda,0x9a,0xb9,0x27,0xe0,0x77,0xc3,
0x4e,0xe6,0x40,0xce,0x93,0xec,0xda,0xd9,
0xe5,0xf2,0x05,0xe6,0x4f,0xf9,0x9f,0xf2,
0xb2,0xff,0x6a,0xff,0x21,0x06,0x39,0x0c,
0x85,0x0c,0xda,0x18,0xd9,0x12,0x10,0x25,
0x24,0x19,0xb9,0x30,0x48,0x1f,0x99,0x3b,
0x5a,0x25,0x8a,0x45,0x3e,0x2b,0x66,0x4e,
0xf8,0x30,0x05,0x56,0x84,0x36,0x49,0x5c,
0xd6,0x3b,0x1f,0x61,0xea,0x40,0x68,0x64,
0xc5,0x45,0x24,0x66,0x4e,0x4a,0x40,0x66,
0x97,0x4e,0xc5,0x64,0x63,0x51,0x01,0x1e,
0xe1,0x8a,0x52,0xb3,0x61,0x2e,0x56,0x18,
0x5d,0x78,0x59,0x08,0x57,0x6c,0x5c,0x9c,
0x4f,0xfe,0x5e,0xeb,0x46,0x36,0x61,0x22,
0x3d,0x07,0x63,0x5f,0x32,0x7a,0x64,0xd6,
0x26,0x81,0x65,0xae,0x1a,0x2a,0x66,0x1b,
0x0e,0x64,0x66,0x51,0x01,0x3d,0x66,0x7e,
0xf4,0xaf,0x65,0xdf,0xe7,0xb5,0x64,0x9b,
0xdb,0x5d,0x63,0xee,0xcf,0x9b,0x61,0xff,
0xc4,0x7c,0x5f,0xff,0xba,0xf9,0x5c,0x12,
0xb2,0x1d,0x5a,0x63,0xaa,0xdd,0x56,0x04,
0xa4,0x54,0x53,0x23,0x9f,0x68,0x4f,0xb8,
0x9b,0x38,0x4b,0xee,0x99,0xb8,0x46,0xb4,
0x99,0xeb,0x41,0x1b,0x9b,0xea,0x3c,0x17,
0x9e,0x9a,0x37,0x9a,0xa2,0x22,0x32,0x95,
0xa8,0x6d,0x2c,0xf2,0xaf,0x90,0x26,0x8c,
0xb8,0x8b,0x20,0x4b,0xc2,0x66,0x1a,0xfd,
0xcc,0xe5,0xb1,0x01,0x1f,0xe0,0x26,0x14,
0x7e,0xd8,0xd1,0x0d,0x9d,0xe4,0x6f,0x07,
0x2e,0xf1,0x06,0x01,0xf3,0xfd,0x99,0xfa,
0xc8,0x0a,0x38,0xf4,0x6e,0x17,0xda,0xed,
0xb2,0x23,0x96,0xe7,0x70,0x2f,0x66,0xe1,
0x67,0x3a,0x55,0xdb,0x76,0x44,0x6d,0xd5,
0x76,0x4d,0xaa,0xcf,0x35,0x55,0x1d,0xca,
0xa8,0x5b,0xc1,0xc4,0xa3,0x60,0xa5,0xbf,
0x1e,0x64,0xc8,0xba,0x06,0x66,0x2f,0xb6,
0x51,0x66,0xe5,0xb1,0x05,0x65,0xe1,0xad,
0x21,0x62,0x3b,0xaa,0xb0,0x5d,0xe1,0xa6,
0xcd,0x57,0xe7,0xa3,0x81,0x50,0x47,0xa1,
0xf9,0x47,0x05,0x9f,0x4c,0x3e,0x2a,0x9d,
0xa2,0x33,0xab,0x9b,0x31,0x28,0x96,0x9a,
0x12,0x1c,0xe7,0x99,0x90,0x0f,0x9a,0x99,
0xc4,0x02,0xbb,0x99,0xf3,0xf5,0x3d,0x9a,
0x4a,0xe9,0x27,0x9b,0xf9,0xdc,0xba,0x9b,
0x01,0x20,0xdf,0x78,0x9c,0x39,0xd1,0x2d,
0x9e,0x32,0xc6,0x40,0xa0,0x13,0xbc,0xbc,
0xa2,0x0a,0xb3,0x8a,0xa5,0x2d,0xab,0xbf,
0xa8,0xb1,0xa4,0x42,0xac,0x97,0x9f,0x21,
0xb0,0x0d,0x9c,0x4a,0xb4,0x09,0x9a,0xc5,
0xb8,0xaa,0x99,0x81,0xbd,0xda,0x9a,0x86,
0xc2,0xaf,0x9d,0xc4,0xc7,0x00,0xa2,0x3f,
0xcd,0xd9,0xa7,0xea,0xd2,0x07,0xaf,0xc3,
0xd8,0x85,0xb7,0xc5,0xde,0x21,0xc1,0xe4,
0xe4,0xba,0xcb,0x25,0xeb,0x28,0xd7,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,0x1a,
0x1a,0x1a,0x1a,0xf5,0x65,0x04,-1
//Number of channels: 2
//Sample Rate: 44100 Khz
};