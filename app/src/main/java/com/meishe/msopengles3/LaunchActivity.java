package com.meishe.msopengles3;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class LaunchActivity extends AppCompatActivity {

    private Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lanuch);
        mContext=this;
    }

    public void startToRenderEffect(View view) {
        startActivity(new Intent(mContext,RenderEffectActivity.class));
    }

    public void startViewPlayer(View view) {
        startActivity(new Intent(mContext,ViewPlayerActivity.class));
    }
}