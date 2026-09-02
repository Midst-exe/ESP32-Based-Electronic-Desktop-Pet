# wifi模式是STA + AP
1. 利用AP+web服务器获取WLAN的ssid和password配网

2. 利用事件通知方式，wifi_handler监听**websocket的回传信息事件**。有则进行连接。