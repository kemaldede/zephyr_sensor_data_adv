#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

// OUR CUSTOM SERVICE
#define BT_UUID_CUSTOM_SERVICE BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x10, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA0)
// CUSTOM TEMPERATURE CHARACTERISTIC
#define BT_CUSTOM_UUID_TEMP BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x11, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA1)
// CUSTOM HUMIDITY CHARACTERISTIC
#define BT_CUSTOM_UUID_HUM BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x11, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA2)

void custom_service_init();
int64_t read_T(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
int64_t read_H(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void T_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);
void H_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);


uint8_t bTNotify; // Temperature Notify 
uint8_t bHNotify; // Humidity Notify

void T_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	bTNotify = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

void H_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	bHNotify = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

// read temperature
float T_vals[1] = {0};
int64_t read_T(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			   void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(T_vals));
}	

// read humidity
float H_vals[1] = {0};
int64_t read_H(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			   void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(H_vals));
}	

struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERVICE),
		BT_GATT_CHARACTERISTIC(BT_CUSTOM_UUID_TEMP, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ_AUTHEN, read_T, NULL,T_vals),
		BT_GATT_CCC(T_ccc_cfg_changed, BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN ),
		BT_GATT_CHARACTERISTIC(BT_CUSTOM_UUID_HUM, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ_AUTHEN, read_H, NULL,H_vals),
		BT_GATT_CCC(H_ccc_cfg_changed, BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN ),
};

struct bt_gatt_service th_svc = BT_GATT_SERVICE(attrs);

void custom_service_init()
{
	bt_gatt_service_register(&th_svc);
}
