# 什么是内存池？

内存池就像一个水池一样，是提前申请好一大片空间，然后等需要使用内存的时候，就从这个内存池里分配所需要的空间，如果内存使用完需要释放的时候，先不把内存还给系统，而是把不要的内存放到内存池里，以供下次需要时继续分配。

# 为什么需要内存池？

那先来说一下不要内存池的缺点：

- 容易造成外部碎片：

  举个例子：

  ![](https://img-blog.csdnimg.cn/20190515131943661.png)

  这些蓝色的区域表示分配出去的内存，如果这个时候我想申请一个128字节的区域，就不可以被申请到了，因为，白色的区域是分散开的，并不连续，所以无法分配。

- 效率低：

  比如我们一次申请4个字节，需要申请10000次，那么每次都需要向系统申请空间，这就很明显的造成了低效的结果。

# 对象池：

这里我先实现了一个对象池，就是每次开辟一段空间，这段空间可以放下N个对象。每次开辟的空间是一个2倍增长的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190515132056306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjY3ODUwNw==,size_16,color_FFFFFF,t_70)

这里是用一个链表把每一块内存连起来，每次当 _free_list没有结点的时候，就从最后一次申请的内存块里分配所需要大小的字节数。当用户用完需要还回来的时候，就把还回来的内存头插到 _free_list中。下次优先从 _free_list中分配内存，是一个头删操作。

