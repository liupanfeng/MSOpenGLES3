package com.meishe.msopengles3;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * All rights Reserved, Designed By www.meishesdk.com
 *
 * @Author: lpf
 * @CreateDate: 2022/10/24 下午2:54
 * @Description:
 * @Copyright: www.meishesdk.com Inc. All rights reserved.
 */
public class MSPlayer implements GLSurfaceView.Renderer {


    private Context mContext;

    public MSPlayer(Context mContext) {
        this.mContext = mContext;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        AssetManager assetManager = mContext.getAssets();
        ndkInitGL(assetManager);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        ndkResizeGL(i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        ndkPaintGL();
    }

    /**
     * 视频播放器初始化
     */
    public void InitVideoPlayer()
    {
        ndkInitVideoPlayer();
    }

    /**
     * 设置视频的宽高
     * @param fileString
     */
    public void StartVideoPlayerWithPath(String fileString)
    {
        ndkStartPlayerWithFile(fileString);
    }

    /**
     * 暂停视频
     */
    public void PauseVideoPlayer()
    {
        ndkPauseVideoPlay();
    }

    /**
     * 停止视频
     */
    public void StopVideoPlayer()
    {
        ndkStopVideoPlayer();
    }

    /**
     * 获取视频的宽高比
     * @return
     */
    public float GetVideoSizeRatio()
    {
        float ratio = ndkGetVideoSizeRatio();
        return ratio;
    }

    /**
     * 获取视频的总时长
     * @return
     */
    public float GetVideoTotalSeconds()
    {
        float ratio = ndkGetVideoTotalSeconds();
        return ratio;
    }

    /**
     * 调到指定的位置
     * @param pos
     */
    public void SeekVideoPlayer(float pos)
    {
        ndkSeekMedia(pos);
    }


    public void OnVideoRenderCallback()
    {
        ViewPlayerActivity activity = (ViewPlayerActivity)mContext;
        if(activity != null) {
            activity.UpdateVideoRenderCallback();
        }

    }

    /*-------------------------------------------native---------------------------------------*/

    static {
        System.loadLibrary("msopengles3");
    }


    private native  void ndkInitVideoPlayer();

    private native  void ndkStartPlayerWithFile(String fileName);
    private native  void ndkPauseVideoPlay();
    private native  void ndkStopVideoPlayer();
    private native  float ndkGetVideoSizeRatio();
    private native  float ndkGetVideoTotalSeconds();

    private native  void ndkSeekMedia(float nPos);
    private native  void ndkInitGL(AssetManager assetManager);
    private native  void ndkPaintGL();
    private native  void ndkResizeGL(int width,int height);

}
