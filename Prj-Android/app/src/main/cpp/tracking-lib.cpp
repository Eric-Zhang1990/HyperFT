#include <jni.h>
#include <string>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include "LandmarkTracking.h"


#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "mytest",__VA_ARGS__)



std::string jstring2str(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    std::string stemp(rtn);
    free(rtn);
    return stemp;
}
extern "C" {


JNIEXPORT void JNICALL
Java_trackingsoft_tracking_FaceTracking_initTracking(JNIEnv *env, jobject obj, jbyteArray yuv,
                                                         jint height, jint width, jlong handle) {

        jbyte *pBuf = (jbyte *) env->GetByteArrayElements(yuv, 0);
        cv::Mat image(height + height / 2, width, CV_8UC1, (unsigned char *) pBuf);
        cv::Mat mBgr;
        cvtColor(image, mBgr, cv::COLOR_YUV2BGR_NV21);
        cv::transpose(mBgr, mBgr);
        cv::flip(mBgr, mBgr, -1);
        FaceTracking *trackingSession = (FaceTracking *) handle;
        trackingSession->Init(mBgr);

}



JNIEXPORT void JNICALL
Java_trackingsoft_tracking_FaceTracking_update(JNIEnv *env, jobject obj, jbyteArray yuv,
                                                jint height, jint width, jlong handle) {

    jbyte *pBuf = (jbyte *) env->GetByteArrayElements(yuv, 0);
    cv::Mat image(height + height / 2, width, CV_8UC1, (unsigned char *) pBuf);
    cv::Mat mBgr;
    cvtColor(image, mBgr, cv::COLOR_YUV2BGR_NV21);
    cv::transpose(mBgr, mBgr);
    cv::flip(mBgr, mBgr, -1);
    FaceTracking *trackingSession = (FaceTracking *) handle;
    trackingSession->update(mBgr);

}



JNIEXPORT jint JNICALL
Java_trackingsoft_tracking_FaceTracking_getTrackingNum(JNIEnv *env, jobject obj, jlong handle) {
    FaceTracking *trackingSession = (FaceTracking *) handle;

    return  (jint)trackingSession->trackingFace.size();

}


JNIEXPORT jintArray JNICALL
Java_trackingsoft_tracking_FaceTracking_getTrackingLandmarkByIndex(JNIEnv *env, jobject obj,jint target, jlong handle) {
    FaceTracking *trackingSession = (FaceTracking *) handle;
    jintArray jarr = env->NewIntArray(5*2);
    jint *arr = env->GetIntArrayElements(jarr, NULL);
    const Face &info= trackingSession->trackingFace[target];
    for(int i = 0; i < 5; i++){
        arr[i*2+0] = info.faceBbox.ppoint[i];
        arr[i*2+1] = info.faceBbox.ppoint[i + 5];
    }

    env->ReleaseIntArrayElements(jarr, arr, 0);
    return jarr;
}


JNIEXPORT jintArray JNICALL
Java_trackingsoft_tracking_FaceTracking_getTrackingLocationByIndex(JNIEnv *env, jobject obj,jint target, jlong handle) {
    FaceTracking *trackingSession = (FaceTracking *) handle;
    jintArray jarr = env->NewIntArray(4);
    jint *arr = env->GetIntArrayElements(jarr, NULL);
    const Face &info= trackingSession->trackingFace[target];

    arr[0] = info.faceBbox.x1;
    arr[1] = info.faceBbox.y1;
    //arr[2] = info.faceBbox.x2;
    //arr[3] = info.faceBbox.y2;
    arr[2] = info.faceBbox.x2 - info.faceBbox.x1;
    arr[3] = info.faceBbox.y2 - info.faceBbox.y1;

    __android_log_print(ANDROID_LOG_DEBUG,"pos","height %d width %d",trackingSession->UI_height,trackingSession->UI_width);
    __android_log_print(ANDROID_LOG_DEBUG,"pos","x:%d . y:%d , w:%d ,h:%d",arr[0],arr[1],arr[2],arr[3]);
    env->ReleaseIntArrayElements(jarr, arr, 0);
    return jarr;
}



JNIEXPORT jint JNICALL
Java_trackingsoft_tracking_FaceTracking_getTrackingIDByIndex(JNIEnv *env, jobject obj,jint target, jlong handle) {
    FaceTracking *trackingSession = (FaceTracking *) handle;
    const Face &info= trackingSession->trackingFace[target];
    return (jint)info.face_id;
}


JNIEXPORT jlong JNICALL
Java_trackingsoft_tracking_FaceTracking_createSession(JNIEnv *env, jobject obj, jstring folder) {

    std::string detector_path = jstring2str(env, folder);
    FaceTracking *aliveDetector = new FaceTracking(detector_path);
    aliveDetector->UI_width=-1;
    aliveDetector->UI_height=-2;

    return (jlong) aliveDetector;

}


JNIEXPORT void JNICALL
Java_trackingsoft_tracking_FaceTracking_releaseSession(JNIEnv *env, jobject obj, jlong handle) {
    FaceTracking *trackingSession = (FaceTracking *) handle;
    delete trackingSession;
}

}


