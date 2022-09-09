#ifndef H_CRYPTID_NUMBERS
#define H_CRYPTID_NUMBERS

  #include "cryptid-types.h"

  uint8_t mask_0[3 * 6] = {
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
  };
  pixel_mask_t numberMask_0 = {mask_0, 3, 6};

  uint8_t mask_1[3 * 6] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
  };
  pixel_mask_t numberMask_1 = {mask_1, 3, 6};

  uint8_t mask_2[3 * 6] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
  };
  pixel_mask_t numberMask_2 = {mask_2, 3, 6};

  uint8_t mask_3[3 * 6] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
  };
  pixel_mask_t numberMask_3 = {mask_3, 3, 6};

  uint8_t mask_4[3 * 6] = {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
  };
  pixel_mask_t numberMask_4 = {mask_4, 3, 6};

  uint8_t mask_5[3 * 6] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
  };
  pixel_mask_t numberMask_5 = {mask_5, 3, 6};

  uint8_t mask_6[3 * 6] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
  };
  pixel_mask_t numberMask_6 = {mask_6, 3, 6};

  uint8_t mask_7[3 * 6] = {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
  };
  pixel_mask_t numberMask_7 = {mask_7, 3, 6};

  uint8_t mask_8[3 * 6] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
  };
  pixel_mask_t numberMask_8 = {mask_8, 3, 6};

  uint8_t mask_9[3 * 6] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
  };
  pixel_mask_t numberMask_9 = {mask_9, 3, 6};

  pixel_mask_t numberMasks[10] = {
    numberMask_0,
    numberMask_1,
    numberMask_2,
    numberMask_3,
    numberMask_4,
    numberMask_5,
    numberMask_6,
    numberMask_7,
    numberMask_8,
    numberMask_9
  };

#endif
