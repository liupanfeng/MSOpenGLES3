package com.meishe.msopengles3.capture;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * All rights Reserved, Designed By www.meishesdk.com
 *
 * @Author: lpf
 * @CreateDate: 2022/10/28 下午1:39
 * @Description:
 * @Copyright: www.meishesdk.com Inc. All rights reserved.
 */
public class MSOpenCVRender implements GLSurfaceView.Renderer {

    private Context m_contex;


    public MSOpenCVRender(Context ctx){
        m_contex = ctx;
    }


    public void UpdateCameraFrame(byte[] bytes, int width, int height) {
        nativeUpdateCameraFrame(bytes,width,height);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        AssetManager assets = m_contex.getAssets();
        nativeInitGL(assets);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        nativeResize(i,i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        nativeDrawGL();
    }



    public void UpdateRequestGLRender()
    {
        if(m_contex != null){
            CaptureActivity activity = (CaptureActivity) m_contex;
            activity.RequestOpenGLRender();
        }
    }


    /*---------------------------------native-------------------------------------*/


    static {
        System.loadLibrary("msopengles3");
        System.loadLibrary("opencv_java4");
    }


    private native void nativeInitGL(AssetManager assetManager);

    private native void nativeDrawGL();

    private native void nativeResize(int width,int height);

    private native void nativeUpdateCameraFrame(byte[] yuvFrame,int width,int height);


}
