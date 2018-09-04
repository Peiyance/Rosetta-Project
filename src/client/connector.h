

// interfaces
int init_connector(char remoteIP[], short remotePort);                                 // 与服务器建立连接。将会创建新的线程pthread
int req_authentication(char *str_username, char *str_password, void (*callback)(int)); // 发送登陆请求。结果将在回调中给出

//
static void *pthread(void *arg); // socket接收线程。所有回调函数也将在这一线程中执行，所以回调函数应尽可能简短，以免阻塞消息接受
void socket_write(char *msg);
//void socket_read_cb(int fd, short event, void *arg);