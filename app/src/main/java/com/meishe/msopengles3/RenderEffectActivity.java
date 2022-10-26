package com.meishe.msopengles3;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;


public class RenderEffectActivity extends AppCompatActivity {



    private GLSurfaceView m_glSurfaceView;
    private MSOpenGLJNIRender mMsOpenGLJNIRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_glSurfaceView = new GLSurfaceView(this);
        m_glSurfaceView.setEGLContextClientVersion(3);

        //m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY); // GLSurfaceView.RENDERMODE_WHEN_DIRTY
        mMsOpenGLJNIRender = new MSOpenGLJNIRender(this);
        m_glSurfaceView.setRenderer(mMsOpenGLJNIRender);
        m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        setContentView(m_glSurfaceView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }
}