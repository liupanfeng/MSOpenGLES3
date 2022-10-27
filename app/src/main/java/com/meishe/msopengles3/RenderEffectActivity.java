package com.meishe.msopengles3;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;


public class RenderEffectActivity extends AppCompatActivity {



    private GLSurfaceView m_glSurfaceView;
    private MSOpenGLJNIRender mMsOpenGLJNIRender;

    private int mType;

    public static void actionRenderEffectActivity(Context context,int type){
        Intent intent=new Intent(context,RenderEffectActivity.class);
        intent.putExtra("type",type);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_glSurfaceView = new GLSurfaceView(this);
        m_glSurfaceView.setEGLContextClientVersion(3);
        mType= getIntent().getIntExtra("type",0);
        //m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY); // GLSurfaceView.RENDERMODE_WHEN_DIRTY
        mMsOpenGLJNIRender = new MSOpenGLJNIRender(this,mType);
        m_glSurfaceView.setRenderer(mMsOpenGLJNIRender);
        m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        setContentView(m_glSurfaceView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }
}