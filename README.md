# SuperPost

我给它起这个名字的意思是超级投递，主要实现多任务的UDP的发送和接收。

- 该软件采用VC++进行开发的网络通信软件。
- 该软件可创建多个通信任务，实现和多个接收机同时进行UDP通信，每个任务都提供了独立的进度显示。该软件最大特点是多任务，数据吞吐量大，尽可能不丢包。
- 该软件的难点是多线程，用户可根据需要实时添加任务，每添加一个任务就需要创建一个通信的线程。
