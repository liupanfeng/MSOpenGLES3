package com.meishe.msopengles3;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.TextView;

import com.meishe.msopengles3.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {



    private GLSurfaceView m_glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_glSurfaceView = new GLSurfaceView(this);
        m_glSurfaceView.setEGLContextClientVersion(3);

        //m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY); // GLSurfaceView.RENDERMODE_WHEN_DIRTY
        m_glSurfaceView.setRenderer(new MSOpenGLJNIRender(this));

        setContentView(m_glSurfaceView);
    }

}