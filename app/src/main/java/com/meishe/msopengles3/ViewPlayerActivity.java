package com.meishe.msopengles3;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

public class ViewPlayerActivity extends AppCompatActivity {

    private final int FILE_VIDEO_REQUEST_CODE = 100;
    private final String TAG = "ViewPlayerActivity";
    private final int REQUEST_EXTERNAL_STORAGE = 1;
    private String[] PERMISSIONS_STORAGE = {Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private GLSurfaceView mGlSurfaceView;
    private MSPlayer mMSPlayer;
    private SeekBar mSeekBar;

    private float mVideoRatio;
    private float mVideoTotalSeconds;
    private TextView mTotalDuration;
    private TextView mCurrentPlaytime;
    private ImageView mBtnPlay;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        verifyStoragePermissions(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_player);

        mVideoRatio = 0;
        mVideoTotalSeconds = 0;

        mSeekBar = (SeekBar) findViewById(R.id.seekBar);
        mTotalDuration = (TextView) findViewById(R.id.totalDuration);
        mCurrentPlaytime = (TextView) findViewById(R.id.currentPlaytime);
        mBtnPlay = (ImageView) findViewById(R.id.btn_play);

        mSeekBar.setProgress(0);
        mSeekBar.setOnSeekBarChangeListener(onSeekBarChangeListener);

        mMSPlayer = new MSPlayer(this);

        mGlSurfaceView = (GLSurfaceView) findViewById(R.id.surfaceView);
        mGlSurfaceView.setEGLContextClientVersion(3);
        mGlSurfaceView.setRenderer(mMSPlayer);
        mGlSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY); // GLSurfaceView.RENDERMODE_WHEN_DIRTY


        initActionBar();
        initListener();
    }

    private void initListener() {
        mBtnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int playStatus = mMSPlayer.getPlayStatus();
                if (playStatus==0){
                    mMSPlayer.onPauseViewPlay();
                }else{
                    mMSPlayer.onReStartVideoPlay();
                }
            }
        });
    }

    private void initActionBar() {
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
            actionBar.setCustomView(R.layout.actionbar_layout);

            TextView barText = (TextView) actionBar.getCustomView().findViewById(R.id.actionbar_textView);
            Button barBtn = (Button) actionBar.getCustomView().findViewById(R.id.actionbar_button);

            barBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                    intent.setType("video/*");
                    intent.addCategory(Intent.CATEGORY_OPENABLE);
                    try {
                        startActivityForResult(Intent.createChooser(intent, "Select a File to Upload"), FILE_VIDEO_REQUEST_CODE);
                    } catch (android.content.ActivityNotFoundException ex) {
                        Log.d(TAG, "Open File Dialog Error...");
                    }
                }
            });
        }

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);   //应用运行时，保持屏幕高亮，不锁屏
    }

    @Override
    protected void onStart() {
        super.onStart();
        mMSPlayer.InitVideoPlayer();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        mMSPlayer.StopVideoPlayer();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case FILE_VIDEO_REQUEST_CODE:
                if (resultCode == RESULT_OK) {
                    Uri uri = data.getData();
                    String path = FileUtils.getFilePathFromUri(this, uri);
                    Log.d(TAG, "File Path: " + path);
                    mMSPlayer.StartVideoPlayerWithPath(path);
                    mVideoRatio = mMSPlayer.GetVideoSizeRatio();
                    mVideoTotalSeconds = mMSPlayer.GetVideoTotalSeconds();
                    Log.d(TAG, "video ratio: " + mVideoRatio);
                    Log.d(TAG, "video total seconds: " + mVideoTotalSeconds);
                    mSeekBar.setProgress(0);
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);

        updateVideoSize();
    }

    public void UpdateVideoRenderCallback() {
        mGlSurfaceView.requestRender();
    }

    private void updateVideoSize() {

        WindowManager manager = this.getWindowManager();
        DisplayMetrics outMetrics = new DisplayMetrics();
        manager.getDefaultDisplay().getMetrics(outMetrics);
        int width = outMetrics.widthPixels;
        int height = outMetrics.heightPixels;


        int topMargin = 0;
        if (mVideoRatio > 1) {
            topMargin = 200;
        }

        RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) mGlSurfaceView.getLayoutParams();
        layoutParams.topMargin = topMargin;
        layoutParams.leftMargin = 0;
        layoutParams.rightMargin = 0;
        layoutParams.width = width;
        layoutParams.height = (int) (width / mVideoRatio);

        mGlSurfaceView.setLayoutParams(layoutParams);
    }

    private SeekBar.OnSeekBarChangeListener onSeekBarChangeListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if (fromUser){
                if (progress % 10 == 0) {
                    Log.d(TAG, "当前进度：" + progress);
                    onSeekingVideo(progress);
                }
            }
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            Log.d(TAG, "结束：" + seekBar.getProgress());
            int currValue = seekBar.getProgress();
            onSeekingVideo(currValue);
        }
    };

    private void onSeekingVideo(float aValue) {

        float currSliderRatio = aValue / 1000.0f;
        float seekingTime = currSliderRatio * mVideoTotalSeconds;

        if (seekingTime > mVideoTotalSeconds) {
            seekingTime = mVideoTotalSeconds;
        }
        Log.d(TAG, "Seeking Time: " + seekingTime);
        mMSPlayer.SeekVideoPlayer(seekingTime);
    }

    private void verifyStoragePermissions(Activity activity) {

        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
        }
    }


    public void onVideoPlayTime(float time){
       runOnUiThread(new Runnable() {
           @Override
           public void run() {
               mSeekBar.setProgress((int) time);
               mCurrentPlaytime.setText(getMinutes((int) time) + ":" + getSeconds((int) time));
           }
       });
    }

    public void onPrepare(float totalDuration){
       runOnUiThread(new Runnable() {
           @Override
           public void run() {
               mTotalDuration.setText(getMinutes((int) totalDuration) + ":" + getSeconds((int) totalDuration));
               mSeekBar.setMax((int) totalDuration);
           }
       });
    }

    public void onPlayStatus(int state){
        Log.d("=====","state"+state);
       runOnUiThread(new Runnable() {
           @Override
           public void run() {
               if (state==0){
                   mBtnPlay.setBackgroundResource(R.mipmap.icon_edit_pause);
               }else {
                   mBtnPlay.setBackgroundResource(R.mipmap.icon_edit_play);
               }
           }
       });
    }

    /*给我一个duration，转换成xxx分钟*/
    private String getMinutes(int duration) {
        int minutes = duration / 60;
        if (minutes <= 9) {
            return "0" + minutes;
        }
        return "" + minutes;
    }

    /*给我一个duration，转换成xxx秒*/
    private String getSeconds(int duration) { //
        int seconds = duration % 60;
        if (seconds <= 9) {
            return "0" + seconds;
        }
        return "" + seconds;
    }

}