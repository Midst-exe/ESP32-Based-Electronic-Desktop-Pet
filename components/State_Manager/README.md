# 全局状态管理层
用于向所有组件声明所有活动状态，必要时提供某些任务的状态管理（修改和查询）


# 对于  配网状态（wifi_config_status）的管理
wifi_config_status的存储在state_manager.cpp中，方便其他组件访问。
（不存储在wifi_init_task.cpp中原因：其他组件没有设置接收wifi_config_status的接口）

对于查询状态：所有组件可以使用相应API查询当前状态。

对于修改当前状态：仅WiFi组件拥有修改wifi_config_status的权限。但由于wifi_config_status存放在state_manager.cpp中，这里为避免头文件私有化问题（state_manager中的头文件无法只向wifi组件提供API），采用事件回调形式，由wifi_connnect发布WIFI_CONFIG_EVENT_STATUS_CHANGED_REQUIRE状态修改请求事件，通知state_manager修改状态。

**以下是常用API**
- 查询配网状态：wifi_config_get_status()
- 查询wifi状态： wifi_get_status()