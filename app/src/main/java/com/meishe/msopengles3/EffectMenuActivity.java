package com.meishe.msopengles3;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

public class EffectMenuActivity extends AppCompatActivity {

    private static final int MS_VAO_SAMPLE = 100;
    private static final int MS_TRANSITION_SAMPLE = 101;
    private static final int MS_HIGH_LIGHT_SAMPLE = 102;
    private static final int MS_BURN_PAGE_SAMPLE = 103;
    private static final int MS_IMAGE_FADE_SAMPLE = 104;
    private static final int MS_MOSAIC_SAMPLE = 105;
    private static final int MS_WATER_SAMPLE = 106;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_effect_menu);
    }

    public void onRenderCubeEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_VAO_SAMPLE);
    }

    public void onRenderTransitionEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_TRANSITION_SAMPLE);
    }

    public void onRenderHighLightEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_HIGH_LIGHT_SAMPLE);
    }

    public void onRenderBurnPaperEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_BURN_PAGE_SAMPLE);
    }

    public void onRenderImageFadeEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_IMAGE_FADE_SAMPLE);
    }

    public void onRenderMosaicEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_MOSAIC_SAMPLE);
    }

    public void onRenderWaterEffect(View view) {
        RenderEffectActivity.actionRenderEffectActivity(this,MS_WATER_SAMPLE);
    }
}