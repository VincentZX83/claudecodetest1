// ============================================================
// test_stm32_gpio.cpp — Unit tests for STM32 GPIO driver
// NOTE: These tests validate register manipulation logic without
// requiring actual STM32 hardware (mock-mode tests).
// ============================================================

#include <gtest/gtest.h>
#include <cstdint>
#include "stm32_gpio.hpp"

using namespace cccs::stm32;

// ========== GPIOPin ==========

TEST(GPIOPinTest, Values) {
    GPIOPin p0{0};
    GPIOPin p7{7};
    GPIOPin p15{15};

    EXPECT_EQ(p0.value(), 0u);
    EXPECT_EQ(p0.mask(), 0x0001u);
    EXPECT_EQ(p7.mask(), 0x0080u);
    EXPECT_EQ(p15.mask(), 0x8000u);
}

// ========== GPIOMode Enum Values ==========

TEST(GPIOModeTest, EnumValues) {
    // Verify register-level values match STM32 TRM
    EXPECT_EQ(static_cast<uint32_t>(GPIOMode::Input),     0x0u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOMode::Output),    0x1u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOMode::Alternate), 0x2u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOMode::Analog),    0x3u);
}

// ========== GPIOSpeed — verify the bug fix ==========

TEST(GPIOSpeedTest, EnumValuesMatchTRM) {
    // Critical: verify "High" is 0x3, not 0x2
    // (this is the bug Claude Code v1 introduced)
    EXPECT_EQ(static_cast<uint32_t>(GPIOSpeed::Low),    0x0u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOSpeed::Medium), 0x1u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOSpeed::Fast),   0x2u);
    EXPECT_EQ(static_cast<uint32_t>(GPIOSpeed::High),   0x3u);
}

// ========== GPIOPort Construction ==========

TEST(GPIOPortTest, Construction) {
    GPIOPort port{GPIOA_BASE};
    EXPECT_EQ(port.base_address(), GPIOA_BASE);

    // Move construction
    GPIOPort moved{std::move(port)};
    EXPECT_EQ(moved.base_address(), GPIOA_BASE);
    EXPECT_EQ(port.base_address(), 0u);  // source is nulled
}

// ========== GPIOManager RAII ==========

TEST(GPIOManagerTest, Construction) {
    GPIOManager mgr;
    // Manager owns three ports with correct base addresses
    EXPECT_EQ(mgr.port_a().base_address(), GPIOA_BASE);
    EXPECT_EQ(mgr.port_b().base_address(), GPIOB_BASE);
    EXPECT_EQ(mgr.port_c().base_address(), GPIOC_BASE);
}

// ========== Register address calculations ==========

TEST(RegisterTest, AddressCalculations) {
    EXPECT_EQ(GPIOA_BASE + 0x00, 0x40020000u);  // MODER
    EXPECT_EQ(GPIOA_BASE + 0x14, 0x40020014u);  // ODR
    EXPECT_EQ(GPIOA_BASE + 0x18, 0x40020018u);  // BSRR
    EXPECT_EQ(GPIOB_BASE, 0x40020400u);
    EXPECT_EQ(RCC_BASE,   0x40023800u);
}
