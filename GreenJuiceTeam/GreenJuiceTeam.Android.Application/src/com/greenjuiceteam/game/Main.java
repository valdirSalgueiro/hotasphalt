
package com.greenjuiceteam.game;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.view.MotionEvent;


public class Main extends NativeActivity
{

	public boolean wantsMultitouch=false;

	public void onCreate(Bundle savedInstanceState) 
	{
		org.fmod.FMOD.init(this);
		Log.v("Main","org.fmod.FMOD.init(this);");
		requestWindowFeature(Window.FEATURE_NO_TITLE);  
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  WindowManager.LayoutParams.FLAG_FULLSCREEN);  
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		super.onCreate(savedInstanceState);		
		
		Log.v("Main","org.fmod.FMOD.init(this); end");
				

    }

    @Override
    protected void onDestroy()
    {
		super.onDestroy();
        org.fmod.FMOD.close();
    }


	static
	{
		Log.v("Main","loading");
		System.loadLibrary("fmodL"); 
		System.loadLibrary("fmodstudio"); 		
		System.loadLibrary("GreenJuiceTeam"); 
	}





}
