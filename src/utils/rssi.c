#include <zephyr/types.h>

#include <net/buf.h>
#include <bluetooth/hci.h>

#include <rssi.h>

int8_t rssi = 0;
uint16_t conn_handle;

int8_t get_rssi() {
  return rssi;
}

void read_rssi(struct bt_conn *conn)
{
	struct net_buf *rsp;
	struct net_buf *command;
	int err;

	err = bt_hci_get_conn_handle(conn, &conn_handle);
	if (err) {
			printk("No connection handle (err %d)\n", err);
	}
	// uint16_t conn_handle = 0; // connection handle is always zero and is not accessible in bt_conn anyway
	command = bt_hci_cmd_create(BT_HCI_OP_READ_RSSI, 2);
	if (!command) {
		printk("No HCI command buffer!\n");
		return;
	}
	// add the connection handle argument to the buffer
	net_buf_add_le16(command, conn_handle);
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_RSSI, command, &rsp);
	if (err) {
		printk("HCI command failed (err %d)\n", err);
		rssi = -127;
	} else {
	    rssi = (int8_t) rsp->data[3];
	}
	printk("RSSI: %d db \n",rssi);
    net_buf_unref(rsp);
}