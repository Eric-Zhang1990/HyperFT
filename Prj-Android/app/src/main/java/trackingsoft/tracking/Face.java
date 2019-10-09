package trackingsoft.tracking;

/**
 * Created by yujinke on 17/07/2018.
 */



public class Face {


    public int ID;
    public int left;
    public int top;
    public int right;
    public int bottom;
    public int height;
    public int width;
    public int roll;
    public int yaw;
    public int pitch;
    public int[] landmarks;
    public boolean isStable;








    Face(int x1,int y1,int x2,int y2)
    {
        left= x1;
        top = y1;
        right = x2;
        bottom = y2;
        height = y2-y1;
        width  = x2-x1;
        landmarks = new int[5*2];
        roll = yaw = pitch = 0;
    }


    Face(int x1,int y1,int _width,int _height,int id)
    {
        left= x1;
        top = y1;
        right = x1 + _width;
        bottom = y1 + _height;
        width = _width;
        height  = _height;
        ID = id;
        landmarks = new int[5*2];
        roll = yaw = pitch = 0;
    }


    Face(int x1,int y1,int _width,int _height,int[] landmark,int id) {
        left= x1;
        top = y1;
        right = x1 + _width;
        bottom = y1 + _height;
        width = _width;
        height  = _height;
        landmarks = new int[5*2];
        for(int i=0;i<10;i++) {
            landmarks[i] = landmark[i];
        }
        ID = id;
        roll = yaw = pitch = 0;
    }







}
