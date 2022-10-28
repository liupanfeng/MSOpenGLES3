实现的效果：
* 1.各种特效渲染
* 2.ffmpeg视频播放器（包含seek功能，暂停，开始等）
  * DTS（Decoding Time Stamp）：即解码时间戳，这个时间戳的意义在于告诉播放器该在什么时候解码这一帧的数据。
  * PTS（Presentation Time Stamp）：即显示时间戳，这个时间戳用来告诉播放器该在什么时候显示这一帧的数据。
* 3.拍摄通过opengles进行渲染



源码介绍：
* 1.ms_opengles 这个文件夹里边放置的是特效相关的源码
* 2.ms_video_player 这个文件夹放置的是ffmpeg播放器相关的源码

