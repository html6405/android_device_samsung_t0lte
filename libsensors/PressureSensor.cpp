/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cstring>

#include <cutils/log.h>

#include "PressureSensor.h"

#define LOG_TAG "PressureSensor"

/*
 * The BMP driver gives pascal values.
 * It needs to be changed into hectoPascal
 */
#define PRESSURE_HECTO (1.0f/100.0f)

#include <stdlib.h>

/*****************************************************************************/

PressureSensor::PressureSensor()
    : SensorBase(NULL, "pressure_sensor"),
      mEnabled(0),
      mInputReader(4),
      mHasPendingEvent(false)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_PR;
    mPendingEvent.type = SENSOR_TYPE_PRESSURE;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);
        enable(0, 1);
    }
}

PressureSensor::~PressureSensor() {
    if (mEnabled) {
        enable(0, 0);
    }
}

int PressureSensor::setInitialState() {
    struct input_absinfo absinfo;
    if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_PRESSURE), &absinfo)) {
        // make sure to report an event immediately
        mHasPendingEvent = true;
        mPendingEvent.pressure = absinfo.value * PRESSURE_HECTO;
    }
    return 0;
}

static void* set_initial_state_fn(void *data) {
    PressureSensor *sensor = (PressureSensor*)data;

    ALOGE("%s: start", __func__);
    usleep(100000); // 100ms
    sensor->setDelay(0, 100000);
    ALOGE("%s: end", __func__);

    return NULL;
}

static void set_initial_state_thread(PressureSensor *sensor) {
       pthread_attr_t thread_attr;
       pthread_t setdelay_thread;

       pthread_attr_init(&thread_attr);
       pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
       int rc = pthread_create(&setdelay_thread, &thread_attr, set_initial_state_fn, (void*)sensor);
       if (rc < 0) {
           ALOGE("%s: Unable to create thread", __func__);
       }
}

int PressureSensor::enable(int32_t handle, int en) {
    int flags = en ? 1 : 0;
    int err;
    if (flags != mEnabled) {
         err = sspEnable(LOG_TAG, SSP_PRESS, en);
         if(err >= 0){
             mEnabled = flags;
             setInitialState();
             set_initial_state_thread(this);

             return 0;
         }
         return -1;
    }
    return 0;
}

bool PressureSensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int PressureSensor::setDelay(int32_t handle, int64_t ns)
{
    int fd;

    strcpy(&input_sysfs_path[input_sysfs_path_len], "pressure_poll_delay");
    fd = open(input_sysfs_path, O_RDWR);
    if (fd >= 0) {
        char buf[80];
        sprintf(buf, "%lld", ns);
        write(fd, buf, strlen(buf)+1);
        close(fd);
        return 0;
    }
    return -1;
}


int PressureSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_REL) {
            if (event->code == EVENT_TYPE_PRESSURE) {
                mPendingEvent.pressure = event->value * PRESSURE_HECTO;
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("%s: unknown event (type=%d, code=%d)", LOG_TAG,
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}
