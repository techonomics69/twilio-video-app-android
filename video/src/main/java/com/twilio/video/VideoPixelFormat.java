package com.twilio.video;

public enum VideoPixelFormat {
    NV21,
    RGBA_8888;

    private int value = Integer.MIN_VALUE;

    /*
     * We delay the JNI call until this is invoked because we know library has been loaded
     */
    int getValue() {
        if (unset()) {
            // Cache the value for later use
            value = nativeGetValue(name());
        }
        return value;
    }

    boolean unset() {
        return value == Integer.MIN_VALUE;
    }

    private native int nativeGetValue(String name);
}
