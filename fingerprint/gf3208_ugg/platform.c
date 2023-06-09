/*
 * platform indepent driver interface
 *
 * Coypritht (c) 2017 Goodix
 * Copyright (C) 2018 XiaoMi, Inc.
 */
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/timer.h>
#include <linux/err.h>

#include "gf_spi.h"

#if defined(USE_SPI_BUS)
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#elif defined(USE_PLATFORM_BUS)
#include <linux/platform_device.h>
#endif

int xiaomi_ugg_gf_parse_dts(struct gf_dev* gf_dev)
{
	int rc = 0;

#if 1
	gf_dev->pwr_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_ldo", 0);
	if (!gpio_is_valid(gf_dev->pwr_gpio)) {
		pr_info("gpio ldo is invalid\n");
		return -1;
	} else {
		pr_debug("gf:ldo gpio:%d\n", gf_dev->pwr_gpio);
		rc = gpio_request(gf_dev->pwr_gpio, "goodix_pwr");
		if (rc) {
			dev_err(&gf_dev->spi->dev, "Failed to request PWR GPIO. rc = %d\n", rc);
			return -1;
		}
		gpio_direction_output(gf_dev->pwr_gpio, 1);

	}
#endif
	msleep(11);
 	printk("gf3208 msleep 11ms\n");

	gf_dev->reset_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_reset", 0);
	if (!gpio_is_valid(gf_dev->reset_gpio)) {
		pr_info("RESET GPIO is invalid.\n");
		return -1;
	} else {
		pr_debug("gf:reset_gpio:%d\n", gf_dev->reset_gpio);
#if 1
		rc = gpio_request(gf_dev->reset_gpio, "goodix_reset");
		if (rc) {
			dev_err(&gf_dev->spi->dev, "Failed to request RESET GPIO. rc = %d\n", rc);
			return -1;
		}
		gpio_direction_output(gf_dev->reset_gpio, 1);
		gpio_free(gf_dev->reset_gpio);
#endif
	}
	gf_dev->irq_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_irq", 0);
	if (!gpio_is_valid(gf_dev->irq_gpio)) {
		pr_info("IRQ GPIO is invalid.\n");
		return -1;
	} else {
		pr_info("gf:irq_gpio:%d\n", gf_dev->irq_gpio);
#if 1
		rc = gpio_request(gf_dev->irq_gpio, "goodix_irq");
		if (rc) {
			dev_err(&gf_dev->spi->dev, "Failed to request IRQ GPIO. rc = %d\n", rc);

		}
		gpio_direction_input(gf_dev->irq_gpio);
		gpio_free(gf_dev->irq_gpio);
#endif
	}

	return 0;
}

void xiaomi_ugg_gf_cleanup(struct gf_dev	* gf_dev)
{
	pr_info("[info] %s\n", __func__);
	if (gpio_is_valid(gf_dev->irq_gpio)) {
		gpio_free(gf_dev->irq_gpio);
		pr_info("remove irq_gpio success\n");
	}
	if (gpio_is_valid(gf_dev->reset_gpio)) {
		gpio_free(gf_dev->reset_gpio);
		pr_info("remove reset_gpio success\n");
	}
}

int xiaomi_ugg_gf_power_on(struct gf_dev* gf_dev)
{
	int rc = 0;

	msleep(10);
	pr_info("---- power on ok ----\n");

	return rc;
}

int xiaomi_ugg_gf_power_off(struct gf_dev* gf_dev)
{
	int rc = 0;

	pr_info("---- power off ----\n");
	return rc;
}

int xiaomi_ugg_gf_hw_reset(struct gf_dev *gf_dev, unsigned int delay_ms)
{
	int rc = 0;

	if (gf_dev == NULL) {
		pr_info("Input Device is NULL.\n");
		return -1;
	}

	if (gpio_is_valid(gf_dev->reset_gpio)) {
		rc = gpio_request(gf_dev->reset_gpio, "goodix_reset");
		if (rc) {
			dev_err(&gf_dev->spi->dev, "Failed to request RESET GPIO. rc = %d\n", rc);

		}
		gpio_direction_output(gf_dev->reset_gpio, 1);

		gpio_direction_output(gf_dev->reset_gpio, 1);
		gpio_set_value(gf_dev->reset_gpio, 0);
		mdelay(20);
		gpio_set_value(gf_dev->reset_gpio, 1);
		mdelay(delay_ms);
		gpio_free(gf_dev->reset_gpio);
	} else {
		dev_err(&gf_dev->spi->dev, "reset gpio not valid when reset delay time is %d\n", delay_ms);
		return -1;
	}

	return rc;
}

int xiaomi_ugg_gf_irq_num(struct gf_dev *gf_dev)
{
	if (gf_dev == NULL) {
		pr_info("Input Device is NULL.\n");
		return -1;
	} else {
		if (gpio_is_valid(gf_dev->irq_gpio))
			return gpio_to_irq(gf_dev->irq_gpio);
		else
			return -1;
	}
}

