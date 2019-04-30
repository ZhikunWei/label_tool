# 使用方法：
##需要安装opencv, 更换main中的video路径 <br>
mkdir build <br>
cd build  <br>
cmake ..  <br>
make  <br>
./label_tool <br>

### 之后你需要在build文件夹中添加以下文件：<br>
* video.avi 内容为需标记的视频   <br>
现在不需要添加文件夹，也暂时不需要做负例  <br>
### 数据集共享
每标记一个id的装甲板，在build文件夹中都应出现该id对应的文件夹，将其压缩并在其文件名末尾添加当前日期，如“id3_0430.zip”以便于区分。。
百度网盘群组： https://pan.baidu.com/mbox/homepage?short=pNqIv8n  <br>
加入后上传进来，我们将数据集中在这里，需要使用时再来全部下载

# 可以运行之后就可以按照提示操作 <br>
当目标变化不大时，可以一直按住空格键或r  <br>
 注意：1）只有按下space才会储存标签。对box进行操作时也会进入下一帧，但是box不会被储存。 <br>
  2） box应当完全包含完整的装甲板，包括数字和两侧灯条。 <br>
  3）选择标签的原则为标记尽可能多状态下的装甲板，如角度，亮度，大小，模糊程度等。可以不用在同一个场景（如视频中车没有动了）下一直标记，
  在那个场景标记了一定数量（100张）后，可适当跳过一些重复或者较相似的帧。

# 操作指南
space : 选定当前鼠标位置/储存当前box为label，并进入下一帧 <br>
r : 重新进入选取box模式/跳过当前图片 <br>
w : 向上移动box <br>
s : 向下移动box <br>
a : 向左移动box <br>
d : 向右移动box <br>
q : 等比缩小box <br>
e : 等比放大box <br>
j : 水平减小box <br>
l : 水平放大box <br>
i : 垂直放大box <br>
k : 垂直减小box <br>
 <br>



