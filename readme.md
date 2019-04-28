# 使用方法：
mkdir build <br>
cd build  <br>
cmake ..  <br>
make  <br>

# 之后你需要在build文件夹中添加以下文件：<br>

video.avi 内容为需标记的视频   <br>
添加两个文件夹：positive，negative   <br>

# 可以运行之后就可以按照提示操作 <br>
当目标变化不大时，可以一直按住空格键  <br>
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



