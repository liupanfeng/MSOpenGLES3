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
 * @CreateDate: 2022/10/21 下午2:09
 * @Description:
 * @Copyright: www.meishesdk.com Inc. All rights reserved.
 */
public class MSOpenGLJNIRender implements GLSurfaceView.Renderer {




    private Context mContext;

    private int mType;



    public MSOpenGLJNIRender(Context context,int type){
        mContext = context;
        mType=type;
    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        AssetManager assetManager = mContext.getAssets();
        jniInitGL(assetManager,mType);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        jniResizeGL(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        jniPaintGL();
    }


    static {
        System.loadLibrary("msopengles3");
    }


    private native  void jniInitGL(AssetManager assetManager,int type);
    private native  void jniPaintGL();
    private native  void jniResizeGL(int width,int height);

    private native void jniOnDestroy();

}
