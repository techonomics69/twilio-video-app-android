/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "TSCoreSDKTypes.h"
#include "TSCoreConstants.h"
#include "TSCoreSDK.h"
#include "TSCEndpoint.h"
#include "TSCEndpointObserver.h"
#include "TSCConfiguration.h"
#include "TSCLogger.h"
#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/modules/video_capture/video_capture_internal.h"
#include "webrtc/modules/video_render/video_render_internal.h"

#include "webrtc/modules/audio_device/android/audio_manager.h"
#include "webrtc/modules/audio_device/android/opensles_player.h"
#include "webrtc/modules/video_capture/android/device_info_android.h"

#include <string.h>
#include "com_twilio_signal_impl_TwilioRTCImpl.h"

#include "talk/app/webrtc/java/jni/jni_helpers.h"
#include "talk/app/webrtc/java/jni/classreferenceholder.h"

#define TAG  "TwilioSDK(native)"

using namespace twiliosdk;

/*
 * Class:     com_twilio_signal_impl_TwilioRTCImpl
 * Method:    initCore
 * Signature: (Landroid/content/Context;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_twilio_signal_impl_TwilioRTCImpl_initCore(JNIEnv *env, jobject obj, jobject context) {

	bool failure = false;
	JavaVM * cachedJVM = NULL;

	env->GetJavaVM(&cachedJVM);

	// Perform webrtc_jni initialization to enable peerconnection_jni.cc JNI bindings.
	jint ret = webrtc_jni::InitGlobalJniVariables(cachedJVM);
	if(ret < 0) {
		TS_CORE_LOG_ERROR("TwilioSDK.InitGlobalJniVariables() failed");
		return failure;
	} else {
		webrtc_jni::LoadGlobalClassReferenceHolder();
	}

	TSCSDK* tscSdk = TSCSDK::instance();

	webrtc::videocapturemodule::DeviceInfoAndroid::Initialize(env);

	webrtc::OpenSLESPlayer::SetAndroidAudioDeviceObjects(cachedJVM, context);

	failure |= webrtc::SetCaptureAndroidVM(cachedJVM, context);
	failure |= webrtc::SetRenderAndroidVM(cachedJVM);

	TS_CORE_LOG_DEBUG("Calling DA Magic formula");
	failure |= webrtc::VoiceEngine::SetAndroidObjects(cachedJVM, context);

	if (tscSdk != NULL && tscSdk->isInitialized())
	{
		return JNI_TRUE;
	}

	return JNI_FALSE;
}


JNIEXPORT jlong JNICALL Java_com_twilio_signal_impl_TwilioRTCImpl_createEndpoint
  (JNIEnv *env, jobject obj, jstring token, jlong nativeEndpointObserver) {

	if (token == NULL) {
		TS_CORE_LOG_ERROR("token is null");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);

	TSCOptions options;
	options.insert(std::make_pair(kTSCTokenKey, tokenStr));

	if (!nativeEndpointObserver)
	{
		TS_CORE_LOG_ERROR("nativeEndpointObserver is null");
		return 0;
	}

	TSCEndpointObserverObjectRef eObserverRef =
			TSCEndpointObserverObjectRef(reinterpret_cast<TSCEndpointObserverObject*>(nativeEndpointObserver));

	TSCEndpointObjectRef endpoint = TSCSDK::instance()->createEndpoint(options, eObserverRef);

	return (jlong) endpoint.release();
}

