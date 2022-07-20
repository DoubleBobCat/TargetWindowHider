# TargetWindowHider_v1.1.4
# 博客地址
[**欢迎大家来看我的博客~**](https://blog.doublecat.cn/post/TargetWindowHider/)

# 更新

2022.7.20 更新v1.1.4 (Code + releases)

2022.7.19 更新v1.1.1 (Code only)

2022.7.18 更新v1.1.0 (Code + releases)

2022.7.13 更新v1.0.0 (Code + releases)

# 操作方法
1. 第一次打开程序需要根据提示输入目标程序的类名、窗口名，若不匹配则输入nullptr
2. 若需重新匹配请删除程序同目录下的config.json
3. 左右停靠
   - 拖动目标窗口至目标窗口左(右)边在屏幕左(右)侧边缘及更左(右)即可激活停靠
   - 鼠标移动至最左(右)即可唤出窗口
   - 拖动目标窗口离开停靠位置即可解除停靠状态
4. 顶部停靠
   - 拖动目标窗口使其顶部到/超过屏幕顶端即可激活停靠
   - 若目标窗口左(右)边距离屏幕左侧(右侧)距离5px，则会被识别为左(右)停靠
   - 鼠标移至显示屏顶端目标窗口x坐标范围即可唤出窗口
   - 拖动目标窗口使其顶部离开屏幕顶端即可解除停靠
5. 在停靠时，若鼠标离开目标窗口，则会自动隐藏目标窗口直到唤出
6. 拖动文件/文本等时可以唤出目标窗口
7. 点击右下角托盘即可显示/隐藏程序窗口，右键即可唤出菜单
# 下载链接(v1.1.4)

[Github releases](https://github.com/DoubleBobCat/TargetWindowHider/releases)

[蓝奏网盘(密码:exan)](https://wwm.lanzoul.com/b03j52u4j)

[百度网盘(密码:exan)](https://pan.baidu.com/s/1NV9xLTx2U8iEs5IWIo80aA?pwd=exan)

# 后续计划

1. 增加动画
3. 增加多显示器支持
3. 增加GUI

# 更新日志

- v1.1.4
  1. 增加了右下角托盘以便快速操作
  2. 增加了开机启动
- v1.1.1
  1. 修复了部分bug

- v1.1.0
  1. 新增通过配置文件自定义目标窗口
  2. 优化了代码逻辑
  3. 实现了多线程

- v1.0.0
  1. 实现了功能
