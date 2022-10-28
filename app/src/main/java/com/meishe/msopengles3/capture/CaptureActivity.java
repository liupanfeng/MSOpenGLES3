package com.meishe.msopengles3.capture;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import com.meishe.msopengles3.R;

public class CaptureActivity extends AppCompatActivity {

    private  final int FILE_VIDEO_REQUEST_CODE = 100;
    private  final int REQUEST_EXTERNAL_STORAGE = 1;

    private  final int REQUEST_CAMERA = 2;
    private  String[] PERMISSIONS_STORAGE = {Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE };
    private  String[] PERMISSIONS_CAMERA = new String[]{Manifest.permission.CAMERA};

    private MSCamera    m_cvCamera;
    private GLSurfaceView m_glSurfaceView;
    private MSOpenCVRender          m_cvRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        requestWindowFeature(Window.FEATURE_NO_TITLE);// 隐藏标题栏
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);// 隐藏状态栏

        setContentView(R.layout.activity_capture);

        m_cvRender = new MSOpenCVRender(this);
        m_glSurfaceView = findViewById(R.id.glSurfaceView);
        m_glSurfaceView.setEGLContextClientVersion(3);
        m_glSurfaceView.setRenderer(m_cvRender);
        m_glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        m_glSurfaceView.bringToFront();

        m_cvCamera=new MSCamera(m_cvRender,m_glSurfaceView);

        verifyStoragePermissions();


    }


    private  void verifyStoragePermissions() {
        int i = ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA);
        if (i!= PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this,PERMISSIONS_CAMERA,REQUEST_CAMERA);
        }else{
            m_cvCamera.InitCamPermissionGranted();
        }

        int ret=ActivityCompat.checkSelfPermission(this,Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (ret!=PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this,PERMISSIONS_STORAGE,REQUEST_EXTERNAL_STORAGE);
        }
    }

    /**
     * native 层反射调用这个方法来刷新数据
     */
    public void RequestOpenGLRender()
    {
        m_glSurfaceView.requestRender();
    }


    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        m_cvCamera.DestoryCamera();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == REQUEST_CAMERA&&grantResults.length>0&&
                grantResults[0]==PackageManager.PERMISSION_GRANTED){
            m_cvCamera.InitCamPermissionGranted();
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
}