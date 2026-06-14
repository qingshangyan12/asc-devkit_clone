#ifndef BANK_CONFLICT_CONFIG_H
#define BANK_CONFLICT_CONFIG_H

#include "kernel_operator.h"

constexpr uint32_t TOTAL_LENGTH = 4096;

#if CASE_NUM == 1
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x20000;

#elif CASE_NUM == 2

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x10000;

#elif CASE_NUM == 3

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10000;
constexpr uint32_t zLocalAddr = 0x14100;

#elif CASE_NUM == 4

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x4000;

#elif CASE_NUM == 5

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x4020;

#elif CASE_NUM == 6

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x10100;

#elif CASE_NUM == 7

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x10120;

#elif CASE_NUM == 8

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x10100;

#elif CASE_NUM == 9

constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x10100;
constexpr uint32_t zLocalAddr = 0x10100;

#endif

#endif // BANK_CONFLICT_CONFIG_H
