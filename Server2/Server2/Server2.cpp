// Server2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <enet/enet.h>
using namespace std;
int main()
{

    //初始化enet库
    if (enet_initialize())
    {
        cout << "初始化失败" << endl;
        return 0;
    }
    cout << "初始化成功" << endl;
    //下面创建服务器
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 5000;

    ENetHost* server;
    server = enet_host_create(&address, //主机地址
        32,        //允许的连接数
        2,
        0, //自动管理带宽
        0);                        //********************************修改，这行是自己新增的
    if (server == NULL)
    {
        cout << "创建主机对象失败" << endl;
        enet_deinitialize();
        return 0;
    }
    cout << "创建Server主机对象成功" << endl;

    //下面开始收数据等
    ENetEvent event;
    //ENetPacketFlag eNetPacketFlag;
    //int i = 0;
    while (enet_host_service(server, &event, 5000) >= 0)
    {
        if (event.type == ENET_EVENT_TYPE_CONNECT) //有客户机连接成功
        {
            static unsigned int num = 0;
            ENetAddress remote = event.peer->address; //远程地址
            char ip[256];
            enet_address_get_host_ip(&remote, ip, 256);
            cout << "ip:" << ip << " 已经连接,序号:" << num << endl;
            event.peer->data = (void*)num++;

            /* 创建一个包含“packet\0”的大小为 7 的可靠数据包 */
            ENetPacket* packet = enet_packet_create("packet",
                strlen("数据包") + 1,
                ENET_PACKET_FLAG_RELIABLE);
            /* 扩展数据包并附加字符串“foo”，所以现在 */
                /* 包含 "packetfoo\0" */
            enet_packet_resize(packet, strlen("packetfoo") + 1);
            strcpy((char*)&packet -> data[strlen("packet")], "foo");

            /* 通过通道 id 0 将数据包发送给对等方。 */
            /* 也可以通过 */ 
                /* enet_host_broadcast（主机，0，数据包）；*/
                enet_peer_send(event.peer, 0, packet);

                /* 可以只使用 enet_host_service() 代替。*/
                enet_host_flush(server);
                cout << "发送数据:" << (char*)packet->data << endl;
        }
        else if (event.type == ENET_EVENT_TYPE_RECEIVE) //收到数据
        {
            cout << "收到序号" << event.peer->data << "的数据,从" << event.channelID << "通道发送" << endl;
            cout << "数据大小:" << event.packet->dataLength << endl;
            cout << "数据:" << (char*)event.packet->data << endl;
            enet_packet_destroy(event.packet);    //注意释放空间
            cout << endl;
        }
        else if (event.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
        {
            cout << "序号" << event.peer->data << "远程已经关闭连接" << endl;
        }
       // i++;
        //cout << i << endl;
    }

    enet_deinitialize();
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
