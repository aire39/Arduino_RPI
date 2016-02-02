package csusb.cse541.william.cse541robotics;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.RelativeLayout;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;

import java.io.IOException;

public class MainActivity extends Activity {
    private WifiController wifiCtrl = null;
    private String WifiIP = Constants.REMOTE_IP_ADDRESS;

    private int axisX1_value = 0;
    private int axisX2_value = 0;
    private int axisY1_value = 0;
    private int axisY2_value = 0;
    private int nTouch = 0;
    private final int boundY = 1546;
    private boolean isConnected = false;
    private int mode = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        setContentView(R.layout.activity_main);

        wifiCtrl = new WifiController();

        Button btn_changeip = (Button) findViewById(R.id.btn_changeip);
        btn_changeip.setOnClickListener(ocl_changeip);

        Button btn_forward = (Button) findViewById(R.id.forwardButton);
        btn_forward.setOnClickListener(ocl_forward);

        Button btn_backward = (Button) findViewById(R.id.backwardButton);
        btn_backward.setOnClickListener(ocl_backward);

        Button btn_right = (Button) findViewById(R.id.rightButton);
        btn_right.setOnClickListener(ocl_right);

        Button btn_left = (Button) findViewById(R.id.leftButton);
        btn_left.setOnClickListener(ocl_left);

        Button btn_stop = (Button) findViewById(R.id.stopButton);
        btn_stop.setOnClickListener(ocl_stop);

        Button btn_connect = (Button) findViewById(R.id.connectButton);
        btn_connect.setOnClickListener(ocl_connect);

        Button btn_disconnect = (Button) findViewById(R.id.disconnectButton);
        btn_disconnect.setOnClickListener(ocl_disconnect);

        SeekBar skbrSpeed = (SeekBar) findViewById(R.id.skbrSpeedBar);
        skbrSpeed.setOnSeekBarChangeListener(oclChangeSpeed);

        RelativeLayout rl_mainactivity = (RelativeLayout) findViewById(R.id.activity_main);
        rl_mainactivity.setOnTouchListener(otl_wheelcontrol);

        Button btn_mode = (Button) findViewById(R.id.modeButton);
        btn_mode.setOnClickListener(ocl_mode);

    }

    /* Disconnect from the arduino robot */
    View.OnClickListener ocl_disconnect = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.disconnect();
            } catch (IOException e) {
                sendEHandler();
            }

            isConnected = false;
            String mdata = "a";
            char spd = (char) 1;
            mdata += spd;
            try {
                wifiCtrl.send(mdata);
            } catch (IOException e) {
                e.printStackTrace();
            }
            mdata = "b";
            spd = (char) 1;
            mdata += spd;
            try {
                wifiCtrl.send(mdata);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    /* Connect to the arduino robot */
    View.OnClickListener ocl_connect = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            wifiCtrl.connect(WifiIP, Constants.REMOTE_PORT, MainActivity.this);
        }
    };

    View.OnClickListener ocl_forward = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.send(Constants.FORWARD_SIG);
            } catch (Exception e) {
                sendEHandler();
            }
        }
    };

    View.OnClickListener ocl_backward = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.send(Constants.BACKWARD_SIG);
            } catch (Exception e) {
                sendEHandler();
            }
        }
    };

    View.OnClickListener ocl_right = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.send(Constants.RIGHT_SIG);
            } catch (Exception e) {
                sendEHandler();
            }
        }
    };

    View.OnClickListener ocl_left = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.send(Constants.LEFT_SIG);
            } catch (Exception e) {
                sendEHandler();
            }
        }
    };

    View.OnClickListener ocl_stop = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            try {
                wifiCtrl.send(Constants.STOP_SIG);
            } catch (Exception e) {
                sendEHandler();
            }
        }
    };

    View.OnClickListener ocl_changeip = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            EditText et_changeip = (EditText) findViewById(R.id.et_changeip);
            WifiIP = et_changeip.getText().toString();
        }
    };

    SeekBar.OnSeekBarChangeListener oclChangeSpeed = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            String newSpeed = "k";
            SeekBar skbrSpeed = (SeekBar) findViewById(R.id.skbrSpeedBar);
            int _spd = skbrSpeed.getProgress();
            char spd = (char) _spd;
            newSpeed += spd;
            try {
                wifiCtrl.send(newSpeed);    // This should be a single digit to avoid fast speed changes
            } catch (Exception e) {
                new AlertDialog.Builder(MainActivity.this).setTitle(R.string.connectionErrDialogTitle)
                        .setMessage(R.string.noConnMsg).setCancelable(true).setPositiveButton(R.string.posBtnTxt, null)
                        .create().show();
            }
        }

    };

    private void sendEHandler () {
        new AlertDialog.Builder (MainActivity.this).setTitle (R.string.connectionErrDialogTitle)
                .setMessage (R.string.noConnMsg).setCancelable(true).setPositiveButton(R.string.posBtnTxt, null)
                .create ().show ();
    }


    View.OnClickListener ocl_mode = new View.OnClickListener() {
        @Override
        public void onClick(View v) {

            if(mode == 1)
                mode = 2;
            else
                mode = 1;

            String m_mode = "m";
            char _m = (char) mode;
            m_mode += _m;

            try {
                wifiCtrl.send(m_mode);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    View.OnTouchListener otl_wheelcontrol = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            axisX1_value = (int)event.getX(0);
            nTouch = event.getPointerCount();

            if(nTouch == 1) {
                if (axisX1_value >= 0 && axisX1_value <= 1299) { // Left touch area is the left wheel
                    axisY1_value = (boundY - (int)event.getY(0)) - (boundY/2);
                } else { // Right touch area
                    axisY2_value = (boundY - (int)event.getY(0)) - (boundY/2);
                }
            } else if(nTouch == 2) {

                axisX2_value = (int)event.getX(1);
                axisY2_value = (int)event.getY(1);

                if (axisX1_value >= 0 && axisX1_value <= 1299) { // Left touch area is the left wheel
                    axisY1_value = (boundY - (int)event.getY(0)) - (boundY/2);
                } else {
                    axisY2_value = (boundY - (int)event.getY(0)) - (boundY/2);
                }

                if (axisX2_value >= 0 && axisX2_value <= 1299) { // Left touch area is the left wheel
                    axisY1_value = (boundY - (int)event.getY(1)) - (boundY/2);
                } else {
                    axisY2_value = (boundY - (int)event.getY(1)) - (boundY/2);
                }
            }

            String mdata = "a";
            float __spd = ((float)axisY1_value/800.0f) * 255.0f;
            int _spd = (int)__spd;
            if(_spd < 0)
                mdata = "c";
            char spd = (char) Math.abs(_spd);

            mdata += spd;

            if(isConnected) {
                try {
                    wifiCtrl.send(mdata);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }


            String mdata2 = "b";
            __spd = ((float)axisY2_value/800.0f) * 255.0f;
            _spd = (int)__spd;
            if(_spd < 0)
                mdata2 = "d";
            spd = (char) Math.abs(_spd);

            mdata2 += spd;

            if(isConnected) {
                try {
                    wifiCtrl.send(mdata2);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            Log.i("Touch Y", Integer.toString(axisY1_value) + " " + Integer.toString(axisY2_value) + " " + mdata);

            return true;
        }
    };
}

   
