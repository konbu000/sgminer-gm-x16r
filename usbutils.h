/*
 * Copyright 2012 Andrew Smith
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#ifndef USBUTILS_H
#define USBUTILS_H

#include <libusb.h>

// Use the device defined timeout
#define DEVTIMEOUT 0

// For endpoints defined in usb_find_devices.eps,
// the first two must be the default IN and OUT
#define DEFAULT_EP_IN 0
#define DEFAULT_EP_OUT 1

struct usb_endpoints {
	uint8_t att;
	uint16_t size;
	unsigned char ep;
	bool found;
};

struct usb_find_devices {
	int drv;
	const char *name;
	uint16_t idVendor;
	uint16_t idProduct;
	int config;
	int interface;
	unsigned int timeout;
	int epcount;
	struct usb_endpoints *eps;
};

struct cg_usb_device {
	struct usb_find_devices *found;
	libusb_device_handle *handle;
	pthread_mutex_t *mutex;
	struct libusb_device_descriptor *descriptor;
	uint8_t bus_number;
	uint8_t device_address;
	uint16_t usbver;
	int speed;
	char *prod_string;
	char *manuf_string;
	char *serial_string;
	unsigned char fwVersion;	// ??
	unsigned char interfaceVersion;	// ??
};

enum usb_cmds {
	C_PING = 0,
	C_CLEAR,
	C_REQUESTVERSION,
	C_GETVERSION,
	C_REQUESTFPGACOUNT,
	C_GETFPGACOUNT,
	C_STARTPROGRAM,
	C_STARTPROGRAMSTATUS,
	C_PROGRAM,
	C_PROGRAMSTATUS,
	C_PROGRAMSTATUS2,
	C_FINALPROGRAMSTATUS,
	C_SETCLOCK,
	C_REPLYSETCLOCK,
	C_REQUESTUSERCODE,
	C_GETUSERCODE,
	C_REQUESTTEMPERATURE,
	C_GETTEMPERATURE,
	C_SENDWORK,
	C_SENDWORKSTATUS,
	C_REQUESTWORKSTATUS,
	C_GETWORKSTATUS,
	C_MAX
};

struct device_api;
struct cgpu_info;

void usb_uninit(struct cgpu_info *cgpu);
bool usb_init(struct cgpu_info *cgpu, struct libusb_device *dev, struct usb_find_devices *found);
void usb_detect(struct device_api *api, bool (*device_detect)(struct libusb_device *, struct usb_find_devices *));
struct api_data *api_usb_stats(int *count);
void update_usb_stats(struct cgpu_info *cgpu);
int _usb_read(struct cgpu_info *cgpu, int ep, char *buf, size_t bufsiz, int *processed, unsigned int timeout, int eol, enum usb_cmds);
int _usb_write(struct cgpu_info *cgpu, int ep, char *buf, size_t bufsiz, int *processed, unsigned int timeout, enum usb_cmds);
void usb_cleanup();

#define usb_read(cgpu, buf, bufsiz, read, cmd) \
	_usb_read(cgpu, DEFAULT_EP_IN, buf, bufsiz, read, DEVTIMEOUT, -1, cmd)

#define usb_read_ep(cgpu, ep, buf, bufsiz, read, cmd) \
	_usb_read(cgpu, ep, buf, bufsiz, read, DEVTIMEOUT, -1, cmd)

#define usb_read_timeout(cgpu, buf, bufsiz, read, timeout, cmd) \
	_usb_read(cgpu, DEFAULT_EP_IN, buf, bufsiz, read, timeout, -1, cmd)

#define usb_read_ep_timeout(cgpu, ep, buf, bufsiz, read, timeout, cmd) \
	_usb_read(cgpu, ep, buf, bufsiz, read, timeout, -1, cmd)

#define usb_write(cgpu, buf, bufsiz, wrote, cmd) \
	_usb_write(cgpu, DEFAULT_EP_OUT, buf, bufsiz, wrote, DEVTIMEOUT, cmd)

#define usb_write_ep(cgpu, ep, buf, bufsiz, wrote, cmd) \
	_usb_write(cgpu, ep, buf, bufsiz, wrote, DEVTIMEOUT, cmd)

#define usb_write_timeout(cgpu, buf, bufsiz, wrote, timeout, cmd) \
	_usb_write(cgpu, DEFAULT_EP_OUT, buf, bufsiz, wrote, timeout, cmd)

#define usb_write_ep_timeout(cgpu, ep, buf, bufsiz, wrote, timeout, cmd) \
	_usb_write(cgpu, ep, buf, bufsiz, wrote, timeout, cmd)

#endif
