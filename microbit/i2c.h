/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_I2C_H_
#define MICROBIT_I2C_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief      Initializes the I2C peripheral
 */
void i2c_init(void);

/**
 * @brief      Writes data to the I2C device
 *
 * @param[in]  dev_addr  The 7 bit I2C device address
 * @param[in]  reg_addr  The register address
 * @param[in]  data      The data buffer
 * @param[in]  length    The data length
 *
 * @return     Returns the number of bytes written to the device or negative
 *             value on error
 */
int i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, size_t length);

/**
 * @brief      Reads data from the I2C device
 *
 * @param[in]  dev_addr  The 7 bit I2C device address
 * @param[in]  reg_addr  The register address
 * @param      data      The data buffer
 * @param[in]  length    The data length
 *
 * @return     Returns the number of bytes read from the device or negative
 *             value on error
 */
int i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t length);

#endif /* MICROBIT_I2C_H_ */
