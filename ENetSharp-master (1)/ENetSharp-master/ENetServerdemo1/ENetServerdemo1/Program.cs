using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;
using ENet;


namespace ENetServerdemo1
{
    internal class Program
    {
        private static void Server()
        {
            using (var host = new Host())
            {
                host.Create(5000, 1);
                var peer = new Peer();

                while (host.Service(1) >= 0)
                {
                    Event @event;

                    while (host.CheckEvents(out @event) > 0)
                    {
                        //Console.WriteLine("Server: " + @event.Type.ToString());

                        switch (@event.Type)
                        {
                            case EventType.Connect:
                                peer = @event.Peer;
                                for (var i = 0; i < 200; i++)
                                {
                                    peer.Send((byte)i, new byte[] { 0, 0 });
                                }
                                break;

                            case EventType.Receive:
                                var data = @event.Packet.GetBytes();
                                var value = BitConverter.ToUInt16(data, 0);
                                if (value % 1000 == 1)
                                {
                                    Console.WriteLine("  Server: Ch={0} Recv={1}", @event.ChannelID, value);
                                }
                                value++;
                                peer.Send(@event.ChannelID, BitConverter.GetBytes(value));
                                @event.Packet.Dispose();
                                break;
                        }
                    }
                }
            }
        }
        static void Main(string[] args)
        {
            Console.WriteLine("ENet server");
            Library.Initialize();

            var server = new Thread(Server);
            server.Start();
            Thread.Sleep(250);
            //var client = new Thread(Client);
            //client.Start();

             server.Join();
           // client.Join();

            Library.Deinitialize();
            Console.WriteLine("Press Enter to exit");
            Console.ReadLine();
        }
    }
}
