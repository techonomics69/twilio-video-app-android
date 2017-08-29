/*
 * Copyright (C) 2017 Twilio, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "com_twilio_video_LocalAudioTrack.h"
#include "webrtc/sdk/android/src/jni/jni_helpers.h"
#include "class_reference_holder.h"

namespace twilio_video_jni {

static const char *const kLocalAudioTrackConstructorSignature = "("
        "J"
        "Ljava/lang/String;"
        "Ljava/lang/String;"
        "Z"
        "Lcom/twilio/video/MediaFactory;"
        ")V";

std::shared_ptr<twilio::media::LocalAudioTrack> getLocalAudioTrack(jlong local_audio_track_handle) {
    LocalAudioTrackContext* local_audio_track_context =
            reinterpret_cast<LocalAudioTrackContext *>(local_audio_track_handle);

    return local_audio_track_context->getLocalAudioTrack();
}

jobject createJavaLocalAudioTrack(jobject j_media_factory,
                                  std::shared_ptr<twilio::media::LocalAudioTrack> local_audio_track) {
    JNIEnv *jni = webrtc_jni::GetEnv();
    jclass j_local_audio_track_class = twilio_video_jni::FindClass(jni,
                                                                   "com/twilio/video/LocalAudioTrack");
    jmethodID j_local_audio_track_ctor_id = webrtc_jni::GetMethodID(jni,
                                                                    j_local_audio_track_class,
                                                                    "<init>",
                                                                    kLocalAudioTrackConstructorSignature);
    LocalAudioTrackContext* local_audio_track_context =
            new LocalAudioTrackContext(local_audio_track);
    jstring j_name = webrtc_jni::JavaStringFromStdString(jni, local_audio_track->getName());
    jobject j_local_audio_track = jni->NewObject(j_local_audio_track_class,
                                                 j_local_audio_track_ctor_id,
                                                 webrtc_jni::jlongFromPointer(local_audio_track_context),
                                                 webrtc_jni::JavaStringFromStdString(jni, local_audio_track->getTrackId()),
                                                 j_name,
                                                 local_audio_track->isEnabled(),
                                                 j_media_factory);
    CHECK_EXCEPTION(jni) << "Failed to create LocalVideoTrack instance";

    return j_local_audio_track;
}

JNIEXPORT jboolean JNICALL Java_com_twilio_video_LocalAudioTrack_nativeIsEnabled(JNIEnv *jni,
                                                                                 jobject j_local_audio_track,
                                                                                 jlong local_audio_track_handle) {
    std::shared_ptr<twilio::media::LocalAudioTrack> local_audio_track =
            getLocalAudioTrack(local_audio_track_handle);

    return local_audio_track->isEnabled();
}

JNIEXPORT void JNICALL Java_com_twilio_video_LocalAudioTrack_nativeEnable(JNIEnv *jni,
                                                                          jobject j_local_audio_track,
                                                                          jlong local_audio_track_handle,
                                                                          jboolean enabled) {
    std::shared_ptr<twilio::media::LocalAudioTrack> local_audio_track =
            getLocalAudioTrack(local_audio_track_handle);

    local_audio_track->setEnabled(enabled);
}

JNIEXPORT void JNICALL Java_com_twilio_video_LocalAudioTrack_nativeRelease(JNIEnv *jni,
                                                                           jobject j_local_audio_track,
                                                                           jlong local_audio_track_handle) {
    LocalAudioTrackContext* local_audio_track_context =
            reinterpret_cast<LocalAudioTrackContext *>(local_audio_track_handle);

    delete local_audio_track_context;
}

}
