# 区间扫描线

#### 1. 算法思想

区间扫描线，基本思想是扫描线，从上往下扫描，对于一个高度上的扫描线，我们知道两个边之间的颜色是固定的

<img src="./docs/Screen Shot 2019-01-05 at 12.21.48 PM.png" width=600px>

为了实现目标，首先需要维护ET(边表)和AET(活化边表)。ET以**每条边的最大y值**分类存储了每条边的信息，当一轮扫描开始的时候，将ET中当前y值的边加入到AET当中，AET会把那些已经走到尽头的边给舍弃。AET此时就是当前高度的所有边的信息。此时我们需要先按x值对AET进行排序，然后在相邻的两条边之间进行求出深度最小的边是谁，然后进行渲染。

求深度最小可以通过一个set进行维护，将复杂度降到o(n)，具体可见代码。

所以其实也可以不需要APT表和PT表。

#### 2. 编译

本代码只在mac上测试过，但是没有用任何操作系统相关函数并使用cmake编译，应该会很容易复现

直接在终端下运行buildAll.sh即可编译

```bash
cd scanline
bash buildAll.sh
```

#### 3. 运行

编译完成后会在文件夹中发现test可执行文件，test文件默认读取的是models/al.obj, 通过添加参数可以直接读取，下面是一些例子

```bash
./test
./test -read ./models/Low-Poly-Racing-Car.obj
# 相对路径
./test -read /Users/basasuya/Desktop/scanline/models/Low-Poly-Racing-Car.obj
# 绝对路径
```

#### 4. 未来工作

本代码没有实现贯穿的处理，没有实现视角转动