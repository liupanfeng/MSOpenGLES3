package com.meishe.msopengles3.capture;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.Surface;

import java.io.IOException;

/**
 * All rights Reserved, Designed By www.meishesdk.com
 *
 * @Author: lpf
 * @CreateDate: 2022/10/28 下午1:38
 * @Description:
 * @Copyright: www.meishesdk.com Inc. All rights reserved.
 */
public class MSCamera implements Camera.PreviewCallback {

    private Camera m_camera;
    private int m_camID;
    private Camera.Size m_videoSize;
    private MSOpenCVRender m_cvRender;

    private int[] m_camTexture = new int[1];
    /**
     * 注意这个是如何使用的，这个承担什么样的责任   纹理为了创建SurfaceTexture  预览通过SurfaceTexture方式预览
     */
    private SurfaceTexture m_texture;
    private GLSurfaceView m_glView;

    public MSCamera(MSOpenCVRender render, GLSurfaceView glView) {
        m_cvRender = render;
        m_glView = glView;
        m_camID = Camera.CameraInfo.CAMERA_FACING_FRONT;

        createOesTexture();
        m_texture = new SurfaceTexture(m_camTexture[0]);
    }

    public void InitCamPermissionGranted() {
        m_camera = initCamera();
        try {
//            updateDisplayOrientation(m_camID, m_camera);
            Camera.Parameters params = m_camera.getParameters();
            params.setPreviewFormat(ImageFormat.NV21);
            m_camera.setParameters(params);
            m_camera.setPreviewTexture(m_texture);
            m_camera.setPreviewCallback(this);
            m_camera.startPreview();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private Camera initCamera() {
        Camera cam;
        try {
            cam = Camera.open(m_camID);
        } catch (Exception e) {
            cam = null;
            e.printStackTrace();
        }
        return cam;
    }


    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
//        Log.d("lpf","onPreviewFrame----");
        m_videoSize = camera.getParameters().getPreviewSize();
        if (m_cvRender != null) {
            m_cvRender.UpdateCameraFrame(bytes, m_videoSize.width, m_videoSize.height);
        }
    }

    private void createOesTexture() {
        GLES30.glGenTextures(1, m_camTexture, 0);
    }

    public void DestoryCamera() {
        if (m_camera != null) {
            m_camera.setPreviewCallback(null);
            m_camera.stopPreview();
            m_camera.release();
            m_camera = null;
        }
    }

    /**
     * 1.opencv 层如果做了旋转  这里可以不用处理
     * 2.SurfaceTexture 存在的意义何在
     * @param id
     * @param cam
     */
    private void updateDisplayOrientation(int id, Camera cam) {
        if (cam == null) {
            return;
        }
        Activity targetActivity = (Activity) m_glView.getContext();
        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(id, info);
        int rotation = targetActivity.getWindowManager().getDefaultDisplay().getRotation();
        int degree = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degree=0;
                break;
            case Surface.ROTATION_90:
                degree=90;
                break;
            case Surface.ROTATION_180:
                degree=180;
                break;
            case Surface.ROTATION_270:
                degree=270;
                break;
        }

        int displayDegree;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT){
            displayDegree=(info.orientation+degree)%360;
            displayDegree=(360-displayDegree)%360;
        }else{
            displayDegree=(info.orientation-degree+360)%360;
        }

        Log.d("lpf","displayDegree----"+displayDegree);
        cam.setDisplayOrientation(displayDegree);
    }


}
